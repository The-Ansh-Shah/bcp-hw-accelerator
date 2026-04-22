// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsat_solver.h for the primary calling header

#include "Vsat_solver_cam_sram_row.h"
#include "Vsat_solver__Syms.h"

//==========

VL_CTOR_IMP(Vsat_solver_cam_sram_row) {
    // Reset internal values
    // Reset structure values
    _ctor_var_reset();
}

void Vsat_solver_cam_sram_row::__Vconfigure(Vsat_solver__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vsat_solver_cam_sram_row::~Vsat_solver_cam_sram_row() {
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__0__KET____DOT__u_row__1(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__0__KET____DOT__u_row__1\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_settle__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__0__KET____DOT__u_row__25(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_settle__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__0__KET____DOT__u_row__25\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    this->matchline = (((IData)(vlTOPp->sat_submodule__DOT__search_en) 
                        & (IData)(this->__PVT__u_cam__DOT__valid)) 
                       & (this->__PVT__u_cam__DOT__stored_vid 
                          == vlTOPp->vid_in));
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__1__KET____DOT__u_row__2(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__1__KET____DOT__u_row__2\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [1U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [1U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [1U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [1U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__2__KET____DOT__u_row__3(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__2__KET____DOT__u_row__3\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [2U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [2U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [2U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [2U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__3__KET____DOT__u_row__4(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__3__KET____DOT__u_row__4\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [3U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [3U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [3U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [3U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__4__KET____DOT__u_row__5(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__4__KET____DOT__u_row__5\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [4U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [4U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [4U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [4U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__5__KET____DOT__u_row__6(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__5__KET____DOT__u_row__6\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [5U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [5U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [5U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [5U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__6__KET____DOT__u_row__7(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__6__KET____DOT__u_row__7\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [6U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [6U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [6U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [6U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__7__KET____DOT__u_row__8(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__7__KET____DOT__u_row__8\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [7U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [7U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [7U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [7U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__8__KET____DOT__u_row__9(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__8__KET____DOT__u_row__9\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [8U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [8U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [8U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [8U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__9__KET____DOT__u_row__10(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__9__KET____DOT__u_row__10\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [9U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [9U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [9U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [9U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__10__KET____DOT__u_row__11(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__10__KET____DOT__u_row__11\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xaU]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xaU]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xaU]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xaU]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__11__KET____DOT__u_row__12(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__11__KET____DOT__u_row__12\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xbU]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xbU]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xbU]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xbU]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__12__KET____DOT__u_row__13(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__12__KET____DOT__u_row__13\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xcU]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xcU]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xcU]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xcU]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__13__KET____DOT__u_row__14(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__13__KET____DOT__u_row__14\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xdU]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xdU]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xdU]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xdU]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__14__KET____DOT__u_row__15(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__14__KET____DOT__u_row__15\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xeU]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xeU]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xeU]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xeU]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__15__KET____DOT__u_row__16(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__15__KET____DOT__u_row__16\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xfU]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0xfU]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xfU]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0xfU]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__16__KET____DOT__u_row__17(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__16__KET____DOT__u_row__17\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x10U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x10U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x10U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x10U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__17__KET____DOT__u_row__18(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__17__KET____DOT__u_row__18\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x11U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x11U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x11U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x11U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__18__KET____DOT__u_row__19(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__18__KET____DOT__u_row__19\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x12U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x12U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x12U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x12U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__19__KET____DOT__u_row__20(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__19__KET____DOT__u_row__20\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x13U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x13U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x13U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x13U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__20__KET____DOT__u_row__21(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__20__KET____DOT__u_row__21\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x14U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x14U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x14U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x14U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__21__KET____DOT__u_row__22(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__21__KET____DOT__u_row__22\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x15U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x15U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x15U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x15U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__22__KET____DOT__u_row__23(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__22__KET____DOT__u_row__23\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x16U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x16U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x16U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x16U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

VL_INLINE_OPT void Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__23__KET____DOT__u_row__24(Vsat_solver__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_sequent__TOP__sat_submodule__DOT__u_array__DOT__rows__BRA__23__KET____DOT__u_row__24\n"); );
    Vsat_solver* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x17U]) {
            this->__PVT__u_cam__DOT__valid = 1U;
        }
    } else {
        this->__PVT__u_cam__DOT__valid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__cam_wr_sel
            [0x17U]) {
            this->__PVT__u_cam__DOT__stored_vid = vlTOPp->vid_in;
        }
    } else {
        this->__PVT__u_cam__DOT__stored_vid = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x17U]) {
            this->__PVT__u_sram__DOT__polarity = vlTOPp->pol_in;
        }
    } else {
        this->__PVT__u_sram__DOT__polarity = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->sat_submodule__DOT__u_array__DOT__sram_wr_sel
            [0x17U]) {
            this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
        } else {
            if (((IData)(vlTOPp->sat_submodule__DOT__cam_act_wr) 
                 & (IData)(this->matchline))) {
                this->__PVT__u_sram__DOT__value = vlTOPp->sat_submodule__DOT__arr_val_in;
            }
        }
    } else {
        this->__PVT__u_sram__DOT__value = 2U;
    }
}

void Vsat_solver_cam_sram_row::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vsat_solver_cam_sram_row::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    rst_n = VL_RAND_RESET_I(1);
    cam_wr_en = VL_RAND_RESET_I(1);
    cam_rd_en = VL_RAND_RESET_I(1);
    cam_search_en = VL_RAND_RESET_I(1);
    sram_wr_en = VL_RAND_RESET_I(1);
    sram_rd_en = VL_RAND_RESET_I(1);
    cam_act_wr = VL_RAND_RESET_I(1);
    vid_in = VL_RAND_RESET_I(20);
    vid_out = VL_RAND_RESET_I(20);
    pol_in = VL_RAND_RESET_I(1);
    val_in = VL_RAND_RESET_I(2);
    pol_out = VL_RAND_RESET_I(1);
    val_out = VL_RAND_RESET_I(2);
    pol_stored = VL_RAND_RESET_I(1);
    val_stored = VL_RAND_RESET_I(2);
    matchline = VL_RAND_RESET_I(1);
    __PVT__u_cam__DOT__stored_vid = VL_RAND_RESET_I(20);
    __PVT__u_cam__DOT__valid = VL_RAND_RESET_I(1);
    __PVT__u_sram__DOT__polarity = VL_RAND_RESET_I(1);
    __PVT__u_sram__DOT__value = VL_RAND_RESET_I(2);
}
