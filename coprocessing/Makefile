VERILATOR_ROOT ?= $(shell verilator -getenv VERILATOR_ROOT 2>/dev/null)
ifeq ($(VERILATOR_ROOT),)
VERILATOR_ROOT := /usr/share/verilator
endif
VINC = $(VERILATOR_ROOT)/include

CXX      ?= g++
CXXFLAGS  = -O2 -std=c++14 -Wall -Wno-unused-parameter \
            -I. -I$(VINC) -I$(VINC)/vltstd

# Flat behavioral model → no per-submodule class (no cam_sram_row file).
# After re-verilating on the server, check which .cpp files were generated
# and update VOBJS accordingly.  The two files below cover Verilator 4.x;
# Verilator 5.x splits into more files (see Vsat_solver_classes.mk).
VOBJS   = Vsat_solver.o Vsat_solver__Syms.o
GLBOBJS = verilated.o

cdcl_solver: cdcl_solver.o $(VOBJS) $(GLBOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

cdcl_solver.o: cdcl_solver.cpp Vsat_solver.h
	$(CXX) $(CXXFLAGS) -c -o $@ cdcl_solver.cpp

Vsat_solver.o: Vsat_solver.cpp Vsat_solver.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver.cpp

Vsat_solver__Syms.o: Vsat_solver__Syms.cpp Vsat_solver__Syms.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver__Syms.cpp

verilated.o: $(VINC)/verilated.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: cdcl_solver
	./cdcl_solver ../tests/example.cnf

clean:
	rm -f cdcl_solver *.o

.PHONY: clean run
