#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <tuple>

#include "../structure/ir.hh"
#include "../structure/symtable.hh"
#include "../structure/llir.hh"
#include "../structure/baseclass.hh"
#include "../structure/value.hh"

using std::ostream;
using std::vector;
using std::string;
using std::map;
using std::tuple;
using std::get;
using std::make_tuple;

class lvnNode {
public:
    SRC dst;
    int src1;
    int src2;
    BinOp op;
public:
    lvnNode() {
        dst = SRC();
        src1 = -1;
        src2 = -1;
    }
    lvnNode(SRC dst, int src1, BinOp op, int src2) {
        this->dst = dst;
        this->src1 = src1;
        this->src2 = src2;
        this->op = op;
    }
    lvnNode(SRC dst) {
        cout << "construct node" << endl;
        this->dst = dst;
        this->src1 = -1;
        this->src2 = -1;
        cout << ">> current node: " << this->dst.ToString() << " " <<this->src1 << " " <<this->src2 << endl;
    }
    lvnNode(SRC dst, int src1) {
        this->dst = dst;
        this->src1 = src1;
        this->src2 = -1;
    }

    // bool is_computation_equal(int src1, int src2, string op) {
    //     return (this->src1 == src1 && this->src2 == src2 && this->op == op);
    // }

    bool operator == (lvnNode l) {
        return (dst == l.dst && src1 == l.src1 && src2 == l.src2 && op == l.op);
    }

    bool operator == (tuple<int, int, BinOp> l) {
        return (src1 == get<0>(l) && src2 == get<1>(l) &&  op == get<2>(l));
    }
};

class LVN{
public:
    int idx;
    // unordered_multimap<string, tuple<int, int, string>> lvn_map;
    vector<lvnNode> lvn_list;
public:
    void ValueNumber(vector<Inst *> *bb);
    bool FindDst(lvnNode dst);
    int ProcessInst(vector<Inst *> *bb, Inst *inst, BinOp op, SRC dst, SRC src1, SRC src2);
    void ProcessInst(vector<Inst *> *bb, string op, string dst, string src);
    void Remove(string dst, vector<Inst *> *bb);
    pair<int, SRC> FindTuple(tuple<int, int, BinOp> t);
    void ProcessBB(vector<Inst *> *bb, SRC dst, SRC instead, Inst *i);
    int GetIndex(SRC dst);
    void display();
};


