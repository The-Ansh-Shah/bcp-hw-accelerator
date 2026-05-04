// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VSAT_SOLVER__SYMS_H_
#define _VSAT_SOLVER__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vsat_solver.h"
#include "Vsat_solver_sat_submodule.h"

// DPI TYPES for DPI Export callbacks (Internal use)

// SYMS CLASS
class Vsat_solver__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vsat_solver*                   TOPp;
    Vsat_solver_sat_submodule      TOP__sat_submodule;
    
    // SCOPE NAMES
    VerilatedScope __Vscope_sat_submodule;
    
    // CREATORS
    Vsat_solver__Syms(Vsat_solver* topp, const char* namep);
    ~Vsat_solver__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
