// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_solver.h for the primary calling header

#include "Vsat_solver_sat_submodule.h"
#include "Vsat_solver__Syms.h"

#include "verilated_dpi.h"

//==========

VL_CTOR_IMP(Vsat_solver_sat_submodule) {
    // Reset internal values
    // Reset structure values
    _ctor_var_reset();
}

void Vsat_solver_sat_submodule::__Vconfigure(Vsat_solver__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vsat_solver_sat_submodule::~Vsat_solver_sat_submodule() {
}

VL_INLINE_OPT void Vsat_solver_sat_submodule::_sequent__TOP__sat_submodule__1(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_solver_sat_submodule::_sequent__TOP__sat_submodule__1\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*2:0*/ __Vdly__state;
    CData/*1:0*/ __Vdly__cit_lit_idx;
    // Body
    __Vdly__cit_lit_idx = vlSymsp->TOP__sat_submodule.__PVT__cit_lit_idx;
    __Vdly__state = vlSymsp->TOP__sat_submodule.__PVT__state;
    if (vlTOPp->rst_n) {
        if ((4U & (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) {
            if ((2U & (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) {
                __Vdly__state = 0U;
            } else {
                if ((1U & (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) {
                    if ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__cit_lit_idx))) {
                        __Vdly__state = 0U;
                        __Vdly__cit_lit_idx = 0U;
                    } else {
                        __Vdly__cit_lit_idx = (3U & 
                                               ((IData)(1U) 
                                                + (IData)(vlSymsp->TOP__sat_submodule.__PVT__cit_lit_idx)));
                    }
                } else {
                    vlSymsp->TOP__sat_submodule.__PVT__unnamedblk9__DOT__r = 0U;
                    while ((0x1000U > vlSymsp->TOP__sat_submodule.__PVT__unnamedblk9__DOT__r)) {
                        if ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                             [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk9__DOT__r)] 
                             & (vlSymsp->TOP__sat_submodule.vid_store
                                [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk9__DOT__r)] 
                                == vlTOPp->vid_in))) {
                            vlSymsp->TOP__sat_submodule.val_store[(0xfffU 
                                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk9__DOT__r)] = 1U;
                        }
                        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk9__DOT__r 
                            = ((IData)(1U) + vlSymsp->TOP__sat_submodule.__PVT__unnamedblk9__DOT__r);
                    }
                    __Vdly__state = 0U;
                }
            }
        } else {
            if ((2U & (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) {
                if ((1U & (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) {
                    __Vdly__state = 0U;
                } else {
                    vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r = 0U;
                    while ((0x1000U > vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r)) {
                        if ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                             [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r)] 
                             & (vlSymsp->TOP__sat_submodule.vid_store
                                [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r)] 
                                == vlTOPp->vid_in))) {
                            vlSymsp->TOP__sat_submodule.val_store[(0xfffU 
                                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r)] 
                                = vlTOPp->val_in;
                            vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r)] = 1U;
                        } else {
                            vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r)] = 0U;
                        }
                        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r 
                            = ((IData)(1U) + vlSymsp->TOP__sat_submodule.__PVT__unnamedblk8__DOT__r);
                    }
                    __Vdly__state = 3U;
                }
            } else {
                if ((1U & (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) {
                    vlSymsp->TOP__sat_submodule.vid_store[vlTOPp->row_addr] 
                        = vlTOPp->vid_in;
                    vlSymsp->TOP__sat_submodule.pol_store[vlTOPp->row_addr] 
                        = vlTOPp->pol_in;
                    vlSymsp->TOP__sat_submodule.val_store[vlTOPp->row_addr] = 1U;
                    vlSymsp->TOP__sat_submodule.__PVT__row_valid[vlTOPp->row_addr] = 1U;
                    __Vdly__state = 0U;
                } else {
                    if ((4U & (IData)(vlTOPp->op))) {
                        if ((2U & (IData)(vlTOPp->op))) {
                            __Vdly__state = 0U;
                        } else {
                            if ((1U & (IData)(vlTOPp->op))) {
                                __Vdly__state = 0U;
                            } else {
                                __Vdly__state = 5U;
                                __Vdly__cit_lit_idx = 0U;
                            }
                        }
                    } else {
                        __Vdly__state = ((2U & (IData)(vlTOPp->op))
                                          ? ((1U & (IData)(vlTOPp->op))
                                              ? 4U : 2U)
                                          : ((1U & (IData)(vlTOPp->op))
                                              ? 1U : 0U));
                    }
                }
            }
        }
    } else {
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r = 0U;
        __Vdly__state = 0U;
        __Vdly__cit_lit_idx = 0U;
        while ((0x1000U > vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r)) {
            vlSymsp->TOP__sat_submodule.vid_store[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r)] = 0U;
            vlSymsp->TOP__sat_submodule.pol_store[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r)] = 0U;
            vlSymsp->TOP__sat_submodule.val_store[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r)] = 1U;
            vlSymsp->TOP__sat_submodule.__PVT__row_valid[(0xfffU 
                                                          & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r)] = 0U;
            vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r)] = 0U;
            vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r 
                = ((IData)(1U) + vlSymsp->TOP__sat_submodule.__PVT__unnamedblk7__DOT__r);
        }
    }
    vlSymsp->TOP__sat_submodule.__PVT__cit_lit_idx 
        = __Vdly__cit_lit_idx;
    vlSymsp->TOP__sat_submodule.__PVT__state = __Vdly__state;
    vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c = 0U;
    while ((0x400U > vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)) {
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] = 0U;
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] = 0U;
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
               << 2U);
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_u[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (vlSymsp->TOP__sat_submodule.__PVT__row_valid
                  [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                  & (1U == vlSymsp->TOP__sat_submodule.val_store
                     [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_t[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                   [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (~ vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                      [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
                  & ((1U & vlSymsp->TOP__sat_submodule.val_store
                      [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                     == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_f[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (1U & ((~ vlSymsp->TOP__sat_submodule.__PVT__row_valid
                         [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                        | ((~ vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                            [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
                           & ((1U & vlSymsp->TOP__sat_submodule.val_store
                               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                              == vlSymsp->TOP__sat_submodule.pol_store
                              [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))));
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.__PVT__row_valid
               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(1U) + (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_u[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                   [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 1U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_t[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                    [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                          [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                          >> 1U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0xfffU 
                                                  & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 1U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_f[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (2U & (((~ vlSymsp->TOP__sat_submodule.__PVT__row_valid
                          [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                                >> 1U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0xfffU 
                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0xfffU 
                                             & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 1U)));
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.__PVT__row_valid
               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(2U) + (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_u[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                   [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_t[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                    [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                          [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                          >> 2U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0xfffU 
                                                  & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_f[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (4U & (((~ vlSymsp->TOP__sat_submodule.__PVT__row_valid
                          [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                                >> 2U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0xfffU 
                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0xfffU 
                                             & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 2U)));
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.__PVT__row_valid
               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(3U) + (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_u[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                   [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 3U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_t[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                    [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                          [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                          >> 3U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0xfffU 
                                                  & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 3U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_f[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (8U & (((~ vlSymsp->TOP__sat_submodule.__PVT__row_valid
                          [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                                >> 3U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0xfffU 
                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0xfffU 
                                             & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 3U)));
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.__PVT__row_valid
               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.proc_done[(0x3ffU 
                                               & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (0U != vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]);
        vlSymsp->TOP__sat_submodule.proc_conf[(0x3ffU 
                                               & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                & vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               & (0xfU == vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                  [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]));
        vlSymsp->TOP__sat_submodule.proc_up[(0x3ffU 
                                             & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (((((((1U == vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                     [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
                    | (2U == vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                       [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
                   | (4U == vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                      [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
                  | (8U == vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                     [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
                 & (0xfU == (vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                             [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                             | vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                             [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]))) 
                & (~ vlSymsp->TOP__sat_submodule.proc_done
                   [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
               & vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]);
        vlSymsp->TOP__sat_submodule.__PVT__proc_ulp[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((1U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])
                ? 0U : ((2U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                         [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])
                         ? 1U : ((4U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                                  [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])
                                  ? 2U : 3U)));
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
            = ((IData)(1U) + vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c);
    }
    vlSymsp->TOP__sat_submodule.__PVT__tree_conf = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__tree_up = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__tree_done = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__tree_cid = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__tree_ulp = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i = 0x3ffU;
    while (VL_LTES_III(1,32,32, 0U, vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i)) {
        if (vlSymsp->TOP__sat_submodule.proc_conf[(0x3ffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i)]) {
            vlSymsp->TOP__sat_submodule.__PVT__tree_conf = 1U;
            vlSymsp->TOP__sat_submodule.__PVT__tree_cid 
                = (0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i);
        }
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i 
            = (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i 
               - (IData)(1U));
    }
    if ((1U & (~ (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_conf)))) {
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i = 0x3ffU;
        while (VL_LTES_III(1,32,32, 0U, vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i)) {
            if (vlSymsp->TOP__sat_submodule.proc_up
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i)]) {
                vlSymsp->TOP__sat_submodule.__PVT__tree_up = 1U;
                vlSymsp->TOP__sat_submodule.__PVT__tree_cid 
                    = (0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i);
                vlSymsp->TOP__sat_submodule.__PVT__tree_ulp 
                    = vlSymsp->TOP__sat_submodule.__PVT__proc_ulp
                    [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i)];
            }
            vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i 
                = (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i 
                   - (IData)(1U));
        }
        if ((1U & (~ (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_up)))) {
            vlSymsp->TOP__sat_submodule.__PVT__tree_done = 1U;
            vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i = 0U;
            while ((0x400U > vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i)) {
                if ((1U & (~ vlSymsp->TOP__sat_submodule.proc_done
                           [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i)]))) {
                    vlSymsp->TOP__sat_submodule.__PVT__tree_done = 0U;
                }
                vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i 
                    = ((IData)(1U) + vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i);
            }
        }
    }
}

void Vsat_solver_sat_submodule::_settle__TOP__sat_submodule__2(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_solver_sat_submodule::_settle__TOP__sat_submodule__2\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c = 0U;
    while ((0x400U > vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)) {
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] = 0U;
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] = 0U;
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
               << 2U);
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_u[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (vlSymsp->TOP__sat_submodule.__PVT__row_valid
                  [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                  & (1U == vlSymsp->TOP__sat_submodule.val_store
                     [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_t[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                   [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (~ vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                      [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
                  & ((1U & vlSymsp->TOP__sat_submodule.val_store
                      [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                     == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_f[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (1U & ((~ vlSymsp->TOP__sat_submodule.__PVT__row_valid
                         [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                        | ((~ vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                            [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
                           & ((1U & vlSymsp->TOP__sat_submodule.val_store
                               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                              == vlSymsp->TOP__sat_submodule.pol_store
                              [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))));
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.__PVT__row_valid
               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(1U) + (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_u[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                   [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 1U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_t[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                    [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                          [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                          >> 1U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0xfffU 
                                                  & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 1U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_f[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (2U & (((~ vlSymsp->TOP__sat_submodule.__PVT__row_valid
                          [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                                >> 1U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0xfffU 
                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0xfffU 
                                             & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 1U)));
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.__PVT__row_valid
               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(2U) + (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_u[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                   [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_t[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                    [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                          [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                          >> 2U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0xfffU 
                                                  & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_f[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (4U & (((~ vlSymsp->TOP__sat_submodule.__PVT__row_valid
                          [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                                >> 2U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0xfffU 
                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0xfffU 
                                             & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 2U)));
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.__PVT__row_valid
               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(3U) + (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_u[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                   [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 3U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_t[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.__PVT__row_valid
                    [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                          [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                          >> 3U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0xfffU 
                                                  & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 3U));
        vlSymsp->TOP__sat_submodule.__PVT__c_lit_f[(0x3ffU 
                                                    & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               | (8U & (((~ vlSymsp->TOP__sat_submodule.__PVT__row_valid
                          [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                                >> 3U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0xfffU 
                                              & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0xfffU 
                                             & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 3U)));
        vlSymsp->TOP__sat_submodule.__PVT__c_ml_any[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0xfffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.__PVT__c_any_valid[(0x3ffU 
                                                        & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.__PVT__row_valid
               [(0xfffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.proc_done[(0x3ffU 
                                               & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (0U != vlSymsp->TOP__sat_submodule.__PVT__c_lit_t
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]);
        vlSymsp->TOP__sat_submodule.proc_conf[(0x3ffU 
                                               & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((vlSymsp->TOP__sat_submodule.__PVT__c_any_valid
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                & vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
               & (0xfU == vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                  [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]));
        vlSymsp->TOP__sat_submodule.proc_up[(0x3ffU 
                                             & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = (((((((1U == vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                     [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]) 
                    | (2U == vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                       [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
                   | (4U == vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                      [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
                  | (8U == vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                     [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
                 & (0xfU == (vlSymsp->TOP__sat_submodule.__PVT__c_lit_f
                             [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
                             | vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                             [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]))) 
                & (~ vlSymsp->TOP__sat_submodule.proc_done
                   [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])) 
               & vlSymsp->TOP__sat_submodule.__PVT__c_ml_any
               [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)]);
        vlSymsp->TOP__sat_submodule.__PVT__proc_ulp[(0x3ffU 
                                                     & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)] 
            = ((1U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])
                ? 0U : ((2U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                         [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])
                         ? 1U : ((4U & vlSymsp->TOP__sat_submodule.__PVT__c_lit_u
                                  [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c)])
                                  ? 2U : 3U)));
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c 
            = ((IData)(1U) + vlSymsp->TOP__sat_submodule.__PVT__unnamedblk1__DOT__c);
    }
    vlSymsp->TOP__sat_submodule.__PVT__tree_conf = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__tree_up = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__tree_done = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__tree_cid = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__tree_ulp = 0U;
    vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i = 0x3ffU;
    while (VL_LTES_III(1,32,32, 0U, vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i)) {
        if (vlSymsp->TOP__sat_submodule.proc_conf[(0x3ffU 
                                                   & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i)]) {
            vlSymsp->TOP__sat_submodule.__PVT__tree_conf = 1U;
            vlSymsp->TOP__sat_submodule.__PVT__tree_cid 
                = (0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i);
        }
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i 
            = (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk4__DOT__i 
               - (IData)(1U));
    }
    if ((1U & (~ (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_conf)))) {
        vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i = 0x3ffU;
        while (VL_LTES_III(1,32,32, 0U, vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i)) {
            if (vlSymsp->TOP__sat_submodule.proc_up
                [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i)]) {
                vlSymsp->TOP__sat_submodule.__PVT__tree_up = 1U;
                vlSymsp->TOP__sat_submodule.__PVT__tree_cid 
                    = (0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i);
                vlSymsp->TOP__sat_submodule.__PVT__tree_ulp 
                    = vlSymsp->TOP__sat_submodule.__PVT__proc_ulp
                    [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i)];
            }
            vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i 
                = (vlSymsp->TOP__sat_submodule.__PVT__unnamedblk5__DOT__i 
                   - (IData)(1U));
        }
        if ((1U & (~ (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_up)))) {
            vlSymsp->TOP__sat_submodule.__PVT__tree_done = 1U;
            vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i = 0U;
            while ((0x400U > vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i)) {
                if ((1U & (~ vlSymsp->TOP__sat_submodule.proc_done
                           [(0x3ffU & vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i)]))) {
                    vlSymsp->TOP__sat_submodule.__PVT__tree_done = 0U;
                }
                vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i 
                    = ((IData)(1U) + vlSymsp->TOP__sat_submodule.__PVT__unnamedblk6__DOT__i);
            }
        }
    }
    vlSymsp->TOP__sat_submodule.__PVT__rd_row = 0U;
    if (((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
         & (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_up))) {
        vlSymsp->TOP__sat_submodule.__PVT__rd_row = 
            (0xfffU & (((IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_cid) 
                        << 2U) + (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_ulp)));
    } else {
        if ((5U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) {
            vlSymsp->TOP__sat_submodule.__PVT__rd_row 
                = (0xfffU & (((IData)(vlTOPp->cid_in) 
                              << 2U) + (IData)(vlSymsp->TOP__sat_submodule.__PVT__cit_lit_idx)));
        }
    }
}

VL_INLINE_OPT void Vsat_solver_sat_submodule::_combo__TOP__sat_submodule__3(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_solver_sat_submodule::_combo__TOP__sat_submodule__3\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlSymsp->TOP__sat_submodule.__PVT__rd_row = 0U;
    if (((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
         & (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_up))) {
        vlSymsp->TOP__sat_submodule.__PVT__rd_row = 
            (0xfffU & (((IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_cid) 
                        << 2U) + (IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_ulp)));
    } else {
        if ((5U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state))) {
            vlSymsp->TOP__sat_submodule.__PVT__rd_row 
                = (0xfffU & (((IData)(vlTOPp->cid_in) 
                              << 2U) + (IData)(vlSymsp->TOP__sat_submodule.__PVT__cit_lit_idx)));
        }
    }
}

void Vsat_solver_sat_submodule::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_solver_sat_submodule::_ctor_var_reset\n"); );
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
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            vid_store[__Vi0] = VL_RAND_RESET_I(20);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            pol_store[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            val_store[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            __PVT__row_valid[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            ml_q[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            proc_conf[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            proc_up[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            proc_done[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            __PVT__proc_ulp[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            __PVT__c_lit_u[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            __PVT__c_lit_t[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            __PVT__c_lit_f[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            __PVT__c_ml_any[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            __PVT__c_any_valid[__Vi0] = VL_RAND_RESET_I(1);
    }}
    __PVT__tree_conf = VL_RAND_RESET_I(1);
    __PVT__tree_up = VL_RAND_RESET_I(1);
    __PVT__tree_done = VL_RAND_RESET_I(1);
    __PVT__tree_cid = VL_RAND_RESET_I(10);
    __PVT__tree_ulp = VL_RAND_RESET_I(2);
    __PVT__state = VL_RAND_RESET_I(3);
    __PVT__cit_lit_idx = VL_RAND_RESET_I(2);
    __PVT__rd_row = VL_RAND_RESET_I(12);
    __PVT__unnamedblk1__DOT__c = 0;
    __PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r = 0;
    __PVT__unnamedblk4__DOT__i = 0;
    __PVT__unnamedblk5__DOT__i = 0;
    __PVT__unnamedblk6__DOT__i = 0;
    __PVT__unnamedblk8__DOT__r = 0;
    __PVT__unnamedblk9__DOT__r = 0;
    __PVT__unnamedblk7__DOT__r = 0;
}
