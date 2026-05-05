// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_submodule.h for the primary calling header

#include "Vsat_submodule_sat_submodule.h"
#include "Vsat_submodule__Syms.h"

#include "verilated_dpi.h"

//==========

VL_CTOR_IMP(Vsat_submodule_sat_submodule) {
    // Reset internal values
    // Reset structure values
    _ctor_var_reset();
}

void Vsat_submodule_sat_submodule::__Vconfigure(Vsat_submodule__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vsat_submodule_sat_submodule::~Vsat_submodule_sat_submodule() {
}

void Vsat_submodule_sat_submodule::_settle__TOP__sat_submodule__1(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_submodule_sat_submodule::_settle__TOP__sat_submodule__1\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c = 0U;
    while ((0x2000U > vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)) {
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] = 0U;
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] = 0U;
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = (vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
               << 2U);
        vlSymsp->TOP__sat_submodule.c_lit_u[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (vlSymsp->TOP__sat_submodule.row_valid
                  [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                  & (1U == vlSymsp->TOP__sat_submodule.val_store
                     [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])));
        vlSymsp->TOP__sat_submodule.c_lit_t[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.c_lit_t
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.row_valid
                   [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (~ vlSymsp->TOP__sat_submodule.c_lit_u
                      [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
                  & ((1U & vlSymsp->TOP__sat_submodule.val_store
                      [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                     == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))));
        vlSymsp->TOP__sat_submodule.c_lit_f[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.c_lit_f
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (1U & ((~ vlSymsp->TOP__sat_submodule.row_valid
                         [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                        | ((~ vlSymsp->TOP__sat_submodule.c_lit_u
                            [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
                           & ((1U & vlSymsp->TOP__sat_submodule.val_store
                               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                              == vlSymsp->TOP__sat_submodule.pol_store
                              [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))));
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_any_valid
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.row_valid
               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__i = 1U;
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(1U) + (vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_u[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.row_valid
                   [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 1U));
        vlSymsp->TOP__sat_submodule.c_lit_t[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.c_lit_t
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.row_valid
                    [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.c_lit_u
                          [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                          >> 1U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0x7fffU 
                                                  & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 1U));
        vlSymsp->TOP__sat_submodule.c_lit_f[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.c_lit_f
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (2U & (((~ vlSymsp->TOP__sat_submodule.row_valid
                          [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.c_lit_u
                                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                                >> 1U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0x7fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0x7fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 1U)));
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_any_valid
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.row_valid
               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__i = 2U;
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(2U) + (vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_u[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.row_valid
                   [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_t[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.c_lit_t
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.row_valid
                    [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.c_lit_u
                          [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                          >> 2U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0x7fffU 
                                                  & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_f[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.c_lit_f
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (4U & (((~ vlSymsp->TOP__sat_submodule.row_valid
                          [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.c_lit_u
                                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                                >> 2U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0x7fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0x7fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 2U)));
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_any_valid
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.row_valid
               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__i = 3U;
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(3U) + (vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_u[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.row_valid
                   [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 3U));
        vlSymsp->TOP__sat_submodule.c_lit_t[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.c_lit_t
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.row_valid
                    [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.c_lit_u
                          [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                          >> 3U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0x7fffU 
                                                  & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 3U));
        vlSymsp->TOP__sat_submodule.c_lit_f[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.c_lit_f
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (8U & (((~ vlSymsp->TOP__sat_submodule.row_valid
                          [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.c_lit_u
                                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                                >> 3U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0x7fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0x7fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 3U)));
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_any_valid
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.row_valid
               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__i = 4U;
        vlSymsp->TOP__sat_submodule.proc_done[(0x1fffU 
                                               & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (0U != vlSymsp->TOP__sat_submodule.c_lit_t
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]);
        vlSymsp->TOP__sat_submodule.proc_conf[(0x1fffU 
                                               & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((vlSymsp->TOP__sat_submodule.c_any_valid
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                & vlSymsp->TOP__sat_submodule.c_ml_any
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               & (0xfU == vlSymsp->TOP__sat_submodule.c_lit_f
                  [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]));
        vlSymsp->TOP__sat_submodule.proc_up[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (((((((1U == vlSymsp->TOP__sat_submodule.c_lit_u
                     [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
                    | (2U == vlSymsp->TOP__sat_submodule.c_lit_u
                       [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
                   | (4U == vlSymsp->TOP__sat_submodule.c_lit_u
                      [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
                  | (8U == vlSymsp->TOP__sat_submodule.c_lit_u
                     [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
                 & (0xfU == (vlSymsp->TOP__sat_submodule.c_lit_f
                             [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                             | vlSymsp->TOP__sat_submodule.c_lit_u
                             [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]))) 
                & (~ vlSymsp->TOP__sat_submodule.proc_done
                   [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
               & vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]);
        vlSymsp->TOP__sat_submodule.proc_ulp[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((1U & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])
                ? 0U : ((2U & vlSymsp->TOP__sat_submodule.c_lit_u
                         [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])
                         ? 1U : ((4U & vlSymsp->TOP__sat_submodule.c_lit_u
                                  [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])
                                  ? 2U : 3U)));
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
            = ((IData)(1U) + vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c);
    }
    vlSymsp->TOP__sat_submodule.valid_out = ((((1U 
                                                == (IData)(vlSymsp->TOP__sat_submodule.state)) 
                                               | (3U 
                                                  == (IData)(vlSymsp->TOP__sat_submodule.state))) 
                                              | (4U 
                                                 == (IData)(vlSymsp->TOP__sat_submodule.state))) 
                                             | (5U 
                                                == (IData)(vlSymsp->TOP__sat_submodule.state)));
    vlSymsp->TOP__sat_submodule.tree_conf = 0U;
    vlSymsp->TOP__sat_submodule.tree_up = 0U;
    vlSymsp->TOP__sat_submodule.tree_done = 0U;
    vlSymsp->TOP__sat_submodule.tree_cid = 0U;
    vlSymsp->TOP__sat_submodule.tree_ulp = 0U;
    vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i = 0x1fffU;
    while (VL_LTES_III(1,32,32, 0U, vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i)) {
        if (vlSymsp->TOP__sat_submodule.proc_conf[(0x1fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i)]) {
            vlSymsp->TOP__sat_submodule.tree_conf = 1U;
            vlSymsp->TOP__sat_submodule.tree_cid = 
                (0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i);
        }
        vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i 
            = (vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i 
               - (IData)(1U));
    }
    if ((1U & (~ (IData)(vlSymsp->TOP__sat_submodule.tree_conf)))) {
        vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i = 0x1fffU;
        while (VL_LTES_III(1,32,32, 0U, vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i)) {
            if (vlSymsp->TOP__sat_submodule.proc_up
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i)]) {
                vlSymsp->TOP__sat_submodule.tree_up = 1U;
                vlSymsp->TOP__sat_submodule.tree_cid 
                    = (0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i);
                vlSymsp->TOP__sat_submodule.tree_ulp 
                    = vlSymsp->TOP__sat_submodule.proc_ulp
                    [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i)];
            }
            vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i 
                = (vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i 
                   - (IData)(1U));
        }
        if ((1U & (~ (IData)(vlSymsp->TOP__sat_submodule.tree_up)))) {
            vlSymsp->TOP__sat_submodule.tree_done = 1U;
            vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i = 0U;
            while ((0x2000U > vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i)) {
                if ((1U & (~ vlSymsp->TOP__sat_submodule.proc_done
                           [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i)]))) {
                    vlSymsp->TOP__sat_submodule.tree_done = 0U;
                }
                vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i 
                    = ((IData)(1U) + vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i);
            }
        }
    }
    vlSymsp->TOP__sat_submodule.conf_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.state)) 
                                            & (IData)(vlSymsp->TOP__sat_submodule.tree_conf));
    vlSymsp->TOP__sat_submodule.done_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.state)) 
                                            & (IData)(vlSymsp->TOP__sat_submodule.tree_done));
    vlSymsp->TOP__sat_submodule.cid_out = vlSymsp->TOP__sat_submodule.tree_cid;
    vlSymsp->TOP__sat_submodule.up_lit_pos = vlSymsp->TOP__sat_submodule.tree_ulp;
    vlSymsp->TOP__sat_submodule.up_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.state)) 
                                          & (IData)(vlSymsp->TOP__sat_submodule.tree_up));
    vlSymsp->TOP__sat_submodule.rd_row = 0U;
    if (((3U == (IData)(vlSymsp->TOP__sat_submodule.state)) 
         & (IData)(vlSymsp->TOP__sat_submodule.tree_up))) {
        vlSymsp->TOP__sat_submodule.rd_row = (0x7fffU 
                                              & (((IData)(vlSymsp->TOP__sat_submodule.tree_cid) 
                                                  << 2U) 
                                                 + (IData)(vlSymsp->TOP__sat_submodule.tree_ulp)));
    } else {
        if ((5U == (IData)(vlSymsp->TOP__sat_submodule.state))) {
            vlSymsp->TOP__sat_submodule.rd_row = (0x7fffU 
                                                  & (((IData)(vlSymsp->TOP__sat_submodule.cid_in) 
                                                      << 2U) 
                                                     + (IData)(vlSymsp->TOP__sat_submodule.cit_lit_idx)));
        }
    }
    vlSymsp->TOP__sat_submodule.vid_out = vlSymsp->TOP__sat_submodule.vid_store
        [vlSymsp->TOP__sat_submodule.rd_row];
    vlSymsp->TOP__sat_submodule.pol_out = vlSymsp->TOP__sat_submodule.pol_store
        [vlSymsp->TOP__sat_submodule.rd_row];
}

VL_INLINE_OPT void Vsat_submodule_sat_submodule::_sequent__TOP__sat_submodule__2(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_submodule_sat_submodule::_sequent__TOP__sat_submodule__2\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*2:0*/ __Vdly__state;
    CData/*1:0*/ __Vdly__cit_lit_idx;
    // Body
    __Vdly__cit_lit_idx = vlSymsp->TOP__sat_submodule.cit_lit_idx;
    __Vdly__state = vlSymsp->TOP__sat_submodule.state;
    if (vlSymsp->TOP__sat_submodule.rst_n) {
        if ((4U & (IData)(vlSymsp->TOP__sat_submodule.state))) {
            if ((2U & (IData)(vlSymsp->TOP__sat_submodule.state))) {
                __Vdly__state = 0U;
            } else {
                if ((1U & (IData)(vlSymsp->TOP__sat_submodule.state))) {
                    if ((3U == (IData)(vlSymsp->TOP__sat_submodule.cit_lit_idx))) {
                        __Vdly__state = 0U;
                        __Vdly__cit_lit_idx = 0U;
                    } else {
                        __Vdly__cit_lit_idx = (3U & 
                                               ((IData)(1U) 
                                                + (IData)(vlSymsp->TOP__sat_submodule.cit_lit_idx)));
                    }
                } else {
                    vlSymsp->TOP__sat_submodule.unnamedblk9__DOT__r = 0U;
                    while ((0x8000U > vlSymsp->TOP__sat_submodule.unnamedblk9__DOT__r)) {
                        if ((vlSymsp->TOP__sat_submodule.row_valid
                             [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk9__DOT__r)] 
                             & (vlSymsp->TOP__sat_submodule.vid_store
                                [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk9__DOT__r)] 
                                == vlSymsp->TOP__sat_submodule.vid_in))) {
                            vlSymsp->TOP__sat_submodule.val_store[(0x7fffU 
                                                                   & vlSymsp->TOP__sat_submodule.unnamedblk9__DOT__r)] = 1U;
                        }
                        vlSymsp->TOP__sat_submodule.unnamedblk9__DOT__r 
                            = ((IData)(1U) + vlSymsp->TOP__sat_submodule.unnamedblk9__DOT__r);
                    }
                    __Vdly__state = 0U;
                }
            }
        } else {
            if ((2U & (IData)(vlSymsp->TOP__sat_submodule.state))) {
                if ((1U & (IData)(vlSymsp->TOP__sat_submodule.state))) {
                    __Vdly__state = 0U;
                } else {
                    vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r = 0U;
                    while ((0x8000U > vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r)) {
                        if ((vlSymsp->TOP__sat_submodule.row_valid
                             [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r)] 
                             & (vlSymsp->TOP__sat_submodule.vid_store
                                [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r)] 
                                == vlSymsp->TOP__sat_submodule.vid_in))) {
                            vlSymsp->TOP__sat_submodule.val_store[(0x7fffU 
                                                                   & vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r)] 
                                = vlSymsp->TOP__sat_submodule.val_in;
                            vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                              & vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r)] = 1U;
                        } else {
                            vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                              & vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r)] = 0U;
                        }
                        vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r 
                            = ((IData)(1U) + vlSymsp->TOP__sat_submodule.unnamedblk8__DOT__r);
                    }
                    __Vdly__state = 3U;
                }
            } else {
                if ((1U & (IData)(vlSymsp->TOP__sat_submodule.state))) {
                    vlSymsp->TOP__sat_submodule.vid_store[vlSymsp->TOP__sat_submodule.row_addr] 
                        = vlSymsp->TOP__sat_submodule.vid_in;
                    vlSymsp->TOP__sat_submodule.pol_store[vlSymsp->TOP__sat_submodule.row_addr] 
                        = vlSymsp->TOP__sat_submodule.pol_in;
                    vlSymsp->TOP__sat_submodule.val_store[vlSymsp->TOP__sat_submodule.row_addr] = 1U;
                    vlSymsp->TOP__sat_submodule.row_valid[vlSymsp->TOP__sat_submodule.row_addr] = 1U;
                    __Vdly__state = 0U;
                } else {
                    if ((4U & (IData)(vlSymsp->TOP__sat_submodule.op))) {
                        if ((2U & (IData)(vlSymsp->TOP__sat_submodule.op))) {
                            __Vdly__state = 0U;
                        } else {
                            if ((1U & (IData)(vlSymsp->TOP__sat_submodule.op))) {
                                __Vdly__state = 0U;
                            } else {
                                __Vdly__state = 5U;
                                __Vdly__cit_lit_idx = 0U;
                            }
                        }
                    } else {
                        __Vdly__state = ((2U & (IData)(vlSymsp->TOP__sat_submodule.op))
                                          ? ((1U & (IData)(vlSymsp->TOP__sat_submodule.op))
                                              ? 4U : 2U)
                                          : ((1U & (IData)(vlSymsp->TOP__sat_submodule.op))
                                              ? 1U : 0U));
                    }
                }
            }
        }
    } else {
        vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r = 0U;
        __Vdly__state = 0U;
        __Vdly__cit_lit_idx = 0U;
        while ((0x8000U > vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r)) {
            vlSymsp->TOP__sat_submodule.vid_store[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r)] = 0U;
            vlSymsp->TOP__sat_submodule.pol_store[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r)] = 0U;
            vlSymsp->TOP__sat_submodule.val_store[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r)] = 1U;
            vlSymsp->TOP__sat_submodule.row_valid[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r)] = 0U;
            vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r)] = 0U;
            vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r 
                = ((IData)(1U) + vlSymsp->TOP__sat_submodule.unnamedblk7__DOT__r);
        }
    }
    vlSymsp->TOP__sat_submodule.cit_lit_idx = __Vdly__cit_lit_idx;
    vlSymsp->TOP__sat_submodule.state = __Vdly__state;
    vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c = 0U;
    while ((0x2000U > vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)) {
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] = 0U;
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] = 0U;
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = (vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
               << 2U);
        vlSymsp->TOP__sat_submodule.c_lit_u[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (vlSymsp->TOP__sat_submodule.row_valid
                  [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                  & (1U == vlSymsp->TOP__sat_submodule.val_store
                     [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])));
        vlSymsp->TOP__sat_submodule.c_lit_t[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.c_lit_t
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.row_valid
                   [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (~ vlSymsp->TOP__sat_submodule.c_lit_u
                      [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
                  & ((1U & vlSymsp->TOP__sat_submodule.val_store
                      [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                     == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))));
        vlSymsp->TOP__sat_submodule.c_lit_f[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xeU & vlSymsp->TOP__sat_submodule.c_lit_f
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (1U & ((~ vlSymsp->TOP__sat_submodule.row_valid
                         [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                        | ((~ vlSymsp->TOP__sat_submodule.c_lit_u
                            [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
                           & ((1U & vlSymsp->TOP__sat_submodule.val_store
                               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                              == vlSymsp->TOP__sat_submodule.pol_store
                              [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))));
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_any_valid
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.row_valid
               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__i = 1U;
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(1U) + (vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_u[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.row_valid
                   [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 1U));
        vlSymsp->TOP__sat_submodule.c_lit_t[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.c_lit_t
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.row_valid
                    [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.c_lit_u
                          [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                          >> 1U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0x7fffU 
                                                  & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 1U));
        vlSymsp->TOP__sat_submodule.c_lit_f[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xdU & vlSymsp->TOP__sat_submodule.c_lit_f
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (2U & (((~ vlSymsp->TOP__sat_submodule.row_valid
                          [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.c_lit_u
                                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                                >> 1U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0x7fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0x7fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 1U)));
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_any_valid
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.row_valid
               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__i = 2U;
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(2U) + (vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_u[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.row_valid
                   [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_t[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.c_lit_t
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.row_valid
                    [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.c_lit_u
                          [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                          >> 2U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0x7fffU 
                                                  & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_f[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((0xbU & vlSymsp->TOP__sat_submodule.c_lit_f
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (4U & (((~ vlSymsp->TOP__sat_submodule.row_valid
                          [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.c_lit_u
                                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                                >> 2U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0x7fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0x7fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 2U)));
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_any_valid
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.row_valid
               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__i = 3U;
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(3U) + (vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
                              << 2U));
        vlSymsp->TOP__sat_submodule.c_lit_u[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | ((vlSymsp->TOP__sat_submodule.row_valid
                   [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (1U == vlSymsp->TOP__sat_submodule.val_store
                      [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])) 
                  << 3U));
        vlSymsp->TOP__sat_submodule.c_lit_t[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.c_lit_t
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (((vlSymsp->TOP__sat_submodule.row_valid
                    [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                    & (~ (vlSymsp->TOP__sat_submodule.c_lit_u
                          [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                          >> 3U))) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                       [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                      == (1U & (~ vlSymsp->TOP__sat_submodule.pol_store
                                                [(0x7fffU 
                                                  & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))) 
                  << 3U));
        vlSymsp->TOP__sat_submodule.c_lit_f[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((7U & vlSymsp->TOP__sat_submodule.c_lit_f
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               | (8U & (((~ vlSymsp->TOP__sat_submodule.row_valid
                          [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                         | ((~ (vlSymsp->TOP__sat_submodule.c_lit_u
                                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                                >> 3U)) & ((1U & vlSymsp->TOP__sat_submodule.val_store
                                            [(0x7fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                           == vlSymsp->TOP__sat_submodule.pol_store
                                           [(0x7fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                        << 3U)));
        vlSymsp->TOP__sat_submodule.c_ml_any[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.ml_q[(0x7fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.c_any_valid[(0x1fffU 
                                                 & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (vlSymsp->TOP__sat_submodule.c_any_valid
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
               | vlSymsp->TOP__sat_submodule.row_valid
               [(0x7fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__unnamedblk2__DOT__i = 4U;
        vlSymsp->TOP__sat_submodule.proc_done[(0x1fffU 
                                               & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (0U != vlSymsp->TOP__sat_submodule.c_lit_t
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]);
        vlSymsp->TOP__sat_submodule.proc_conf[(0x1fffU 
                                               & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((vlSymsp->TOP__sat_submodule.c_any_valid
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                & vlSymsp->TOP__sat_submodule.c_ml_any
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
               & (0xfU == vlSymsp->TOP__sat_submodule.c_lit_f
                  [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]));
        vlSymsp->TOP__sat_submodule.proc_up[(0x1fffU 
                                             & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = (((((((1U == vlSymsp->TOP__sat_submodule.c_lit_u
                     [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]) 
                    | (2U == vlSymsp->TOP__sat_submodule.c_lit_u
                       [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
                   | (4U == vlSymsp->TOP__sat_submodule.c_lit_u
                      [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
                  | (8U == vlSymsp->TOP__sat_submodule.c_lit_u
                     [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
                 & (0xfU == (vlSymsp->TOP__sat_submodule.c_lit_f
                             [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
                             | vlSymsp->TOP__sat_submodule.c_lit_u
                             [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]))) 
                & (~ vlSymsp->TOP__sat_submodule.proc_done
                   [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])) 
               & vlSymsp->TOP__sat_submodule.c_ml_any
               [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)]);
        vlSymsp->TOP__sat_submodule.proc_ulp[(0x1fffU 
                                              & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)] 
            = ((1U & vlSymsp->TOP__sat_submodule.c_lit_u
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])
                ? 0U : ((2U & vlSymsp->TOP__sat_submodule.c_lit_u
                         [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])
                         ? 1U : ((4U & vlSymsp->TOP__sat_submodule.c_lit_u
                                  [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c)])
                                  ? 2U : 3U)));
        vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c 
            = ((IData)(1U) + vlSymsp->TOP__sat_submodule.unnamedblk1__DOT__c);
    }
    vlSymsp->TOP__sat_submodule.valid_out = ((((1U 
                                                == (IData)(vlSymsp->TOP__sat_submodule.state)) 
                                               | (3U 
                                                  == (IData)(vlSymsp->TOP__sat_submodule.state))) 
                                              | (4U 
                                                 == (IData)(vlSymsp->TOP__sat_submodule.state))) 
                                             | (5U 
                                                == (IData)(vlSymsp->TOP__sat_submodule.state)));
    vlSymsp->TOP__sat_submodule.tree_conf = 0U;
    vlSymsp->TOP__sat_submodule.tree_up = 0U;
    vlSymsp->TOP__sat_submodule.tree_done = 0U;
    vlSymsp->TOP__sat_submodule.tree_cid = 0U;
    vlSymsp->TOP__sat_submodule.tree_ulp = 0U;
    vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i = 0x1fffU;
    while (VL_LTES_III(1,32,32, 0U, vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i)) {
        if (vlSymsp->TOP__sat_submodule.proc_conf[(0x1fffU 
                                                   & vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i)]) {
            vlSymsp->TOP__sat_submodule.tree_conf = 1U;
            vlSymsp->TOP__sat_submodule.tree_cid = 
                (0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i);
        }
        vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i 
            = (vlSymsp->TOP__sat_submodule.unnamedblk4__DOT__i 
               - (IData)(1U));
    }
    if ((1U & (~ (IData)(vlSymsp->TOP__sat_submodule.tree_conf)))) {
        vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i = 0x1fffU;
        while (VL_LTES_III(1,32,32, 0U, vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i)) {
            if (vlSymsp->TOP__sat_submodule.proc_up
                [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i)]) {
                vlSymsp->TOP__sat_submodule.tree_up = 1U;
                vlSymsp->TOP__sat_submodule.tree_cid 
                    = (0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i);
                vlSymsp->TOP__sat_submodule.tree_ulp 
                    = vlSymsp->TOP__sat_submodule.proc_ulp
                    [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i)];
            }
            vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i 
                = (vlSymsp->TOP__sat_submodule.unnamedblk5__DOT__i 
                   - (IData)(1U));
        }
        if ((1U & (~ (IData)(vlSymsp->TOP__sat_submodule.tree_up)))) {
            vlSymsp->TOP__sat_submodule.tree_done = 1U;
            vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i = 0U;
            while ((0x2000U > vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i)) {
                if ((1U & (~ vlSymsp->TOP__sat_submodule.proc_done
                           [(0x1fffU & vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i)]))) {
                    vlSymsp->TOP__sat_submodule.tree_done = 0U;
                }
                vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i 
                    = ((IData)(1U) + vlSymsp->TOP__sat_submodule.unnamedblk6__DOT__i);
            }
        }
    }
    vlSymsp->TOP__sat_submodule.conf_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.state)) 
                                            & (IData)(vlSymsp->TOP__sat_submodule.tree_conf));
    vlSymsp->TOP__sat_submodule.done_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.state)) 
                                            & (IData)(vlSymsp->TOP__sat_submodule.tree_done));
    vlSymsp->TOP__sat_submodule.cid_out = vlSymsp->TOP__sat_submodule.tree_cid;
    vlSymsp->TOP__sat_submodule.up_lit_pos = vlSymsp->TOP__sat_submodule.tree_ulp;
    vlSymsp->TOP__sat_submodule.up_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.state)) 
                                          & (IData)(vlSymsp->TOP__sat_submodule.tree_up));
}

VL_INLINE_OPT void Vsat_submodule_sat_submodule::_combo__TOP__sat_submodule__3(Vsat_submodule__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_submodule_sat_submodule::_combo__TOP__sat_submodule__3\n"); );
    Vsat_submodule* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlSymsp->TOP__sat_submodule.rd_row = 0U;
    if (((3U == (IData)(vlSymsp->TOP__sat_submodule.state)) 
         & (IData)(vlSymsp->TOP__sat_submodule.tree_up))) {
        vlSymsp->TOP__sat_submodule.rd_row = (0x7fffU 
                                              & (((IData)(vlSymsp->TOP__sat_submodule.tree_cid) 
                                                  << 2U) 
                                                 + (IData)(vlSymsp->TOP__sat_submodule.tree_ulp)));
    } else {
        if ((5U == (IData)(vlSymsp->TOP__sat_submodule.state))) {
            vlSymsp->TOP__sat_submodule.rd_row = (0x7fffU 
                                                  & (((IData)(vlSymsp->TOP__sat_submodule.cid_in) 
                                                      << 2U) 
                                                     + (IData)(vlSymsp->TOP__sat_submodule.cit_lit_idx)));
        }
    }
    vlSymsp->TOP__sat_submodule.vid_out = vlSymsp->TOP__sat_submodule.vid_store
        [vlSymsp->TOP__sat_submodule.rd_row];
    vlSymsp->TOP__sat_submodule.pol_out = vlSymsp->TOP__sat_submodule.pol_store
        [vlSymsp->TOP__sat_submodule.rd_row];
}

void Vsat_submodule_sat_submodule::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_submodule_sat_submodule::_ctor_var_reset\n"); );
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
    { int __Vi0=0; for (; __Vi0<32768; ++__Vi0) {
            vid_store[__Vi0] = VL_RAND_RESET_I(20);
    }}
    { int __Vi0=0; for (; __Vi0<32768; ++__Vi0) {
            pol_store[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<32768; ++__Vi0) {
            val_store[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<32768; ++__Vi0) {
            row_valid[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<32768; ++__Vi0) {
            ml_q[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            proc_conf[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            proc_up[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            proc_done[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            proc_ulp[__Vi0] = VL_RAND_RESET_I(2);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            c_lit_u[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            c_lit_t[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            c_lit_f[__Vi0] = VL_RAND_RESET_I(4);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            c_ml_any[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<8192; ++__Vi0) {
            c_any_valid[__Vi0] = VL_RAND_RESET_I(1);
    }}
    tree_conf = VL_RAND_RESET_I(1);
    tree_up = VL_RAND_RESET_I(1);
    tree_done = VL_RAND_RESET_I(1);
    tree_cid = VL_RAND_RESET_I(13);
    tree_ulp = VL_RAND_RESET_I(2);
    state = VL_RAND_RESET_I(3);
    cit_lit_idx = VL_RAND_RESET_I(2);
    rd_row = VL_RAND_RESET_I(15);
    unnamedblk1__DOT__c = 0;
    unnamedblk1__DOT__unnamedblk2__DOT__i = 0;
    unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r = 0;
    unnamedblk4__DOT__i = 0;
    unnamedblk5__DOT__i = 0;
    unnamedblk6__DOT__i = 0;
    unnamedblk8__DOT__r = 0;
    unnamedblk9__DOT__r = 0;
    unnamedblk7__DOT__r = 0;
}
