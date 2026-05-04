// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VSAT_SOLVER_H_
#define _VSAT_SOLVER_H_  // guard

#include "verilated.h"
#include "Vsat_solver__Dpi.h"

//==========

class Vsat_solver__Syms;
class Vsat_solver_sat_submodule;


//----------

VL_MODULE(Vsat_solver) {
  public:
    // CELLS
    // Public to allow access to /*verilator_public*/ items;
    // otherwise the application code can consider these internals.
    Vsat_solver_sat_submodule* sat_submodule;
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
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
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk;
    CData/*0:0*/ __Vclklast__TOP__rst_n;
    
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
    static void _combo__TOP__3(Vsat_solver__Syms* __restrict vlSymsp);
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
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
