// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vsat_solver.h for the primary calling header

#ifndef _VSAT_SOLVER_CAM_SRAM_ROW_H_
#define _VSAT_SOLVER_CAM_SRAM_ROW_H_  // guard

#include "verilated.h"

//==========

class Vsat_solver__Syms;

//----------

VL_MODULE(Vsat_solver_cam_sram_row) {
  public:
    
    // PORTS
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(cam_wr_en,0,0);
    VL_IN8(cam_rd_en,0,0);
    VL_IN8(cam_search_en,0,0);
    VL_IN8(sram_wr_en,0,0);
    VL_IN8(sram_rd_en,0,0);
    VL_IN8(cam_act_wr,0,0);
    VL_IN8(pol_in,0,0);
    VL_IN8(val_in,1,0);
    VL_OUT8(pol_out,0,0);
    VL_OUT8(val_out,1,0);
    VL_OUT8(pol_stored,0,0);
    VL_OUT8(val_stored,1,0);
    VL_OUT8(matchline,0,0);
    VL_IN(vid_in,19,0);
    VL_OUT(vid_out,19,0);
    
    // LOCAL SIGNALS
    CData/*0:0*/ __PVT__u_cam__DOT__valid;
    CData/*0:0*/ __PVT__u_sram__DOT__polarity;
    CData/*1:0*/ __PVT__u_sram__DOT__value;
    IData/*19:0*/ __PVT__u_cam__DOT__stored_vid;
    
    // INTERNAL VARIABLES
  private:
    Vsat_solver__Syms* __VlSymsp;  // Symbol table
  public:
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vsat_solver_cam_sram_row);  ///< Copying not allowed
  public:
    Vsat_solver_cam_sram_row(const char* name = "TOP");
    ~Vsat_solver_cam_sram_row();
    
    // INTERNAL METHODS
    void __Vconfigure(Vsat_solver__Syms* symsp, bool first);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__0__KET____DOT__u_row__1(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__10__KET____DOT__u_row__11(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__11__KET____DOT__u_row__12(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__12__KET____DOT__u_row__13(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__13__KET____DOT__u_row__14(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__14__KET____DOT__u_row__15(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__15__KET____DOT__u_row__16(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__16__KET____DOT__u_row__17(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__17__KET____DOT__u_row__18(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__18__KET____DOT__u_row__19(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__19__KET____DOT__u_row__20(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__1__KET____DOT__u_row__2(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__20__KET____DOT__u_row__21(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__21__KET____DOT__u_row__22(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__22__KET____DOT__u_row__23(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__23__KET____DOT__u_row__24(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__2__KET____DOT__u_row__3(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__3__KET____DOT__u_row__4(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__4__KET____DOT__u_row__5(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__5__KET____DOT__u_row__6(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__6__KET____DOT__u_row__7(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__7__KET____DOT__u_row__8(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__8__KET____DOT__u_row__9(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__9__KET____DOT__u_row__10(Vsat_solver__Syms* __restrict vlSymsp);
    void _settle__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__0__KET____DOT__u_row__25(Vsat_solver__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
