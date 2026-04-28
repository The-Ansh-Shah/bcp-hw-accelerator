# tb/Makefile — sat_submodule testbench runner (VCS)
#
# Compiles and runs the testbenches in this directory.  Each testbench
# builds into its own subdirectory under build/ so VCS's csrc/, daidir/,
# and waveform files don't collide.
#
# Quick reference (run `make help` for the full list):
#   make help            Show all targets
#   make sat             Run the original 3-SAT testbench
#   make 4sat            Run all 4-SAT sub-tests (A, B, C)
#   make 4sat TEST=A     Run only sub-test A
#   make 4sat-A          Shorthand for sub-test A
#   make all             Run every testbench
#   make wave-4sat       Open waveform from the last 4sat run
#   make clean           Remove all build artifacts

SHELL = /bin/bash
# Source the project's VCS env script automatically.  Use an absolute path
# so the rule still works after a `cd` into build/<tb>/.
BASHRC = $(abspath ../eecs151source.bashrc)
ENV    = source $(BASHRC) &&

SRC_DIR = ../src
BUILD   = build

# Common RTL sources for sat_submodule
SAT_SRCS = \
    $(SRC_DIR)/cam_cell.sv \
    $(SRC_DIR)/cam_sram_array.sv \
    $(SRC_DIR)/cam_sram_row.sv \
    $(SRC_DIR)/combining_logic.sv \
    $(SRC_DIR)/processing_logic.sv \
    $(SRC_DIR)/sat_submodule.sv \
    $(SRC_DIR)/sram_row.sv

# Hierarchy sources (sub-SAT → group → array)
ARRAY_SRCS = \
    $(SAT_SRCS) \
    $(SRC_DIR)/sat_group_submodule.sv \
    $(SRC_DIR)/sat_array.sv

ARRAY_SRCS_REL = $(addprefix ../../,$(ARRAY_SRCS))

VCS_OPTS = \
    -full64 \
    -sverilog \
    +v2k \
    +incdir+$(SRC_DIR) \
    -timescale=1ns/1ps \
    -debug_access+all \
    +lint=all,noVCDE,noONGS,noUI \
    +warn=noTMR

# Sources are referenced from inside build/<tb>/ (two levels up).
SAT_SRCS_REL = $(addprefix ../../,$(SAT_SRCS))

# Optional sub-test filter (forwarded as a +TEST=<name> plusarg to the sim).
TEST    ?=
PLUSARGS = $(if $(TEST),+TEST=$(TEST),)

.DEFAULT_GOAL := help
.PHONY: help all clean \
        sat 4sat 4sat-A 4sat-B 4sat-C array array-large \
        wave-sat wave-4sat wave-array wave-array-large

help:
	@echo "sat_submodule testbench runner"
	@echo ""
	@echo "Run a testbench:"
	@echo "  make sat              Original 3-SAT testbench (sat_submodule_tb.sv)"
	@echo "  make 4sat             All 4-SAT sub-tests (A, B, C)"
	@echo "  make 4sat TEST=A      Only sub-test A inside the 4-SAT TB"
	@echo "  make 4sat TEST=B      Only sub-test B"
	@echo "  make 4sat TEST=C      Only sub-test C"
	@echo "  make 4sat-A           Shorthand: only sub-test A"
	@echo "  make 4sat-B           Shorthand: only sub-test B"
	@echo "  make 4sat-C           Shorthand: only sub-test C"
	@echo "  make array            sat_array hierarchy smoke test (16 clauses)"
	@echo "  make array-large      sat_array large-scale integration test (4096 clauses)"
	@echo "  make all              Run every testbench"
	@echo ""
	@echo "View waveforms (after a run):"
	@echo "  make wave-sat         Open sat_submodule_tb waveform in DVE"
	@echo "  make wave-4sat        Open sat_submodule_4sat_tb waveform in DVE"
	@echo "  make wave-array       Open sat_array_tb waveform in DVE"
	@echo "  make wave-array-large Open sat_array_large_tb waveform in DVE"
	@echo ""
	@echo "Misc:"
	@echo "  make clean            Remove all build artifacts (build/)"

# ── Build rules ──────────────────────────────────────────────────────────
# Each TB compiles inside its own subdirectory.

$(BUILD)/sat/simv: sat_submodule_tb.sv $(SAT_SRCS)
	@mkdir -p $(BUILD)/sat
	$(ENV) cd $(BUILD)/sat && vcs $(VCS_OPTS) -top sat_submodule_tb \
	    $(SAT_SRCS_REL) ../../sat_submodule_tb.sv -o simv

$(BUILD)/4sat/simv: sat_submodule_4sat_tb.sv $(SAT_SRCS)
	@mkdir -p $(BUILD)/4sat
	$(ENV) cd $(BUILD)/4sat && vcs $(VCS_OPTS) -top sat_submodule_tb \
	    $(SAT_SRCS_REL) ../../sat_submodule_4sat_tb.sv -o simv

$(BUILD)/array/simv: sat_array_tb.sv $(ARRAY_SRCS)
	@mkdir -p $(BUILD)/array
	$(ENV) cd $(BUILD)/array && vcs $(VCS_OPTS) -top sat_array_tb \
	    $(ARRAY_SRCS_REL) ../../sat_array_tb.sv -o simv

$(BUILD)/array-large/simv: sat_array_large_tb.sv $(ARRAY_SRCS)
	@mkdir -p $(BUILD)/array-large
	$(ENV) cd $(BUILD)/array-large && vcs $(VCS_OPTS) -top sat_array_tb \
	    $(ARRAY_SRCS_REL) ../../sat_array_large_tb.sv -o simv

# ── Run rules ────────────────────────────────────────────────────────────

sat: $(BUILD)/sat/simv
	@echo "── Running sat_submodule_tb ──"
	@cd $(BUILD)/sat && ./simv $(PLUSARGS)

4sat: $(BUILD)/4sat/simv
	@echo "── Running sat_submodule_4sat_tb$(if $(TEST), [$(TEST)],) ──"
	@cd $(BUILD)/4sat && ./simv $(PLUSARGS)

4sat-A: $(BUILD)/4sat/simv
	@echo "── Running sat_submodule_4sat_tb [A] ──"
	@cd $(BUILD)/4sat && ./simv +TEST=A

4sat-B: $(BUILD)/4sat/simv
	@echo "── Running sat_submodule_4sat_tb [B] ──"
	@cd $(BUILD)/4sat && ./simv +TEST=B

4sat-C: $(BUILD)/4sat/simv
	@echo "── Running sat_submodule_4sat_tb [C] ──"
	@cd $(BUILD)/4sat && ./simv +TEST=C

array: $(BUILD)/array/simv
	@echo "── Running sat_array_tb (hierarchy smoke test) ──"
	@cd $(BUILD)/array && ./simv $(PLUSARGS)

array-large: $(BUILD)/array-large/simv
	@echo "── Running sat_array_large_tb (large-scale integration test) ──"
	@cd $(BUILD)/array-large && ./simv $(PLUSARGS)

# ── Waveform viewers ─────────────────────────────────────────────────────

wave-sat: $(BUILD)/sat/simv
	@cd $(BUILD)/sat && $(ENV) dve -full64 -vpd waves.vpd &

wave-4sat: $(BUILD)/4sat/simv
	@cd $(BUILD)/4sat && $(ENV) dve -full64 -vpd waves_4sat.vpd &

wave-array: $(BUILD)/array/simv
	@cd $(BUILD)/array && $(ENV) dve -full64 -vpd waves_array.vpd &

wave-array-large: $(BUILD)/array-large/simv
	@cd $(BUILD)/array-large && $(ENV) dve -full64 -vpd waves_array_large.vpd &

# ── Aggregate ────────────────────────────────────────────────────────────

# `all` runs the small sat_submodule TBs plus the small hierarchy smoke;
# `array-large` is opt-in (longer elaboration / sim time).
all: sat 4sat array

clean:
	rm -rf $(BUILD)
