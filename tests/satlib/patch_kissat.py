#!/usr/bin/env python3
"""
patch_kissat.py — Run Kissat on every instance in an existing CSV and fill in
kissat_cpu_s / kissat_got columns without rerunning any other solver.

Usage:
    python3 tests/satlib/patch_kissat.py --kissat kissat/build/kissat results/*.csv
"""

import argparse
import csv
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT       = Path(__file__).resolve().parents[2]
SATLIB_DIR = Path(__file__).resolve().parent

_RE_KS_TIME = re.compile(r"process-time:\s+([\d.]+)")


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


def run_kissat(cnf_path: Path, kissat: Path, timeout: int) -> tuple:
    tmp = strip_satlib_tmp(cnf_path)
    cpu_s, got = None, None
    try:
        proc = subprocess.Popen(
            [str(kissat), str(tmp)],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            universal_newlines=True)
        stdout, stderr = proc.communicate(timeout=timeout)
        combined = stdout + stderr
        if "s SATISFIABLE" in combined:
            got = "SAT"
        elif "s UNSATISFIABLE" in combined:
            got = "UNSAT"
        m = _RE_KS_TIME.search(combined)
        if m:
            cpu_s = float(m.group(1))
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.communicate()
        cpu_s = float(timeout)
    finally:
        if tmp.exists():
            tmp.unlink()
    return cpu_s, got


def patch_csv(csv_path: Path, kissat: Path, timeout: int):
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
        cpu_s, got = run_kissat(cnf_path, kissat, timeout)
        row["kissat_cpu_s"] = cpu_s if cpu_s is not None else ""
        row["kissat_got"]   = got   if got   is not None else ""
        if (i + 1) % 100 == 0:
            print(f"    {i+1}/{n} done", flush=True)

    with csv_path.open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        w.writerows(rows)
    print(f"    done, written to {csv_path.name}")


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

    for csv_path in [Path(c) for c in args.csvs]:
        if not csv_path.exists():
            print(f"SKIP (not found): {csv_path}")
            continue
        patch_csv(csv_path, kissat, args.timeout)


if __name__ == "__main__":
    main()
