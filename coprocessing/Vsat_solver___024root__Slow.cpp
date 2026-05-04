// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_solver.h for the primary calling header

#include "Vsat_solver__pch.h"

void Vsat_solver___024root___ctor_var_reset(Vsat_solver___024root* vlSelf);

Vsat_solver___024root::Vsat_solver___024root(Vsat_solver__Syms* symsp, const char* namep)
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vsat_solver___024root___ctor_var_reset(this);
}

void Vsat_solver___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vsat_solver___024root::~Vsat_solver___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
