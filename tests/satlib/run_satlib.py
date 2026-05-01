#!/usr/bin/env python3
"""
run_satlib.py — Run a sample of SATLIB Uniform Random-3-SAT benchmarks
against the RTL via gen_tb.py + VCS.

Usage:
    python3 tests/satlib/run_satlib.py uf20-91 -n 5
    python3 tests/satlib/run_satlib.py uf50-218 -n 3 --seed 1
    python3 tests/satlib/run_satlib.py uuf50-218 -n 3 --max-decisions 5000

For each sampled .cnf the script:
    1. Regenerates tb/sat_submodule_tb.sv with gen_tb.py (which runs the
       Python DPLL model to produce the expected per-step trace).
    2. Builds + runs the testbench via `make -C tb sat`.
    3. Parses the simulation log for "ALL TESTS PASSED".

PASS means the RTL trace matches the Python DPLL trace step-for-step on this
instance — so the sub-SAT geometry, BCP timing, and combining priority all
agree with the reference model.  It does *not* verify that DPLL itself
reached SAT/UNSAT (UNSAT instances at the phase transition can exhaust the
DPLL decision budget; lower `--max-decisions` to keep generation fast).
"""

import argparse
import os
import random
import subprocess
import sys
import time
from pathlib import Path

ROOT       = Path(__file__).resolve().parents[2]
SATLIB_DIR = ROOT / "tests" / "satlib"
TB_DIR     = ROOT / "tb"
TB_FILE    = TB_DIR / "sat_submodule_tb.sv"
GEN_TB     = ROOT / "gen_tb.py"


def run_one(cnf_path: Path, expect_sat: bool,
            max_decisions: int, timeout_s: int) -> dict:
    out = {
        "cnf": cnf_path.name,
        "expect": "SAT" if expect_sat else "UNSAT",
        "status": "?",
        "t_s": 0.0,
    }

    # 1. Regenerate the TB from this CNF.
    g = subprocess.run(
        ["python3", str(GEN_TB), str(cnf_path),
         "-o", str(TB_FILE),
         "--max-decisions", str(max_decisions)],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        universal_newlines=True, timeout=timeout_s)
    if g.returncode != 0:
        out["status"] = "GEN_FAIL"
        return out

    # 2. Build + run via the existing tb/Makefile target.
    t0 = time.time()
    try:
        r = subprocess.run(
            ["make", "-s", "-C", str(TB_DIR), "sat"],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        universal_newlines=True, timeout=timeout_s)
    except subprocess.TimeoutExpired:
        out["status"] = "TIMEOUT"
        out["t_s"]    = float(timeout_s)
        return out
    out["t_s"] = round(time.time() - t0, 2)

    log = r.stdout + r.stderr
    if "ALL TESTS PASSED" in log:
        out["status"] = "PASS"
    elif "FAIL [" in log or r.returncode != 0:
        out["status"] = "FAIL"
        out["log_tail"] = "\n".join(log.splitlines()[-20:])
    else:
        out["status"] = "UNKNOWN"
        out["log_tail"] = "\n".join(log.splitlines()[-20:])
    return out


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("set", help="benchmark set name (e.g. uf20-91, uf50-218, uuf50-218)")
    p.add_argument("-n", type=int, default=5,
                   help="number of instances to sample (default: 5)")
    p.add_argument("--seed", type=int, default=0,
                   help="random seed (default: 0; 0 means deterministic order)")
    p.add_argument("--timeout", type=int, default=180,
                   help="per-instance timeout in seconds (default: 180)")
    p.add_argument("--max-decisions", type=int, default=20000,
                   help="DPLL decision budget passed to gen_tb.py (default: 20000)")
    args = p.parse_args()

    set_dir = SATLIB_DIR / args.set
    cnfs = sorted(set_dir.glob("*.cnf"))
    if not cnfs:
        print(f"No .cnf files in {set_dir}", file=sys.stderr)
        sys.exit(1)

    if args.seed != 0:
        random.seed(args.seed)
        sample = random.sample(cnfs, min(args.n, len(cnfs)))
    else:
        sample = cnfs[:args.n]

    expect_sat = not args.set.startswith("uuf")

    print(f"Running {len(sample)} instance(s) from {args.set} "
          f"(expect={'SAT' if expect_sat else 'UNSAT'}, "
          f"max_decisions={args.max_decisions}, timeout={args.timeout}s)")
    print()

    results = []
    for cnf in sample:
        res = run_one(cnf, expect_sat, args.max_decisions, args.timeout)
        marker = "✓" if res["status"] == "PASS" else "✗"
        print(f"  {marker} {res['cnf']:28s}  status={res['status']:8s}  "
              f"t={res['t_s']:.2f}s")
        if "log_tail" in res:
            for ln in res["log_tail"].splitlines():
                print(f"      | {ln}")
        results.append(res)

    n_pass = sum(1 for r in results if r["status"] == "PASS")
    print()
    print(f"{n_pass}/{len(results)} PASS")
    sys.exit(0 if n_pass == len(results) else 1)


if __name__ == "__main__":
    main()
