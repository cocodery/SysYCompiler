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

class lvnNode {
public:
    BinOp op;
    SRC src1;
    SRC src2;
public:
    lvnNode(BinOp _op, SRC _src1, SRC _src2) : op(_op), src1(_src1), src2(_src2) { }

    bool operator == (lvnNode node) {
        return op == node.op && src1 == node.src1 && src2 == node.src2;
    }
};

class LVN{
public:
    Function *function;
    vector<map<VirtReg *, SRC>> replaceMap;
    vector<map<VirtReg *, lvnNode>> lvnNodes;
public:
    LVN(Function *func) : function(func) {
        replaceMap.clear();
        lvnNodes.clear();
    }
    void runLVN();
    void ValueNumber(vector<Inst> *bb);
    int ProcessInst(vector<Inst> *bb, Inst *inst, BinOp op, SRC dst, SRC src1, SRC src2);
    void Remove(string dst, vector<Inst> *bb);
    pair<int, string> FindTuple(tuple<int, int, string> t);
    int GetIndex(SRC dst);
    void display();
};
