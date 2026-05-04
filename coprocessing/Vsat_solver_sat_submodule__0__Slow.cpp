// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_solver.h for the primary calling header

#include "Vsat_solver__pch.h"

VL_ATTR_COLD void Vsat_solver_sat_submodule___ctor_var_reset(Vsat_solver_sat_submodule* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vsat_solver_sat_submodule___ctor_var_reset\n"); );
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
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->vid_store[__Vi0] = VL_SCOPED_RAND_RESET_I(20, __VscopeHash, 16391576545666375011ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->pol_store[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4920485615533028289ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->val_store[__Vi0] = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12731198652024135540ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->__PVT__row_valid[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15516042521133067708ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->ml_q[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8111944953188570115ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->proc_conf[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13538216676960754400ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->proc_up[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4752629128264588512ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->proc_done[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8039399294169862840ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->__PVT__proc_ulp[__Vi0] = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6814625133532045438ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->__PVT__c_lit_u[__Vi0] = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14894711918779877057ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->__PVT__c_lit_t[__Vi0] = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6712314498256210523ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->__PVT__c_lit_f[__Vi0] = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14923499422947107823ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->__PVT__c_ml_any[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1068039379251771095ull);
    }
    vlSelf->__PVT__tree_conf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2800367846882221806ull);
    vlSelf->__PVT__tree_up = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5504224841205599643ull);
    vlSelf->__PVT__tree_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14176781286628922223ull);
    vlSelf->__PVT__tree_cid = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 8796564848206189822ull);
    vlSelf->__PVT__tree_ulp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17459399593453132560ull);
    vlSelf->__PVT__state = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 9404372463396948974ull);
    vlSelf->__PVT__cit_lit_idx = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9690722689688358911ull);
    vlSelf->__PVT__rd_row = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 10368909357096406593ull);
    vlSelf->__PVT__unnamedblk5__DOT__i = 0;
    vlSelf->__PVT__unnamedblk6__DOT__i = 0;
    vlSelf->__PVT__unnamedblk8__DOT__r = 0;
    vlSelf->__PVT__unnamedblk9__DOT__r = 0;
    vlSelf->__PVT__unnamedblk7__DOT__r = 0;
}
