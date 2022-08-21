#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class Dce {
public:
    Function *function;

    set<pair<int32_t, bool>> usefulRegSet;
    set<Inst *> uselessInstSet;
    vector<BasicBlock *> dceAccessQueue;

    vector<list<BasicBlock *>> reverseOrder;
public:
    Dce(Function *func) : function(func) { 
        usefulRegSet.clear();
        dceAccessQueue.clear();
    }
    void insertToSet(VirtReg *reg);
    bool checkInSet(VirtReg *reg);
    void dfsReverseOrder(list<BasicBlock *> list, BasicBlock *block);
    void buildUsefulRegSet(Function *);
    void removeUselessInst();
    void runDeadCodeElim();
};
