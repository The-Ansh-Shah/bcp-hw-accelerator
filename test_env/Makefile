SRC_DIR = src
TB_DIR  = tb
TEST_DIR = tests
OUT_DIR  = output

# List your RTL source files
VERILOG_SRCS = \
    $(SRC_DIR)/bcp_engine.v \
    $(SRC_DIR)/cdcl_unit.v \
    $(SRC_DIR)/vsids.v \
    $(SRC_DIR)/clause_database.v \
    $(SRC_DIR)/sat_solver_top.v

TB_SRC = $(TB_DIR)/sat_solver_tb.v

SIMV = simv

VCS_OPTS = \
    -full64 \
    -sverilog \
    +v2k \
    +incdir+$(SRC_DIR) \
    +vcs+initreg+random \
    -timescale=1ns/1ps \
    -debug_access+all \
    +lint=all,noVCDE,noONGS,noUI \
    +warn=noTMR

# Compile
$(SIMV): $(VERILOG_SRCS) $(TB_SRC)
	vcs $(VCS_OPTS) -top sat_solver_tb $(VERILOG_SRCS) $(TB_SRC) -o $(SIMV)

# Run a specific test:  make run test=simple_3sat
test ?= simple_3sat
timeout_cycles ?= 100000

run: $(SIMV)
	mkdir -p $(OUT_DIR)
	./$(SIMV) +loadcnf=$(TEST_DIR)/$(test).cnf +max-cycles=$(timeout_cycles) 2>&1 | tee $(OUT_DIR)/$(test).out

# Run all tests
ALL_TESTS = $(basename $(notdir $(wildcard $(TEST_DIR)/*.cnf)))

run-all: $(SIMV)
	mkdir -p $(OUT_DIR)
	@for t in $(ALL_TESTS); do \
	    echo "=== Running $$t ==="; \
	    ./$(SIMV) +loadcnf=$(TEST_DIR)/$$t.cnf +max-cycles=$(timeout_cycles) 2> $(OUT_DIR)/$$t.out; \
	    perl -ne 'print "  [$$1] $$ARGV\t$$2\n" if /\*{3}(.{8})\*{3}(.*)/' $(OUT_DIR)/$$t.out; \
	done

# Waveform dump for debugging
dvrun: $(SIMV)
	mkdir -p $(OUT_DIR)
	./$(SIMV) +loadcnf=$(TEST_DIR)/$(test).cnf +max-cycles=$(timeout_cycles) +vcdplusfile=$(OUT_DIR)/$(test).vpd 2>&1 | tee $(OUT_DIR)/$(test).out
	dve -full64 -vpd $(OUT_DIR)/$(test).vpd &

clean:
	rm -rf $(SIMV) $(SIMV).daidir csrc $(OUT_DIR) ucli.key DVEfiles
