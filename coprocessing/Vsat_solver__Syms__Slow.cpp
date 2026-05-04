// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vsat_solver__pch.h"

Vsat_solver__Syms::Vsat_solver__Syms(VerilatedContext* contextp, const char* namep, Vsat_solver* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp{modelp}
    // Setup top module instance
    , TOP{this, namep}
{
    // Check resources
    Verilated::stackCheck(366);
    // Setup sub module instances
    TOP__sat_submodule.ctor(this, "sat_submodule");
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-12);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    TOP.sat_submodule = &TOP__sat_submodule;
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(true);
    TOP__sat_submodule.__Vconfigure(true);
    // Setup scopes
    __Vscopep_sat_submodule = new VerilatedScope{this, "sat_submodule", "sat_submodule", "<null>", 0, VerilatedScope::SCOPE_OTHER};
    // Setup export functions - final: 0
    // Setup export functions - final: 1
    // Setup public variables
    __Vscopep_sat_submodule->varInsert("ml_q", &(TOP__sat_submodule.ml_q), false, VLVT_UINT8, VLVD_NODIR|VLVF_PUB_RW, 1, 0 ,0,4095);
    __Vscopep_sat_submodule->varInsert("pol_store", &(TOP__sat_submodule.pol_store), false, VLVT_UINT8, VLVD_NODIR|VLVF_PUB_RW, 1, 0 ,0,4095);
    __Vscopep_sat_submodule->varInsert("proc_conf", &(TOP__sat_submodule.proc_conf), false, VLVT_UINT8, VLVD_NODIR|VLVF_PUB_RW, 1, 0 ,0,1023);
    __Vscopep_sat_submodule->varInsert("proc_done", &(TOP__sat_submodule.proc_done), false, VLVT_UINT8, VLVD_NODIR|VLVF_PUB_RW, 1, 0 ,0,1023);
    __Vscopep_sat_submodule->varInsert("proc_up", &(TOP__sat_submodule.proc_up), false, VLVT_UINT8, VLVD_NODIR|VLVF_PUB_RW, 1, 0 ,0,1023);
    __Vscopep_sat_submodule->varInsert("val_store", &(TOP__sat_submodule.val_store), false, VLVT_UINT8, VLVD_NODIR|VLVF_PUB_RW, 1, 1 ,0,4095 ,1,0);
    __Vscopep_sat_submodule->varInsert("vid_store", &(TOP__sat_submodule.vid_store), false, VLVT_UINT32, VLVD_NODIR|VLVF_PUB_RW, 1, 1 ,0,4095 ,19,0);
}

Vsat_solver__Syms::~Vsat_solver__Syms() {
    // Tear down scopes
    VL_DO_CLEAR(delete __Vscopep_sat_submodule, __Vscopep_sat_submodule = nullptr);
    // Tear down sub module instances
    TOP__sat_submodule.dtor();
}
