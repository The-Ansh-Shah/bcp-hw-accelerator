// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_submodule.h for the primary calling header

#include "Vsat_submodule.h"
#include "Vsat_submodule__Syms.h"

#include "verilated_dpi.h"

//==========

VL_CTOR_IMP(Vsat_submodule) {
    Vsat_submodule__Syms* __restrict vlSymsp = __VlSymsp = new Vsat_submodule__Syms(this, name());
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    VL_CELL(sat_submodule, Vsat_submodule_sat_submodule);
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vsat_submodule::__Vconfigure(Vsat_submodule__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vsat_submodule::~Vsat_submodule() {
    delete __VlSymsp; __VlSymsp=NULL;
}

void Vsat_submodule::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vsat_submodule::eval\n"); );
    Vsat_submodule__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

void Vsat_submodule::_eval_initial_loop(Vsat_submodule__Syms* __restrict vlSymsp) {
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

VL_INLINE_OPT void Vsat_submodule::_combo__TOP__1(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_combo__TOP__1\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlSymsp->TOP__sat_submodule.phase = vlTOPp->phase;
    vlSymsp->TOP__sat_submodule.cid_in = vlTOPp->cid_in;
    vlSymsp->TOP__sat_submodule.clk = vlTOPp->clk;
}

void Vsat_submodule::_settle__TOP__2(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_settle__TOP__2\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlSymsp->TOP__sat_submodule.phase = vlTOPp->phase;
    vlSymsp->TOP__sat_submodule.cid_in = vlTOPp->cid_in;
    vlSymsp->TOP__sat_submodule.clk = vlTOPp->clk;
    vlSymsp->TOP__sat_submodule.rst_n = vlTOPp->rst_n;
    vlSymsp->TOP__sat_submodule.row_addr = vlTOPp->row_addr;
    vlSymsp->TOP__sat_submodule.pol_in = vlTOPp->pol_in;
    vlSymsp->TOP__sat_submodule.val_in = vlTOPp->val_in;
    vlSymsp->TOP__sat_submodule.vid_in = vlTOPp->vid_in;
    vlSymsp->TOP__sat_submodule.op = vlTOPp->op;
}

void Vsat_submodule::_settle__TOP__3(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_settle__TOP__3\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->valid_out = vlSymsp->TOP__sat_submodule.valid_out;
    vlTOPp->conf_out = vlSymsp->TOP__sat_submodule.conf_out;
    vlTOPp->done_out = vlSymsp->TOP__sat_submodule.done_out;
    vlTOPp->cid_out = vlSymsp->TOP__sat_submodule.cid_out;
    vlTOPp->up_lit_pos = vlSymsp->TOP__sat_submodule.up_lit_pos;
    vlTOPp->up_out = vlSymsp->TOP__sat_submodule.up_out;
    vlTOPp->vid_out = vlSymsp->TOP__sat_submodule.vid_out;
    vlTOPp->pol_out = vlSymsp->TOP__sat_submodule.pol_out;
}

VL_INLINE_OPT void Vsat_submodule::_sequent__TOP__4(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_sequent__TOP__4\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->valid_out = vlSymsp->TOP__sat_submodule.valid_out;
    vlTOPp->conf_out = vlSymsp->TOP__sat_submodule.conf_out;
    vlTOPp->done_out = vlSymsp->TOP__sat_submodule.done_out;
    vlTOPp->cid_out = vlSymsp->TOP__sat_submodule.cid_out;
    vlTOPp->up_lit_pos = vlSymsp->TOP__sat_submodule.up_lit_pos;
    vlTOPp->up_out = vlSymsp->TOP__sat_submodule.up_out;
}

VL_INLINE_OPT void Vsat_submodule::_combo__TOP__5(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_combo__TOP__5\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlSymsp->TOP__sat_submodule.rst_n = vlTOPp->rst_n;
    vlSymsp->TOP__sat_submodule.vid_in = vlTOPp->vid_in;
    vlSymsp->TOP__sat_submodule.val_in = vlTOPp->val_in;
    vlSymsp->TOP__sat_submodule.row_addr = vlTOPp->row_addr;
    vlSymsp->TOP__sat_submodule.pol_in = vlTOPp->pol_in;
    vlSymsp->TOP__sat_submodule.op = vlTOPp->op;
}

VL_INLINE_OPT void Vsat_submodule::_combo__TOP__6(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_combo__TOP__6\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->vid_out = vlSymsp->TOP__sat_submodule.vid_out;
    vlTOPp->pol_out = vlSymsp->TOP__sat_submodule.pol_out;
}

void Vsat_submodule::_eval(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_eval\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    if ((((IData)(vlSymsp->TOP__sat_submodule.clk) 
          & (~ (IData)(vlTOPp->__Vclklast__TOP__sat_submodule__clk))) 
         | ((~ (IData)(vlTOPp->__VinpClk__TOP__sat_submodule__rst_n)) 
            & (IData)(vlTOPp->__Vclklast__TOP____VinpClk__TOP__sat_submodule__rst_n)))) {
        vlSymsp->TOP__sat_submodule._sequent__TOP__sat_submodule__2(vlSymsp);
        vlTOPp->_sequent__TOP__4(vlSymsp);
    }
    vlTOPp->_combo__TOP__5(vlSymsp);
    vlSymsp->TOP__sat_submodule._combo__TOP__sat_submodule__3(vlSymsp);
    vlTOPp->_combo__TOP__6(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__sat_submodule__clk = vlSymsp->TOP__sat_submodule.clk;
    vlTOPp->__Vclklast__TOP____VinpClk__TOP__sat_submodule__rst_n 
        = vlTOPp->__VinpClk__TOP__sat_submodule__rst_n;
    vlTOPp->__VinpClk__TOP__sat_submodule__rst_n = vlSymsp->TOP__sat_submodule.rst_n;
}

void Vsat_submodule::_eval_initial(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_eval_initial\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__sat_submodule__clk = vlSymsp->TOP__sat_submodule.clk;
    vlTOPp->__Vclklast__TOP____VinpClk__TOP__sat_submodule__rst_n 
        = vlTOPp->__VinpClk__TOP__sat_submodule__rst_n;
}

void Vsat_submodule::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::final\n"); );
    // Variables
    Vsat_submodule__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vsat_submodule::_eval_settle(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_eval_settle\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
    vlSymsp->TOP__sat_submodule._settle__TOP__sat_submodule__1(vlSymsp);
    vlTOPp->_settle__TOP__3(vlSymsp);
}

VL_INLINE_OPT QData Vsat_submodule::_change_request(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_change_request\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    __req |= ((vlSymsp->TOP__sat_submodule.rst_n ^ vlTOPp->__Vchglast__TOP__sat_submodule__rst_n));
    VL_DEBUG_IF( if(__req && ((vlSymsp->TOP__sat_submodule.rst_n ^ vlTOPp->__Vchglast__TOP__sat_submodule__rst_n))) VL_DBG_MSGF("        CHANGE: src/sat_submodule.sv:18: rst_n\n"); );
    // Final
    vlTOPp->__Vchglast__TOP__sat_submodule__rst_n = vlSymsp->TOP__sat_submodule.rst_n;
    return __req;
}

#ifdef VL_DEBUG
void Vsat_submodule::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_eval_debug_assertions\n"); );
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
    if (VL_UNLIKELY((row_addr & 0x8000U))) {
        Verilated::overWidthError("row_addr");}
    if (VL_UNLIKELY((cid_in & 0xe000U))) {
        Verilated::overWidthError("cid_in");}
}
#endif  // VL_DEBUG

void Vsat_submodule::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_submodule::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    rst_n = VL_RAND_RESET_I(1);
    op = VL_RAND_RESET_I(3);
    phase = VL_RAND_RESET_I(1);
    vid_in = VL_RAND_RESET_I(20);
    val_in = VL_RAND_RESET_I(2);
    pol_in = VL_RAND_RESET_I(1);
    row_addr = VL_RAND_RESET_I(15);
    cid_in = VL_RAND_RESET_I(13);
    conf_out = VL_RAND_RESET_I(1);
    up_out = VL_RAND_RESET_I(1);
    done_out = VL_RAND_RESET_I(1);
    cid_out = VL_RAND_RESET_I(13);
    up_lit_pos = VL_RAND_RESET_I(2);
    vid_out = VL_RAND_RESET_I(20);
    pol_out = VL_RAND_RESET_I(1);
    valid_out = VL_RAND_RESET_I(1);
    __VinpClk__TOP__sat_submodule__rst_n = VL_RAND_RESET_I(1);
    __Vchglast__TOP__sat_submodule__rst_n = VL_RAND_RESET_I(1);
}
