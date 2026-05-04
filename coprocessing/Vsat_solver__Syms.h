// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VSAT_SOLVER__SYMS_H_
#define VERILATED_VSAT_SOLVER__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vsat_solver.h"

// INCLUDE MODULE CLASSES
#include "Vsat_solver___024root.h"
#include "Vsat_solver_sat_submodule.h"

// DPI TYPES for DPI Export callbacks (Internal use)

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vsat_solver__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vsat_solver* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vsat_solver___024root          TOP;
    Vsat_solver_sat_submodule      TOP__sat_submodule;

    // SCOPE NAMES
    VerilatedScope* __Vscopep_sat_submodule;

    // CONSTRUCTORS
    Vsat_solver__Syms(VerilatedContext* contextp, const char* namep, Vsat_solver* modelp);
    ~Vsat_solver__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
