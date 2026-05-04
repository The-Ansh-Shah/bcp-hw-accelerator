// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vsat_solver__pch.h"

//============================================================
// Constructors

Vsat_solver::Vsat_solver(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vsat_solver__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , op{vlSymsp->TOP.op}
    , phase{vlSymsp->TOP.phase}
    , val_in{vlSymsp->TOP.val_in}
    , pol_in{vlSymsp->TOP.pol_in}
    , conf_out{vlSymsp->TOP.conf_out}
    , up_out{vlSymsp->TOP.up_out}
    , done_out{vlSymsp->TOP.done_out}
    , up_lit_pos{vlSymsp->TOP.up_lit_pos}
    , pol_out{vlSymsp->TOP.pol_out}
    , valid_out{vlSymsp->TOP.valid_out}
    , row_addr{vlSymsp->TOP.row_addr}
    , cid_in{vlSymsp->TOP.cid_in}
    , cid_out{vlSymsp->TOP.cid_out}
    , vid_in{vlSymsp->TOP.vid_in}
    , vid_out{vlSymsp->TOP.vid_out}
    , sat_submodule{vlSymsp->TOP.sat_submodule}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vsat_solver::Vsat_solver(const char* _vcname__)
    : Vsat_solver(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vsat_solver::~Vsat_solver() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vsat_solver___024root___eval_debug_assertions(Vsat_solver___024root* vlSelf);
#endif  // VL_DEBUG
void Vsat_solver___024root___eval_static(Vsat_solver___024root* vlSelf);
void Vsat_solver___024root___eval_initial(Vsat_solver___024root* vlSelf);
void Vsat_solver___024root___eval_settle(Vsat_solver___024root* vlSelf);
void Vsat_solver___024root___eval(Vsat_solver___024root* vlSelf);

void Vsat_solver::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vsat_solver::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vsat_solver___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vsat_solver___024root___eval_static(&(vlSymsp->TOP));
        Vsat_solver___024root___eval_initial(&(vlSymsp->TOP));
        Vsat_solver___024root___eval_settle(&(vlSymsp->TOP));
        vlSymsp->__Vm_didInit = true;
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vsat_solver___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vsat_solver::eventsPending() { return false; }

uint64_t Vsat_solver::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vsat_solver::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vsat_solver___024root___eval_final(Vsat_solver___024root* vlSelf);

VL_ATTR_COLD void Vsat_solver::final() {
    contextp()->executingFinal(true);
    Vsat_solver___024root___eval_final(&(vlSymsp->TOP));
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vsat_solver::hierName() const { return vlSymsp->name(); }
const char* Vsat_solver::modelName() const { return "Vsat_solver"; }
unsigned Vsat_solver::threads() const { return 1; }
void Vsat_solver::prepareClone() const { contextp()->prepareClone(); }
void Vsat_solver::atClone() const {
    contextp()->threadPoolpOnClone();
}
