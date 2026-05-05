// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vsat_submodule__Syms.h"
#include "Vsat_submodule.h"
#include "Vsat_submodule_sat_submodule.h"



// FUNCTIONS
Vsat_submodule__Syms::Vsat_submodule__Syms(Vsat_submodule* topp, const char* namep)
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
        __Vscope_sat_submodule.varInsert(__Vfinal,"ml_q", &(TOP__sat_submodule.ml_q), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,32767,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"pol_store", &(TOP__sat_submodule.pol_store), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,32767,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"proc_conf", &(TOP__sat_submodule.proc_conf), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,8191,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"proc_done", &(TOP__sat_submodule.proc_done), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,8191,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"proc_up", &(TOP__sat_submodule.proc_up), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,1 ,8191,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"val_store", &(TOP__sat_submodule.val_store), VLVT_UINT8,VLVD_NODIR|VLVF_PUB_RW,2 ,1,0 ,32767,0);
        __Vscope_sat_submodule.varInsert(__Vfinal,"vid_store", &(TOP__sat_submodule.vid_store), VLVT_UINT32,VLVD_NODIR|VLVF_PUB_RW,2 ,19,0 ,32767,0);
    }
}
