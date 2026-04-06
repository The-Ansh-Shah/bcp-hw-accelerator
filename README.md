# HW-Accelerated SAT Solver: BCP + CDCL RTL Integration

Complete RTL integration of hardware Boolean Constraint Propagation (BCP) and Conflict-Driven Clause Learning (CDCL) into a self-contained, synthesizable hardware SAT solver. This project builds on existing hardware BCP and CDCL research and focuses on integrating the core components — BCP engine, conflict analysis unit, and VSIDS decision logic — into a unified design, measuring control complexity and area overhead of full integration.

## Directory Structure

```
├── src/                # RTL source files (Verilog)
│   ├── bcp_engine.v        # Boolean Constraint Propagation unit
│   ├── cdcl_unit.v         # Conflict-Driven Clause Learning / conflict analysis
│   ├── vsids.v             # VSIDS decision heuristic logic
│   ├── clause_database.v   # Clause storage and management
│   ├── sat_solver_top.v    # Top-level integration
│   ├── hw_bcp_defs.vh      # HW-BCP shared `defines (CE status, literals, FSM states)
│   ├── hw_bcp_propagate.v  # HW-BCP BCP FSM (trail → CAM → clause eval → SRAM → implications)
│   └── hw_bcp_sim_stubs.v  # Optional simulation stubs for CAM / SRAM / CE / trail (ifdef)
├── tb/                 # Testbenches
│   └── sat_solver_tb.v     # Main simulation testbench
├── test_env/           # Environment sanity check (sample VCS flow)
│   ├── sample_counter.v       # Simple 4-bit counter
│   ├── sample_counter_tb.v    # Testbench for sample counter
│   └── Makefile               # Compile & run the sample
├── sim/                # HW-BCP-only simulation sandbox
│   ├── tb/tb_hw_bcp.sv      # HW-BCP skeleton testbench (SV)
│   └── Makefile             # Compile & run (see HW-BCP sandbox below)
├── tests/              # Test CNF instances
├── output/             # Simulation logs and waveforms
├── Makefile            # Build and simulation flow
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

### HW-BCP-only sandbox (`sim/`)

`sim/` is for iterating on the HW-BCP skeleton only, using placeholder CAM, SRAM, clause-eval, and trail models so the control FSM can be simulated without full peripheral RTL.

**RTL layout (`src/`):**

| File | Purpose |
|------|---------|
| `hw_bcp_defs.vh` | Included by the other HW-BCP sources: clause-eval status codes, 2-bit literal values, and FSM state encodings. |
| `hw_bcp_propagate.v` | `hw_bcp_propagate` module: Boolean Constraint Propagation FSM and port-level handshakes to trail, CAM, SRAM, clause evaluator, conflict, and unit-propagation outputs. |
| `hw_bcp_sim_stubs.v` | Combinational/behavioral stubs (`stub_trail`, `stub_cam`, `stub_sram_litval`, `stub_clause_eval`), compiled only when the Verilog macro `HW_BCP_SIM_STUBS` is defined, so real CAM/SRAM/CE/trail RTL can replace them in other flows. |

The `sim/Makefile` compiles `hw_bcp_propagate.v` and `hw_bcp_sim_stubs.v`, adds `+incdir+../src` for `` `include "hw_bcp_defs.vh" ``, and defines `HW_BCP_SIM_STUBS` so the stubs are elaborated. The testbench is `sim/tb/tb_hw_bcp.sv` (top `tb_hw_bcp`).

```bash
cd sim
make clean && make run
```

Waveforms are requested via `+vcdplusfile=waves.vpd`, but VCS may emit `vcdplus.vpd` depending on configuration. To view whichever file was produced:

```bash
ls *.vpd
dve -vpd vcdplus.vpd &
```

### Integrated solver commands (main project)

| Command | Description |
|---|---|
| `make` | Compile the design |
| `make run test=<name>` | Run a single test |
| `make run-all` | Run all tests in `tests/` |
| `make clean` | Remove build artifacts |

## Team

- **Ansh Shah | University of California, Berkeley**
- **Evan Wong | University of California, Berkeley**

UC Berkeley — Spring 2026