# sim Makefile — HW-BCP-only simulation flow (mirrors test_env/Makefile)
SHELL = /bin/bash
ENV   = source ../eecs151source.bashrc &&

SRC_DIR = ../src
TB_DIR  = tb

SRCS = $(SRC_DIR)/hw_bcp.sv
TB   = $(TB_DIR)/tb_hw_bcp.sv
SIMV = simv

VCS_OPTS = \
    -full64 \
    -sverilog \
    +v2k \
    -timescale=1ns/1ps \
    -debug_access+all \
    +lint=all,noVCDE,noONGS,noUI \
    +warn=noTMR \
    +define+HW_BCP_SIM_STUBS

# Compile
$(SIMV): $(SRCS) $(TB)
	$(ENV) vcs $(VCS_OPTS) -top tb_hw_bcp $(SRCS) $(TB) -o $(SIMV)

# Run simulation and produce waveform dump
run: $(SIMV)
	$(ENV) ./$(SIMV) +vcdplusfile=waves.vpd

clean:
	# NFS can leave behind .nfs* files that make `rm -rf DVEfiles` fail.
	rm -rf $(SIMV) $(SIMV).daidir csrc ucli.key *.vpd *.vcd *.log
	rm -rf DVEfiles/* 2>/dev/null || true
	rmdir DVEfiles 2>/dev/null || true

