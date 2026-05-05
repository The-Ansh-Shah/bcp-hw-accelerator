#!/usr/bin/env python3
"""
bench_cdcl.py — Run all SATLIB .cnf benchmarks through cdcl_solver and log metrics.

Correctness rule:
  directory name starts with "uu" or "UUF" (case-insensitive) → expect UNSATISFIABLE
  otherwise → expect SATISFIABLE

Usage:
    python3 tests/satlib/bench_cdcl.py [--solver PATH] [--kissat PATH] [--timeout S] [--csv FILE] [--sets GLOB]

Examples:
    python3 tests/satlib/bench_cdcl.py --sets "uf20*,uuf50*"
    python3 tests/satlib/bench_cdcl.py --kissat kissat/build/kissat --sets "uf20*"
    python3 tests/satlib/bench_cdcl.py --csv results.csv --timeout 10
"""

import argparse
import csv
import os
import re
import statistics
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT       = Path(__file__).resolve().parents[2]
SATLIB_DIR = Path(__file__).resolve().parent
SOLVER     = ROOT / "coprocessing" / "cdcl_solver"

_RE_STATS   = re.compile(
    r"originals=(\d+).*learnts=(\d+).*decisions=(\d+).*conflicts=(\d+)"
    r".*propagations=(\d+).*hw_bcp=(\d+).*hw_undo=(\d+).*hw_load=(\d+)")
_RE_HW_TIME = re.compile(r"hw_bcp_cycles=(\d+).*bcp_time=([\d.]+)ns")
_RE_SW_TIME = re.compile(r"sw_cdcl_time=([\d.eE+\-]+)ns")
_RE_KS_TIME = re.compile(r"process-time:\s+([\d.]+)")


def expected_result(cnf_path: Path) -> str:
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
        "hw_undo":      None,
        "hw_load":      None,
        "hw_bcp_cycles":None,
        "bcp_time_ns":  None,
        "sw_time_ns":   None,
        "cyc_per_prop": None,
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
        result["hw_undo"]      = int(m.group(7))
        result["hw_load"]      = int(m.group(8))

    m = _RE_HW_TIME.search(stderr)
    if m:
        result["hw_bcp_cycles"] = int(m.group(1))
        result["bcp_time_ns"]   = float(m.group(2))

    m = _RE_SW_TIME.search(stderr)
    if m:
        result["sw_time_ns"] = float(m.group(1))

    if result["hw_bcp_cycles"] is not None and result["propagations"]:
        result["cyc_per_prop"] = result["hw_bcp_cycles"] / result["propagations"]

    return result


def run_kissat(cnf_path: Path, kissat: Path, timeout: int) -> dict:
    """Run KISSAT on a SATLIB CNF (strips the % 0 terminator) and return timing."""
    result = {"kissat_wall_s": None, "kissat_cpu_s": None, "kissat_got": None}

    # SATLIB files end with %\n0\n which KISSAT rejects; strip both lines.
    with tempfile.NamedTemporaryFile(mode="w", suffix=".cnf", delete=False) as tf:
        tmp = Path(tf.name)
        for line in cnf_path.read_text().splitlines():
            if line.strip() in ("%", "") and line.strip() != "":
                continue
            # Skip bare "0" that follows the % terminator
            if line.strip() == "0" and "%" in cnf_path.read_text().split(line)[0][-5:]:
                continue
            tf.write(line + "\n")

    t0 = time.monotonic()
    try:
        proc = subprocess.Popen(
            [str(kissat), str(tmp)],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            universal_newlines=True)
        stdout, stderr = proc.communicate(timeout=timeout)
        result["kissat_wall_s"] = round(time.monotonic() - t0, 6)
        combined = stdout + stderr
        if "s SATISFIABLE" in combined:
            result["kissat_got"] = "SAT"
        elif "s UNSATISFIABLE" in combined:
            result["kissat_got"] = "UNSAT"
        m = _RE_KS_TIME.search(combined)
        if m:
            result["kissat_cpu_s"] = float(m.group(1))
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.communicate()
        result["kissat_wall_s"] = timeout
    finally:
        if tmp.exists(): tmp.unlink()

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
    p.add_argument("--kissat",  default=None,
                   help="path to kissat binary (enables side-by-side comparison)")
    p.add_argument("--timeout", type=int, default=30,
                   help="per-instance timeout in seconds (default: 30)")
    p.add_argument("--csv",     default=None,
                   help="write results to this CSV file")
    p.add_argument("--sets",    default=None,
                   help="comma-separated glob patterns to select benchmark sets")
    args = p.parse_args()

    solver = Path(args.solver)
    if not solver.exists():
        print(f"ERROR: solver not found at {solver}", file=sys.stderr)
        sys.exit(1)

    kissat = Path(args.kissat) if args.kissat else None
    if kissat and not kissat.exists():
        print(f"ERROR: kissat not found at {kissat}", file=sys.stderr)
        sys.exit(1)

    set_globs = [s.strip() for s in args.sets.split(",")] if args.sets else []
    cnfs = collect_cnfs(SATLIB_DIR, set_globs)
    if not cnfs:
        print("No .cnf files found.", file=sys.stderr)
        sys.exit(1)

    print(f"Found {len(cnfs)} instances  solver={solver}  timeout={args.timeout}s"
          + (f"  kissat={kissat}" if kissat else ""))

    if kissat:
        print(f"{'SET':<14} {'FILE':<22} {'OK':<5} "
              f"{'DEC':>5} {'CNFL':>5} {'LRN':>5} {'PROP':>5} {'BCP':>5} "
              f"{'UNDO':>5} {'LOAD':>5} {'CYC/P':>6} "
              f"{'HW_ns':>8} {'HW+SW_us':>9} {'KS_us':>8} {'WIN':>6}")
        print("-" * 140)
    else:
        print(f"{'SET':<24} {'FILE':<26} {'EXP':<6} {'GOT':<6} {'OK':<7} "
              f"{'WALL':>7} {'HW_BCP':>7} {'BCP_ns':>9} {'SW_ns':>12}")
        print("-" * 110)

    all_results = []
    n_pass = n_wrong = n_error = n_timeout = 0
    n_hw_wins = n_ks_wins = 0

    for cnf in cnfs:
        r = run_one(cnf, solver, args.timeout)

        if kissat:
            kr = run_kissat(cnf, kissat, args.timeout)
            r.update(kr)
        else:
            r.update({"kissat_wall_s": None, "kissat_cpu_s": None, "kissat_got": None})

        all_results.append(r)

        ok_str = ""
        if   r["status"] == "PASS":    ok_str = "✓";      n_pass    += 1
        elif r["status"] == "WRONG":   ok_str = "✗";      n_wrong   += 1
        elif r["status"] == "TIMEOUT": ok_str = "T+OVF" if r["hw_overflow"] else "T"; n_timeout += 1
        else:                          ok_str = "E";      n_error   += 1

        if kissat:
            # Projected coprocessor time = hw_bcp_ns + pure SW overhead.
            # sw_time_ns is wall-clock and dominated by Verilated simulation,
            # so we report hw_bcp_ns (hardware model) and sw_time_ns separately.
            bcp_ns  = r["bcp_time_ns"]
            sw_ns   = r["sw_time_ns"]
            ks_wall = r["kissat_wall_s"]

            # hw+sw_us: projected hardware BCP time + software CDCL overhead.
            # Note: sw_time_ns includes simulation overhead so this overstates
            # the software portion; bcp_time_ns is the honest hardware estimate.
            hw_sw_us = (bcp_ns + sw_ns) / 1e3 if (bcp_ns is not None and sw_ns is not None) else None
            ks_us    = ks_wall * 1e6 if ks_wall is not None else None

            # Winner based on projected hw BCP time vs KISSAT wall time
            winner = ""
            if bcp_ns and ks_us:
                hw_us = bcp_ns / 1e3
                if hw_us < ks_us:
                    winner = "HW-BCP"
                    n_hw_wins += 1
                else:
                    winner = "kissat"
                    n_ks_wins += 1

            bcp_str   = f"{bcp_ns:.0f}"   if bcp_ns  else "?"
            hwsw_str  = f"{hw_sw_us:.1f}" if hw_sw_us else "?"
            ks_str    = f"{ks_us:.1f}"    if ks_us is not None else "?"
            cp_str    = f"{r['cyc_per_prop']:.2f}" if r['cyc_per_prop'] is not None else "?"

            def _s(v): return str(v) if v is not None else "?"
            print(f"{r['set']:<14} {r['file']:<22} {ok_str:<5} "
                  f"{_s(r['decisions']):>5} {_s(r['conflicts']):>5} "
                  f"{_s(r['learnts']):>5} {_s(r['propagations']):>5} "
                  f"{_s(r['hw_bcp']):>5} {_s(r['hw_undo']):>5} {_s(r['hw_load']):>5} "
                  f"{cp_str:>6} "
                  f"{bcp_str+'ns':>8} {hwsw_str+'us':>9} {ks_str+'us':>8} {winner:>6}")
        else:
            print(f"{r['set']:<24} {r['file']:<26} {r['expect']:<6} "
                  f"{str(r['got'] or '?'):<6} {ok_str:<7} "
                  f"{str(r['wall_s'] or '')+'s':>7} "
                  f"{str(r['hw_bcp'] or ''):>7} "
                  f"{str(r['bcp_time_ns'] or ''):>9} "
                  f"{str(r['sw_time_ns'] or ''):>12}")

    total = len(all_results)
    print("-" * (95 if kissat else 110))
    summary = f"PASS={n_pass}/{total}  WRONG={n_wrong}  ERROR={n_error}  TIMEOUT={n_timeout}"
    if kissat:
        summary += f"  |  HW-BCP wins={n_hw_wins}  kissat wins={n_ks_wins}"
    print(summary)

    def _dist(name, vals, fmt="{:.2f}"):
        vals = [v for v in vals if v is not None]
        if not vals:
            print(f"  {name:<22} (no data)")
            return
        vals_s = sorted(vals)
        n = len(vals_s)
        p50  = statistics.median(vals_s)
        p95  = vals_s[max(0, int(0.95 * (n - 1)))]
        mean = statistics.mean(vals_s)
        print(f"  {name:<22} n={n:<4} min={fmt.format(vals_s[0]):>8} "
              f"p50={fmt.format(p50):>8} mean={fmt.format(mean):>8} "
              f"p95={fmt.format(p95):>8} max={fmt.format(vals_s[-1]):>8}")

    print()
    print("Distribution across instances:")
    _dist("decisions",      [r['decisions']      for r in all_results], "{:.0f}")
    _dist("conflicts",      [r['conflicts']      for r in all_results], "{:.0f}")
    _dist("learnts",        [r['learnts']        for r in all_results], "{:.0f}")
    _dist("propagations",   [r['propagations']   for r in all_results], "{:.0f}")
    _dist("hw_bcp ops",     [r['hw_bcp']         for r in all_results], "{:.0f}")
    _dist("hw_undo ops",    [r['hw_undo']        for r in all_results], "{:.0f}")
    _dist("hw_load ops",    [r['hw_load']        for r in all_results], "{:.0f}")
    _dist("hw_bcp_cycles",  [r['hw_bcp_cycles']  for r in all_results], "{:.0f}")
    _dist("cycles/prop",    [r['cyc_per_prop']   for r in all_results], "{:.3f}")
    if kissat:
        _dist("kissat_wall_us", [r['kissat_wall_s']*1e6 if r['kissat_wall_s'] else None
                                 for r in all_results], "{:.0f}")

    if kissat:
        print()
        print("NOTE: HW_ns = projected hardware BCP time at 1GHz (hw_bcp_cycles × 1ns).")
        print("      HW+SW_us adds sw_cdcl_time which is dominated by Verilated simulation")
        print("      overhead and overstates true coprocessor SW cost.")
        print("      CYC/P (cycles per propagation) is the architecture-level FOM and")
        print("      does NOT depend on Fmax — use this for HW-vs-HW comparisons.")

    if args.csv:
        fields = list(all_results[0].keys())
        with open(args.csv, "w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=fields)
            w.writeheader()
            w.writerows(all_results)
        print(f"Results written to {args.csv}")


if __name__ == "__main__":
    main()
