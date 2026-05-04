// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vsat_solver.h for the primary calling header

#ifndef VERILATED_VSAT_SOLVER___024ROOT_H_
#define VERILATED_VSAT_SOLVER___024ROOT_H_  // guard

#include "verilated.h"
class Vsat_solver_sat_submodule;


class Vsat_solver__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vsat_solver___024root final {
  public:
    // CELLS
    Vsat_solver_sat_submodule* sat_submodule;

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(op,2,0);
    VL_IN8(phase,0,0);
    VL_IN8(val_in,1,0);
    VL_IN8(pol_in,0,0);
    VL_OUT8(conf_out,0,0);
    VL_OUT8(up_out,0,0);
    VL_OUT8(done_out,0,0);
    VL_OUT8(up_lit_pos,1,0);
    VL_OUT8(pol_out,0,0);
    VL_OUT8(valid_out,0,0);
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VstlPhaseResult;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __VicoPhaseResult;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
    CData/*0:0*/ __VactPhaseResult;
    CData/*0:0*/ __VnbaPhaseResult;
    VL_IN16(row_addr,11,0);
    VL_IN16(cid_in,9,0);
    VL_OUT16(cid_out,9,0);
    VL_IN(vid_in,19,0);
    VL_OUT(vid_out,19,0);
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VicoTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vsat_solver__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vsat_solver___024root(Vsat_solver__Syms* symsp, const char* namep);
    ~Vsat_solver___024root();
    VL_UNCOPYABLE(Vsat_solver___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
