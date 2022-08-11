#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class Dce {
public:
    Function *function;

    set<VirtReg *> usefulRegSet;
    vector<BasicBlock *> dceAccessQueue;
public:
    Dce(Function *func) : function(func) { 
        usefulRegSet.clear();
        dceAccessQueue.clear();
    }
    void insertToSet(VirtReg *reg);
    bool checkInSet(VirtReg *reg);
    void buildUsefulRegSet();
    void removeUselessInst();
    void runDeadCodeElim();
};
