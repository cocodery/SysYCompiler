#pragma once

#include "../common.hh"
#include "../structure/ir.hh"
#include <stack>

using std::stack;

class Mem2Reg {
public:
    Function *function;

    vector<set<BasicBlock *>> defBlocks;
    vector<LLIR_ALLOCA *> allocaInsts;
    map<LLIR_ALLOCA*, int32_t> allocaLoopup;
    map<LLIR_PHI *, int32_t> phi2AllocaMap;
public:
    Mem2Reg(Function *func) : function(func) {
        defBlocks.clear();
        allocaInsts.clear();
        allocaLoopup.clear();
        phi2AllocaMap.clear();
    }
    set<pair<int32_t, bool>> initDelVarSet();
    void removeUsedVar(set<pair<int32_t, bool>> &del_variable);
    LLIR_ALLOCA *getAllocaInst(VirtReg *reg);
    bool inDefBlocks(int32_t index ,BasicBlock *block);
    void runMem2Reg();
};

class RenameData {
public:
    BasicBlock *block;
    BasicBlock *pred;
    vector<SRC> values;
public:
    RenameData(BasicBlock *_bb, BasicBlock *_pred, vector<SRC> _v) 
        : block(_bb), pred(_pred), values(_v) { }
};
