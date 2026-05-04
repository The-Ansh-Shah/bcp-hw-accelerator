// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_solver.h for the primary calling header

#include "Vsat_solver.h"
#include "Vsat_solver__Syms.h"

#include "verilated_dpi.h"

//==========

VL_CTOR_IMP(Vsat_solver) {
    Vsat_solver__Syms* __restrict vlSymsp = __VlSymsp = new Vsat_solver__Syms(this, name());
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    VL_CELL(sat_submodule, Vsat_solver_sat_submodule);
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vsat_solver::__Vconfigure(Vsat_solver__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vsat_solver::~Vsat_solver() {
    delete __VlSymsp; __VlSymsp=NULL;
}

void Vsat_solver::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vsat_solver::eval\n"); );
    Vsat_solver__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("src/sat_submodule.sv", 11, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vsat_solver::_eval_initial_loop(Vsat_solver__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("src/sat_submodule.sv", 11, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vsat_solver::_sequent__TOP__1(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_sequent__TOP__1\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->valid_out = ((((1U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                           | (3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) 
                          | (4U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) 
                         | (5U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)));
    vlTOPp->conf_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                        & (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_conf));
    vlTOPp->done_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                        & (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_done));
    vlTOPp->cid_out = vlSymsp->TOP__sat_submodule.__PVT__tree_cid;
    vlTOPp->up_lit_pos = vlSymsp->TOP__sat_submodule.__PVT__tree_ulp;
    vlTOPp->up_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                      & (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_up));
}

void Vsat_solver::_settle__TOP__2(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_settle__TOP__2\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->valid_out = ((((1U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                           | (3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) 
                          | (4U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) 
                         | (5U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)));
    vlTOPp->conf_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                        & (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_conf));
    vlTOPp->done_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                        & (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_done));
    vlTOPp->cid_out = vlSymsp->TOP__sat_submodule.__PVT__tree_cid;
    vlTOPp->up_lit_pos = vlSymsp->TOP__sat_submodule.__PVT__tree_ulp;
    vlTOPp->up_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                      & (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_up));
    vlTOPp->pol_out = vlSymsp->TOP__sat_submodule.pol_store
        [vlSymsp->TOP__sat_submodule.__PVT__rd_row];
    vlTOPp->vid_out = vlSymsp->TOP__sat_submodule.vid_store
        [vlSymsp->TOP__sat_submodule.__PVT__rd_row];
}

VL_INLINE_OPT void Vsat_solver::_combo__TOP__3(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_combo__TOP__3\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->pol_out = vlSymsp->TOP__sat_submodule.pol_store
        [vlSymsp->TOP__sat_submodule.__PVT__rd_row];
    vlTOPp->vid_out = vlSymsp->TOP__sat_submodule.vid_store
        [vlSymsp->TOP__sat_submodule.__PVT__rd_row];
}

void Vsat_solver::_eval(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_eval\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk))) 
         | ((~ (IData)(vlTOPp->rst_n)) & (IData)(vlTOPp->__Vclklast__TOP__rst_n)))) {
        vlSymsp->TOP__sat_submodule._sequent__TOP__sat_submodule__1(vlSymsp);
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    vlSymsp->TOP__sat_submodule._combo__TOP__sat_submodule__3(vlSymsp);
    vlTOPp->_combo__TOP__3(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst_n = vlTOPp->rst_n;
}

void Vsat_solver::_eval_initial(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_eval_initial\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst_n = vlTOPp->rst_n;
}

void Vsat_solver::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::final\n"); );
    // Variables
    Vsat_solver__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vsat_solver::_eval_settle(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_eval_settle\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlSymsp->TOP__sat_submodule._settle__TOP__sat_submodule__2(vlSymsp);
    vlTOPp->_settle__TOP__2(vlSymsp);
}

VL_INLINE_OPT QData Vsat_solver::_change_request(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_change_request\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vsat_solver::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((rst_n & 0xfeU))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY((op & 0xf8U))) {
        Verilated::overWidthError("op");}
    if (VL_UNLIKELY((phase & 0xfeU))) {
        Verilated::overWidthError("phase");}
    if (VL_UNLIKELY((vid_in & 0xfff00000U))) {
        Verilated::overWidthError("vid_in");}
    if (VL_UNLIKELY((val_in & 0xfcU))) {
        Verilated::overWidthError("val_in");}
    if (VL_UNLIKELY((pol_in & 0xfeU))) {
        Verilated::overWidthError("pol_in");}
    if (VL_UNLIKELY((row_addr & 0xf000U))) {
        Verilated::overWidthError("row_addr");}
    if (VL_UNLIKELY((cid_in & 0xfc00U))) {
        Verilated::overWidthError("cid_in");}
}
#endif  // VL_DEBUG

void Vsat_solver::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    rst_n = VL_RAND_RESET_I(1);
    op = VL_RAND_RESET_I(3);
    phase = VL_RAND_RESET_I(1);
    vid_in = VL_RAND_RESET_I(20);
    val_in = VL_RAND_RESET_I(2);
    pol_in = VL_RAND_RESET_I(1);
    row_addr = VL_RAND_RESET_I(12);
    cid_in = VL_RAND_RESET_I(10);
    conf_out = VL_RAND_RESET_I(1);
    up_out = VL_RAND_RESET_I(1);
    done_out = VL_RAND_RESET_I(1);
    cid_out = VL_RAND_RESET_I(10);
    up_lit_pos = VL_RAND_RESET_I(2);
    vid_out = VL_RAND_RESET_I(20);
    pol_out = VL_RAND_RESET_I(1);
    valid_out = VL_RAND_RESET_I(1);
}
