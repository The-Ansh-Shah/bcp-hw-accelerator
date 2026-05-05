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

# MiniSat 2.2
( cd minisat  && make -j )                           # → minisat/build/release/bin/minisat
( cd minisat2 && make -j )                           # → minisat2/build/release/bin/minisat
```

After these complete, the binaries [kissat/build/kissat](kissat/build/kissat), [glucose/simp/glucose](glucose/simp/glucose), [minisat/build/release/bin/minisat](minisat/build/release/bin/minisat), and [minisat2/build/release/bin/minisat](minisat2/build/release/bin/minisat) are what the benchmark harness in [§ Benchmarking](#benchmarking) drives.

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
│       ├── run_satlib.py
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

The `tests/satlib/` directory ships three Uniform Random-3-SAT collections from the [SATLIB Benchmark Problems page](https://www.cs.ubc.ca/~hoos/SATLIB/benchm.html):

| Set | Variables | Clauses | Instances | Expected | Notes |
|-----|-----------|---------|-----------|----------|-------|
| `uf20-91`   |  20 |  91 | 1000 | SAT   | Reliably-runnable through the RTL trace flow today |
| `uf50-218`  |  50 | 218 | 1000 | SAT   | Phase-transition ratio; runnable via the coprocessor |
| `uuf50-218` |  50 | 218 | 1000 | UNSAT | Phase-transition ratio; runnable via the coprocessor |

All three sets sit at α ≈ 4.27, the SAT phase-transition ratio where random 3-SAT is hardest. `uf*` sets are crafted SAT, `uuf*` are crafted UNSAT.

### Running through the Verilog trace flow (uf20-91)

Each instance is run by re-generating `tb/sat_submodule_tb.sv` from the SATLIB `.cnf` (via `gen_tb.py`, which executes the Python DPLL model to capture an expected per-step trace) and then driving the live RTL through that trace. PASS means the RTL agrees with the DPLL reference on every BCP / UNDO step.

```bash
cd tb

make satlib                       # uf20-91 sample, default N=5, seed=0
make satlib N=10                  # 10 instances
make satlib N=20 SEED=42          # reproducible random sample

make satlib-uf20                  # alias of `make satlib`
make satlib-uf50                  # SAT, harder
make satlib-uuf50                 # UNSAT, harder
```

A 5-instance `uf20-91` sample takes ≈ 60 s and produces 12 000+ per-step `PASS` checks across the trace. Each instance regenerates the testbench, which forces a VCS recompile (≈ 5 s) plus simulation.

`uf50-218` and `uuf50-218` archives are checked in but not runnable through the trace flow yet — `dpll.py` does not drive matchline-gated UP back into its decision loop, so 50-variable instances explode the search and `gen_tb.py` does not return in reasonable time. Run them via the coprocessor instead (after rebuilding Verilator at sufficient `NUM_CLAUSES`).

### Running through the coprocessor

```bash
./coprocessing/cdcl_solver tests/satlib/uf20-91/uf20-01.cnf
./coprocessing/cdcl_solver tests/satlib/uf50-218/uf50-01.cnf
./coprocessing/cdcl_solver tests/satlib/uuf50-218/uuf50-01.cnf
```

---

## Benchmarking

The coprocessor is compared against three software CDCL solvers (Kissat, Glucose, MiniSat) and against itself on a hardware-cycle metric. The four solver submodules listed in [§ First-Time Setup](#first-time-setup-fresh-clone) are the baselines.

### Solvers under comparison

| Binary | Path | What it represents |
|--------|------|-------------------|
| `cdcl_solver`  | `coprocessing/cdcl_solver`           | Our HW-accelerated CDCL (this project) |
| `minisat` | `minisat/build/release/bin/minisat`  | Classic CDCL — closest peer to our software loop |
| `minisat` | `minisat2/build/release/bin/minisat` | Alternate MiniSat checkout (e.g., different build flags) |
| `glucose` | `glucose/simp/glucose`               | LBD-based CDCL, restart heuristic improvements |
| `kissat`  | `kissat/build/kissat`                | SAT-Competition SOTA reference |

All four read DIMACS CNF directly and print the standard `s SATISFIABLE` / `s UNSATISFIABLE` plus per-solver statistics. Wall-clock numbers come from `/usr/bin/time -v` (or `time` builtins); decision/conflict/propagation counts are parsed from each solver's stdout.

### Two metrics, two purposes

**1. Wall-clock (software-vs-software, today).** Run each solver on the same instance set; compare end-to-end runtime and RSS. The coprocessor's wall-clock includes Verilator simulation overhead, so it is *expected* to lose to native-C software solvers on this metric until we project HW-cycle counts through synthesized Fmax (see below). This comparison is still useful to confirm decision/conflict/propagation counts are in the same regime as MiniSat (which our 1-UIP + VSIDS loop is patterned after).

**2. Per-propagation cycle count (HW-vs-published-HW, Fmax-independent).** This is the primary defensible figure-of-merit for the hardware. The coprocessor tracks `n_hw_load`, `n_hw_bcp`, `n_hw_undo`; mapping each to its FSM cycle count from [src/sat_submodule.sv](src/sat_submodule.sv):

| Op    | Cycles | FSM path |
|-------|--------|----------|
| LOAD  | 2      | `S_IDLE → S_LOAD → S_IDLE` |
| BCP   | 3      | `S_IDLE → S_BCP1 → S_BCP2 → S_IDLE` |
| UNDO  | 2      | `S_IDLE → S_UNDO → S_IDLE` |

so total `hw_cycles = 2*hw_load + 3*hw_bcp + 2*hw_undo`. The headline figure is **`hw_cycles / propagations` ("cycles per implication")** — directly comparable to numbers reported by published ASIC SAT-solver papers and independent of Fmax / process node.

Once TSMC 65 nm synthesis completes (see [benchmarkingplan.md](benchmarkingplan.md)), `hw_time = hw_cycles / Fmax` converts the cycle counts to wall-clock for the wall-clock comparison.

### Running a single instance against every solver

```bash
INSTANCE=tests/satlib/uf20-91/uf20-01.cnf

/usr/bin/time -v ./coprocessing/cdcl_solver         "$INSTANCE"
/usr/bin/time -v ./kissat/build/kissat              "$INSTANCE"
/usr/bin/time -v ./glucose/simp/glucose             "$INSTANCE"
/usr/bin/time -v ./minisat/build/release/bin/minisat "$INSTANCE"
```

The coprocessor's stderr line

```
c originals=91 learnts=… decisions=… conflicts=… propagations=… hw_bcp=… hw_undo=… hw_load=…
```

is what feeds the cycle-per-implication metric.

### Running a benchmark suite

The simplest reproducible suite is SATLIB `uf20-91` (small enough that every solver finishes in milliseconds, large enough to give stable medians):

```bash
for cnf in tests/satlib/uf20-91/uf20-0{1,2,3,4,5}.cnf; do
  echo "=== $cnf ==="
  /usr/bin/time -f "wall=%e rss=%M"  ./coprocessing/cdcl_solver "$cnf" 2>&1 | tail -2
  /usr/bin/time -f "wall=%e rss=%M"  ./kissat/build/kissat        "$cnf" 2>&1 | tail -2
  /usr/bin/time -f "wall=%e rss=%M"  ./glucose/simp/glucose       "$cnf" 2>&1 | tail -2
done
```

Repeat 10× per instance to get stable medians (sub-millisecond runtimes are noise-dominated otherwise) and aggregate into a CSV — that CSV is what the final report draws from.

For the harder 50-variable sets:

```bash
./coprocessing/cdcl_solver tests/satlib/uf50-218/uf50-01.cnf       # SAT
./coprocessing/cdcl_solver tests/satlib/uuf50-218/uuf50-01.cnf     # UNSAT
./kissat/build/kissat       tests/satlib/uuf50-218/uuf50-01.cnf
```

Note: the coprocessor's pre-generated Verilator model is sized for `NUM_CLAUSES = 16`; instances with more original clauses than the hardware capacity require a Verilator regeneration at a larger `NUM_CLAUSES` (see [§ Regenerating the Verilated model](#regenerating-the-verilated-model)).

### Why four solver submodules

- **MiniSat** is the algorithmic peer — our software loop implements the same 1-UIP analysis and VSIDS heuristic. If the coprocessor's decision/conflict/propagation counts diverge wildly from MiniSat's, that's a bug in our CDCL, not a property of the hardware.
- **Glucose** stresses LBD-driven clause management; it tells us whether more aggressive learnt-clause policies would buy anything for instances at our capacity.
- **Kissat** is the SOTA upper bound — anything Kissat solves much faster than us *and* MiniSat indicates algorithmic ceiling, not hardware ceiling.
- **minisat2** is a second checkout slot for evaluating a different MiniSat revision or build configuration in parallel without disturbing the primary baseline.

The full methodology — instance-set curation, repeat counts, ratio-of-ratios analysis, and TSMC 65 nm synthesis numbers — lives in [benchmarkingplan.md](benchmarkingplan.md).

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
