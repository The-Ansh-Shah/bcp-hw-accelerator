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
_RE_HW_TIME = re.compile(r"hw_cycles=(\d+)\s+hw_time=([\d.eE+\-]+)ns\s+sw_time=([\d.eE+\-]+)ns")
_RE_KS_TIME = re.compile(r"process-time:\s+([\d.]+)")
_RE_CPU_TIME = re.compile(r"CPU time\s*:\s*([\d.]+)")  # MiniSat and Glucose


def strip_satlib_tmp(cnf_path: Path) -> Path:
    """Write a SATLIB CNF without the trailing %/0 lines to a temp file."""
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


def run_external(cnf_path: Path, binary: Path, timeout: int,
                 sat_str: str, unsat_str: str, time_re) -> dict:
    """Generic runner for external SAT solvers. Returns cpu_s and got."""
    result = {"cpu_s": None, "got": None}
    tmp = strip_satlib_tmp(cnf_path)
    try:
        proc = subprocess.Popen(
            [str(binary), str(tmp)],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            universal_newlines=True)
        stdout, stderr = proc.communicate(timeout=timeout)
        combined = stdout + stderr
        if sat_str in combined:
            result["got"] = "SAT"
        elif unsat_str in combined:
            result["got"] = "UNSAT"
        m = time_re.search(combined)
        if m:
            result["cpu_s"] = float(m.group(1))
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.communicate()
        result["cpu_s"] = float(timeout)
    finally:
        if tmp.exists():
            tmp.unlink()
    return result


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
        "hw_cycles":    None,
        "hw_time_ns":   None,
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
        result["hw_cycles"]  = int(m.group(1))
        result["hw_time_ns"] = float(m.group(2))
        result["sw_time_ns"] = float(m.group(3))

    if result["hw_cycles"] is not None and result["propagations"]:
        result["cyc_per_prop"] = result["hw_cycles"] / result["propagations"]

    return result


def run_kissat(cnf_path: Path, binary: Path, timeout: int) -> dict:
    r = run_external(cnf_path, binary, timeout, "s SATISFIABLE", "s UNSATISFIABLE", _RE_KS_TIME)
    return {"kissat_cpu_s": r["cpu_s"], "kissat_got": r["got"]}


def run_minisat2(cnf_path: Path, binary: Path, timeout: int) -> dict:
    r = run_external(cnf_path, binary, timeout, "SATISFIABLE", "UNSATISFIABLE", _RE_CPU_TIME)
    return {"minisat2_cpu_s": r["cpu_s"], "minisat2_got": r["got"]}


def run_glucose(cnf_path: Path, binary: Path, timeout: int) -> dict:
    r = run_external(cnf_path, binary, timeout, "s SATISFIABLE", "s UNSATISFIABLE", _RE_CPU_TIME)
    return {"glucose_cpu_s": r["cpu_s"], "glucose_got": r["got"]}


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
                   help="path to kissat binary")
    p.add_argument("--minisat2", default=None,
                   help="path to minisat2 binary")
    p.add_argument("--glucose", default=None,
                   help="path to glucose binary")
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

    def _load_bin(arg, name):
        if not arg: return None
        p = Path(arg)
        if not p.exists():
            print(f"ERROR: {name} not found at {p}", file=sys.stderr)
            sys.exit(1)
        return p

    kissat   = _load_bin(args.kissat,   "kissat")
    minisat2 = _load_bin(args.minisat2, "minisat2")
    glucose  = _load_bin(args.glucose,  "glucose")
    sw_solvers = {k: v for k, v in [("kissat", kissat), ("minisat2", minisat2), ("glucose", glucose)] if v}

    set_globs = [s.strip() for s in args.sets.split(",")] if args.sets else []
    cnfs = collect_cnfs(SATLIB_DIR, set_globs)
    if not cnfs:
        print("No .cnf files found.", file=sys.stderr)
        sys.exit(1)

    sw_label = "  ".join(sw_solvers.keys())
    print(f"Found {len(cnfs)} instances  solver={solver}  timeout={args.timeout}s"
          + (f"  sw_solvers=[{sw_label}]" if sw_solvers else ""))

    if sw_solvers:
        sw_cols = "".join(f" {n.upper()+'_us':>9}" for n in sw_solvers)
        print(f"{'SET':<14} {'FILE':<22} {'OK':<5} "
              f"{'DEC':>5} {'CNFL':>5} {'LRN':>5} {'PROP':>5} {'BCP':>5} "
              f"{'UNDO':>5} {'LOAD':>5} {'CYC/P':>6} "
              f"{'HW_ns':>8} {'SW_us':>8} {'TOT_us':>9}{sw_cols} {'WIN':>8}")
        print("-" * (148 + 10 * len(sw_solvers)))
    else:
        print(f"{'SET':<24} {'FILE':<26} {'EXP':<6} {'GOT':<6} {'OK':<7} "
              f"{'WALL':>7} {'HW_BCP':>7} {'HW_ns':>9} {'SW_ns':>12}")
        print("-" * 110)

    all_results = []
    n_pass = n_wrong = n_error = n_timeout = 0
    n_hw_wins = n_sw_wins = 0

    for cnf in cnfs:
        r = run_one(cnf, solver, args.timeout)

        if kissat:
            r.update(run_kissat(cnf, kissat, args.timeout))
        else:
            r.update({"kissat_cpu_s": None, "kissat_got": None})
        if minisat2:
            r.update(run_minisat2(cnf, minisat2, args.timeout))
        else:
            r.update({"minisat2_cpu_s": None, "minisat2_got": None})
        if glucose:
            r.update(run_glucose(cnf, glucose, args.timeout))
        else:
            r.update({"glucose_cpu_s": None, "glucose_got": None})

        all_results.append(r)

        ok_str = ""
        if   r["status"] == "PASS":    ok_str = "✓";      n_pass    += 1
        elif r["status"] == "WRONG":   ok_str = "✗";      n_wrong   += 1
        elif r["status"] == "TIMEOUT": ok_str = "T+OVF" if r["hw_overflow"] else "T"; n_timeout += 1
        else:                          ok_str = "E";      n_error   += 1

        if sw_solvers:
            hw_ns    = r["hw_time_ns"]
            sw_ns    = r["sw_time_ns"]
            total_us = (hw_ns + sw_ns) / 1e3 if (hw_ns is not None and sw_ns is not None) else None

            sw_times_us = {
                "kissat":  r["kissat_cpu_s"]  * 1e6 if r["kissat_cpu_s"]  else None,
                "minisat2": r["minisat2_cpu_s"] * 1e6 if r["minisat2_cpu_s"] else None,
                "glucose": r["glucose_cpu_s"] * 1e6 if r["glucose_cpu_s"] else None,
            }
            best_sw = min((v for v in sw_times_us.values() if v is not None), default=None)

            winner = ""
            if total_us is not None and best_sw is not None:
                if total_us < best_sw:
                    winner = "HW"
                    n_hw_wins += 1
                else:
                    winner = min(sw_times_us, key=lambda k: sw_times_us[k] or float("inf"))
                    n_sw_wins += 1

            def _fmt_us(v): return f"{v:.1f}" if v is not None else "?"
            def _s(v): return str(v) if v is not None else "?"

            hw_str    = f"{hw_ns:.0f}"     if hw_ns    is not None else "?"
            sw_str    = f"{sw_ns/1e3:.1f}" if sw_ns    is not None else "?"
            total_str = f"{total_us:.1f}"  if total_us is not None else "?"
            sw_cols_str = "".join(f" {_fmt_us(sw_times_us.get(n))+'us':>9}" for n in sw_solvers)

            cp_str = f"{r['cyc_per_prop']:.2f}" if r['cyc_per_prop'] is not None else "?"
            print(f"{r['set']:<14} {r['file']:<22} {ok_str:<5} "
                  f"{_s(r['decisions']):>5} {_s(r['conflicts']):>5} "
                  f"{_s(r['learnts']):>5} {_s(r['propagations']):>5} "
                  f"{_s(r['hw_bcp']):>5} {_s(r['hw_undo']):>5} {_s(r['hw_load']):>5} "
                  f"{cp_str:>6} "
                  f"{hw_str+'ns':>8} {sw_str+'us':>8} {total_str+'us':>9}{sw_cols_str} {winner:>8}")
        else:
            print(f"{r['set']:<24} {r['file']:<26} {r['expect']:<6} "
                  f"{str(r['got'] or '?'):<6} {ok_str:<7} "
                  f"{str(r['wall_s'] or '')+'s':>7} "
                  f"{str(r['hw_bcp'] or ''):>7} "
                  f"{str(r['hw_time_ns'] or ''):>9} "
                  f"{str(r['sw_time_ns'] or ''):>12}")

    total = len(all_results)
    print("-" * (148 + 10 * len(sw_solvers) if sw_solvers else 110))
    summary = f"PASS={n_pass}/{total}  WRONG={n_wrong}  ERROR={n_error}  TIMEOUT={n_timeout}"
    if sw_solvers:
        summary += f"  |  HW wins={n_hw_wins}  SW wins={n_sw_wins}"
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
    _dist("hw_cycles",      [r['hw_cycles']      for r in all_results], "{:.0f}")
    _dist("cycles/prop",    [r['cyc_per_prop']   for r in all_results], "{:.3f}")
    if sw_solvers:
        for name in sw_solvers:
            key = f"{name}_cpu_s"
            _dist(f"{name}_cpu_us",
                  [r[key]*1e6 if r[key] else None for r in all_results], "{:.0f}")

    if sw_solvers:
        print()
        print("NOTE: HW_ns  = (load+bcp+undo) ops × 1ns (1 memory-access cycle @ 1GHz).")
        print("      SW_us  = pure CDCL logic time, Verilator simulation overhead removed.")
        print("      TOT_us = HW_ns + SW_us = projected coprocessor solve time.")
        print("      *_us   = solver self-reported CPU time.")
        print("      WIN    = HW if TOT_us < best SW solver; otherwise winning SW solver name.")
        print("      CYC/P  = hw_cycles / propagations — architecture FOM, independent of Fmax.")

    if args.csv:
        fields = list(all_results[0].keys())
        with open(args.csv, "w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=fields)
            w.writeheader()
            w.writerows(all_results)
        print(f"Results written to {args.csv}")


if __name__ == "__main__":
    main()
