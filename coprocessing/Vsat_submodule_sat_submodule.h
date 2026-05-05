// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vsat_submodule.h for the primary calling header

#ifndef _VSAT_SUBMODULE_SAT_SUBMODULE_H_
#define _VSAT_SUBMODULE_SAT_SUBMODULE_H_  // guard

#include "verilated.h"
#include "Vsat_submodule__Dpi.h"

//==========

class Vsat_submodule__Syms;

//----------

VL_MODULE(Vsat_submodule_sat_submodule) {
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
    VL_IN16(row_addr,14,0);
    VL_IN16(cid_in,12,0);
    VL_OUT16(cid_out,12,0);
    VL_IN(vid_in,19,0);
    VL_OUT(vid_out,19,0);
    
    // LOCAL SIGNALS
    CData/*0:0*/ tree_conf;
    CData/*0:0*/ tree_up;
    CData/*0:0*/ tree_done;
    CData/*1:0*/ tree_ulp;
    CData/*2:0*/ state;
    CData/*1:0*/ cit_lit_idx;
    SData/*12:0*/ tree_cid;
    SData/*14:0*/ rd_row;
    IData/*31:0*/ unnamedblk1__DOT__c;
    IData/*31:0*/ unnamedblk1__DOT__unnamedblk2__DOT__i;
    IData/*31:0*/ unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r;
    IData/*31:0*/ unnamedblk4__DOT__i;
    IData/*31:0*/ unnamedblk5__DOT__i;
    IData/*31:0*/ unnamedblk6__DOT__i;
    IData/*31:0*/ unnamedblk8__DOT__r;
    IData/*31:0*/ unnamedblk9__DOT__r;
    IData/*31:0*/ unnamedblk7__DOT__r;
    IData/*19:0*/ vid_store[32768];
    CData/*0:0*/ pol_store[32768];
    CData/*1:0*/ val_store[32768];
    CData/*0:0*/ row_valid[32768];
    CData/*0:0*/ ml_q[32768];
    CData/*0:0*/ proc_conf[8192];
    CData/*0:0*/ proc_up[8192];
    CData/*0:0*/ proc_done[8192];
    CData/*1:0*/ proc_ulp[8192];
    CData/*3:0*/ c_lit_u[8192];
    CData/*3:0*/ c_lit_t[8192];
    CData/*3:0*/ c_lit_f[8192];
    CData/*0:0*/ c_ml_any[8192];
    CData/*0:0*/ c_any_valid[8192];
    
    // INTERNAL VARIABLES
  private:
    Vsat_submodule__Syms* __VlSymsp;  // Symbol table
  public:
    
    // PARAMETERS
    enum _IDataVID_WIDTH { VID_WIDTH = 0x14U};
    enum _IDataNUM_CLAUSES { NUM_CLAUSES = 0x2000U};
    enum _IDataLITS_PER_CLAUSE { LITS_PER_CLAUSE = 4U};
    enum _IDataCID_WIDTH { CID_WIDTH = 0xdU};
    enum _IDataOP_IDLE { OP_IDLE = 0U};
    enum _IDataOP_LOAD { OP_LOAD = 1U};
    enum _IDataOP_BCP { OP_BCP = 2U};
    enum _IDataOP_UNDO { OP_UNDO = 3U};
    enum _IDataOP_CIT_READ { OP_CIT_READ = 4U};
    enum _IDataVAL_ZERO { VAL_ZERO = 0U};
    enum _IDataVAL_ONE { VAL_ONE = 3U};
    enum _IDataVAL_U { VAL_U = 1U};
    enum _IDataNUM_ROWS { NUM_ROWS = 0x8000U};
    enum _IDataROW_ADDR_W { ROW_ADDR_W = 0xfU};
    enum _IDataLIT_IDX_W { LIT_IDX_W = 2U};
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vsat_submodule_sat_submodule);  ///< Copying not allowed
  public:
    Vsat_submodule_sat_submodule(const char* name = "TOP");
    ~Vsat_submodule_sat_submodule();
    
    // INTERNAL METHODS
    void __Vconfigure(Vsat_submodule__Syms* symsp, bool first);
    static void _combo__TOP__sat_submodule__3(Vsat_submodule__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _sequent__TOP__sat_submodule__2(Vsat_submodule__Syms* __restrict vlSymsp);
    static void _settle__TOP__sat_submodule__1(Vsat_submodule__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
