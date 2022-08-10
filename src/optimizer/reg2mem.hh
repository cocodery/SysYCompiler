#pragma once

#include "../common.hh"
#include "../structure/ir.hh"
#include "mem2reg.hh"

class Reg2Mem {
public:
    Function *function;

    vector<LLIR_ALLOCA *> allocaInsts;
    map<LLIR_PHI *, int32_t> phi2AllocaMap;
public:
    Reg2Mem(Function *func, Mem2Reg &mem2reg) : function(func) {
        allocaInsts = mem2reg.allocaInsts;
        phi2AllocaMap = mem2reg.phi2AllocaMap;
    }
    void runReg2Mem();
};
