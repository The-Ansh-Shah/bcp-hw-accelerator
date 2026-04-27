# Benchmarking Plan: BCP-HW-Accelerator vs Software & Hardware SAT Solvers

## Context

The team has built a 16-clause × 4-literal hardware BCP engine driven by a software CDCL coprocessor. They now need a **defensible benchmarking story** to compare it against (a) software SAT solvers (MiniSat / Glucose / Kissat) and (b) published hardware accelerators. Key constraints from the user:

- **Hardware capacity is fixed at 16 clauses** for this iteration; benchmark only at this scale (multi-block scale-out to ~1M clauses is acknowledged future work).
- **Synthesis target is TSMC 65 nm** with the team's existing hard macros for the same node — this means area/Fmax/power numbers will come from a real industrial flow, not just gate-count estimates.
- They worry that "without synthesis this might not be possible" — partly true: cycle counts alone don't give wall-clock, but cycle-count-based comparisons against published hardware solvers ARE valid and don't require synthesis to be set up first.
- The benchmark source they want to draw from is `https://benchmark-database.de/` (Iser's database of SAT-Competition instances). Challenge: nearly all entries have thousands of clauses, so we need a careful subset filter plus complementary random instances.

The intended outcome: a methodology + scripts + a final report comparing (i) wall-clock SW vs projected HW time, (ii) per-propagation cycles vs published HW solvers, (iii) area / Fmax / power numbers from the TSMC 65 nm synthesis flow.

---

## Phase 1 — Benchmark Suite Curation

The single hardest design decision is **what set of instances to run**, given the 16-clause cap. Three layers:

**Layer A — Random 3-SAT (primary, controllable):**
- Use existing `gen_cnf.py --vars N --clauses M` to generate sweeps:
  - **Easy** (ratio 2.0): 5 vars × 8 clauses, 8 vars × 12 clauses, 8 vars × 16 clauses
  - **Phase transition** (ratio 4.27): 3 vars × 13 clauses, 4 vars × 16 clauses
  - **Over-constrained / UNSAT-skewed** (ratio 6.0): 3 vars × 16 clauses
- 10 seeds per configuration → ~60 instances total. Reproducible by seed.

**Layer B — Tiny SAT-Competition instances from benchmark-database.de:**
- The database exposes a query interface (or a downloadable index). Filter:
  - `clauses ≤ 16` AND `variables ≤ 12` AND `max_clause_length ≤ 4`
  - Likely returns very few instances; expect to fall back to truncating larger ones (which changes semantics — flag this clearly in the report).
- Goal: 5–10 "real" instances even if from contrived corners of the database, just to anchor the random results.

**Layer C — Hand-crafted edge cases:**
- Pigeonhole PHP(3,2): 6 vars, 9 clauses, UNSAT, classic CDCL stress test.
- One 4-literal-clause instance — verifies the new LITS_PER_CLAUSE=4 path isn't dead code (current `tests/` only has 3-literal instances).
- The existing `tests/example.cnf` (SAT) and `tests/unsat_3var.cnf` (UNSAT) stay as smoke tests.

**Deliverable:** `benchmarks/` directory with subdirs `random/`, `database/`, `crafted/`, plus `manifest.json` listing instance metadata (name, vars, clauses, max-clause-len, expected SAT/UNSAT, source).

---

## Phase 2 — Software Baselines

Install three solvers in `baselines/`:
- **MiniSat 2.2** — classic CDCL baseline. Most directly comparable to our software CDCL loop because both implement 1-UIP + VSIDS in a similar style.
- **Glucose 4.2.1** — improved CDCL with LBD-based clause management.
- **Kissat** — current SAT-Competition winner; SOTA reference.

Wrapper script `bench/run_sw.py`:
- Runs each solver on each manifest instance with `time -v` (collects wall-clock + RSS).
- Repeats 10× per instance (median + IQR) — important because tiny problems solve in microseconds and noise dominates wall-clock measurement.
- Records: wall-clock, decisions, conflicts, propagations (parsed from solver stdout).
- Output: `results/sw_<solver>.csv` — one row per (solver, instance, repeat).

---

## Phase 3 — Hardware Cycle-Accurate Measurement (no synthesis needed)

The C++ coprocessor already tracks `n_hw_bcp`, `n_hw_undo`, `n_hw_load`. Each maps to a fixed cycle count from the FSM in [src/sat_submodule.sv](src/sat_submodule.sv):
- LOAD: 2 cycles (S_IDLE → S_LOAD → S_IDLE)
- BCP: 3 cycles (S_IDLE → S_BCP1 → S_BCP2 → S_IDLE)
- UNDO: 2 cycles (S_IDLE → S_UNDO → S_IDLE)

**Modify [coprocessing/cdcl_solver.cpp](coprocessing/cdcl_solver.cpp):**
- Add an end-of-run line:
  ```
  c hw_cycles=N
  ```
  computed as `2*n_hw_load + 3*n_hw_bcp + 2*n_hw_undo`.
- Optionally add a `--csv` flag that emits one CSV row to make `bench/run_hw.py` aggregation trivial.

**Wrapper `bench/run_hw.py`:** runs `coprocessing/cdcl_solver` on each manifest instance, parses the new stats line, writes `results/hw.csv`.

**Headline metric for HW-vs-HW comparison:** `hw_cycles / n_propagations` ("cycles per implication"). This is directly comparable to numbers reported by published hardware SAT solvers (e.g., Davis et al., Skliarova-Sklyarov FPGA SAT) and is the most defensible comparison because it doesn't depend on Fmax — it's a pure architectural figure-of-merit.

---

## Phase 4 — TSMC 65 nm Synthesis (high-level)

We don't need to set up the flow yet — the team already plans to do this on TSMC 65 nm with their own hard macros. The benchmarking plan needs to be **clear about what numbers we'll harvest from it**:

**What synthesis gives us:**
| Number | Where it comes from | What we use it for |
|--------|---------------------|---------------------|
| Fmax (MHz) | Synopsys DC timing report (post-synth) or post-PnR STA | Convert HW cycles → wall-clock: `hw_time = hw_cycles / Fmax` |
| Cell area (µm²) | DC `report_area` | Area Pareto vs published HW solvers |
| Gate count | DC `report_area` "combinational + sequential" | Cross-tool sanity check |
| Power (mW) | DC `report_power` (SAIF-driven if we capture activity from the Verilator runs) | Energy/solve = power × hw_time |

**Where the hard macros plug in:**
- The 64 rows of `cam_sram_row` are FF-based today (`src/cam_cell.sv` and `src/sram_row.sv`). At 192 bits total, FF-based is appropriate-for-scale; macro substitution mainly buys credibility for the scale-out story (1M clauses), not area at 16 clauses.
- For the team's existing 65 nm hard macros: write a thin wrapper module that exposes the same interface as `cam_sram_row.sv` but instantiates the macro internally. Synthesis treats it as a black box; only the wrapper is touched.
- Defer the actual swap to after the FF-based numbers are in hand — that gives a clean before/after comparison.

**Generating switching activity for power:**
- Add VCD/SAIF dump to a Verilator run on a representative benchmark instance.
- Hand the SAIF to DC `read_saif` for activity-driven power.
- Without this, DC will use default toggle rates which under-estimate dynamic power.

**Recommended order of operations** (so benchmarking isn't blocked on synthesis):
1. Phases 1–3 done first → cycle-count and SW results in hand.
2. Synthesis: get Fmax + area first (no macros, no SAIF).
3. Project HW wall-clock using Fmax; produce the comparison plots.
4. Macro substitution and power runs as a follow-up if time permits.

---

## Phase 5 — Comparison Methodology

Generate four plots in `report/`:

1. **Wall-clock SW vs projected HW time** (per-instance bar chart).
   HW projected time = `hw_cycles / Fmax_from_synthesis`.
   Honest caveat in caption: "HW time excludes the software CDCL loop and Verilator simulation overhead; SW time is the entire solve including I/O."

2. **Cycles-per-propagation** (vs published HW solvers) — table comparing our number against Davis et al. (UCSB hw SAT 2008), Skliarova-Sklyarov FPGA SAT, and any other accelerator that reports cycles/implication. **This is the most defensible HW-vs-HW comparison and does NOT depend on synthesis numbers — produce it in Phase 3 already.**

3. **Operation breakdown** (stacked bars: LOAD vs BCP vs UNDO cycles per instance). Shows where time is spent and exposes the LOAD overhead at small problem sizes.

4. **Area-throughput Pareto** (one point: our 16-clause design at its synthesized Fmax / area). Plot against literature points where reported. Mark our point with the explicit caveat "16 clauses, FF-storage".

**Critical limitations to call out in the report:**
- 16-clause cap means published instances mostly don't fit; comparisons are at toy scale.
- Tiny problems → SW wall-clock <1 ms → high measurement noise → cycle-count comparisons are more meaningful than wall-clock.
- LOAD overhead is amortized poorly at small problem sizes (linear in clause count); show this in plot 3.
- FF-storage area is not the macro-storage area; the scale-out story will use macros.

---

## Critical Files

**Existing, will be referenced or modified:**
- [coprocessing/cdcl_solver.cpp](coprocessing/cdcl_solver.cpp) — add cycle-stat output line (Phase 3); already tracks `main_time`, `n_hw_bcp`, `n_hw_undo`, `n_hw_load`.
- [gen_cnf.py](gen_cnf.py) — reused as-is for Layer A random generation.
- [src/sat_submodule.sv](src/sat_submodule.sv) — synthesis target; no edits needed for benchmarking.
- [src/cam_cell.sv](src/cam_cell.sv), [src/sram_row.sv](src/sram_row.sv) — eventual macro wrapper targets (Phase 4 stretch).
- [run_tests.py](run_tests.py) — VCS testbench pipeline. **NOT** used in this plan; the C++ coprocessor is the measurement vehicle. (Note: this pipeline is also currently broken vs the new RTL geometry — out of scope to fix.)

**New, to create:**
- `benchmarks/{random,database,crafted}/` + `benchmarks/manifest.json` — instance set.
- `bench/run_sw.py`, `bench/run_hw.py`, `bench/aggregate.py` — measurement harness.
- `baselines/` — solver binaries / build scripts.
- `report/plots/` — generated comparison figures.

---

## Verification

1. **Suite curation**: `python3 bench/list_instances.py` prints the manifest with instance counts per layer; every instance has clause-count ≤ 16 and max-clause-len ≤ 4.
2. **Software baselines**: `bench/run_sw.py --solver minisat tests/example.cnf` returns SAT in <1 s with parseable stats; same for glucose and kissat.
3. **Hardware run**: `coprocessing/cdcl_solver tests/example.cnf` ends with the new `c hw_cycles=N` line; SAT/UNSAT verdicts unchanged from current behavior.
4. **End-to-end aggregation**: `bench/aggregate.py` produces `results/combined.csv` with one row per (instance, solver) covering every manifest instance.
5. **Cycle-per-propagation hand-check**: pick one debug trace from `SAT_DEBUG=1 cdcl_solver tests/unsat_3var.cnf`, count BCP/UNDO/LOAD events by hand, recompute `hw_cycles / n_propagations`, and confirm it matches the CSV.
6. **Synthesis sanity (when set up)**: a clean DC run completes; `report_area` shows reasonable ballpark (FF-based 64-row design should be a few thousand flops + small combinational logic — well under 10 K gates total).

---

## What we're explicitly NOT doing (and why)

- **Scaling NUM_CLAUSES to 1k/10k for "real" benchmarks** — user confirmed out-of-scope for this iteration.
- **Wiring up `src/hw_bcp_propagate.v` stubs** — that's a separate, alternative BCP architecture; doesn't affect the coprocessor design we're benchmarking.
- **Setting up the synthesis flow now** — user is handling that separately; this plan only specifies what numbers to harvest from it and how they feed into the comparison.
- **GPU SAT solver comparisons** — out of scope; would need very different methodology.
- **Fixing the legacy VCS testbench pipeline** (`run_tests.py` / `gen_tb.py` / `tb/`) — broken vs new RTL geometry but not on the critical path; the C++ coprocessor is the measurement vehicle.
