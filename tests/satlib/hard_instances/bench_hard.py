#!/usr/bin/env python3
"""
bench_hard.py — Run the hard-instance suite (pigeonhole, Tseitin, k-colouring)
through our HW-BCP coprocessor and the three reference SW solvers, and print
+ CSV-dump a comparison table.

Time reported for the coprocessor is sw_time + hw_time (i.e. the timing
methodology in §4.4 of the project report) recomputed at the synthesis
target of 10.1 ns / HW cycle.

Usage:
    python3 bench_hard.py
    python3 bench_hard.py --timeout 30 --csv hard_results.csv
"""
import argparse
import csv
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT      = Path(__file__).resolve().parents[3]
HARD_DIR  = Path(__file__).resolve().parent
SOLVER    = ROOT / "coprocessing" / "cdcl_solver"
KISSAT    = ROOT / "kissat" / "build" / "kissat"
MINISAT2  = ROOT / "minisat2" / "build" / "release" / "bin" / "minisat"
GLUCOSE   = ROOT / "glucose" / "simp" / "glucose"

HW_CYCLE_NS = 10.1   # synthesis target — matches report.tex methodology

_RE_HW = re.compile(
    r"hw_cycles=(\d+)\s+hw_time=([\d.eE+\-]+)ns\s+sw_time=([\d.eE+\-]+)ns")
_RE_KS = re.compile(r"process-time:\s+([\d.]+)")
_RE_CPU = re.compile(r"CPU time\s*:\s*([\d.]+)")  # MiniSat / Glucose


def run_hw(cnf, timeout):
    t0 = time.monotonic()
    try:
        p = subprocess.run([str(SOLVER), str(cnf)],
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                           universal_newlines=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return {"got": "TIMEOUT", "us": timeout * 1e6}
    wall = time.monotonic() - t0
    got = "SAT" if "s SATISFIABLE" in p.stdout else \
          "UNSAT" if "s UNSATISFIABLE" in p.stdout else "ERROR"
    m = _RE_HW.search(p.stderr)
    if m:
        hw_cycles = int(m.group(1))
        sw_ns = float(m.group(3))
        # Recompute hw_time at 10.1 ns/cycle (match report methodology)
        hw_ns = hw_cycles * HW_CYCLE_NS
        total_us = (hw_ns + sw_ns) / 1000.0
    else:
        total_us = wall * 1e6
    return {"got": got, "us": total_us}


def run_external(cnf, binary, sat_str, unsat_str, time_re, timeout,
                 startup_baseline_us=0.0):
    """Returns reported CPU time; if the solver's self-clock rounds to 0
    (Kissat/MiniSat process-time has ~10 ms precision), falls back to
    wall-clock minus a measured subprocess startup baseline.  The caller
    is expected to pass `startup_baseline_us` for that solver."""
    t0 = time.monotonic()
    try:
        p = subprocess.run([str(binary), str(cnf)],
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                           universal_newlines=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return {"got": "TIMEOUT", "us": timeout * 1e6}
    wall_us = (time.monotonic() - t0) * 1e6
    out = p.stdout + p.stderr
    got = "SAT" if sat_str in out else \
          "UNSAT" if unsat_str in out else "ERROR"
    m = time_re.search(out)
    reported_us = float(m.group(1)) * 1e6 if m else float("nan")
    # Self-clock granularity fallback: if the binary reports 0 but the run
    # was clearly not free, use the wall-clock minus startup overhead.
    if reported_us <= 0 or reported_us != reported_us:  # 0 or NaN
        reported_us = max(wall_us - startup_baseline_us, 0.0)
    return {"got": got, "us": reported_us}


def measure_startup(binary, n=3):
    """Mean wall-clock cost of invoking `binary --help` (or no args).
    Used as a baseline to subtract from full-run wall-clock when the
    solver's self-clock is too coarse."""
    times = []
    for _ in range(n):
        t0 = time.monotonic()
        try:
            subprocess.run([str(binary), "--help"],
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                           timeout=5)
        except Exception:
            pass
        times.append((time.monotonic() - t0) * 1e6)
    return sum(times) / len(times) if times else 0.0


SUITE = [
    ("PHP_3",         "UNSAT", HARD_DIR / "php_3.cnf"),
    ("PHP_4",         "UNSAT", HARD_DIR / "php_4.cnf"),
    ("PHP_5",         "UNSAT", HARD_DIR / "php_5.cnf"),
    ("PHP_6",         "UNSAT", HARD_DIR / "php_6.cnf"),
    ("PHP_7",         "UNSAT", HARD_DIR / "php_7.cnf"),
    ("PHP_8",         "UNSAT", HARD_DIR / "php_8.cnf"),
    ("Tseitin-C5",    "UNSAT", HARD_DIR / "tseitin_c5.cnf"),
    ("Tseitin-C7",    "UNSAT", HARD_DIR / "tseitin_c7.cnf"),
    ("Tseitin-C9",    "UNSAT", HARD_DIR / "tseitin_c9.cnf"),
    ("Grötzsch-3col", "UNSAT", HARD_DIR / "grotzsch_3col.cnf"),
    ("Grötzsch-4col", "SAT",   HARD_DIR / "grotzsch_4col.cnf"),
]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--timeout", type=int, default=60)
    ap.add_argument("--csv", default=str(HARD_DIR / "hard_results.csv"))
    args = ap.parse_args()

    for n, b in [("cdcl_solver", SOLVER), ("kissat", KISSAT),
                 ("minisat2", MINISAT2), ("glucose", GLUCOSE)]:
        if not Path(b).exists():
            print(f"ERROR: {n} not built at {b}", file=sys.stderr)
            sys.exit(1)
        if not all(s.exists() for _, _, s in SUITE):
            print("ERROR: some CNFs missing — run gen_hard.py first.")
            sys.exit(1)

    # Per-binary subprocess-startup baseline (wall-clock when the binary's
    # own clock reports 0).  Measured once.
    base_ks = measure_startup(KISSAT)
    base_ms = measure_startup(MINISAT2)
    base_gl = measure_startup(GLUCOSE)
    print(f"# subprocess-startup baselines (us): "
          f"kissat={base_ks:.0f}  minisat2={base_ms:.0f}  glucose={base_gl:.0f}\n")

    print(f"{'INSTANCE':<14} {'EXP':<5} {'HW-BCP_us':>12} {'KISSAT_us':>12} "
          f"{'MINISAT2_us':>12} {'GLUCOSE_us':>12}  WIN")
    print("-" * 80)

    rows = []
    for name, expect, cnf in SUITE:
        if not cnf.exists():
            continue
        hw  = run_hw(cnf, args.timeout)
        ks  = run_external(cnf, KISSAT,   "s SATISFIABLE", "s UNSATISFIABLE",
                           _RE_KS,  args.timeout, base_ks)
        ms  = run_external(cnf, MINISAT2, "SATISFIABLE",   "UNSATISFIABLE",
                           _RE_CPU, args.timeout, base_ms)
        gl  = run_external(cnf, GLUCOSE,  "s SATISFIABLE", "s UNSATISFIABLE",
                           _RE_CPU, args.timeout, base_gl)
        # decide winner among finite numerics
        cand = {"HW": hw["us"], "KS": ks["us"],
                "MS": ms["us"], "GL": gl["us"]}
        win = min(cand, key=lambda k: cand[k])
        # correctness
        for tag, r in [("HW", hw), ("KS", ks), ("MS", ms), ("GL", gl)]:
            if r["got"] not in (expect, "TIMEOUT"):
                r["got"] = r["got"] + "!"
        print(f"{name:<14} {expect:<5} "
              f"{hw['us']:>12.1f} {ks['us']:>12.1f} "
              f"{ms['us']:>12.1f} {gl['us']:>12.1f}  {win}")
        rows.append({
            "instance": name, "expect": expect,
            "hw_us": round(hw["us"], 2),  "hw_got": hw["got"],
            "kissat_us": round(ks["us"], 2), "kissat_got": ks["got"],
            "minisat2_us": round(ms["us"], 2), "minisat2_got": ms["got"],
            "glucose_us": round(gl["us"], 2), "glucose_got": gl["got"],
            "winner": win,
        })

    if args.csv:
        with open(args.csv, "w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
            w.writeheader()
            w.writerows(rows)
        print(f"\nCSV written to {args.csv}")


if __name__ == "__main__":
    main()
