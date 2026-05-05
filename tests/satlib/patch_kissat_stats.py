#!/usr/bin/env python3
"""
patch_kissat_stats.py — Add kissat_decisions and kissat_propagations columns
to existing CSVs. Runs Kissat once per instance (no timing repetitions needed
since decision/propagation counts are deterministic).

Usage:
    python3 tests/satlib/patch_kissat_stats.py --kissat kissat/build/kissat results/*.csv
"""

import argparse
import csv
import re
import subprocess
import sys
import tempfile
from pathlib import Path

SATLIB_DIR = Path(__file__).resolve().parent

_RE_DECISIONS    = re.compile(r"decisions:\s+(\d+)")
_RE_PROPAGATIONS = re.compile(r"propagations:\s+(\d+)")


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


def run_kissat_stats(cnf_path: Path, kissat: Path, timeout: int) -> tuple:
    """Single Kissat run; returns (decisions, propagations) or (None, None)."""
    tmp = strip_satlib_tmp(cnf_path)
    try:
        proc = subprocess.Popen(
            [str(kissat), str(tmp)],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            universal_newlines=True)
        try:
            stdout, stderr = proc.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.communicate()
            return None, None

        combined = stdout + stderr
        md = _RE_DECISIONS.search(combined)
        mp = _RE_PROPAGATIONS.search(combined)
        decisions    = int(md.group(1)) if md else None
        propagations = int(mp.group(1)) if mp else None
        return decisions, propagations
    finally:
        if tmp.exists():
            tmp.unlink()


def patch_csv(csv_path: Path, kissat: Path, timeout: int):
    rows = list(csv.DictReader(csv_path.open()))
    if not rows:
        print(f"  SKIP (empty): {csv_path.name}")
        return

    fields = list(rows[0].keys())
    if "kissat_cpu_s" not in fields:
        print(f"  SKIP (no kissat columns): {csv_path.name}")
        return

    # Add new columns after kissat_got if not present
    for col in ("kissat_decisions", "kissat_propagations"):
        if col not in fields:
            idx = fields.index("kissat_got") + 1
            fields.insert(idx, col)
            for row in rows:
                row[col] = ""

    n = len(rows)
    print(f"  {csv_path.name}: {n} instances", flush=True)

    for i, row in enumerate(rows):
        cnf_path = SATLIB_DIR / row["set"] / row["file"]
        if not cnf_path.exists():
            print(f"    WARNING: {cnf_path} not found, skipping")
            continue
        decisions, propagations = run_kissat_stats(cnf_path, kissat, timeout)
        row["kissat_decisions"]    = decisions    if decisions    is not None else ""
        row["kissat_propagations"] = propagations if propagations is not None else ""
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

    for csv_path in [Path(c) for c in args.csvs]:
        if not csv_path.exists():
            print(f"SKIP (not found): {csv_path}")
            continue
        patch_csv(csv_path, kissat, args.timeout)


if __name__ == "__main__":
    main()
