// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vsat_solver.h for the primary calling header

#ifndef VERILATED_VSAT_SOLVER_SAT_SUBMODULE_H_
#define VERILATED_VSAT_SOLVER_SAT_SUBMODULE_H_  // guard

#include "verilated.h"


class Vsat_solver__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vsat_solver_sat_submodule final {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ clk;
    CData/*0:0*/ rst_n;
    CData/*2:0*/ op;
    CData/*0:0*/ phase;
    CData/*1:0*/ val_in;
    CData/*0:0*/ pol_in;
    CData/*0:0*/ conf_out;
    CData/*0:0*/ up_out;
    CData/*0:0*/ done_out;
    CData/*1:0*/ up_lit_pos;
    CData/*0:0*/ pol_out;
    CData/*0:0*/ valid_out;
    CData/*0:0*/ __PVT__tree_conf;
    CData/*0:0*/ __PVT__tree_up;
    CData/*0:0*/ __PVT__tree_done;
    CData/*1:0*/ __PVT__tree_ulp;
    CData/*2:0*/ __PVT__state;
    CData/*1:0*/ __PVT__cit_lit_idx;
    SData/*11:0*/ row_addr;
    SData/*9:0*/ cid_in;
    SData/*9:0*/ cid_out;
    SData/*9:0*/ __PVT__tree_cid;
    SData/*11:0*/ __PVT__rd_row;
    IData/*19:0*/ vid_in;
    IData/*19:0*/ vid_out;
    IData/*31:0*/ __PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r;
    IData/*31:0*/ __PVT__unnamedblk5__DOT__i;
    IData/*31:0*/ __PVT__unnamedblk6__DOT__i;
    IData/*31:0*/ __PVT__unnamedblk8__DOT__r;
    IData/*31:0*/ __PVT__unnamedblk9__DOT__r;
    IData/*31:0*/ __PVT__unnamedblk7__DOT__r;
    VlUnpacked<IData/*19:0*/, 4096> vid_store;
    VlUnpacked<CData/*0:0*/, 4096> pol_store;
    VlUnpacked<CData/*1:0*/, 4096> val_store;
    VlUnpacked<CData/*0:0*/, 4096> __PVT__row_valid;
    VlUnpacked<CData/*0:0*/, 4096> ml_q;
    VlUnpacked<CData/*0:0*/, 1024> proc_conf;
    VlUnpacked<CData/*0:0*/, 1024> proc_up;
    VlUnpacked<CData/*0:0*/, 1024> proc_done;
    VlUnpacked<CData/*1:0*/, 1024> __PVT__proc_ulp;
    VlUnpacked<CData/*3:0*/, 1024> __PVT__c_lit_u;
    VlUnpacked<CData/*3:0*/, 1024> __PVT__c_lit_t;
    VlUnpacked<CData/*3:0*/, 1024> __PVT__c_lit_f;
    VlUnpacked<CData/*0:0*/, 1024> __PVT__c_ml_any;
    VlNBACommitQueue<VlUnpacked<CData/*1:0*/, 4096>, false, CData/*1:0*/, 1> __VdlyCommitQueueval_store;
    VlNBACommitQueue<VlUnpacked<CData/*0:0*/, 4096>, false, CData/*0:0*/, 1> __VdlyCommitQueueml_q;
    VlNBACommitQueue<VlUnpacked<IData/*19:0*/, 4096>, false, IData/*19:0*/, 1> __VdlyCommitQueuevid_store;
    VlNBACommitQueue<VlUnpacked<CData/*0:0*/, 4096>, false, CData/*0:0*/, 1> __VdlyCommitQueuepol_store;
    VlNBACommitQueue<VlUnpacked<CData/*0:0*/, 4096>, false, CData/*0:0*/, 1> __VdlyCommitQueuerow_valid;

    // INTERNAL VARIABLES
    Vsat_solver__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vsat_solver_sat_submodule();
    ~Vsat_solver_sat_submodule();
    void ctor(Vsat_solver__Syms* symsp, const char* namep);
    void dtor();
    VL_UNCOPYABLE(Vsat_solver_sat_submodule);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
