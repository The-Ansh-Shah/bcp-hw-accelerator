#!/usr/bin/env python3
"""
gen_cnf.py — Generate random 3-SAT problem instances in DIMACS CNF format.

Usage:
    # 10 easy SAT problems (ratio 2.0, low clause density):
    python3 gen_cnf.py --count 10 --vars 20 --ratio 2.0 --sat easy -o problems/

    # 10 hard near-phase-transition problems (ratio ~4.27):
    python3 gen_cnf.py --count 10 --vars 20 --ratio 4.27 -o problems/

    # Mix: 5 SAT + 5 UNSAT, labelled:
    python3 gen_cnf.py --count 5 --vars 20 --ratio 2.5 --sat sat -o problems/sat/
    python3 gen_cnf.py --count 5 --vars 20 --ratio 6.0 --sat unsat -o problems/unsat/

    # Single file to stdout:
    python3 gen_cnf.py --vars 10 --clauses 4 --stdout

Phase transition:
    For 3-SAT, the SAT/UNSAT phase transition occurs at clause/variable
    ratio ≈ 4.267.  Below this, most instances are SAT; above, most are UNSAT.
    Hard instances cluster near the transition.

Hardware constraints:
    - NUM_CLAUSES must be a power of 2 (gen_tb.py pads automatically).
    - Variables are 1-indexed in DIMACS; gen_tb.py converts to 0-indexed VIDs.
    - Keep num_vars <= 2^VID_WIDTH (default VID_WIDTH=20 supports up to 1M vars).
    - Avoid variable 0 in DIMACS (it's the clause terminator).
"""

import argparse
import random
import sys
import os
from pathlib import Path


def gen_random_3sat(num_vars, num_clauses, rng):
    """Generate a random 3-SAT instance.

    Returns list of clauses, each clause is a list of 3 DIMACS literals
    (1-indexed, signed: positive = positive literal, negative = negated).
    No duplicate literals within a clause; may have duplicate clauses.
    """
    clauses = []
    while len(clauses) < num_clauses:
        # Pick 3 distinct variables
        if num_vars < 3:
            raise ValueError("Need at least 3 variables for 3-SAT")
        vars3 = rng.sample(range(1, num_vars + 1), 3)
        # Assign random polarity
        lits = [v if rng.random() < 0.5 else -v for v in vars3]
        clauses.append(lits)
    return clauses


def write_dimacs(clauses, num_vars, path=None, comment=""):
    """Write a DIMACS CNF file.  path=None prints to stdout."""
    lines = []
    if comment:
        for line in comment.splitlines():
            lines.append("c {}".format(line))
    lines.append("p cnf {} {}".format(num_vars, len(clauses)))
    for clause in clauses:
        lines.append(" ".join(str(l) for l in clause) + " 0")
    text = "\n".join(lines) + "\n"
    if path is None:
        sys.stdout.write(text)
    else:
        Path(path).write_text(text)


def main():
    parser = argparse.ArgumentParser(
        description="Generate random 3-SAT problem instances in DIMACS CNF format.")

    parser.add_argument("--vars", type=int, default=20,
                        help="Number of variables (default: 20)")
    parser.add_argument("--clauses", type=int, default=0,
                        help="Exact number of clauses (overrides --ratio)")
    parser.add_argument("--ratio", type=float, default=4.27,
                        help="Clause-to-variable ratio (default: 4.27, phase transition)")
    parser.add_argument("--count", type=int, default=1,
                        help="Number of problems to generate (default: 1)")
    parser.add_argument("--seed", type=int, default=None,
                        help="Random seed for reproducibility")
    parser.add_argument("-o", "--out-dir", default="problems",
                        help="Output directory for generated .cnf files (default: problems/)")
    parser.add_argument("--stdout", action="store_true",
                        help="Print a single problem to stdout (ignores --count and --out-dir)")
    parser.add_argument("--prefix", default="problem",
                        help="Filename prefix (default: 'problem' → problem_001.cnf)")

    args = parser.parse_args()

    rng = random.Random(args.seed)

    num_vars    = args.vars
    num_clauses = args.clauses if args.clauses > 0 else max(1, int(round(num_vars * args.ratio)))

    if args.stdout:
        clauses = gen_random_3sat(num_vars, num_clauses, rng)
        comment = "Random 3-SAT: {} vars, {} clauses (ratio={:.3f})".format(
            num_vars, num_clauses, num_clauses / num_vars)
        write_dimacs(clauses, num_vars, path=None, comment=comment)
        return

    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    width = len(str(args.count))
    generated = []
    for i in range(args.count):
        clauses = gen_random_3sat(num_vars, num_clauses, rng)
        fname   = "{}_{:0{}d}.cnf".format(args.prefix, i + 1, width)
        fpath   = out_dir / fname
        comment = "Random 3-SAT instance {}/{}\nvars={} clauses={} ratio={:.3f} seed={}".format(
            i + 1, args.count, num_vars, num_clauses,
            num_clauses / num_vars, args.seed)
        write_dimacs(clauses, num_vars, path=str(fpath), comment=comment)
        generated.append(str(fpath))

    print("Generated {} problem(s) in {}/".format(len(generated), args.out_dir),
          file=sys.stderr)
    print("  vars={}, clauses={}, ratio={:.3f}".format(
        num_vars, num_clauses, num_clauses / num_vars), file=sys.stderr)


if __name__ == "__main__":
    main()
