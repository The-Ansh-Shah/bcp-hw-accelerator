VERILATOR_ROOT ?= $(shell verilator -getenv VERILATOR_ROOT 2>/dev/null)
ifeq ($(VERILATOR_ROOT),)
VERILATOR_ROOT := /usr/share/verilator
endif
VINC = $(VERILATOR_ROOT)/include

CXX      ?= g++
CXXFLAGS  = -O2 -std=c++14 -faligned-new -Wall -Wno-unused-parameter \
            -I. -I$(VINC) -I$(VINC)/vltstd

# v4 Verilator generated objects for the behavioral sat_submodule (NUM_CLAUSES=1024)
VOBJS   = Vsat_solver.o Vsat_solver_sat_submodule.o Vsat_solver__Dpi.o Vsat_solver__Syms.o
GLBOBJS = verilated.o verilated_dpi.o

cdcl_solver: cdcl_solver.o $(VOBJS) $(GLBOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

cdcl_solver.o: cdcl_solver.cpp Vsat_solver.h Vsat_solver_sat_submodule.h
	$(CXX) $(CXXFLAGS) -c -o $@ cdcl_solver.cpp

Vsat_solver.o: Vsat_solver.cpp Vsat_solver.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver.cpp

Vsat_solver_sat_submodule.o: Vsat_solver_sat_submodule.cpp Vsat_solver_sat_submodule.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver_sat_submodule.cpp

Vsat_solver__Dpi.o: Vsat_solver__Dpi.cpp Vsat_solver__Dpi.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver__Dpi.cpp

Vsat_solver__Syms.o: Vsat_solver__Syms.cpp Vsat_solver__Syms.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver__Syms.cpp

verilated.o: $(VINC)/verilated.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

verilated_dpi.o: $(VINC)/verilated_dpi.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: cdcl_solver
	./cdcl_solver ../tests/example.cnf

clean:
	rm -f cdcl_solver *.o

.PHONY: clean run
