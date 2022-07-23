#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class Mem2Reg {
public:
    Function *function;

    set<VirtReg *> global;
    set<BasicBlock *> blocks;
public:
    Mem2Reg(Function *func) : function(func) {
        global.clear();
        blocks.clear();
    }
    void removeUsedVar();
    void initMem2Reg();
};
