#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "../../common.hh"
#include "../structure/ir.hh"
#include "../structure/symtable.hh"
#include "../structure/llir.hh"

using std::ostream;
using std::vector;
using std::map;

enum NodeOp {  
    Add, Sub, Mul, Div, Rem, 
    fAdd, fSub, fMul, fDiv, 
    iCmp, fCmp , Gep
};

class lvnNode {
public:
    NodeOp op;
    SRC src1;
    SRC src2;
public:
    lvnNode(NodeOp _op, SRC _src1, SRC _src2) : op(_op), src1(_src1), src2(_src2) { }

    bool operator == (lvnNode node) {
        return op == node.op && src1 == node.src1 && src2 == node.src2;
    }
};

class LVN{
public:
    Function *function;
    map<int32_t, int32_t> bbIdxMapIdx;
    vector<map<SRC, lvnNode *>> lvnMapNodes;
public:
    LVN(Function *func) : function(func) {
        bbIdxMapIdx.clear();
        lvnMapNodes.clear();
    }
    void runLVN();
    void valueNumber(BasicBlock *block);
    bool checkInMap(BasicBlock *block, lvnNode *node);
};
