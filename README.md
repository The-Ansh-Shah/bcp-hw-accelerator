# HW-Accelerated SAT Solver: BCP + CDCL RTL Integration

Complete RTL integration of hardware Boolean Constraint Propagation (BCP) and Conflict-Driven Clause Learning (CDCL) into a self-contained, synthesizable hardware SAT solver. This project builds on existing hardware BCP and CDCL research and focuses on integrating the core components — BCP engine, conflict analysis unit, and VSIDS decision logic — into a unified design, measuring control complexity and area overhead of full integration.

## Directory Structure

```
├── src/                # RTL source files (Verilog)
│   ├── hw_bcp_defs.vh      # HW-BCP shared `defines (CE status, literals, FSM states)
│   ├── hw_bcp_propagate.v  # HW-BCP BCP FSM (trail → CAM → clause eval → SRAM → implications)
│   └── hw_bcp_sim_stubs.v  # Simulation stubs for CAM / SRAM / CE / trail (ifdef HW_BCP_SIM_STUBS)
├── sim/                # HW-BCP-only simulation sandbox
│   ├── tb/tb_hw_bcp.sv      # HW-BCP testbench (SV)
│   └── Makefile             # Compile & run (see HW-BCP sandbox below)
├── test_env/           # Environment sanity check (sample VCS flow)
│   ├── sample_counter.v       # Simple 4-bit counter
│   ├── sample_counter_tb.v    # Testbench for sample counter
│   └── Makefile               # Compile & run the sample
└── README.md
```

## Build & Simulation

Requires Synopsys VCS.

On the Berkeley EECS instructional machines (`eda-[1-4].eecs.berkeley.edu`), source the tool setup first:

```bash
source /home/ff/eecs151/eecs151source.bashrc
```

### Environment sanity check (`test_env/`)

The `test_env/` directory contains a simple 4-bit counter and testbench to confirm VCS works. The Makefile sources the environment automatically, so no manual setup is needed.

```bash
cd test_env
make clean && make run
```

You should see `ALL TESTS PASSED`. This also produces a `waves.vpd` waveform dump. To view it, RDP into the machine and run:

```bash
dve -vpd waves.vpd &
```

Run `make clean` to remove all build artifacts.

### HW-BCP sandbox (`sim/`)

`sim/` is for iterating on the HW-BCP skeleton only, using placeholder CAM, SRAM, clause-eval, and trail models so the control FSM can be simulated without full peripheral RTL.

**RTL layout (`src/`):**

| File | Purpose |
|------|---------|
| `hw_bcp_defs.vh` | Shared `defines: clause-eval status codes, 2-bit literal values, and FSM state encodings. |
| `hw_bcp_propagate.v` | `hw_bcp_propagate` module: Boolean Constraint Propagation FSM and port-level handshakes to trail, CAM, SRAM, clause evaluator, conflict, and unit-propagation outputs. |
| `hw_bcp_sim_stubs.v` | Combinational/behavioral stubs (`stub_trail`, `stub_cam`, `stub_sram_litval`, `stub_clause_eval`), compiled only when `HW_BCP_SIM_STUBS` is defined. |

The `sim/Makefile` compiles `hw_bcp_propagate.v` and `hw_bcp_sim_stubs.v`, adds `+incdir+../src` for `` `include "hw_bcp_defs.vh" ``, and defines `HW_BCP_SIM_STUBS` so the stubs are elaborated. The testbench is `sim/tb/tb_hw_bcp.sv` (top `tb_hw_bcp`).

```bash
cd sim
make clean && make run
```

**Tests covered by `tb_hw_bcp.sv`:**

| Test | What it checks |
|------|----------------|
| 1 — idle | `busy=0`, `done=0` before `start` is ever asserted |
| 2 — zero trail | FSM goes directly to DONE (no pop) when `trail_size == qhead_init` |
| 3 — nonempty trail (n=1, n=4) | FSM completes all pops; `up_valid` and `conflict_valid` stay low when CE returns CLAUSE_DONE |
| 4 — done/start | `done` holds while `start=1`; deasserts one cycle after `start=0` |
| 5 — unit propagation | CE returns CLAUSE_UNIT → `up_valid` asserted with correct literal; no conflict |
| 6 — conflict detection | CE returns CLAUSE_CONFLICT → `conflict_valid` asserted; no unit implication |

Waveforms are requested via `+vcdplusfile=waves.vpd`, but VCS may emit `vcdplus.vpd` depending on configuration. To view whichever file was produced:

```bash
ls *.vpd
dve -vpd vcdplus.vpd &
```

Run `make clean` to remove all build artifacts.

## Clause Learning: Current Limitations and Proposed Hardware Extensions

`hw_bcp_propagate` is a pure BCP engine. It handles unit propagation correctly but has no support for clause learning (the "CL" in CDCL). There are four structural gaps:

### 1. No implication graph traversal

Clause learning requires walking the implication graph backwards from the conflict to find the **1-UIP** (first Unique Implication Point). The module emits `conflict_clause` (the falsified clause ID, i.e. the conflict node) and `up_reason_clause` alongside every implication (the graph edges), so the raw data is present in the system — but the module performs no analysis on it. It stops at `S_REPORT_CONFL` and asserts `done`.

### 2. CAM has no write interface

The CAM is accessed read-only from this module (`cam_q_*` is a query, `cam_r_*` is a response stream). A learned clause must be **added to the watch lists** of its two watched literals. There is no write port on the CAM in the current design.

### 3. No backjump / trail unwind

After identifying the assertion level (the second-highest decision level in the learned clause), the solver must **undo assignments** back to that level. The trail interface exposes only a forward `pop_req`/`pop_valid` pair — there is no `truncate_to_level` port.

### 4. No decision-level metadata

The SRAM stores 2-bit literal values (`VAL0`, `VALU`, `VAL1`) with no level annotation. Knowing *which* assignments to undo during backjumping requires each assignment to carry its decision level. That metadata is absent from the current SRAM model.

### Proposed extensions

`hw_bcp_propagate` is the right BCP component and should remain unchanged. The following modules are added around it, coordinated by a top-level sequencer:

```
          ┌──────────────────────────────────────────────┐
          │              hw_cdcl_top (sequencer)         │
          │                                              │
          │  ┌────────────┐    ┌──────────────────────┐  │
          │  │  hw_bcp_   │    │  hw_conflict_analyze │  │
start ───►│  │  propagate │───►│  (1-UIP computation) │  │
          │  └────────────┘    └──────────┬───────────┘  │
          │       ▲ ▼               learned clause        │
          │  trail/CAM/SRAM    ┌──────────▼───────────┐  │
          │   (shared)         │  hw_clause_add       │  │
          │                    │  (CAM write + watch  │  │
          │                    │   list registration) │  │
          │                    └──────────────────────┘  │
          └──────────────────────────────────────────────┘
```

**`hw_conflict_analyze`** — new module driven by `conflict_clause` + trail (with reason clauses). Iteratively resolves the conflict clause against the reason clauses of implied literals at the current decision level until only one current-level literal remains (the 1-UIP). Outputs the learned clause literals and the assertion level.

**CAM write port** — add `cam_wr_valid`, `cam_wr_var`, `cam_wr_clause_id` to the CAM. Learned clauses watch their two highest-level literals; `hw_clause_add` issues two write transactions to register those watch-list entries. The top-level sequencer arbitrates CAM access between BCP reads and learn writes.

**Trail `truncate_to_level` port** — add a `backjump_level` input and `truncate_req`/`truncate_done` handshake to the trail memory. On receipt, the trail resets `trail_size` to the index of the last literal assigned at or below `backjump_level`. The sequencer then issues `sram_wr_valid` with `VAL = VALU` for every literal above that index to clear assignments.

**SRAM level tagging** — add a parallel level SRAM (width `LEVEL_W`, same index as the literal SRAM) written in lockstep with `sram_wr_val`. The conflict analyzer reads this to determine which literals belong to the current decision level during 1-UIP traversal.

**Top-level sequencer state machine:**

```
DECIDE → BCP_RUN → (done, no conflict) → DECIDE
                 ↘ (conflict)          → CONFLICT_ANALYZE
                                            ↘ (level 0) → UNSAT
                                            → BACKJUMP + CLAUSE_ADD → BCP_RUN
```

---

## SW/HW Co-Design for Clause Learning

Rather than implementing full CDCL in RTL, a practical alternative is to **offload BCP to hardware** while keeping the higher-level CDCL logic (decisions, conflict analysis, clause management, restarts) in a host software solver. This section describes that interface.

### Division of responsibility

| Responsibility | Hardware (`hw_bcp_propagate`) | Software (CDCL solver) |
|---|---|---|
| Unit propagation to fixpoint | Yes — walks the trail, evaluates watched clauses, writes `VAL1` to SRAM | Invokes HW repeatedly until no new implications |
| Decision making | No | Picks next unassigned variable (e.g. VSIDS) |
| Conflict analysis / 1-UIP | No | Reads reason trail, computes learned clause |
| Clause learning + storage | No | Manages clause database; writes new clauses to CAM and clause store |
| Backjumping | No | Resets trail pointer, clears SRAM assignments, restores level metadata |
| Restart policy | No | Decides when to restart |

### Memory-mapped interface

The trail memory and SRAM are dual-ported (or time-multiplexed with arbitration): software writes new entries; hardware reads them during BCP. Software uses a set of memory-mapped control registers to drive the hardware:

| Register / Signal | Direction | Description |
|---|---|---|
| `CTRL.start` | SW → HW | Pulse to begin a BCP pass |
| `CTRL.qhead_init` | SW → HW | Trail index at which this BCP pass should begin |
| `CTRL.trail_size` | SW → HW | Current end of trail (exclusive upper bound for this pass) |
| `STATUS.busy` | HW → SW | High while BCP is running |
| `STATUS.done` | HW → SW | High when the pass completes |
| `STATUS.conflict_valid` | HW → SW | High if a conflict was detected |
| `STATUS.conflict_clause` | HW → SW | Clause ID of the falsified clause |
| `IMPL_FIFO.up_lit` | HW → SW | Each implied literal emitted during propagation |
| `IMPL_FIFO.up_reason_clause` | HW → SW | Reason clause for the implication (implication graph edge) |
| `IMPL_FIFO.up_lit_idx` | HW → SW | Position of the literal within its clause |

### BCP-to-fixpoint loop (software side)

The hardware processes exactly one pass over `[qhead_init, trail_size)`. To reach a BCP fixpoint the software must iterate:

```python
while True:
    hw.qhead_init = qhead
    hw.trail_size = len(trail)
    hw.start = 1

    wait_for(hw.done)

    if hw.conflict_valid:
        conflict_clause = hw.conflict_clause
        break  # handle conflict: analyze, learn, backjump

    # Collect new implications emitted during this pass
    new_lits = drain(hw.IMPL_FIFO)
    if not new_lits:
        break  # fixpoint reached — no new implications

    for lit, reason, idx in new_lits:
        trail.append((lit, reason, idx))
    qhead = hw.qhead_init + (hw.trail_size - hw.qhead_init)
    # next iteration propagates the newly added literals
```

### Conflict analysis and clause learning (software side)

When `conflict_valid` is asserted, the software performs 1-UIP conflict analysis using the reason clauses stored with each trail entry (surfaced via `up_reason_clause` and the software-maintained reason trail). It then:

1. Computes the learned clause and assertion level
2. Writes the new clause literals into the clause store
3. Registers the clause's two watched literals in the CAM via a write port (or a separate software-managed learned-clause list that the hardware CE checks)
4. Backtracks: truncates the trail to the assertion level and issues SRAM writes (via `sram_wr_valid`) to reset each unassigned literal to `VALU`
5. Resumes the BCP loop from the new trail end

### Key interface invariants

- Software must not modify the trail or SRAM while `busy` is high.
- `up_reason_clause` must be written to the software-side reason trail in the same order as `up_lit`, so the implication graph is reconstructable during conflict analysis.
- CAM writes from software (clause learning) and CAM reads from hardware (BCP) must be serialized — the simplest policy is to allow CAM writes only when `busy = 0`.
- The hardware never removes clause watch-list entries; clause deletion (if implemented) is software's responsibility and requires a CAM invalidation mechanism.

---

## Team

- **Ansh Shah | University of California, Berkeley**
- **Evan Wong | University of California, Berkeley**

UC Berkeley — Spring 2026
