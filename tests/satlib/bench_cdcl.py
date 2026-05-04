#!/usr/bin/env python3
"""
bench_cdcl.py — Run all SATLIB .cnf benchmarks through cdcl_solver and log metrics.

Correctness rule:
  directory name starts with "uu" or "UUF" (case-insensitive) → expect UNSATISFIABLE
  otherwise → expect SATISFIABLE

Usage:
    python3 tests/satlib/bench_cdcl.py [--solver PATH] [--timeout S] [--csv FILE] [--sets GLOB]

Examples:
    python3 tests/satlib/bench_cdcl.py
    python3 tests/satlib/bench_cdcl.py --sets "uf20*,uuf50*"
    python3 tests/satlib/bench_cdcl.py --csv results.csv --timeout 10
"""

import argparse
import csv
import os
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT       = Path(__file__).resolve().parents[2]
SATLIB_DIR = Path(__file__).resolve().parent
SOLVER     = ROOT / "coprocessing" / "cdcl_solver"

# Metrics parsed from cdcl_solver stderr lines:
#   c originals=N learnts=N decisions=N conflicts=N propagations=N hw_bcp=N hw_undo=N hw_load=N
#   c hw_bcp_cycles=N (sim_cycles=N/3) @1GHz bcp_time=Nns (Nus)
#   c sw_cdcl_time=Nns (Nus)
_RE_STATS   = re.compile(
    r"originals=(\d+).*learnts=(\d+).*decisions=(\d+).*conflicts=(\d+)"
    r".*propagations=(\d+).*hw_bcp=(\d+).*hw_undo=(\d+).*hw_load=(\d+)")
_RE_HW_TIME = re.compile(r"hw_bcp_cycles=(\d+).*bcp_time=([\d.]+)ns")
_RE_SW_TIME = re.compile(r"sw_cdcl_time=([\d.]+)ns")


def expected_result(cnf_path: Path) -> str:
    """SAT if the containing directory name doesn't start with uu/UUF, else UNSAT."""
    dirname = cnf_path.parent.name.lower()
    return "UNSAT" if dirname.startswith("uu") else "SAT"


def run_one(cnf_path: Path, solver: Path, timeout: int) -> dict:
    result = {
        "file":         cnf_path.name,
        "set":          cnf_path.parent.name,
        "expect":       expected_result(cnf_path),
        "got":          None,
        "correct":      None,
        "wall_s":       None,
        "originals":    None,
        "learnts":      None,
        "decisions":    None,
        "conflicts":    None,
        "propagations": None,
        "hw_bcp":       None,
        "hw_bcp_cycles":None,
        "bcp_time_ns":  None,
        "sw_time_ns":   None,
        "status":       "?",
        "hw_overflow":  False,
    }

    t0 = time.monotonic()
    stdout = ""
    stderr = ""
    try:
        proc = subprocess.Popen(
            [str(solver), str(cnf_path)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True)
        stdout, stderr = proc.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        proc.kill()
        tail_out, tail_err = proc.communicate()
        stdout = (stdout or "") + (tail_out or "")
        stderr = (stderr or "") + (tail_err or "")
        result["status"] = "TIMEOUT"
        result["wall_s"] = round(time.monotonic() - t0, 4)
        result["hw_overflow"] = "overflows HW" in stderr
        return result
    result["wall_s"] = round(time.monotonic() - t0, 4)
    result["hw_overflow"] = "overflows HW" in stderr

    if "s SATISFIABLE" in stdout:
        result["got"] = "SAT"
    elif "s UNSATISFIABLE" in stdout:
        result["got"] = "UNSAT"
    elif proc.returncode != 0:
        result["status"] = "ERROR"
        result["got"]    = stderr.strip().splitlines()[-1] if stderr.strip() else "?"
        return result

    result["correct"] = (result["got"] == result["expect"])
    result["status"]  = "PASS" if result["correct"] else "WRONG"

    m = _RE_STATS.search(stderr)
    if m:
        result["originals"]    = int(m.group(1))
        result["learnts"]      = int(m.group(2))
        result["decisions"]    = int(m.group(3))
        result["conflicts"]    = int(m.group(4))
        result["propagations"] = int(m.group(5))
        result["hw_bcp"]       = int(m.group(6))

    m = _RE_HW_TIME.search(stderr)
    if m:
        result["hw_bcp_cycles"] = int(m.group(1))
        result["bcp_time_ns"]   = float(m.group(2))

    m = _RE_SW_TIME.search(stderr)
    if m:
        result["sw_time_ns"] = float(m.group(1))

    return result


def collect_cnfs(satlib_dir: Path, set_globs: list) -> list:
    cnfs = []
    if set_globs:
        for pattern in set_globs:
            for d in sorted(satlib_dir.glob(pattern)):
                if d.is_dir():
                    cnfs.extend(sorted(d.glob("*.cnf")))
    else:
        for d in sorted(satlib_dir.iterdir()):
            if d.is_dir():
                cnfs.extend(sorted(d.glob("*.cnf")))
    return cnfs


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--solver",  default=str(SOLVER),
                   help="path to cdcl_solver binary")
    p.add_argument("--timeout", type=int, default=30,
                   help="per-instance timeout in seconds (default: 30)")
    p.add_argument("--csv",     default=None,
                   help="write results to this CSV file")
    p.add_argument("--sets",    default=None,
                   help="comma-separated glob patterns to select benchmark sets "
                        "(e.g. 'uf20*,uuf50*'); default: all")
    args = p.parse_args()

    solver = Path(args.solver)
    if not solver.exists():
        print(f"ERROR: solver not found at {solver}", file=sys.stderr)
        sys.exit(1)

    set_globs = [s.strip() for s in args.sets.split(",")] if args.sets else []
    cnfs = collect_cnfs(SATLIB_DIR, set_globs)
    if not cnfs:
        print("No .cnf files found.", file=sys.stderr)
        sys.exit(1)

    print(f"Found {len(cnfs)} instances  solver={solver}  timeout={args.timeout}s")
    print(f"{'SET':<24} {'FILE':<26} {'EXP':<6} {'GOT':<6} {'OK':<5} "
          f"{'WALL':>7} {'HW_BCP':>7} {'BCP_ns':>9} {'SW_ns':>12}")
    print("-" * 110)

    all_results = []
    n_pass = n_wrong = n_error = n_timeout = 0

    for cnf in cnfs:
        r = run_one(cnf, solver, args.timeout)
        all_results.append(r)

        ok_str = ""
        if   r["status"] == "PASS":    ok_str = "✓"; n_pass += 1
        elif r["status"] == "WRONG":   ok_str = "✗"; n_wrong += 1
        elif r["status"] == "TIMEOUT": ok_str = "T"; n_timeout += 1
        else:                          ok_str = "E"; n_error += 1
        if r["hw_overflow"]:
            ok_str += "+OVF"

        print(f"{r['set']:<24} {r['file']:<26} {r['expect']:<6} "
              f"{str(r['got'] or '?'):<6} {ok_str:<5} "
              f"{str(r['wall_s'] or '')+'s':>7} "
              f"{str(r['hw_bcp'] or ''):>7} "
              f"{str(r['bcp_time_ns'] or ''):>9} "
              f"{str(r['sw_time_ns'] or ''):>12}")

    total = len(all_results)
    print("-" * 110)
    print(f"PASS={n_pass}/{total}  WRONG={n_wrong}  ERROR={n_error}  TIMEOUT={n_timeout}")

    if args.csv:
        fields = list(all_results[0].keys())
        with open(args.csv, "w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=fields)
            w.writeheader()
            w.writerows(all_results)
        print(f"Results written to {args.csv}")


if __name__ == "__main__":
    main()
