# test_env Makefile — quick sanity check that VCS works.

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
	vcs $(VCS_OPTS) -top sample_counter_tb $(SRCS) $(TB) -o $(SIMV)

# Run simulation
run: $(SIMV)
	./$(SIMV)

# Run with waveform dump
dvrun: $(SIMV)
	./$(SIMV) +vcdplusfile=waves.vpd
	dve -full64 -vpd waves.vpd &

clean:
	rm -rf $(SIMV) $(SIMV).daidir csrc ucli.key DVEfiles waves.vpd
