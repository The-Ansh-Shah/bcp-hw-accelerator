// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vsat_solver__Syms.h"
#include "Vsat_solver.h"
#include "Vsat_solver_sat_submodule.h"



// FUNCTIONS
Vsat_solver__Syms::Vsat_solver__Syms(Vsat_solver* topp, const char* namep)
    // Setup locals
    : __Vm_namep(namep)
    , __Vm_didInit(false)
    // Setup submodule names
    , TOP__sat_submodule(Verilated::catName(topp->name(), "sat_submodule"))
{
    // Pointer to top level
    TOPp = topp;
    // Setup each module's pointers to their submodules
    TOPp->sat_submodule = &TOP__sat_submodule;
    // Setup each module's pointer back to symbol table (for public functions)
    TOPp->__Vconfigure(this, true);
    TOP__sat_submodule.__Vconfigure(this, true);
    // Setup scopes
    __Vscope_sat_submodule.configure(this, name(), "sat_submodule", "sat_submodule", VerilatedScope::SCOPE_OTHER);
    // Setup export functions
    for (int __Vfinal=0; __Vfinal<2; __Vfinal++) {
        __Vscope_sat_submodule.varInsert(__Vfinal,"ml_q", &(TOP__sat_submodule.ml_q), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,4095,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"pol_store", &(TOP__sat_submodule.pol_store), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,4095,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"proc_conf", &(TOP__sat_submodule.proc_conf), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,1023,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"proc_done", &(TOP__sat_submodule.proc_done), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,1023,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"proc_up", &(TOP__sat_submodule.proc_up), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,1023,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"val_store", &(TOP__sat_submodule.val_store), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,2 ,1,0 ,4095,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"vid_store", &(TOP__sat_submodule.vid_store), VLVT_UINT32,VLVD_NODIR|VLVF_PUB_RW,2 ,19,0 ,4095,0);
    }
}
