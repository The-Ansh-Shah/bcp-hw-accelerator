# test_env Makefile — quick sanity check that VCS works.
SHELL = /bin/bash
ENV   = source ../eecs151source.bashrc &&

SRCS = sample_counter.v
TB   = sample_counter_tb.v
SIMV = simv

VCS_OPTS = \
    -full64 \
    -sverilog \
    +v2k \
    -timescale=1ns/1ps \
    -debug_access+all \
    +lint=all,noVCDE,noONGS,noUI \
    +warn=noTMR

# Compile
$(SIMV): $(SRCS) $(TB)
	$(ENV) vcs $(VCS_OPTS) -top sample_counter_tb $(SRCS) $(TB) -o $(SIMV)

# Run simulation and produce waveform dump
run: $(SIMV)
	$(ENV) ./$(SIMV) +vcdplusfile=waves.vpd

clean:
	rm -rf $(SIMV) $(SIMV).daidir csrc ucli.key DVEfiles waves.vpd
