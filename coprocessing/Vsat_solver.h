// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VSAT_SOLVER_H_
#define _VSAT_SOLVER_H_  // guard

#include "verilated.h"

//==========

class Vsat_solver__Syms;
class Vsat_solver_cam_sram_row;


//----------

VL_MODULE(Vsat_solver) {
  public:
    // CELLS
    // Public to allow access to /*verilator_public*/ items;
    // otherwise the application code can consider these internals.
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__0__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__1__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__2__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__3__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__4__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__5__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__6__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__7__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__8__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__9__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__10__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__11__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__12__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__13__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__14__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__15__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__16__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__17__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__18__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__19__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__20__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__21__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__22__KET____DOT__u_row;
    Vsat_solver_cam_sram_row* __PVT__sat_submodule__DOT__u_array__DOT__rows__BRA__23__KET____DOT__u_row;
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(op,2,0);
    VL_IN8(phase,0,0);
    VL_IN8(val_in,1,0);
    VL_IN8(pol_in,0,0);
    VL_IN8(row_addr,4,0);
    VL_IN8(cid_in,2,0);
    VL_OUT8(conf_out,0,0);
    VL_OUT8(up_out,0,0);
    VL_OUT8(done_out,0,0);
    VL_OUT8(cid_out,2,0);
    VL_OUT8(up_lit_pos,1,0);
    VL_OUT8(pol_out,0,0);
    VL_OUT8(valid_out,0,0);
    VL_IN(vid_in,19,0);
    VL_OUT(vid_out,19,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ sat_submodule__DOT__search_en;
        CData/*0:0*/ sat_submodule__DOT__cam_act_wr;
        CData/*0:0*/ sat_submodule__DOT__addr_wr_en;
        CData/*0:0*/ sat_submodule__DOT__addr_rd_en;
        CData/*4:0*/ sat_submodule__DOT__arr_row_addr;
        CData/*1:0*/ sat_submodule__DOT__arr_val_in;
        CData/*0:0*/ sat_submodule__DOT__arr_pol_out;
        CData/*0:0*/ sat_submodule__DOT__tree_conf;
        CData/*0:0*/ sat_submodule__DOT__tree_up;
        CData/*0:0*/ sat_submodule__DOT__tree_done;
        CData/*2:0*/ sat_submodule__DOT__tree_cid;
        CData/*1:0*/ sat_submodule__DOT__tree_ulp;
        CData/*2:0*/ sat_submodule__DOT__state;
        CData/*1:0*/ sat_submodule__DOT__cit_lit_idx;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__0__KET____DOT__u_proc__DOT__lit_true;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__0__KET____DOT__u_proc__DOT__lit_false;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__0__KET____DOT__u_proc__DOT__lit_unassigned;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__1__KET____DOT__u_proc__DOT__lit_true;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__1__KET____DOT__u_proc__DOT__lit_false;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__1__KET____DOT__u_proc__DOT__lit_unassigned;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__2__KET____DOT__u_proc__DOT__lit_true;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__2__KET____DOT__u_proc__DOT__lit_false;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__2__KET____DOT__u_proc__DOT__lit_unassigned;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__3__KET____DOT__u_proc__DOT__lit_true;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__3__KET____DOT__u_proc__DOT__lit_false;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__3__KET____DOT__u_proc__DOT__lit_unassigned;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__4__KET____DOT__u_proc__DOT__lit_true;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__4__KET____DOT__u_proc__DOT__lit_false;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__4__KET____DOT__u_proc__DOT__lit_unassigned;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__5__KET____DOT__u_proc__DOT__lit_true;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__5__KET____DOT__u_proc__DOT__lit_false;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__5__KET____DOT__u_proc__DOT__lit_unassigned;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__6__KET____DOT__u_proc__DOT__lit_true;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__6__KET____DOT__u_proc__DOT__lit_false;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__6__KET____DOT__u_proc__DOT__lit_unassigned;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__7__KET____DOT__u_proc__DOT__lit_true;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__7__KET____DOT__u_proc__DOT__lit_false;
        CData/*2:0*/ sat_submodule__DOT__proc__BRA__7__KET____DOT__u_proc__DOT__lit_unassigned;
        IData/*19:0*/ sat_submodule__DOT__arr_vid_out;
        CData/*0:0*/ sat_submodule__DOT__pol_stored[24];
        CData/*1:0*/ sat_submodule__DOT__val_stored[24];
        CData/*0:0*/ sat_submodule__DOT__matchlines[24];
        CData/*0:0*/ sat_submodule__DOT__matchlines_q[24];
        CData/*0:0*/ sat_submodule__DOT__proc_conf[8];
        CData/*0:0*/ sat_submodule__DOT__proc_up[8];
        CData/*0:0*/ sat_submodule__DOT__proc_done[8];
        CData/*1:0*/ sat_submodule__DOT__proc_ulp[8];
        CData/*1:0*/ sat_submodule__DOT__proc__BRA__0__KET____DOT__pval[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__0__KET____DOT__ppol[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__0__KET____DOT__pml[3];
        CData/*1:0*/ sat_submodule__DOT__proc__BRA__1__KET____DOT__pval[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__1__KET____DOT__ppol[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__1__KET____DOT__pml[3];
        CData/*1:0*/ sat_submodule__DOT__proc__BRA__2__KET____DOT__pval[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__2__KET____DOT__ppol[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__2__KET____DOT__pml[3];
        CData/*1:0*/ sat_submodule__DOT__proc__BRA__3__KET____DOT__pval[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__3__KET____DOT__ppol[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__3__KET____DOT__pml[3];
        CData/*1:0*/ sat_submodule__DOT__proc__BRA__4__KET____DOT__pval[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__4__KET____DOT__ppol[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__4__KET____DOT__pml[3];
        CData/*1:0*/ sat_submodule__DOT__proc__BRA__5__KET____DOT__pval[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__5__KET____DOT__ppol[3];
    };
    struct {
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__5__KET____DOT__pml[3];
        CData/*1:0*/ sat_submodule__DOT__proc__BRA__6__KET____DOT__pval[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__6__KET____DOT__ppol[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__6__KET____DOT__pml[3];
        CData/*1:0*/ sat_submodule__DOT__proc__BRA__7__KET____DOT__pval[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__7__KET____DOT__ppol[3];
        CData/*0:0*/ sat_submodule__DOT__proc__BRA__7__KET____DOT__pml[3];
        CData/*0:0*/ sat_submodule__DOT__u_array__DOT__cam_wr_sel[24];
        CData/*0:0*/ sat_submodule__DOT__u_array__DOT__cam_rd_sel[24];
        CData/*0:0*/ sat_submodule__DOT__u_array__DOT__sram_wr_sel[24];
        CData/*0:0*/ sat_submodule__DOT__u_array__DOT__sram_rd_sel[24];
        IData/*19:0*/ sat_submodule__DOT__u_array__DOT__row_vid_out[24];
        CData/*0:0*/ sat_submodule__DOT__u_array__DOT__row_pol_out[24];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ sat_submodule__DOT____Vlvbound1;
    CData/*0:0*/ sat_submodule__DOT__u_array__DOT____Vlvbound1;
    CData/*0:0*/ sat_submodule__DOT__u_array__DOT____Vlvbound2;
    CData/*0:0*/ sat_submodule__DOT__u_array__DOT____Vlvbound3;
    CData/*0:0*/ sat_submodule__DOT__u_array__DOT____Vlvbound4;
    CData/*0:0*/ __Vclklast__TOP__clk;
    CData/*0:0*/ __Vclklast__TOP__rst_n;
    CData/*0:0*/ sat_submodule__DOT____Vcellout__u_array__matchlines[24];
    CData/*1:0*/ sat_submodule__DOT____Vcellout__u_array__val_stored[24];
    CData/*0:0*/ sat_submodule__DOT____Vcellout__u_array__pol_stored[24];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__u_combine__up_lit_pos_in[8];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__u_combine__done_in[8];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__u_combine__up_in[8];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__u_combine__conf_in[8];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__0__KET____DOT__u_proc__ml[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__0__KET____DOT__u_proc__pol[3];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__0__KET____DOT__u_proc__val[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__1__KET____DOT__u_proc__ml[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__1__KET____DOT__u_proc__pol[3];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__1__KET____DOT__u_proc__val[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__2__KET____DOT__u_proc__ml[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__2__KET____DOT__u_proc__pol[3];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__2__KET____DOT__u_proc__val[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__3__KET____DOT__u_proc__ml[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__3__KET____DOT__u_proc__pol[3];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__3__KET____DOT__u_proc__val[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__4__KET____DOT__u_proc__ml[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__4__KET____DOT__u_proc__pol[3];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__4__KET____DOT__u_proc__val[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__5__KET____DOT__u_proc__ml[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__5__KET____DOT__u_proc__pol[3];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__5__KET____DOT__u_proc__val[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__6__KET____DOT__u_proc__ml[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__6__KET____DOT__u_proc__pol[3];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__6__KET____DOT__u_proc__val[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__7__KET____DOT__u_proc__ml[3];
    CData/*0:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__7__KET____DOT__u_proc__pol[3];
    CData/*1:0*/ sat_submodule__DOT____Vcellinp__proc__BRA__7__KET____DOT__u_proc__val[3];
    CData/*1:0*/ __Vtablechg1[512];
    static CData/*2:0*/ __Vtable1_sat_submodule__DOT__state[512];
    static CData/*1:0*/ __Vtable1_sat_submodule__DOT__cit_lit_idx[512];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vsat_solver__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vsat_solver);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vsat_solver(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vsat_solver();
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vsat_solver__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vsat_solver__Syms* symsp, bool first);
  private:
    static QData _change_request(Vsat_solver__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__4(Vsat_solver__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vsat_solver__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vsat_solver__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vsat_solver__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vsat_solver__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vsat_solver__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _settle__TOP__3(Vsat_solver__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
