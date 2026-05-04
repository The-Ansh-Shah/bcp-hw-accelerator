// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_solver.h for the primary calling header

#include "Vsat_solver__pch.h"

void Vsat_solver_sat_submodule___ico_sequent__TOP__sat_submodule__0(Vsat_solver_sat_submodule* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_solver_sat_submodule___ico_sequent__TOP__sat_submodule__0\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __PVT__unnamedblk1__DOT__c;
    __PVT__unnamedblk1__DOT__c = 0;
    IData/*31:0*/ __PVT__unnamedblk4__DOT__i;
    __PVT__unnamedblk4__DOT__i = 0;
    // Body
    __PVT__unnamedblk1__DOT__c = 0U;
    while (VL_GTS_III(32, 0x00000400U, __PVT__unnamedblk1__DOT__c)) {
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] = 0U;
        vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = VL_MULS_III(32, (IData)(4U), __PVT__unnamedblk1__DOT__c);
        vlSelfRef.__PVT__c_lit_u[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0eU & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | (vlSelfRef.__PVT__row_valid[(0x00000fffU 
                                              & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                  & (1U == vlSelfRef.val_store[(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])));
        vlSelfRef.__PVT__c_lit_t[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0eU & vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((vlSelfRef.__PVT__row_valid[(0x00000fffU 
                                               & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (~ vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)])) 
                  & ((1U & vlSelfRef.val_store[(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                     == (1U & (~ (IData)(vlSelfRef.pol_store
                                         [(0x00000fffU 
                                           & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))))));
        vlSelfRef.__PVT__c_lit_f[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0eU & vlSelfRef.__PVT__c_lit_f[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((vlSelfRef.__PVT__row_valid[(0x00000fffU 
                                               & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (~ vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)])) 
                  & ((1U & vlSelfRef.val_store[(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                     == vlSelfRef.pol_store[(0x00000fffU 
                                             & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])));
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                          & __PVT__unnamedblk1__DOT__c)] 
               | vlSelfRef.ml_q[(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(1U) + VL_MULS_III(32, (IData)(4U), __PVT__unnamedblk1__DOT__c));
        vlSelfRef.__PVT__c_lit_u[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0dU & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)((vlSelfRef.__PVT__row_valid
                           [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                           & (1U == vlSelfRef.val_store
                              [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 1U));
        vlSelfRef.__PVT__c_lit_t[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0dU & vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 1U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == (1U 
                                                  & (~ (IData)(vlSelfRef.pol_store
                                                               [
                                                               (0x00000fffU 
                                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))))) 
                  << 1U));
        vlSelfRef.__PVT__c_lit_f[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0dU & vlSelfRef.__PVT__c_lit_f[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 1U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == vlSelfRef.pol_store
                                              [(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 1U));
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                          & __PVT__unnamedblk1__DOT__c)] 
               | vlSelfRef.ml_q[(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(2U) + VL_MULS_III(32, (IData)(4U), __PVT__unnamedblk1__DOT__c));
        vlSelfRef.__PVT__c_lit_u[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0bU & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)((vlSelfRef.__PVT__row_valid
                           [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                           & (1U == vlSelfRef.val_store
                              [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 2U));
        vlSelfRef.__PVT__c_lit_t[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0bU & vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 2U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == (1U 
                                                  & (~ (IData)(vlSelfRef.pol_store
                                                               [
                                                               (0x00000fffU 
                                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))))) 
                  << 2U));
        vlSelfRef.__PVT__c_lit_f[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0bU & vlSelfRef.__PVT__c_lit_f[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 2U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == vlSelfRef.pol_store
                                              [(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 2U));
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                          & __PVT__unnamedblk1__DOT__c)] 
               | vlSelfRef.ml_q[(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(3U) + VL_MULS_III(32, (IData)(4U), __PVT__unnamedblk1__DOT__c));
        vlSelfRef.__PVT__c_lit_u[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)((vlSelfRef.__PVT__row_valid
                           [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                           & (1U == vlSelfRef.val_store
                              [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 3U));
        vlSelfRef.__PVT__c_lit_t[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 3U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == (1U 
                                                  & (~ (IData)(vlSelfRef.pol_store
                                                               [
                                                               (0x00000fffU 
                                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))))) 
                  << 3U));
        vlSelfRef.__PVT__c_lit_f[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSelfRef.__PVT__c_lit_f[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 3U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == vlSelfRef.pol_store
                                              [(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 3U));
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                          & __PVT__unnamedblk1__DOT__c)] 
               | vlSelfRef.ml_q[(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSelfRef.proc_done[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (0U != vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)]);
        vlSelfRef.proc_conf[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (0x0000000fU == vlSelfRef.__PVT__c_lit_f
               [(0x000003ffU & __PVT__unnamedblk1__DOT__c)]);
        vlSelfRef.proc_up[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (((((((1U == vlSelfRef.__PVT__c_lit_u
                     [(0x000003ffU & __PVT__unnamedblk1__DOT__c)]) 
                    | (2U == vlSelfRef.__PVT__c_lit_u
                       [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])) 
                   | (4U == vlSelfRef.__PVT__c_lit_u
                      [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])) 
                  | (8U == vlSelfRef.__PVT__c_lit_u
                     [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])) 
                 & (0x0000000fU == (vlSelfRef.__PVT__c_lit_f
                                    [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                    | vlSelfRef.__PVT__c_lit_u
                                    [(0x000003ffU & __PVT__unnamedblk1__DOT__c)]))) 
                & (~ (IData)(vlSelfRef.proc_done[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]))) 
               & vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                            & __PVT__unnamedblk1__DOT__c)]);
        vlSelfRef.__PVT__proc_ulp[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((1U & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)])
                ? 0U : ((2U & vlSelfRef.__PVT__c_lit_u
                         [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])
                         ? 1U : ((4U & vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])
                                  ? 2U : 3U)));
        __PVT__unnamedblk1__DOT__c = ((IData)(1U) + __PVT__unnamedblk1__DOT__c);
    }
    vlSelfRef.__PVT__tree_conf = 0U;
    vlSelfRef.__PVT__tree_up = 0U;
    vlSelfRef.__PVT__tree_done = 0U;
    vlSelfRef.__PVT__tree_cid = 0U;
    vlSelfRef.__PVT__tree_ulp = 0U;
    __PVT__unnamedblk4__DOT__i = 0x000003ffU;
    while (VL_LTES_III(32, 0U, __PVT__unnamedblk4__DOT__i)) {
        if (vlSelfRef.proc_conf[(0x000003ffU & __PVT__unnamedblk4__DOT__i)]) {
            vlSelfRef.__PVT__tree_conf = 1U;
            vlSelfRef.__PVT__tree_cid = (0x000003ffU 
                                         & __PVT__unnamedblk4__DOT__i);
        }
        __PVT__unnamedblk4__DOT__i = (__PVT__unnamedblk4__DOT__i 
                                      - (IData)(1U));
    }
    if ((1U & (~ (IData)(vlSelfRef.__PVT__tree_conf)))) {
        vlSelfRef.__PVT__unnamedblk5__DOT__i = 0x000003ffU;
        while (VL_LTES_III(32, 0U, vlSelfRef.__PVT__unnamedblk5__DOT__i)) {
            if (vlSelfRef.proc_up[(0x000003ffU & vlSelfRef.__PVT__unnamedblk5__DOT__i)]) {
                vlSelfRef.__PVT__tree_up = 1U;
                vlSelfRef.__PVT__tree_cid = (0x000003ffU 
                                             & vlSelfRef.__PVT__unnamedblk5__DOT__i);
                vlSelfRef.__PVT__tree_ulp = vlSelfRef.__PVT__proc_ulp
                    [(0x000003ffU & vlSelfRef.__PVT__unnamedblk5__DOT__i)];
            }
            vlSelfRef.__PVT__unnamedblk5__DOT__i = 
                (vlSelfRef.__PVT__unnamedblk5__DOT__i 
                 - (IData)(1U));
        }
        if ((1U & (~ (IData)(vlSelfRef.__PVT__tree_up)))) {
            vlSelfRef.__PVT__tree_done = 1U;
            vlSelfRef.__PVT__unnamedblk6__DOT__i = 0U;
            while (VL_GTS_III(32, 0x00000400U, vlSelfRef.__PVT__unnamedblk6__DOT__i)) {
                if ((1U & (~ (IData)(vlSelfRef.proc_done
                                     [(0x000003ffU 
                                       & vlSelfRef.__PVT__unnamedblk6__DOT__i)])))) {
                    vlSelfRef.__PVT__tree_done = 0U;
                }
                vlSelfRef.__PVT__unnamedblk6__DOT__i 
                    = ((IData)(1U) + vlSelfRef.__PVT__unnamedblk6__DOT__i);
            }
        }
    }
    vlSelfRef.__PVT__rd_row = 0U;
    if (((3U == (IData)(vlSelfRef.__PVT__state)) & (IData)(vlSelfRef.__PVT__tree_up))) {
        vlSelfRef.__PVT__rd_row = (0x00000fffU & (VL_SHIFTL_III(12,32,32, (IData)(vlSelfRef.__PVT__tree_cid), 2U) 
                                                  + (IData)(vlSelfRef.__PVT__tree_ulp)));
    } else if ((5U == (IData)(vlSelfRef.__PVT__state))) {
        vlSelfRef.__PVT__rd_row = (0x00000fffU & (VL_SHIFTL_III(12,32,32, (IData)(vlSymsp->TOP.cid_in), 2U) 
                                                  + (IData)(vlSelfRef.__PVT__cit_lit_idx)));
    }
}

void Vsat_solver_sat_submodule___nba_sequent__TOP__sat_submodule__0(Vsat_solver_sat_submodule* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_solver_sat_submodule___nba_sequent__TOP__sat_submodule__0\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __PVT__unnamedblk1__DOT__c;
    __PVT__unnamedblk1__DOT__c = 0;
    IData/*31:0*/ __PVT__unnamedblk4__DOT__i;
    __PVT__unnamedblk4__DOT__i = 0;
    CData/*2:0*/ __Vdly__state;
    __Vdly__state = 0;
    CData/*1:0*/ __Vdly__cit_lit_idx;
    __Vdly__cit_lit_idx = 0;
    SData/*11:0*/ __VdlyDim0__val_store__v0;
    __VdlyDim0__val_store__v0 = 0;
    CData/*1:0*/ __VdlyVal__val_store__v1;
    __VdlyVal__val_store__v1 = 0;
    SData/*11:0*/ __VdlyDim0__val_store__v1;
    __VdlyDim0__val_store__v1 = 0;
    SData/*11:0*/ __VdlyDim0__ml_q__v0;
    __VdlyDim0__ml_q__v0 = 0;
    SData/*11:0*/ __VdlyDim0__ml_q__v1;
    __VdlyDim0__ml_q__v1 = 0;
    IData/*19:0*/ __VdlyVal__vid_store__v0;
    __VdlyVal__vid_store__v0 = 0;
    SData/*11:0*/ __VdlyDim0__vid_store__v0;
    __VdlyDim0__vid_store__v0 = 0;
    CData/*0:0*/ __VdlyVal__pol_store__v0;
    __VdlyVal__pol_store__v0 = 0;
    SData/*11:0*/ __VdlyDim0__pol_store__v0;
    __VdlyDim0__pol_store__v0 = 0;
    SData/*11:0*/ __VdlyDim0__val_store__v2;
    __VdlyDim0__val_store__v2 = 0;
    SData/*11:0*/ __VdlyDim0__row_valid__v0;
    __VdlyDim0__row_valid__v0 = 0;
    SData/*11:0*/ __VdlyDim0__vid_store__v1;
    __VdlyDim0__vid_store__v1 = 0;
    SData/*11:0*/ __VdlyDim0__pol_store__v1;
    __VdlyDim0__pol_store__v1 = 0;
    SData/*11:0*/ __VdlyDim0__val_store__v3;
    __VdlyDim0__val_store__v3 = 0;
    SData/*11:0*/ __VdlyDim0__row_valid__v1;
    __VdlyDim0__row_valid__v1 = 0;
    SData/*11:0*/ __VdlyDim0__ml_q__v2;
    __VdlyDim0__ml_q__v2 = 0;
    // Body
    __Vdly__cit_lit_idx = vlSelfRef.__PVT__cit_lit_idx;
    __Vdly__state = vlSelfRef.__PVT__state;
    if (vlSymsp->TOP.rst_n) {
        if ((4U & (IData)(vlSelfRef.__PVT__state))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state))) {
                __Vdly__state = 0U;
            } else if ((1U & (IData)(vlSelfRef.__PVT__state))) {
                if ((3U == (IData)(vlSelfRef.__PVT__cit_lit_idx))) {
                    __Vdly__state = 0U;
                    __Vdly__cit_lit_idx = 0U;
                } else {
                    __Vdly__cit_lit_idx = (3U & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__cit_lit_idx)));
                }
            } else {
                vlSelfRef.__PVT__unnamedblk9__DOT__r = 0U;
                while (VL_GTS_III(32, 0x00001000U, vlSelfRef.__PVT__unnamedblk9__DOT__r)) {
                    if ((vlSelfRef.__PVT__row_valid
                         [(0x00000fffU & vlSelfRef.__PVT__unnamedblk9__DOT__r)] 
                         & (vlSelfRef.vid_store[(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk9__DOT__r)] 
                            == vlSymsp->TOP.vid_in))) {
                        __VdlyDim0__val_store__v0 = 
                            (0x00000fffU & vlSelfRef.__PVT__unnamedblk9__DOT__r);
                        vlSelfRef.__VdlyCommitQueueval_store.enqueue(1U, (IData)(__VdlyDim0__val_store__v0));
                    }
                    vlSelfRef.__PVT__unnamedblk9__DOT__r 
                        = ((IData)(1U) + vlSelfRef.__PVT__unnamedblk9__DOT__r);
                }
                __Vdly__state = 0U;
            }
        } else if ((2U & (IData)(vlSelfRef.__PVT__state))) {
            if ((1U & (IData)(vlSelfRef.__PVT__state))) {
                __Vdly__state = 0U;
            } else {
                vlSelfRef.__PVT__unnamedblk8__DOT__r = 0U;
                while (VL_GTS_III(32, 0x00001000U, vlSelfRef.__PVT__unnamedblk8__DOT__r)) {
                    if ((vlSelfRef.__PVT__row_valid
                         [(0x00000fffU & vlSelfRef.__PVT__unnamedblk8__DOT__r)] 
                         & (vlSelfRef.vid_store[(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk8__DOT__r)] 
                            == vlSymsp->TOP.vid_in))) {
                        __VdlyVal__val_store__v1 = vlSymsp->TOP.val_in;
                        __VdlyDim0__val_store__v1 = 
                            (0x00000fffU & vlSelfRef.__PVT__unnamedblk8__DOT__r);
                        vlSelfRef.__VdlyCommitQueueval_store.enqueue(__VdlyVal__val_store__v1, (IData)(__VdlyDim0__val_store__v1));
                        __VdlyDim0__ml_q__v0 = (0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk8__DOT__r);
                        vlSelfRef.__VdlyCommitQueueml_q.enqueue(1U, (IData)(__VdlyDim0__ml_q__v0));
                    } else {
                        __VdlyDim0__ml_q__v1 = (0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk8__DOT__r);
                        vlSelfRef.__VdlyCommitQueueml_q.enqueue(0U, (IData)(__VdlyDim0__ml_q__v1));
                    }
                    vlSelfRef.__PVT__unnamedblk8__DOT__r 
                        = ((IData)(1U) + vlSelfRef.__PVT__unnamedblk8__DOT__r);
                }
                __Vdly__state = 3U;
            }
        } else if ((1U & (IData)(vlSelfRef.__PVT__state))) {
            __VdlyVal__vid_store__v0 = vlSymsp->TOP.vid_in;
            __VdlyDim0__vid_store__v0 = vlSymsp->TOP.row_addr;
            vlSelfRef.__VdlyCommitQueuevid_store.enqueue(__VdlyVal__vid_store__v0, (IData)(__VdlyDim0__vid_store__v0));
            __VdlyVal__pol_store__v0 = vlSymsp->TOP.pol_in;
            __VdlyDim0__pol_store__v0 = vlSymsp->TOP.row_addr;
            vlSelfRef.__VdlyCommitQueuepol_store.enqueue(__VdlyVal__pol_store__v0, (IData)(__VdlyDim0__pol_store__v0));
            __VdlyDim0__val_store__v2 = vlSymsp->TOP.row_addr;
            vlSelfRef.__VdlyCommitQueueval_store.enqueue(1U, (IData)(__VdlyDim0__val_store__v2));
            __VdlyDim0__row_valid__v0 = vlSymsp->TOP.row_addr;
            vlSelfRef.__VdlyCommitQueuerow_valid.enqueue(1U, (IData)(__VdlyDim0__row_valid__v0));
            __Vdly__state = 0U;
        } else if ((4U & (IData)(vlSymsp->TOP.op))) {
            if ((2U & (IData)(vlSymsp->TOP.op))) {
                __Vdly__state = 0U;
            } else if ((1U & (IData)(vlSymsp->TOP.op))) {
                __Vdly__state = 0U;
            } else {
                __Vdly__state = 5U;
                __Vdly__cit_lit_idx = 0U;
            }
        } else {
            __Vdly__state = ((2U & (IData)(vlSymsp->TOP.op))
                              ? ((1U & (IData)(vlSymsp->TOP.op))
                                  ? 4U : 2U) : ((1U 
                                                 & (IData)(vlSymsp->TOP.op))
                                                 ? 1U
                                                 : 0U));
        }
    } else {
        vlSelfRef.__PVT__unnamedblk7__DOT__r = 0U;
        __Vdly__state = 0U;
        __Vdly__cit_lit_idx = 0U;
        while (VL_GTS_III(32, 0x00001000U, vlSelfRef.__PVT__unnamedblk7__DOT__r)) {
            __VdlyDim0__vid_store__v1 = (0x00000fffU 
                                         & vlSelfRef.__PVT__unnamedblk7__DOT__r);
            vlSelfRef.__VdlyCommitQueuevid_store.enqueue(0U, (IData)(__VdlyDim0__vid_store__v1));
            __VdlyDim0__pol_store__v1 = (0x00000fffU 
                                         & vlSelfRef.__PVT__unnamedblk7__DOT__r);
            vlSelfRef.__VdlyCommitQueuepol_store.enqueue(0U, (IData)(__VdlyDim0__pol_store__v1));
            __VdlyDim0__val_store__v3 = (0x00000fffU 
                                         & vlSelfRef.__PVT__unnamedblk7__DOT__r);
            vlSelfRef.__VdlyCommitQueueval_store.enqueue(1U, (IData)(__VdlyDim0__val_store__v3));
            __VdlyDim0__row_valid__v1 = (0x00000fffU 
                                         & vlSelfRef.__PVT__unnamedblk7__DOT__r);
            vlSelfRef.__VdlyCommitQueuerow_valid.enqueue(0U, (IData)(__VdlyDim0__row_valid__v1));
            __VdlyDim0__ml_q__v2 = (0x00000fffU & vlSelfRef.__PVT__unnamedblk7__DOT__r);
            vlSelfRef.__VdlyCommitQueueml_q.enqueue(0U, (IData)(__VdlyDim0__ml_q__v2));
            vlSelfRef.__PVT__unnamedblk7__DOT__r = 
                ((IData)(1U) + vlSelfRef.__PVT__unnamedblk7__DOT__r);
        }
    }
    vlSelfRef.__VdlyCommitQueuevid_store.commit(vlSelfRef.vid_store);
    vlSelfRef.__PVT__cit_lit_idx = __Vdly__cit_lit_idx;
    vlSelfRef.__PVT__state = __Vdly__state;
    vlSelfRef.__VdlyCommitQueueval_store.commit(vlSelfRef.val_store);
    vlSelfRef.__VdlyCommitQueueml_q.commit(vlSelfRef.ml_q);
    vlSelfRef.__VdlyCommitQueuerow_valid.commit(vlSelfRef.__PVT__row_valid);
    vlSelfRef.__VdlyCommitQueuepol_store.commit(vlSelfRef.pol_store);
    __PVT__unnamedblk1__DOT__c = 0U;
    while (VL_GTS_III(32, 0x00000400U, __PVT__unnamedblk1__DOT__c)) {
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] = 0U;
        vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = VL_MULS_III(32, (IData)(4U), __PVT__unnamedblk1__DOT__c);
        vlSelfRef.__PVT__c_lit_u[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0eU & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | (vlSelfRef.__PVT__row_valid[(0x00000fffU 
                                              & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                  & (1U == vlSelfRef.val_store[(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])));
        vlSelfRef.__PVT__c_lit_t[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0eU & vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((vlSelfRef.__PVT__row_valid[(0x00000fffU 
                                               & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (~ vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)])) 
                  & ((1U & vlSelfRef.val_store[(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                     == (1U & (~ (IData)(vlSelfRef.pol_store
                                         [(0x00000fffU 
                                           & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))))));
        vlSelfRef.__PVT__c_lit_f[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0eU & vlSelfRef.__PVT__c_lit_f[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((vlSelfRef.__PVT__row_valid[(0x00000fffU 
                                               & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                   & (~ vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)])) 
                  & ((1U & vlSelfRef.val_store[(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                     == vlSelfRef.pol_store[(0x00000fffU 
                                             & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])));
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                          & __PVT__unnamedblk1__DOT__c)] 
               | vlSelfRef.ml_q[(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(1U) + VL_MULS_III(32, (IData)(4U), __PVT__unnamedblk1__DOT__c));
        vlSelfRef.__PVT__c_lit_u[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0dU & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)((vlSelfRef.__PVT__row_valid
                           [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                           & (1U == vlSelfRef.val_store
                              [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 1U));
        vlSelfRef.__PVT__c_lit_t[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0dU & vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 1U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == (1U 
                                                  & (~ (IData)(vlSelfRef.pol_store
                                                               [
                                                               (0x00000fffU 
                                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))))) 
                  << 1U));
        vlSelfRef.__PVT__c_lit_f[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0dU & vlSelfRef.__PVT__c_lit_f[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 1U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == vlSelfRef.pol_store
                                              [(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 1U));
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                          & __PVT__unnamedblk1__DOT__c)] 
               | vlSelfRef.ml_q[(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(2U) + VL_MULS_III(32, (IData)(4U), __PVT__unnamedblk1__DOT__c));
        vlSelfRef.__PVT__c_lit_u[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0bU & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)((vlSelfRef.__PVT__row_valid
                           [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                           & (1U == vlSelfRef.val_store
                              [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 2U));
        vlSelfRef.__PVT__c_lit_t[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0bU & vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 2U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == (1U 
                                                  & (~ (IData)(vlSelfRef.pol_store
                                                               [
                                                               (0x00000fffU 
                                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))))) 
                  << 2U));
        vlSelfRef.__PVT__c_lit_f[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((0x0bU & vlSelfRef.__PVT__c_lit_f[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 2U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == vlSelfRef.pol_store
                                              [(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 2U));
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                          & __PVT__unnamedblk1__DOT__c)] 
               | vlSelfRef.ml_q[(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r 
            = ((IData)(3U) + VL_MULS_III(32, (IData)(4U), __PVT__unnamedblk1__DOT__c));
        vlSelfRef.__PVT__c_lit_u[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)((vlSelfRef.__PVT__row_valid
                           [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                           & (1U == vlSelfRef.val_store
                              [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 3U));
        vlSelfRef.__PVT__c_lit_t[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 3U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == (1U 
                                                  & (~ (IData)(vlSelfRef.pol_store
                                                               [
                                                               (0x00000fffU 
                                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)])))))) 
                  << 3U));
        vlSelfRef.__PVT__c_lit_f[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((7U & vlSelfRef.__PVT__c_lit_f[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)]) 
               | ((CData)(((vlSelfRef.__PVT__row_valid
                            [(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)] 
                            & (~ (vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                  >> 3U))) & ((1U & vlSelfRef.val_store
                                               [(0x00000fffU 
                                                 & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]) 
                                              == vlSelfRef.pol_store
                                              [(0x00000fffU 
                                                & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]))) 
                  << 3U));
        vlSelfRef.__PVT__c_ml_any[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                          & __PVT__unnamedblk1__DOT__c)] 
               | vlSelfRef.ml_q[(0x00000fffU & vlSelfRef.__PVT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__r)]);
        vlSelfRef.proc_done[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (0U != vlSelfRef.__PVT__c_lit_t[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)]);
        vlSelfRef.proc_conf[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (0x0000000fU == vlSelfRef.__PVT__c_lit_f
               [(0x000003ffU & __PVT__unnamedblk1__DOT__c)]);
        vlSelfRef.proc_up[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = (((((((1U == vlSelfRef.__PVT__c_lit_u
                     [(0x000003ffU & __PVT__unnamedblk1__DOT__c)]) 
                    | (2U == vlSelfRef.__PVT__c_lit_u
                       [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])) 
                   | (4U == vlSelfRef.__PVT__c_lit_u
                      [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])) 
                  | (8U == vlSelfRef.__PVT__c_lit_u
                     [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])) 
                 & (0x0000000fU == (vlSelfRef.__PVT__c_lit_f
                                    [(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
                                    | vlSelfRef.__PVT__c_lit_u
                                    [(0x000003ffU & __PVT__unnamedblk1__DOT__c)]))) 
                & (~ (IData)(vlSelfRef.proc_done[(0x000003ffU 
                                                  & __PVT__unnamedblk1__DOT__c)]))) 
               & vlSelfRef.__PVT__c_ml_any[(0x000003ffU 
                                            & __PVT__unnamedblk1__DOT__c)]);
        vlSelfRef.__PVT__proc_ulp[(0x000003ffU & __PVT__unnamedblk1__DOT__c)] 
            = ((1U & vlSelfRef.__PVT__c_lit_u[(0x000003ffU 
                                               & __PVT__unnamedblk1__DOT__c)])
                ? 0U : ((2U & vlSelfRef.__PVT__c_lit_u
                         [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])
                         ? 1U : ((4U & vlSelfRef.__PVT__c_lit_u
                                  [(0x000003ffU & __PVT__unnamedblk1__DOT__c)])
                                  ? 2U : 3U)));
        __PVT__unnamedblk1__DOT__c = ((IData)(1U) + __PVT__unnamedblk1__DOT__c);
    }
    vlSelfRef.__PVT__tree_conf = 0U;
    vlSelfRef.__PVT__tree_up = 0U;
    vlSelfRef.__PVT__tree_done = 0U;
    vlSelfRef.__PVT__tree_cid = 0U;
    vlSelfRef.__PVT__tree_ulp = 0U;
    __PVT__unnamedblk4__DOT__i = 0x000003ffU;
    while (VL_LTES_III(32, 0U, __PVT__unnamedblk4__DOT__i)) {
        if (vlSelfRef.proc_conf[(0x000003ffU & __PVT__unnamedblk4__DOT__i)]) {
            vlSelfRef.__PVT__tree_conf = 1U;
            vlSelfRef.__PVT__tree_cid = (0x000003ffU 
                                         & __PVT__unnamedblk4__DOT__i);
        }
        __PVT__unnamedblk4__DOT__i = (__PVT__unnamedblk4__DOT__i 
                                      - (IData)(1U));
    }
    if ((1U & (~ (IData)(vlSelfRef.__PVT__tree_conf)))) {
        vlSelfRef.__PVT__unnamedblk5__DOT__i = 0x000003ffU;
        while (VL_LTES_III(32, 0U, vlSelfRef.__PVT__unnamedblk5__DOT__i)) {
            if (vlSelfRef.proc_up[(0x000003ffU & vlSelfRef.__PVT__unnamedblk5__DOT__i)]) {
                vlSelfRef.__PVT__tree_up = 1U;
                vlSelfRef.__PVT__tree_cid = (0x000003ffU 
                                             & vlSelfRef.__PVT__unnamedblk5__DOT__i);
                vlSelfRef.__PVT__tree_ulp = vlSelfRef.__PVT__proc_ulp
                    [(0x000003ffU & vlSelfRef.__PVT__unnamedblk5__DOT__i)];
            }
            vlSelfRef.__PVT__unnamedblk5__DOT__i = 
                (vlSelfRef.__PVT__unnamedblk5__DOT__i 
                 - (IData)(1U));
        }
        if ((1U & (~ (IData)(vlSelfRef.__PVT__tree_up)))) {
            vlSelfRef.__PVT__tree_done = 1U;
            vlSelfRef.__PVT__unnamedblk6__DOT__i = 0U;
            while (VL_GTS_III(32, 0x00000400U, vlSelfRef.__PVT__unnamedblk6__DOT__i)) {
                if ((1U & (~ (IData)(vlSelfRef.proc_done
                                     [(0x000003ffU 
                                       & vlSelfRef.__PVT__unnamedblk6__DOT__i)])))) {
                    vlSelfRef.__PVT__tree_done = 0U;
                }
                vlSelfRef.__PVT__unnamedblk6__DOT__i 
                    = ((IData)(1U) + vlSelfRef.__PVT__unnamedblk6__DOT__i);
            }
        }
    }
    vlSelfRef.__PVT__rd_row = 0U;
    if (((3U == (IData)(vlSelfRef.__PVT__state)) & (IData)(vlSelfRef.__PVT__tree_up))) {
        vlSelfRef.__PVT__rd_row = (0x00000fffU & (VL_SHIFTL_III(12,32,32, (IData)(vlSelfRef.__PVT__tree_cid), 2U) 
                                                  + (IData)(vlSelfRef.__PVT__tree_ulp)));
    } else if ((5U == (IData)(vlSelfRef.__PVT__state))) {
        vlSelfRef.__PVT__rd_row = (0x00000fffU & (VL_SHIFTL_III(12,32,32, (IData)(vlSymsp->TOP.cid_in), 2U) 
                                                  + (IData)(vlSelfRef.__PVT__cit_lit_idx)));
    }
}
