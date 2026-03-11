# HW-Accelerated SAT Solver: BCP + CDCL RTL Integration

Complete RTL integration of hardware Boolean Constraint Propagation (BCP) and Conflict-Driven Clause Learning (CDCL) into a self-contained, synthesizable hardware SAT solver. This project builds on existing hardware BCP and CDCL research and focuses on integrating the core components — BCP engine, conflict analysis unit, and VSIDS decision logic — into a unified design, measuring control complexity and area overhead of full integration.

## Directory Structure

```
├── src/                # RTL source files (Verilog)
│   ├── bcp_engine.v        # Boolean Constraint Propagation unit
│   ├── cdcl_unit.v         # Conflict-Driven Clause Learning / conflict analysis
│   ├── vsids.v             # VSIDS decision heuristic logic
│   ├── clause_database.v   # Clause storage and management
│   └── sat_solver_top.v    # Top-level integration
├── tb/                 # Testbenches
│   └── sat_solver_tb.v     # Main simulation testbench
├── test_env/           # Environment sanity check (sample VCS flow)
│   ├── sample_counter.v       # Simple 4-bit counter
│   ├── sample_counter_tb.v    # Testbench for sample counter
│   └── Makefile               # Compile & run the sample
├── tests/              # Test CNF instances
├── output/             # Simulation logs and waveforms
├── Makefile            # Build and simulation flow
└── README.md
```

## Build & Simulation

Requires Synopsys VCS. On the Berkeley EECS instructional machines (`eda-[1-4].eecs.berkeley.edu`), source the tool setup first:

```bash
source /home/ff/eecs151/eecs151source.bashrc
```

### Verify your environment (`test_env/`)

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

### Commands (main project)

| Command | Description |
|---|---|
| `make` | Compile the design |
| `make run test=<name>` | Run a single test |
| `make run-all` | Run all tests in `tests/` |
| `make clean` | Remove build artifacts |

## Team

- **Ansh Shah**
- **Evan Wong**

UC Berkeley — Spring 2025