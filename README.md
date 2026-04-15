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

## Team

- **Ansh Shah | University of California, Berkeley**
- **Evan Wong | University of California, Berkeley**

UC Berkeley — Spring 2026
