// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_solver.h for the primary calling header

#include "Vsat_solver__pch.h"

VL_ATTR_COLD void Vsat_solver___024root___eval_static(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___eval_static\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

VL_ATTR_COLD void Vsat_solver___024root___eval_initial(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___eval_initial\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vsat_solver___024root___eval_final(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___eval_final\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsat_solver___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vsat_solver___024root___eval_phase__stl(Vsat_solver___024root* vlSelf);

VL_ATTR_COLD void Vsat_solver___024root___eval_settle(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___eval_settle\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vsat_solver___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("src/sat_submodule.sv", 11, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vsat_solver___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vsat_solver___024root___eval_triggers_vec__stl(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___eval_triggers_vec__stl\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Vsat_solver___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsat_solver___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vsat_solver___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vsat_solver___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___trigger_anySet__stl\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

VL_ATTR_COLD void Vsat_solver___024root___stl_sequent__TOP__0(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___stl_sequent__TOP__0\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.valid_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                           | ((1U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                              | ((4U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                                 | (5U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)))));
}

void Vsat_solver_sat_submodule___ico_sequent__TOP__sat_submodule__0(Vsat_solver_sat_submodule* vlSelf);

VL_ATTR_COLD void Vsat_solver___024root___eval_stl(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___eval_stl\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        vlSelfRef.valid_out = ((3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                               | ((1U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                                  | ((4U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)) 
                                     | (5U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)))));
        Vsat_solver_sat_submodule___ico_sequent__TOP__sat_submodule__0((&vlSymsp->TOP__sat_submodule));
        vlSelfRef.conf_out = ((IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_conf) 
                              & (3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)));
        vlSelfRef.done_out = ((IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_done) 
                              & (3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)));
        vlSelfRef.cid_out = vlSymsp->TOP__sat_submodule.__PVT__tree_cid;
        vlSelfRef.up_lit_pos = vlSymsp->TOP__sat_submodule.__PVT__tree_ulp;
        vlSelfRef.up_out = ((IData)(vlSymsp->TOP__sat_submodule.__PVT__tree_up) 
                            & (3U == (IData)(vlSymsp->TOP__sat_submodule.__PVT__state)));
        vlSelfRef.pol_out = vlSymsp->TOP__sat_submodule.pol_store
            [vlSymsp->TOP__sat_submodule.__PVT__rd_row];
        vlSelfRef.vid_out = vlSymsp->TOP__sat_submodule.vid_store
            [vlSymsp->TOP__sat_submodule.__PVT__rd_row];
    }
}

VL_ATTR_COLD bool Vsat_solver___024root___eval_phase__stl(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___eval_phase__stl\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vsat_solver___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsat_solver___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vsat_solver___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vsat_solver___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vsat_solver___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsat_solver___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vsat_solver___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vsat_solver___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsat_solver___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vsat_solver___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vsat_solver___024root___ctor_var_reset(Vsat_solver___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsat_solver___024root___ctor_var_reset\n"); );
    Vsat_solver__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->op = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3630531923276091163ull);
    vlSelf->phase = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11256959997607747473ull);
    vlSelf->vid_in = VL_SCOPED_RAND_RESET_I(20, __VscopeHash, 9055601309087344438ull);
    vlSelf->val_in = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10078936076736456111ull);
    vlSelf->pol_in = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14864836224909980241ull);
    vlSelf->row_addr = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 14650995578590059117ull);
    vlSelf->cid_in = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 9395697162171788677ull);
    vlSelf->conf_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8755156313291735625ull);
    vlSelf->up_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17342126683359391691ull);
    vlSelf->done_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4118577957371540044ull);
    vlSelf->cid_out = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 5798040078856670059ull);
    vlSelf->up_lit_pos = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9837641194276448460ull);
    vlSelf->vid_out = VL_SCOPED_RAND_RESET_I(20, __VscopeHash, 10544447631160014191ull);
    vlSelf->pol_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5029647737756290172ull);
    vlSelf->valid_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8744939437868816662ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
