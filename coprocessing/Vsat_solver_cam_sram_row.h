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
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__24__KET____DOT__u_row__25(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__25__KET____DOT__u_row__26(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__26__KET____DOT__u_row__27(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__27__KET____DOT__u_row__28(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__28__KET____DOT__u_row__29(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__29__KET____DOT__u_row__30(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__2__KET____DOT__u_row__3(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__30__KET____DOT__u_row__31(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__31__KET____DOT__u_row__32(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__32__KET____DOT__u_row__33(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__33__KET____DOT__u_row__34(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__34__KET____DOT__u_row__35(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__35__KET____DOT__u_row__36(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__36__KET____DOT__u_row__37(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__37__KET____DOT__u_row__38(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__38__KET____DOT__u_row__39(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__39__KET____DOT__u_row__40(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__3__KET____DOT__u_row__4(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__40__KET____DOT__u_row__41(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__41__KET____DOT__u_row__42(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__42__KET____DOT__u_row__43(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__43__KET____DOT__u_row__44(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__44__KET____DOT__u_row__45(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__45__KET____DOT__u_row__46(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__46__KET____DOT__u_row__47(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__47__KET____DOT__u_row__48(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__48__KET____DOT__u_row__49(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__49__KET____DOT__u_row__50(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__4__KET____DOT__u_row__5(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__50__KET____DOT__u_row__51(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__51__KET____DOT__u_row__52(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__52__KET____DOT__u_row__53(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__53__KET____DOT__u_row__54(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__54__KET____DOT__u_row__55(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__55__KET____DOT__u_row__56(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__56__KET____DOT__u_row__57(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__57__KET____DOT__u_row__58(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__58__KET____DOT__u_row__59(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__59__KET____DOT__u_row__60(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__5__KET____DOT__u_row__6(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__60__KET____DOT__u_row__61(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__61__KET____DOT__u_row__62(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__62__KET____DOT__u_row__63(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__63__KET____DOT__u_row__64(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__6__KET____DOT__u_row__7(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__7__KET____DOT__u_row__8(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__8__KET____DOT__u_row__9(Vsat_solver__Syms* __restrict vlSymsp);
    void _sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__9__KET____DOT__u_row__10(Vsat_solver__Syms* __restrict vlSymsp);
    void _settle__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__0__KET____DOT__u_row__65(Vsat_solver__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
