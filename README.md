# HW-Accelerated SAT Solver: BCP + CDCL RTL Integration

A hardware/software co-design SAT solver. The RTL implements a CAM/SRAM Boolean Constraint Propagation (BCP) engine; a C++ CDCL coprocessor drives it via a Verilated model, handling decisions, conflict analysis, clause learning, and backtracking entirely in software.

Every BCP operation — setting a variable's value, detecting unit implications, and flagging conflicts — goes through the RTL. The coprocessor calls the hardware for each propagation step and reads back the result before continuing the CDCL loop.

Two RTL paths exist side-by-side:

- **Single sub-SAT** (`sat_submodule`) — 64 clauses × 4 literals = 256 rows of 18-bit CAM. This is the unit the coprocessor drives via Verilator; it is also what every TB in `tb/` instantiates by default.
- **Full hierarchy** (`sat_array → sat_group_submodule → sat_submodule`) — wraps NUM_GROUPS × NUM_SUBSAT_PER_GROUP sub-SATs and combines their CONF/UP/DONE outputs through three module levels. The default elaboration is the spec geometry (1024 × 8 × 64 = 524 288 clauses, 19 levels of cumulative combining), but every parameter is overridable so simulations can pick a tractable size.

---

## First-Time Setup (Fresh Clone)

The repository pulls in four reference SAT solvers as git submodules so the coprocessor can be benchmarked against published software solvers. Run these once after cloning:

```bash
# 1. Clone with submodules (or initialize them after the fact)
git clone --recurse-submodules <repo-url>
cd bcp-hw-accelerator

# If you already cloned without --recurse-submodules:
git submodule update --init --recursive
```

The submodules are:

| Submodule | Upstream | Role |
|-----------|----------|------|
| [kissat/](kissat/)     | `arminbiere/kissat`        | Current SAT-Competition winner; SOTA reference |
| [glucose/](glucose/)   | `audemard/glucose`         | LBD-based CDCL, improved restart strategy |
| [minisat/](minisat/)   | `stp/minisat`              | Classic CDCL baseline (1-UIP + VSIDS), closest in style to our coprocessor |
| [minisat2/](minisat2/) | `stp/minisat`              | Alternate MiniSat checkout slot (kept separate so a different revision/build flag set can be evaluated independently) |

### Build the coprocessor

```bash
cd coprocessing
make
cd ..
```

This produces `coprocessing/cdcl_solver`. The Verilated C++ files are pre-generated and checked in; `make` only compiles + links. **No VCS or Verilator install required for the coprocessor** — only `g++` and the Verilator runtime headers (already vendored under `coprocessing/`).

### Build the baseline solvers

```bash
# Kissat
( cd kissat   && ./configure && make -j )            # → kissat/build/kissat

# Glucose (parallel + sequential)
( cd glucose  && cd simp && make -j )                # → glucose/simp/glucose

# MiniSat 2.2 — patch out the hardcoded `--static` link flag first
# (the Makefile statically links libstdc++/libz/libm/libc, which the EECS
# instructional machines don't ship as .a archives), then build the release
# (dynamic) binary only.
sed -i.bak 's/--static //' minisat/Makefile minisat2/Makefile
( cd minisat  && make r -j )                         # → minisat/build/release/bin/minisat
( cd minisat2 && make r -j )                         # → minisat2/build/release/bin/minisat
```

After these complete, the binaries [kissat/build/kissat](kissat/build/kissat), [glucose/simp/glucose](glucose/simp/glucose), [minisat/build/release/bin/minisat](minisat/build/release/bin/minisat), and [minisat2/build/release/bin/minisat](minisat2/build/release/bin/minisat) are what the benchmark harness in [§ Benchmarking](#benchmarking) drives.

**Why the MiniSat patch is needed.** Line 99 of `minisat/Makefile` reads
```make
$(BUILD_DIR)/release/bin/$(MINISAT):		MINISAT_LDFLAGS += --static $(MINISAT_RELSYM)
```
which forces a fully static link. On a system without `libz.a`, `libstdc++.a`, `libm.a`, `libc.a` (typical for non-development distros and our EECS machines), this fails with `cannot find -lz / -lstdc++ / -lm / -lc`. The `sed` above strips `--static` so the binary dynamically links those system libraries instead. The resulting `minisat` is functionally identical for benchmarking — only the deployment-time linkage differs. Using `make r` (instead of bare `make`) also skips the `lr`/`lsh` library-only targets you don't need.

### (Optional) RTL simulation prerequisites

Only needed if you'll run anything under [tb/](tb/) or [sim/](sim/): Synopsys VCS. On Berkeley EECS instructional machines, the project's [eecs151source.bashrc](eecs151source.bashrc) handles the environment automatically — see [§ RTL Simulation (VCS)](#rtl-simulation-vcs).

### Smoke test

```bash
./coprocessing/cdcl_solver tests/example.cnf       # expect: s SATISFIABLE
./coprocessing/cdcl_solver tests/unsat_3var.cnf    # expect: s UNSATISFIABLE
./kissat/build/kissat tests/example.cnf            # expect: s SATISFIABLE
```

---

## Directory Structure

```
bcp-hw-accelerator/
├── src/                        # RTL source files
│   ├── sat_submodule.sv            # Sub-SAT block: FSM + CAM/SRAM array + clause eval
│   ├── sat_group_submodule.sv      # Wraps NUM_SUBSAT (default 8) sub-SATs
│   ├── sat_array.sv                # Wraps NUM_GROUPS (default 1024) groups
│   ├── cam_sram_array.sv           # Array of NUM_CLAUSES × LITS_PER_CLAUSE CAM+SRAM rows
│   ├── cam_sram_row.sv             # Single row: one CAM cell + one SRAM cell
│   ├── cam_cell.sv                 # CAM cell: VID storage + associative search
│   ├── sram_row.sv                 # SRAM cell: 2-bit value + 1-bit polarity
│   ├── processing_logic.sv         # Combinational per-clause evaluator (CONF/UP/DONE)
│   ├── combining_logic.sv          # Priority reduction (CONF > UP > DONE)
│   ├── hw_bcp_propagate.v          # BCP FSM skeleton
│   ├── hw_bcp_defs.vh              # Shared defines
│   └── hw_bcp_sim_stubs.v          # Simulation stubs for hw_bcp_propagate
│
├── coprocessing/               # SW/HW coprocessor (main integration)
│   ├── cdcl_solver.cpp             # C++ CDCL solver driving the Verilated RTL
│   ├── Makefile
│   └── Vsat_solver.*               # Pre-generated Verilator model (checked in)
│
├── sim/                        # HW-BCP FSM standalone sandbox (VCS)
│   └── tb/tb_hw_bcp.sv             # Testbench for hw_bcp_propagate with stubs
│
├── tb/                         # Verilog testbenches + runner
│   ├── Makefile                    # Per-testbench targets, sub-test selection, waveforms
│   ├── sat_submodule_tb.sv         # Auto-generated by gen_tb.py from a CNF file
│   ├── sat_submodule_4sat_tb.sv    # Hand-written 4-SAT TB with three sub-tests
│   ├── sat_array_tb.sv             # Hierarchy smoke test (16 clauses)
│   └── sat_array_large_tb.sv       # Large-scale integration test (4096 clauses)
│
├── tests/                      # CNF problems (DIMACS format)
│   ├── example.cnf                 # 7 vars, 4 clauses — SAT
│   ├── unsat_3var.cnf              # 3 vars, 8 clauses — UNSAT
│   └── satlib/                     # SATLIB Uniform Random-3-SAT benchmark suite
│       ├── run_satlib.py           # Drives gen_tb.py + VCS for the RTL trace flow
│       ├── bench_cdcl.py           # Runs cdcl_solver (+ kissat/minisat/glucose) with full metrics
│       ├── uf20-91/                # 1000 SAT instances, 20 vars × 91 clauses
│       ├── uf50-218/               # 1000 SAT instances, 50 vars × 218 clauses
│       └── uuf50-218/              # 1000 UNSAT instances, 50 vars × 218 clauses
│
├── kissat/    minisat/    minisat2/    glucose/   # Baseline SAT solvers (git submodules)
│
├── dpll.py                     # Python behavioral model of the RTL
├── gen_cnf.py                  # Random 3-SAT problem generator
├── gen_tb.py                   # SystemVerilog testbench generator from CNF
├── run_tests.py                # Batch test runner
├── parse_results.py            # VCS log parser
└── README.md
```

---

## Coprocessor: Solving CNF Files

The coprocessor is the primary deliverable. It compiles the C++ CDCL solver against the pre-generated Verilated RTL model. No VCS or additional tool setup is required — only `g++` and the Verilator runtime library.

### Build

```bash
cd coprocessing
make
```

This produces the `cdcl_solver` binary. The Verilated C++ files are pre-generated and checked in; `make` only needs to compile and link.

### Solve any CNF file

```bash
./cdcl_solver <path/to/problem.cnf>
```

Output follows the DIMACS standard:

```
s SATISFIABLE
v 1 2 3 4 5 6 7 0
```

or

```
s UNSATISFIABLE
```

Statistics are printed to stderr:

```
c originals=4 learnts=0 decisions=7 conflicts=0 propagations=7 hw_bcp=8 hw_undo=0 hw_load=16
```

### Examples

```bash
# SAT: 7 variables, 4 clauses
./cdcl_solver ../tests/example.cnf

# UNSAT: 3 variables, all 8 sign-combinations
./cdcl_solver ../tests/unsat_3var.cnf

# A SATLIB instance
./cdcl_solver ../tests/satlib/uf20-91/uf20-01.cnf
```

### Debug output

Set `SAT_DEBUG=1` to print every hardware call, including val_stored / matchline state after each BCP:

```bash
SAT_DEBUG=1 ./cdcl_solver ../tests/example.cnf
```

### Regenerating the Verilated model

If RTL under `src/` changes, regenerate:

```bash
rm -f coprocessing/Vsat_solver* coprocessing/cdcl_solver coprocessing/*.o

verilator --cc \
  src/sat_submodule.sv src/cam_sram_array.sv src/cam_sram_row.sv \
  src/cam_cell.sv src/sram_row.sv \
  src/processing_logic.sv src/combining_logic.sv \
  --Mdir coprocessing --prefix Vsat_solver

cd coprocessing && make
```

If `NUM_CLAUSES` or `LITS_PER_CLAUSE` change in `src/sat_submodule.sv`, also update the matching constants in `coprocessing/cdcl_solver.cpp` (`HW_NUM_CLAUSES`, `HW_LITS_PER_CLAUSE`).

---

## Architecture

### Division of responsibility

| Layer | Runs in | Responsibility |
|-------|---------|----------------|
| `sat_submodule` (RTL) | Hardware (Verilated) | LOAD, BCP, UNDO; CAM search; SRAM write; per-clause and combined CONF/UP/DONE output |
| `cdcl_solver.cpp` (SW) | Software (C++) | Decision heuristics (VSIDS), 1-UIP conflict analysis, clause learning, backtracking, learned-clause propagation |

### RTL operations

| `op` | Cycles | Effect |
|------|--------|--------|
| `OP_LOAD` (1) | 2 | Write VID + polarity into the addressed CAM/SRAM row; init value to unassigned |
| `OP_BCP` (2)  | 3 | CAM search for `vid_in`; SRAM write of `val_in` to all matching rows; eval all clauses; output `conf_out`/`up_out`/`done_out` + implied literal |
| `OP_UNDO` (3) | 2 | Same CAM search; reset all matching rows to unassigned (VAL_U) |

### Value encoding

| Symbol | Bits | Meaning |
|--------|------|---------|
| `VAL_ZERO` | `2'b00` | Assigned false |
| `VAL_ONE`  | `2'b11` | Assigned true |
| `VAL_U`    | `2'b01` | Unassigned |

Polarity: `0` = positive literal (`xᵢ`), `1` = negative literal (`¬xᵢ`).

### Clause-to-row mapping

Inside one `sat_submodule`: clause `c`, literal position `l` → row `4c + l`. DIMACS variables are 1-indexed; VID 0 is reserved for padding (clauses shorter than 4 literals are padded with positive `x₀` and pinned to false via one initial `bcp(0, VAL_ZERO)` after LOAD).

### Hierarchy

`sat_group_submodule` and `sat_array` use the same broadcast-and-combine pattern at successive levels:

- **Routing.** `OP_LOAD` and `OP_CIT_READ` go only to the addressed sub-block (decoded from upper `row_addr`/`cid_in` bits); other sub-blocks see `OP_IDLE`. `OP_BCP` / `OP_UNDO` broadcast to every sub-block in lockstep.
- **Priority combining.** Each level reduces children with `CONF > UP > DONE`; lowest-index child wins ties.
- **CID extension.** Each level prepends `$clog2(N)` selector bits to the child's `cid_out`. Top level: `{ group_sel(10), subsat_sel(3), local_cid(6) } = 19 bits` at spec defaults.
- **DONE aggregation.** AND of all children's `done_out`.

Total combining levels at spec defaults: **6 (sub-SAT) + 3 (group) + 10 (array) = 19**.

---

## Hardware Limits

Each module exposes parameters that fix its geometry at elaboration time.

### `sat_submodule`

| Parameter | Default | Notes |
|-----------|---------|-------|
| `VID_WIDTH` | 18 | ≈ 262 K variables addressable |
| `NUM_CLAUSES` | 64 | Up to 64 clauses per sub-SAT |
| `LITS_PER_CLAUSE` | 4 | Shorter clauses padded with VID 0 |
| `NUM_ROWS` | 256 | `= NUM_CLAUSES × LITS_PER_CLAUSE` |

### `sat_group_submodule`

| Parameter | Default | Notes |
|-----------|---------|-------|
| `NUM_SUBSAT` | 8 | 8 sub-SATs per group |
| `GROUP_CID_WIDTH` | 9 | `SUBSAT_CID_WIDTH(6) + SUBSAT_SEL_WIDTH(3)` |

### `sat_array`

| Parameter | Default | Notes |
|-----------|---------|-------|
| `NUM_GROUPS` | 1024 | 32 × 32 logical layout |
| `ARRAY_CID_WIDTH` | 19 | `GROUP_CID_WIDTH(9) + GROUP_SEL_WIDTH(10)` |

Default geometry: **524 288 clauses (= 2¹⁹)**. Override `NUM_GROUPS` for tractable simulations.

The pre-generated Verilator model under `coprocessing/` was built against an older geometry (`NUM_CLAUSES = 16`, `VID_WIDTH = 20`) and is locked at elaboration time. Regenerate Verilator to bring the coprocessor up to current defaults.

---

## RTL Simulation (VCS)

Requires Synopsys VCS. On Berkeley EECS instructional machines (`eda-[1-4].eecs.berkeley.edu`), the project ships its own VCS environment script at `eecs151source.bashrc` (project root). Every Makefile under `tb/` and `sim/` sources it automatically — **you do not need to `source` anything by hand**.

### HW-BCP FSM sandbox

`sim/` simulates `hw_bcp_propagate` with stub CAM, SRAM, and clause-evaluator models:

```bash
cd sim
make clean && make run
```

Tests: idle state, zero-length trail, multi-literal trail, unit propagation, conflict detection.

### Testbenches (`tb/`)

Each testbench compiles into its own `tb/build/<name>/` subdirectory. Targets recompile only when sources change.

#### Single sub-SAT TBs (small, fast)

| Target | Description |
|--------|-------------|
| `make sat`     | Auto-generated 3-SAT TB driven by `gen_tb.py` + `tests/example.cnf` |
| `make 4sat`    | Hand-written 4-SAT TB; runs sub-tests A, B, C |
| `make 4sat-A`  | Only sub-test A (also `-B`, `-C`) |

Each run finishes with `ALL TESTS PASSED`; mismatches print `FAIL [<name>]: ...`.

#### Hierarchy TBs (multi-level integration)

| Target | Description |
|--------|-------------|
| `make array`       | Smoke test: 2 × 2 × 4 = 16 clauses (~2 s) |
| `make array-large` | Large: 8 × 8 × 64 = 4096 clauses, 16 384 rows (~1 min) |

Both run two sub-tests against the full `sat_array → sat_group_submodule → sat_submodule` stack:

1. **Broadcast-DONE.** Every clause is `(x1 ∨ x2 ∨ x3 ∨ x4)`; one `bcp(x1=1)` should make `done_out = 1`.
2. **Priority CONF.** A single conflicting clause `(¬x1 ∨ ¬x1 ∨ ¬x1 ∨ ¬x1)` is planted at the *last* `(group, sub-SAT, clause)` slot; `bcp(x1=1)` must report `conf_out = 1` with the correct global `cid_out`.

#### View waveforms

```bash
make wave-sat            # build/sat/waves.vpd
make wave-4sat
make wave-array
make wave-array-large
```

#### Clean

```bash
make clean
```

#### Regenerating the auto-generated 3-SAT testbench

```bash
python3 gen_tb.py tests/example.cnf --out tb/sat_submodule_tb.sv
make -C tb sat
```

---

## Benchmarks: SATLIB Uniform Random-3-SAT

The `tests/satlib/` directory ships three Uniform Random-3-SAT collections from the [SATLIB Benchmark Problems page](https://www.cs.ubc.ca/~hoos/SATLIB/benchm.html), all sitting at the α ≈ 4.27 phase-transition ratio where random 3-SAT is hardest:

| Set | Variables | Clauses | Instances | Expected |
|-----|-----------|---------|-----------|----------|
| `uf20-91`   |  20 |  91 | 1000 | SAT   |
| `uf50-218`  |  50 | 218 | 1000 | SAT   |
| `uuf50-218` |  50 | 218 | 1000 | UNSAT |

`uf*` sets are crafted SAT, `uuf*` crafted UNSAT. `bench_cdcl.py` infers the expected result automatically from the directory name (`uu*` → UNSAT, otherwise SAT).

Two harness scripts cover the two flows:

| Script | What it drives | Use when |
|--------|----------------|----------|
| [tests/satlib/run_satlib.py](tests/satlib/run_satlib.py)   | `gen_tb.py` → VCS RTL trace flow | Validating the **RTL** matches the Python DPLL reference step-for-step |
| [tests/satlib/bench_cdcl.py](tests/satlib/bench_cdcl.py)   | `cdcl_solver` (+ optional baselines) | Measuring **coprocessor performance** vs Kissat / Glucose / MiniSat |

---

## RTL Trace-Flow Validation (`run_satlib.py`)

For each sampled `.cnf` the script regenerates `tb/sat_submodule_tb.sv` via `gen_tb.py` (which runs the Python DPLL model to capture an expected per-step trace), then builds + runs the testbench through `make -C tb sat`. PASS means the RTL trace agrees with the DPLL reference on every BCP / UNDO step.

```bash
python3 tests/satlib/run_satlib.py uf20-91 -n 5                  # 5 instances, default seed
python3 tests/satlib/run_satlib.py uf50-218 -n 3 --seed 1         # reproducible sample
python3 tests/satlib/run_satlib.py uuf50-218 -n 3 --max-decisions 5000

# Equivalent through the tb/Makefile:
make -C tb satlib N=5 SEED=42
make -C tb satlib-uf20      # alias of `make satlib`
make -C tb satlib-uf50      # SAT, harder
make -C tb satlib-uuf50     # UNSAT, harder
```

A 5-instance `uf20-91` sample takes ≈ 60 s and produces 12 000+ per-step `PASS` checks. Each instance regenerates the testbench, forcing a VCS recompile (≈ 5 s) plus simulation.

The 50-variable sets are usable here but stress the DPLL reference: `dpll.py` doesn't drive matchline-gated UP back into its decision loop, so the search can explode at the phase transition. `--max-decisions` caps the budget so `gen_tb.py` returns in reasonable time. For pure performance benchmarking on those sets, prefer `bench_cdcl.py` below.

---

## Coprocessor Benchmarking (`bench_cdcl.py`)

[tests/satlib/bench_cdcl.py](tests/satlib/bench_cdcl.py) is the primary benchmarking harness. It walks any subset of SATLIB directories, runs `cdcl_solver` on every `.cnf`, optionally runs Kissat / Glucose / MiniSat on the same instances, and tabulates per-instance metrics plus distribution stats (min / p50 / mean / p95 / max).

### Quick start (coprocessor only)

```bash
python3 tests/satlib/bench_cdcl.py --sets "uf20-91"
python3 tests/satlib/bench_cdcl.py --sets "uf20-91,uuf50-218" --timeout 10 --csv results.csv
```

### Head-to-head against software baselines

Pass any combination of `--kissat`, `--minisat2`, `--glucose` to add columns for those solvers. The harness already strips SATLIB's trailing `%/0` line before handing the CNF to each solver.

```bash
python3 tests/satlib/bench_cdcl.py \
    --sets "uf20-91" \
    --kissat   kissat/build/kissat \
    --minisat2 minisat2/build/release/bin/minisat \
    --glucose  glucose/simp/glucose \
    --timeout 10 \
    --csv      results/uf20-91.csv
```

### Output columns

For each instance the table reports:

| Column | Meaning |
|--------|---------|
| `OK`        | `✓` = correct, `✗` = wrong answer, `T` / `T+OVF` = timeout (with HW overflow flag), `E` = error |
| `DEC` `CNFL` `LRN` `PROP` | decisions, conflicts, learnt clauses, propagations (parsed from `cdcl_solver` stderr) |
| `BCP` `UNDO` `LOAD`       | hardware-op counts |
| `CYC/P`     | `hw_cycles / propagations` — Fmax-independent architecture FOM |
| `HW_ns`     | projected HW time at 1 GHz (`(load+bcp+undo) × 1 ns`) |
| `SW_us`     | pure CDCL software time, Verilator overhead removed |
| `TOT_us`    | `HW_ns + SW_us` — projected total coprocessor solve time |
| `KISSAT_us` `MINISAT2_us` `GLUCOSE_us` | each solver's self-reported CPU time |
| `WIN`       | `HW` if `TOT_us` beats every SW solver; otherwise the winning SW solver name |

The trailing distribution block (`min / p50 / mean / p95 / max`) summarizes each metric across the suite, which is the headline number the final report cites.

### What the cycle counts come from

`cdcl_solver` tracks `n_hw_load`, `n_hw_bcp`, `n_hw_undo` and emits a `c hw_cycles=… hw_time=…ns sw_time=…ns` line that `bench_cdcl.py` parses. The cycle weights come from the [src/sat_submodule.sv](src/sat_submodule.sv) FSM:

| Op | Cycles | FSM path |
|----|--------|----------|
| LOAD | 2 | `S_IDLE → S_LOAD → S_IDLE` |
| BCP  | 3 | `S_IDLE → S_BCP1 → S_BCP2 → S_IDLE` |
| UNDO | 2 | `S_IDLE → S_UNDO → S_IDLE` |

Total `hw_cycles = 2·hw_load + 3·hw_bcp + 2·hw_undo`. **`CYC/P = hw_cycles / propagations`** is the headline figure-of-merit — directly comparable to numbers reported by published ASIC SAT-solver papers, independent of Fmax and process node. Once TSMC 65 nm synthesis lands (see [benchmarkingplan.md](benchmarkingplan.md)), `hw_time = hw_cycles / Fmax` converts cycles to wall-clock.

### Why four solver submodules

- **MiniSat** is the algorithmic peer — our software loop implements the same 1-UIP analysis and VSIDS heuristic. If our decision/conflict/propagation counts diverge wildly from MiniSat's, that's a bug in our CDCL, not a property of the hardware.
- **Glucose** stresses LBD-driven clause management; tells us whether more aggressive learnt-clause policies would buy anything at our capacity.
- **Kissat** is the SOTA upper bound — anything Kissat beats us *and* MiniSat at indicates algorithmic ceiling, not hardware ceiling.
- **minisat2** is a second MiniSat slot so a different revision or build flag set can be evaluated in parallel without disturbing the primary baseline.

### One-off solve

For ad-hoc runs without the full harness:

```bash
./coprocessing/cdcl_solver tests/satlib/uf20-91/uf20-01.cnf
./coprocessing/cdcl_solver tests/satlib/uf50-218/uf50-01.cnf
./coprocessing/cdcl_solver tests/satlib/uuf50-218/uuf50-01.cnf
```

Note: the pre-generated Verilator model is sized for `NUM_CLAUSES = 16`; SATLIB instances exceeding this need a Verilator regeneration at a larger `NUM_CLAUSES` (see [§ Regenerating the Verilated model](#regenerating-the-verilated-model)). `bench_cdcl.py` flags overflow cases with `T+OVF` in the `OK` column.

---

## Generating Test Problems

`gen_cnf.py` generates random 3-SAT instances in DIMACS format:

```bash
# Single problem to stdout
python3 gen_cnf.py --vars 10 --clauses 40 --stdout

# Batch: 20 problems with 7 vars at the phase-transition ratio (~4.27)
python3 gen_cnf.py --vars 7 --count 20 --seed 42 -o problems/

# Then solve one
coprocessing/cdcl_solver problems/problem_001.cnf
```

---

## Python Reference Model

`dpll.py` is a behavioral model of the RTL semantics (matchline-gated UP, `CONF > UP > DONE` priority, lowest-index clause wins) with a DPLL solver on top:

```bash
python3 dpll.py
```

`dpll.py` and `gen_tb.py` are aligned with the live RTL: 4 literals per clause, `{VAL_ZERO=2'b00, VAL_ONE=2'b11, VAL_U=2'b01}` encoding, DIMACS 1-indexed VIDs with VID 0 reserved for clause padding.

---

## Team

- **Ansh Shah | University of California, Berkeley**
- **Evan Wong | University of California, Berkeley**

UC Berkeley — Spring 2026
