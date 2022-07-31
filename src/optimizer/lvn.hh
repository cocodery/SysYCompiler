#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <tuple>

#include "../../common.hh"
#include "../structure/ir.hh"
#include "../structure/symtable.hh"
#include "../structure/llir.hh"
#include "../structure/value.hh"

using std::ostream;
using std::vector;
using std::string;
using std::map;
using std::tuple;

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
    vector<map<VirtReg *, SRC>> replaceMap;
    vector<map<SRC, lvnNode *>> lvnMapNodes;
public:
    LVN(Function *func) : function(func) {
        bbIdxMapIdx.clear();
        replaceMap.clear();
        lvnMapNodes.clear();
    }
    void runLVN();
    void valueNumber(BasicBlock *block);
    void ProcessInst(vector<Inst> *bb, Inst *inst, BinOp op, SRC dst, SRC src1, SRC src2);
    void Remove(string dst, vector<Inst> *bb);
    void FindTuple(tuple<int, int, string> t);
    void GetIndex(SRC dst);
    void display();
};
