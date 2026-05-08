#!/usr/bin/env python3
"""
gen_hard.py — Generate small SAT instances that are provably hard for
resolution-based CDCL solvers (pigeonhole, Tseitin parity over cycles,
graph k-colourability on Mycielski-style triangle-free graphs).

All output CNFs respect the hardware coprocessor's 4-literals-per-clause
limit by Tseitin-splitting wide clauses through auxiliary variables.

Usage:
    python3 gen_hard.py            # writes the standard suite into ./
    python3 gen_hard.py --out DIR
"""
import argparse
import itertools
import os
from pathlib import Path


# ---------------------------------------------------------------------------
# 4-CNF splitter
# ---------------------------------------------------------------------------
def split_to_4cnf(clauses, n_vars, max_w=4):
    """Rewrite any clause wider than max_w into ≤ max_w-literal clauses
    using auxiliary chain variables (Tseitin). Returns (new_clauses, n_vars)."""
    out = []
    next_var = n_vars + 1
    for cl in clauses:
        if len(cl) <= max_w:
            out.append(list(cl))
            continue
        rest = list(cl)
        prev_aux = None
        while len(rest) > max_w:
            head = rest[: max_w - 1]   # take 3 literals
            rest = rest[max_w - 1 :]
            aux = next_var
            next_var += 1
            # head ∨ aux  ;  next clause must contain ¬aux
            if prev_aux is None:
                out.append(head + [aux])
            else:
                out.append([-prev_aux] + head + [aux])
            prev_aux = aux
        # final tail
        if prev_aux is None:
            out.append(rest)
        else:
            out.append([-prev_aux] + rest)
    return out, next_var - 1


def write_dimacs(path, clauses, n_vars, comment=""):
    with open(path, "w") as f:
        if comment:
            f.write(f"c {comment}\n")
        f.write(f"p cnf {n_vars} {len(clauses)}\n")
        for cl in clauses:
            f.write(" ".join(str(int(l)) for l in cl) + " 0\n")


# ---------------------------------------------------------------------------
# 1.  Pigeonhole PHP_n  (n+1 pigeons → n holes, UNSAT, exp. resolution)
# ---------------------------------------------------------------------------
def php(n_holes):
    """PHP_n: pigeons 1..n+1, holes 1..n.  Var x(i,j) = 'pigeon i in hole j'."""
    n = n_holes
    pigeons = n + 1
    def x(i, j):
        return (i - 1) * n + j           # 1-indexed flat var id
    n_vars = pigeons * n
    clauses = []
    # Every pigeon goes to some hole (wide clauses; will be split).
    for i in range(1, pigeons + 1):
        clauses.append([x(i, j) for j in range(1, n + 1)])
    # No two pigeons share a hole (binary).
    for j in range(1, n + 1):
        for i1, i2 in itertools.combinations(range(1, pigeons + 1), 2):
            clauses.append([-x(i1, j), -x(i2, j)])
    return split_to_4cnf(clauses, n_vars)


# ---------------------------------------------------------------------------
# 2.  Tseitin parity on a cycle graph (UNSAT, exp. on expanders;
#     cycles are easy but a controlled hard family at small n).
# ---------------------------------------------------------------------------
def tseitin_cycle(n_vertices, charge=None):
    """Cycle C_n: edges e_0..e_{n-1}; each vertex v has parity charge b_v.
    Charges sum-odd ⇒ UNSAT.  Each parity x⊕y⊕z = b expands to 4 clauses
    of width 3 (degree 3 → degree 2 at endpoints of path; we use a cycle
    so every vertex has degree 2, parity over 2 vars: 2 clauses of width 2).
    Cycle is mild; we use it as a sanity-check for our pipeline."""
    n = n_vertices
    if charge is None:
        # default: vertex 0 charged, all others 0  ⇒ sum=1 (odd) ⇒ UNSAT
        charge = [1] + [0] * (n - 1)
    assert sum(charge) % 2 == 1, "cycle Tseitin only UNSAT when charges odd"
    # Edge i connects vertex i and vertex (i+1) mod n.
    # Var(edge i) = i+1.
    n_vars = n
    clauses = []
    for v in range(n):
        e_in  = ((v - 1) % n) + 1
        e_out = v + 1
        # x_in XOR x_out = b_v
        b = charge[v]
        # XOR via 2 clauses of width 2:
        # b=0: (a∨b)(¬a∨¬b) wait that's a≠b. Actually a XOR b = 0 means a=b.
        # a = b  ↔  (¬a∨b)(a∨¬b)
        # a ≠ b  ↔  (a∨b)(¬a∨¬b)
        if b == 0:
            clauses.append([-e_in,  e_out])
            clauses.append([ e_in, -e_out])
        else:
            clauses.append([ e_in,  e_out])
            clauses.append([-e_in, -e_out])
    return split_to_4cnf(clauses, n_vars)


# ---------------------------------------------------------------------------
# 3.  Graph k-colorability on Mycielski-style graphs (triangle-free,
#     high chromatic number).  Encode as DIMACS; CDCL struggles when
#     k = chromatic_number - 1.
# ---------------------------------------------------------------------------
def k_color(edges, n_vertices, k):
    """Vars: c(v, color) for v in 1..n, color in 1..k.
    Each vertex picks ≥1 colour and adjacent vertices get distinct colours.
    UNSAT iff k < chi(G)."""
    def c(v, col):
        return (v - 1) * k + col
    n_vars = n_vertices * k
    clauses = []
    # at least one colour per vertex (wide -> split)
    for v in range(1, n_vertices + 1):
        clauses.append([c(v, col) for col in range(1, k + 1)])
    # at most one colour per vertex (binary)
    for v in range(1, n_vertices + 1):
        for c1, c2 in itertools.combinations(range(1, k + 1), 2):
            clauses.append([-c(v, c1), -c(v, c2)])
    # adjacent vertices must differ
    for u, v in edges:
        for col in range(1, k + 1):
            clauses.append([-c(u, col), -c(v, col)])
    return split_to_4cnf(clauses, n_vars)


def mycielski_grotzsch():
    """Grötzsch graph, 11 vertices, triangle-free, chromatic number 4.
    k=3 → UNSAT (provably hard for CDCL on small instance)."""
    edges = [
        (1,2),(1,5),(1,7),(1,8),(1,11),
        (2,3),(2,8),(2,9),
        (3,4),(3,9),(3,10),
        (4,5),(4,10),(4,11),
        (5,6),(5,11),
        (6,7),(6,8),(6,9),(6,10),(6,11),
        (7,8),(7,11),
        (8,9),
        (9,10),
        (10,11),
    ]
    # dedupe & undirected
    eset = set()
    for u, v in edges:
        a, b = min(u, v), max(u, v)
        eset.add((a, b))
    return sorted(eset), 11


# ---------------------------------------------------------------------------
# Driver
# ---------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out", default=str(Path(__file__).resolve().parent),
                    help="output directory")
    args = ap.parse_args()
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)

    suite = []

    # Pigeonhole — small but provably exponentially hard for resolution.
    for n in range(3, 9):
        cls, nv = php(n)
        path = out / f"php_{n}.cnf"
        write_dimacs(path, cls, nv,
                     f"PHP_{n}: {n+1} pigeons in {n} holes (UNSAT, "
                     f"exp resolution proof)")
        suite.append((path, "UNSAT", f"PHP_{n}", nv, len(cls)))

    # Tseitin on odd cycles (pipeline check; UNSAT, easy class)
    for n in (5, 7, 9):
        cls, nv = tseitin_cycle(n)
        path = out / f"tseitin_c{n}.cnf"
        write_dimacs(path, cls, nv,
                     f"Tseitin parity on C_{n} (UNSAT)")
        suite.append((path, "UNSAT", f"Tseitin C{n}", nv, len(cls)))

    # 3-colouring of Grötzsch graph (chromatic # = 4 ⇒ UNSAT)
    edges, nverts = mycielski_grotzsch()
    cls, nv = k_color(edges, nverts, 3)
    path = out / "grotzsch_3col.cnf"
    write_dimacs(path, cls, nv,
                 "3-colouring of Grötzsch graph (UNSAT, chi=4)")
    suite.append((path, "UNSAT", "Grötzsch-3col", nv, len(cls)))

    # 4-colouring is satisfiable but requires real search
    cls, nv = k_color(edges, nverts, 4)
    path = out / "grotzsch_4col.cnf"
    write_dimacs(path, cls, nv,
                 "4-colouring of Grötzsch graph (SAT, chi=4)")
    suite.append((path, "SAT", "Grötzsch-4col", nv, len(cls)))

    print(f"Wrote {len(suite)} hard instances to {out}")
    print(f"{'NAME':<14} {'EXPECT':<6} {'VARS':>5} {'CLAUSES':>8}  FILE")
    for path, exp, name, nv, nc in suite:
        print(f"{name:<14} {exp:<6} {nv:>5} {nc:>8}  {path.name}")


if __name__ == "__main__":
    main()
