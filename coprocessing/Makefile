VERILATOR_ROOT ?= /usr/share/verilator
VINC            = $(VERILATOR_ROOT)/include

CXX       ?= g++
CXXFLAGS   = -O2 -std=c++11 -Wall -Wno-unused-parameter -I. -I$(VINC) -I$(VINC)/vltstd

VOBJS   = Vsat_solver.o Vsat_solver__Syms.o Vsat_solver_cam_sram_row.o
GLBOBJS = verilated.o

cdcl_solver: cdcl_solver.o $(VOBJS) $(GLBOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

cdcl_solver.o: cdcl_solver.cpp Vsat_solver.h
	$(CXX) $(CXXFLAGS) -c -o $@ cdcl_solver.cpp

Vsat_solver.o: Vsat_solver.cpp Vsat_solver.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver.cpp

Vsat_solver__Syms.o: Vsat_solver__Syms.cpp Vsat_solver__Syms.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver__Syms.cpp

Vsat_solver_cam_sram_row.o: Vsat_solver_cam_sram_row.cpp Vsat_solver_cam_sram_row.h
	$(CXX) $(CXXFLAGS) -c -o $@ Vsat_solver_cam_sram_row.cpp

verilated.o: $(VINC)/verilated.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: cdcl_solver
	./cdcl_solver ../tests/example.cnf

clean:
	rm -f cdcl_solver *.o

.PHONY: clean run
