// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vsat_solver.h for the primary calling header

#ifndef _VSAT_SOLVER_SAT_SUBMODULE_H_
#define _VSAT_SOLVER_SAT_SUBMODULE_H_  // guard

#include "verilated.h"
#include "Vsat_solver__Dpi.h"

//==========

class Vsat_solver__Syms;

//----------

VL_MODULE(Vsat_solver_sat_submodule) {
  public:
    
    // PORTS
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
    VL_IN16(row_addr,11,0);
    VL_IN16(cid_in,9,0);
    VL_OUT16(cid_out,9,0);
    VL_IN(vid_in,19,0);
    VL_OUT(vid_out,19,0);
    
    // LOCAL SIGNALS
    CData/*0:0*/ __PVT__tree_conf;
    CData/*0:0*/ __PVT__tree_up;
    CData/*0:0*/ __PVT__tree_done;
    CData/*1:0*/ __PVT__tree_ulp;
    CData/*2:0*/ __PVT__state;
    CData/*1:0*/ __PVT__cit_lit_idx;
    SData/*9:0*/ __PVT__tree_cid;
    SData/*11:0*/ __PVT__rd_row;
    IData/*31:0*/ __PVT__unnamedblk1__DOT__c;
    IData/*31:0*/ __PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r;
    IData/*31:0*/ __PVT__unnamedblk4__DOT__i;
    IData/*31:0*/ __PVT__unnamedblk5__DOT__i;
    IData/*31:0*/ __PVT__unnamedblk6__DOT__i;
    IData/*31:0*/ __PVT__unnamedblk8__DOT__r;
    IData/*31:0*/ __PVT__unnamedblk9__DOT__r;
    IData/*31:0*/ __PVT__unnamedblk7__DOT__r;
    IData/*19:0*/ vid_store[4096];
    CData/*0:0*/ pol_store[4096];
    CData/*1:0*/ val_store[4096];
    CData/*0:0*/ __PVT__row_valid[4096];
    CData/*0:0*/ ml_q[4096];
    CData/*0:0*/ proc_conf[1024];
    CData/*0:0*/ proc_up[1024];
    CData/*0:0*/ proc_done[1024];
    CData/*1:0*/ __PVT__proc_ulp[1024];
    CData/*3:0*/ __PVT__c_lit_u[1024];
    CData/*3:0*/ __PVT__c_lit_t[1024];
    CData/*3:0*/ __PVT__c_lit_f[1024];
    CData/*0:0*/ __PVT__c_ml_any[1024];
    CData/*0:0*/ __PVT__c_any_valid[1024];
    
    // INTERNAL VARIABLES
  private:
    Vsat_solver__Syms* __VlSymsp;  // Symbol table
  public:
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vsat_solver_sat_submodule);  ///< Copying not allowed
  public:
    Vsat_solver_sat_submodule(const char* name = "TOP");
    ~Vsat_solver_sat_submodule();
    
    // INTERNAL METHODS
    void __Vconfigure(Vsat_solver__Syms* symsp, bool first);
    static void _combo__TOP__sat_submodule__3(Vsat_solver__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _sequent__TOP__sat_submodule__1(Vsat_solver__Syms* __restrict vlSymsp);
    static void _settle__TOP__sat_submodule__2(Vsat_solver__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
