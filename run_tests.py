#!/usr/bin/env python3
"""
run_tests.py — Batch runner: generate → simulate → parse for many CNF problems.

Usage:
    # Generate 10 problems and run them all:
    python3 run_tests.py --gen --vars 20 --ratio 4.0 --count 10

    # Run all .cnf files in a directory (already generated):
    python3 run_tests.py problems/*.cnf

    # Full sweep: easy + hard + heavy:
    python3 run_tests.py --gen --vars 20 --ratio 2.0 --count 5 --prefix easy
    python3 run_tests.py --gen --vars 20 --ratio 4.27 --count 5 --prefix hard
    python3 run_tests.py problems/*.cnf

Output:
    output/<name>/sim.out     — raw VCS log
    output/<name>/results.txt — parsed human-readable report
    output/summary.txt        — cross-problem BCP cycle stats

Notes:
    - Recompiles only when RTL sources change (make dependency tracking).
    - Each problem gets its own testbench (tb/<name>_tb.sv) and output dir.
    - BCP cycle stats are aggregated across all problems in summary.txt.
"""

import argparse
import subprocess
import sys
import os
import glob
import re
from pathlib import Path


ROOT = Path(__file__).parent.resolve()


def run_cmd(cmd, cwd=None, capture=False):
    """Run a shell command.  Returns (returncode, stdout+stderr)."""
    result = subprocess.run(
        cmd, shell=True, cwd=str(cwd or ROOT),
        stdout=subprocess.PIPE if capture else None,
        stderr=subprocess.STDOUT if capture else None,
    )
    out = result.stdout.decode() if capture and result.stdout else ""
    return result.returncode, out


def gen_problems(vars_, ratio, count, prefix, seed, out_dir):
    """Generate CNF problems via gen_cnf.py."""
    cmd = (
        "python3 gen_cnf.py"
        " --vars {vars} --ratio {ratio} --count {count}"
        " --prefix {prefix} --seed {seed} -o {out_dir}"
    ).format(vars=vars_, ratio=ratio, count=count,
             prefix=prefix, seed=seed, out_dir=out_dir)
    rc, _ = run_cmd(cmd)
    if rc != 0:
        print("ERROR: gen_cnf.py failed", file=sys.stderr)
        sys.exit(1)


def run_one(cnf_path, vid_width, max_decisions, recompile):
    """Generate TB, compile (if needed), simulate, and parse one CNF problem.

    Returns a dict with keys: name, cnf, rc, log_path, results_path, stats.
    """
    cnf_path = Path(cnf_path).resolve()
    name     = cnf_path.stem

    tb_path     = ROOT / "tb" / "{}_tb.sv".format(name)
    out_dir     = ROOT / "output" / name
    sim_log     = out_dir / "sim.out"
    results_txt = out_dir / "results.txt"
    simv_path   = ROOT / "simv_{}".format(name)

    out_dir.mkdir(parents=True, exist_ok=True)

    # ── Step 1: Generate testbench ───────────────────────────────────────
    print("  [{}] Generating testbench...".format(name))
    cmd = "python3 gen_tb.py {} -o {} --vid-width {} --max-decisions {}".format(
        cnf_path, tb_path, vid_width, max_decisions)
    rc, out = run_cmd(cmd, capture=True)
    if rc != 0:
        print("  [{}] ERROR: gen_tb.py failed\n{}".format(name, out), file=sys.stderr)
        return dict(name=name, rc=rc, log_path=None, results_path=None, stats=None)

    # ── Step 2: Compile ──────────────────────────────────────────────────
    if recompile or not simv_path.exists():
        print("  [{}] Compiling...".format(name))
        srcs = " ".join([
            "src/cam_cell.sv", "src/sram_row.sv", "src/processing_logic.sv",
            "src/cam_sram_row.sv", "src/cam_sram_array.sv",
            "src/combining_logic.sv", "src/sat_submodule.sv",
        ])
        vcs_opts = (
            "-full64 -sverilog +v2k +incdir+src +vcs+initreg+random"
            " -timescale=1ns/1ps -debug_access+all +lint=all,noVCDE,noONGS,noUI +warn=noTMR"
        )
        cmd = (
            "source eecs151source.bashrc && "
            "vcs {opts} -top sat_submodule_tb {srcs} {tb} -o {simv}"
        ).format(opts=vcs_opts, srcs=srcs, tb=tb_path, simv=simv_path)
        rc, out = run_cmd(cmd, capture=True)
        (out_dir / "compile.log").write_text(out)
        if rc != 0:
            print("  [{}] ERROR: VCS compile failed (see {})".format(
                name, out_dir / "compile.log"), file=sys.stderr)
            return dict(name=name, rc=rc, log_path=None, results_path=None, stats=None)

    # ── Step 3: Simulate ─────────────────────────────────────────────────
    print("  [{}] Simulating...".format(name))
    cmd = "source eecs151source.bashrc && {simv} 2>&1 | tee {log}".format(
        simv=simv_path, log=sim_log)
    rc, _ = run_cmd(cmd)
    # Move waves.vpd if produced
    waves = ROOT / "waves.vpd"
    if waves.exists():
        waves.rename(out_dir / "waves.vpd")

    # ── Step 4: Parse results ────────────────────────────────────────────
    print("  [{}] Parsing results...".format(name))
    cmd = "python3 parse_results.py --out {} {}".format(results_txt, sim_log)
    run_cmd(cmd)

    # ── Step 5: Extract BCP stats ────────────────────────────────────────
    stats = _extract_stats(sim_log)
    stats['name'] = name
    stats['cnf']  = str(cnf_path)

    return dict(name=name, rc=rc, log_path=str(sim_log),
                results_path=str(results_txt), stats=stats)


def _extract_stats(log_path):
    """Parse BCP_STATS line from simulation log."""
    stats = dict(bcp_calls=0, bcp_cycles=0, undo_calls=0, undo_cycles=0,
                 passed=0, failed=0, final='UNKNOWN')
    try:
        text = Path(log_path).read_text()
    except Exception:
        return stats

    m = re.search(
        r'BCP_STATS: bcp_calls=(\d+) bcp_cycles=(\d+)'
        r' undo_calls=(\d+) undo_cycles=(\d+)', text)
    if m:
        stats['bcp_calls']   = int(m.group(1))
        stats['bcp_cycles']  = int(m.group(2))
        stats['undo_calls']  = int(m.group(3))
        stats['undo_cycles'] = int(m.group(4))

    if 'ALL TESTS PASSED' in text:
        stats['final'] = 'PASS'
    elif 'FINISHED WITH' in text:
        stats['final'] = 'FAIL'

    stats['passed'] = len(re.findall(r'^PASS ', text, re.MULTILINE))
    stats['failed'] = len(re.findall(r'^FAIL ', text, re.MULTILINE))

    return stats


def write_summary(results, summary_path):
    """Write a cross-problem summary with cycle stats."""
    lines = []
    lines.append("=" * 72)
    lines.append("  BATCH SUMMARY")
    lines.append("=" * 72)
    lines.append("")

    # Header
    lines.append("  {:<25s}  {:>6s}  {:>8s}  {:>10s}  {:>9s}  {:>6s}".format(
        "Problem", "Result", "BCP_calls", "BCP_cycles", "cyc/call", "Checks"))
    lines.append("  " + "-" * 68)

    total_bcp_calls   = 0
    total_bcp_cycles  = 0
    total_undo_cycles = 0
    n_pass = 0
    n_fail = 0

    for r in results:
        s = r.get('stats') or {}
        name     = r['name']
        final    = s.get('final', 'ERR')
        calls    = s.get('bcp_calls', 0)
        cycles   = s.get('bcp_cycles', 0)
        cpc      = (cycles / calls) if calls > 0 else 0
        checks   = s.get('passed', 0) + s.get('failed', 0)

        total_bcp_calls   += calls
        total_bcp_cycles  += cycles
        total_undo_cycles += s.get('undo_cycles', 0)
        if final == 'PASS':
            n_pass += 1
        else:
            n_fail += 1

        lines.append("  {:<25s}  {:>6s}  {:>8d}  {:>10d}  {:>8.1f}x  {:>6d}".format(
            name[:25], final, calls, cycles, cpc, checks))

    lines.append("")
    lines.append("  " + "-" * 68)
    avg_cpc = (total_bcp_cycles / total_bcp_calls) if total_bcp_calls > 0 else 0
    lines.append("  {:<25s}  {:>6s}  {:>8d}  {:>10d}  {:>8.1f}x".format(
        "TOTAL", "{}/{}".format(n_pass, n_pass + n_fail),
        total_bcp_calls, total_bcp_cycles, avg_cpc))
    lines.append("")
    lines.append("  BCP cycles / call (avg) : {:.2f}".format(avg_cpc))
    lines.append("  UNDO cycles (total)     : {}".format(total_undo_cycles))
    lines.append("")

    text = "\n".join(lines) + "\n"
    Path(summary_path).write_text(text)
    print(text)


def main():
    parser = argparse.ArgumentParser(
        description="Batch runner for sat_submodule CNF tests.")

    parser.add_argument("cnf_files", nargs="*",
                        help="CNF files to test (supports globs)")
    parser.add_argument("--gen", action="store_true",
                        help="Generate problems first with gen_cnf.py")
    parser.add_argument("--vars",    type=int,   default=20)
    parser.add_argument("--ratio",   type=float, default=4.27)
    parser.add_argument("--count",   type=int,   default=5)
    parser.add_argument("--prefix",  default="problem")
    parser.add_argument("--seed",    type=int,   default=42)
    parser.add_argument("--prob-dir", default="problems",
                        help="Directory for generated problems (default: problems/)")
    parser.add_argument("--vid-width",      type=int, default=20)
    parser.add_argument("--max-decisions",  type=int, default=100000)
    parser.add_argument("--recompile", action="store_true",
                        help="Force recompile even if simv exists")
    parser.add_argument("--summary", default="output/summary.txt",
                        help="Path for cross-problem summary (default: output/summary.txt)")

    args = parser.parse_args()

    # ── Collect CNF files ────────────────────────────────────────────────
    cnf_files = list(args.cnf_files)

    if args.gen:
        gen_problems(args.vars, args.ratio, args.count,
                     args.prefix, args.seed, args.prob_dir)
        # Add generated files
        pat = str(ROOT / args.prob_dir / "{}_*.cnf".format(args.prefix))
        cnf_files += sorted(glob.glob(pat))

    # Expand globs (in case shell didn't)
    expanded = []
    for f in cnf_files:
        g = sorted(glob.glob(f))
        expanded += g if g else [f]
    cnf_files = expanded

    if not cnf_files:
        print("No CNF files specified. Use --gen or provide file paths.", file=sys.stderr)
        sys.exit(1)

    # ── Run each problem ─────────────────────────────────────────────────
    print("Running {} problem(s)...\n".format(len(cnf_files)))
    results = []
    for cnf in cnf_files:
        if not Path(cnf).exists():
            print("  WARNING: {} not found, skipping.".format(cnf), file=sys.stderr)
            continue
        r = run_one(cnf, args.vid_width, args.max_decisions, args.recompile)
        results.append(r)
        print()

    # ── Write summary ────────────────────────────────────────────────────
    summary_path = ROOT / args.summary
    summary_path.parent.mkdir(parents=True, exist_ok=True)
    write_summary(results, str(summary_path))
    print("Summary written to {}".format(summary_path))


if __name__ == "__main__":
    main()
