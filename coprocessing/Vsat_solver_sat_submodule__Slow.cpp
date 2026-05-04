// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_solver.h for the primary calling header

#include "Vsat_solver__pch.h"

void Vsat_solver_sat_submodule___ctor_var_reset(Vsat_solver_sat_submodule* vlSelf);

Vsat_solver_sat_submodule::Vsat_solver_sat_submodule() = default;
Vsat_solver_sat_submodule::~Vsat_solver_sat_submodule() = default;

void Vsat_solver_sat_submodule::ctor(Vsat_solver__Syms* symsp, const char* namep) {
    vlSymsp = symsp;
    vlNamep = strdup(Verilated::catName(vlSymsp->name(), namep));
    // Reset structure values
    Vsat_solver_sat_submodule___ctor_var_reset(this);
}

void Vsat_solver_sat_submodule::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

void Vsat_solver_sat_submodule::dtor() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
