#!/usr/bin/env python3
"""
patch_kissat.py — Run Kissat on every instance in an existing CSV and fill in
kissat_cpu_s / kissat_got columns without rerunning any other solver.

Timing methodology:
  - Kissat self-reports process-time at 0.01s (10ms) resolution.
  - For instances where self-reported time >= 0.01s, use it directly.
  - For instances where self-reported time == 0.00s (sub-10ms solves), run
    N_SHORT_REPS times and use median(wall_clock) - median_startup_s.
  - Startup overhead is measured by running Kissat on a trivially UNSAT CNF
    (single empty clause) so it exits immediately — this isolates pure process
    overhead. Both first-run (cold) and subsequent (warm cache) values are
    reported; the median across all runs is used as the constant.

Usage:
    python3 tests/satlib/patch_kissat.py --kissat kissat/build/kissat results/*.csv
"""

import argparse
import csv
import re
import statistics
import subprocess
import sys
import tempfile
import time
from pathlib import Path

SATLIB_DIR = Path(__file__).resolve().parent

_RE_KS_TIME = re.compile(r"process-time:\s+([\d.]+)")
N_SHORT_REPS = 10   # repetitions for sub-10ms instances
N_STARTUP    = 20   # repetitions for startup calibration


def strip_satlib_tmp(cnf_path: Path) -> Path:
    lines = cnf_path.read_text().splitlines()
    stripped = []
    for line in lines:
        if line.strip() == "%":
            break
        stripped.append(line + "\n")
    tf = tempfile.NamedTemporaryFile(mode="w", suffix=".cnf", delete=False)
    tf.writelines(stripped)
    tf.close()
    return Path(tf.name)


def wall_run(kissat: Path, cnf: Path, timeout: int) -> tuple:
    """Single run: returns (wall_s, cpu_s, got)."""
    t0 = time.monotonic()
    try:
        proc = subprocess.Popen(
            [str(kissat), str(cnf)],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            universal_newlines=True)
        stdout, stderr = proc.communicate(timeout=timeout)
        wall_s = time.monotonic() - t0
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.communicate()
        return float(timeout), float(timeout), None

    combined = stdout + stderr
    got = None
    if "s SATISFIABLE" in combined:
        got = "SAT"
    elif "s UNSATISFIABLE" in combined:
        got = "UNSAT"

    m = _RE_KS_TIME.search(combined)
    cpu_s = float(m.group(1)) if m else None
    return wall_s, cpu_s, got


def measure_startup(kissat: Path) -> float:
    """
    Measure Kissat process startup overhead using a trivially UNSAT CNF
    (single empty clause — Kissat exits immediately after parsing).
    Runs N_STARTUP times to capture both cold and warm cache behaviour.
    Returns the median wall-clock across all runs.
    """
    # Empty clause: immediately UNSAT, zero solve work
    tf = tempfile.NamedTemporaryFile(mode="w", suffix=".cnf", delete=False)
    tf.write("p cnf 1 1\n0\n")  # one variable, one empty clause
    tf.close()
    cnf = Path(tf.name)

    walls = []
    for _ in range(N_STARTUP):
        t0 = time.monotonic()
        subprocess.run([str(kissat), str(cnf)],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        walls.append(time.monotonic() - t0)
    cnf.unlink()

    walls.sort()
    cold = walls[0]
    warm_median = statistics.median(walls[1:])
    overall_median = statistics.median(walls)
    print(f"  Startup calibration ({N_STARTUP} runs):")
    print(f"    cold (first run): {cold*1e3:.2f}ms")
    print(f"    warm median:      {warm_median*1e3:.2f}ms")
    print(f"    overall median:   {overall_median*1e3:.2f}ms  ← using this")
    return overall_median


def run_kissat(cnf_path: Path, kissat: Path, timeout: int,
               startup_s: float) -> tuple:
    tmp = strip_satlib_tmp(cnf_path)
    try:
        # Single run to get self-reported time and verdict
        wall_s, cpu_s, got = wall_run(kissat, tmp, timeout)

        if cpu_s is not None and cpu_s >= 0.01:
            # Self-reported time is reliable — use it directly
            return cpu_s, got

        # Sub-10ms: self-reported rounds to 0. Run N_SHORT_REPS times,
        # take median wall-clock, subtract startup overhead.
        walls = [wall_s]
        for _ in range(N_SHORT_REPS - 1):
            w, _, _ = wall_run(kissat, tmp, timeout)
            walls.append(w)
        effective = max(statistics.median(walls) - startup_s, 0.0)
        return effective, got

    finally:
        if tmp.exists():
            tmp.unlink()


def patch_csv(csv_path: Path, kissat: Path, timeout: int, startup_s: float):
    rows = list(csv.DictReader(csv_path.open()))
    if not rows:
        print(f"  SKIP (empty): {csv_path.name}")
        return

    fields = list(rows[0].keys())
    if "kissat_cpu_s" not in fields:
        print(f"  SKIP (no kissat columns): {csv_path.name}")
        return

    n = len(rows)
    print(f"  {csv_path.name}: {n} instances", flush=True)

    for i, row in enumerate(rows):
        cnf_path = SATLIB_DIR / row["set"] / row["file"]
        if not cnf_path.exists():
            print(f"    WARNING: {cnf_path} not found, skipping")
            continue
        cpu_s, got = run_kissat(cnf_path, kissat, timeout, startup_s)
        row["kissat_cpu_s"] = cpu_s if cpu_s is not None else ""
        row["kissat_got"]   = got   if got   is not None else ""
        if (i + 1) % 100 == 0:
            print(f"    {i+1}/{n} done", flush=True)

    with csv_path.open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        w.writerows(rows)
    print(f"    done -> {csv_path.name}")


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--kissat",  required=True, help="path to kissat binary")
    p.add_argument("--timeout", type=int, default=60)
    p.add_argument("csvs", nargs="+", help="CSV files to patch")
    args = p.parse_args()

    kissat = Path(args.kissat)
    if not kissat.exists():
        print(f"ERROR: kissat not found at {kissat}", file=sys.stderr)
        sys.exit(1)

    print("Measuring Kissat startup overhead...")
    startup_s = measure_startup(kissat)

    for csv_path in [Path(c) for c in args.csvs]:
        if not csv_path.exists():
            print(f"SKIP (not found): {csv_path}")
            continue
        patch_csv(csv_path, kissat, args.timeout, startup_s)


if __name__ == "__main__":
    main()
