#!/usr/bin/env python3
"""
parse_results.py — Parse VCS simulation log(s) and print a human-readable summary.

Usage:
    # Single log:
    python3 parse_results.py output/sim.out

    # Multiple logs:
    python3 parse_results.py output/*.log

    # Write to file (no ANSI colours):
    python3 parse_results.py --out output/results.txt output/sim.out

    # From stdin:
    make run 2>&1 | python3 parse_results.py -

Output:
    Per-test PASS/FAIL table, then an overall summary.
"""

import sys
import re
import argparse
from pathlib import Path


# ── ANSI colours (disabled when writing to file or non-tty) ───────────────
USE_COLOR = sys.stdout.isatty()

def green(s):  return f"\033[32m{s}\033[0m" if USE_COLOR else s
def red(s):    return f"\033[31m{s}\033[0m" if USE_COLOR else s
def yellow(s): return f"\033[33m{s}\033[0m" if USE_COLOR else s
def bold(s):   return f"\033[1m{s}\033[0m"  if USE_COLOR else s
def dim(s):    return f"\033[2m{s}\033[0m"  if USE_COLOR else s

def strip_ansi(s):
    return re.sub(r'\033\[[0-9;]*m', '', s)


# ── Regex patterns matching testbench $display output ─────────────────────
RE_PASS    = re.compile(r'PASS \[(.+?)\]: conf=(\d) up=(\d) done=(\d)')
RE_FAIL    = re.compile(r'FAIL \[(.+?)\]: conf=(\d) up=(\d) done=(\d)\s+expected conf=(\d) up=(\d) done=(\d)')
RE_FINAL   = re.compile(r'(ALL TESTS PASSED|FINISHED WITH (\d+) ERROR\(S\))')
RE_STATS   = re.compile(
    r'BCP_STATS: bcp_calls=(\d+) bcp_cycles=(\d+)'
    r' undo_calls=(\d+) undo_cycles=(\d+)')


def decode_status(conf, up, done):
    """Convert signal values to a readable clause status string."""
    c, u, d = int(conf), int(up), int(done)
    if c:   return red("CONFLICT")
    if u:   return yellow("UNIT-PROP")
    if d:   return green("SAT")
    return dim("OPEN")


def parse_log(text: str, label: str) -> dict:
    """
    Parse one simulation log.
    Returns a dict with:
        label, tests (list of dicts), errors (int), passed (int), final_ok (bool)
    """
    tests   = []
    errors   = 0
    passed   = 0
    final_ok = None
    bcp_stats = None

    for line in text.splitlines():
        line = line.strip()

        m = RE_PASS.search(line)
        if m:
            name, conf, up, done = m.group(1), m.group(2), m.group(3), m.group(4)
            tests.append(dict(
                name=name, result="PASS",
                conf=conf, up=up, done=done,
                status=decode_status(conf, up, done),
            ))
            passed += 1
            continue

        m = RE_FAIL.search(line)
        if m:
            name = m.group(1)
            conf, up, done = m.group(2), m.group(3), m.group(4)
            exp_conf, exp_up, exp_done = m.group(5), m.group(6), m.group(7)
            tests.append(dict(
                name=name, result="FAIL",
                conf=conf, up=up, done=done,
                exp_conf=exp_conf, exp_up=exp_up, exp_done=exp_done,
                status=decode_status(conf, up, done),
            ))
            errors += 1
            continue

        m = RE_FINAL.search(line)
        if m:
            final_ok = (m.group(2) is None)
            continue

        m = RE_STATS.search(line)
        if m:
            bcp_stats = dict(
                bcp_calls=int(m.group(1)),   bcp_cycles=int(m.group(2)),
                undo_calls=int(m.group(3)),  undo_cycles=int(m.group(4)),
            )

    return dict(
        label=label,
        tests=tests,
        errors=errors,
        passed=passed,
        final_ok=final_ok,
        bcp_stats=bcp_stats,
    )


def print_run(result: dict):
    """Pretty-print one simulation run."""
    label = result["label"]
    tests = result["tests"]

    print(bold(f"\n{'═' * 60}"))
    print(bold(f"  {label}"))
    print(bold(f"{'═' * 60}"))

    if not tests:
        print(dim("  (no PASS/FAIL entries found in log)"))
    else:
        # Column widths
        name_w = max(len(t["name"]) for t in tests)
        name_w = max(name_w, 20)

        header = f"  {'Test':<{name_w}}  {'Result':6}  {'conf':4}  {'up':2}  {'done':4}  Status"
        print(dim(header))
        print(dim("  " + "-" * (len(header) - 2)))

        for t in tests:
            result_str = green("PASS") if t["result"] == "PASS" else red("FAIL")
            row = (f"  {t['name']:<{name_w}}  {result_str}    "
                   f"  {t['conf']}    {t['up']}    {t['done']}    {t['status']}")
            print(row)

            if t["result"] == "FAIL":
                print(dim(f"    expected: conf={t['exp_conf']} up={t['exp_up']} done={t['exp_done']}"))

    # BCP cycle stats
    s = result.get("bcp_stats")
    if s:
        calls  = s["bcp_calls"]
        cycles = s["bcp_cycles"]
        cpc    = (cycles / calls) if calls > 0 else 0
        print(dim("  BCP: {} calls, {} cycles ({:.1f} cyc/call)  |  "
                  "UNDO: {} calls, {} cycles".format(
                      calls, cycles, cpc,
                      s["undo_calls"], s["undo_cycles"])))

    # Final verdict
    total = result["passed"] + result["errors"]
    print()
    if result["final_ok"] is True:
        print(f"  {green('✓ ALL TESTS PASSED')}  ({total} checks)")
    elif result["final_ok"] is False:
        n_err = result["errors"]
        print(f"  {red(f'✗ {n_err} / {total} FAILED')}")
    else:
        print(dim("  (simulation did not reach $finish cleanly)"))


def print_summary(results: list):
    """Print a one-line-per-run summary table."""
    if len(results) < 2:
        return

    print(bold(f"\n{'═' * 60}"))
    print(bold("  SUMMARY"))
    print(bold(f"{'═' * 60}"))

    label_w = max(len(r["label"]) for r in results)
    label_w = max(label_w, 20)

    for r in results:
        total = r["passed"] + r["errors"]
        if r["final_ok"] is True:
            verdict = green(f"PASS ({total}/{total})")
        elif r["final_ok"] is False:
            verdict = red(f"FAIL ({r['passed']}/{total})")
        else:
            verdict = yellow("INCOMPLETE")
        print(f"  {r['label']:<{label_w}}  {verdict}")

    all_ok = all(r["final_ok"] is True for r in results)
    print()
    if all_ok:
        print(green(bold(f"  All {len(results)} simulation(s) passed.")))
    else:
        n_fail = sum(1 for r in results if r["final_ok"] is not True)
        print(red(bold(f"  {n_fail} / {len(results)} simulation(s) had failures.")))


def main():
    global USE_COLOR

    parser = argparse.ArgumentParser(description="Parse VCS simulation log(s).")
    parser.add_argument("logs", nargs="*", help="Log file(s) to parse (use - for stdin)")
    parser.add_argument("--out", metavar="FILE", help="Write report to FILE (plain text, no colour)")
    args = parser.parse_args()

    if not args.logs:
        print(__doc__)
        sys.exit(0)

    sources = []
    if args.logs == ["-"]:
        sources = [("stdin", sys.stdin.read())]
    else:
        for path in args.logs:
            p = Path(path)
            if not p.exists():
                print(f"Warning: {path} not found, skipping.", file=sys.stderr)
                continue
            sources.append((p.stem, p.read_text()))

    if not sources:
        print("No valid log files found.", file=sys.stderr)
        sys.exit(1)

    results = [parse_log(text, label) for label, text in sources]

    if args.out:
        # Capture output without colour, write to file and also print to terminal
        import io
        USE_COLOR = False
        buf = io.StringIO()
        old_stdout = sys.stdout
        sys.stdout = buf
        for r in results:
            print_run(r)
        print_summary(results)
        sys.stdout = old_stdout
        report = buf.getvalue()
        Path(args.out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.out).write_text(report)
        # Also print coloured version to terminal
        USE_COLOR = old_stdout.isatty()
        for r in results:
            print_run(r)
        print_summary(results)
    else:
        for r in results:
            print_run(r)
        print_summary(results)


if __name__ == "__main__":
    main()
