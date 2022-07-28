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

class lvnNode {
public:
    SRC dst;
    int src1;
    int src2;
    string op;
public:
    lvnNode() {
        dst = SRC();
        src1 = -1;
        src2 = -1;
        op = "";
    }
    lvnNode(SRC dst, int src1, string op, int src2) {
        this->dst = dst;
        this->src1 = src1;
        this->src2 = src2;
        this->op = op;
    }
    lvnNode(SRC dst) {
        this->dst = dst;
        this->src1 = -1;
        this->src2 = -1;
        this->op = "";
    }
    lvnNode(SRC dst, int src1) {
        this->dst = dst;
        this->src1 = src1;
        this->src2 = -1;
        this->op = "";
    }

    bool is_computation_equal(int src1, int src2, string op) {
        return (this->src1 == src1 && this->src2 == src2 && this->op == op);
    }

    // bool operator == (lvnNode l) {
    //     return (dst == l.dst && src1 == l.src1 && src2 == l.src2 && op == l.op);
    // }

    bool operator == (tuple<int, int, string> l) {
        return (src1 == get<0>(l) && src2 == get<1>(l) && op == get<2>(l));
    }
};

class lvn{
public:
    int idx;
    // unordered_multimap<string, tuple<int, int, string>> lvn_map;
    vector<lvnNode> lvn_list;
public:
    void ValueNumber(vector<Inst> *bb);
    int ProcessInst(vector<Inst> *bb, Inst *inst, BinOp op, SRC dst, SRC src1, SRC src2);
    void ProcessInst(vector<Inst> *bb, string op, string dst, string src);
    void Remove(string dst, vector<Inst> *bb);
    pair<int, string> FindTuple(tuple<int, int, string> t);
    void ProcessBB(vector<Inst> *bb, string dst, string instead, auto i);
    int GetIndex(SRC dst);
    // bool MatchExp(Inst * inst1, Inst * inst2);
    void display();
};

void lvn::ValueNumber(vector<Inst> *bb) {
    int cnt = 0;
    for (auto i = bb->begin(); i != bb->end(); i++) {
        int retval = 0;
        cnt++;
        cout << "round " << cnt << endl;
        Case(LLIR_BIN, bin_inst, &(*i)) {
            retval = ProcessInst(bb, bin_inst, bin_inst->op, bin_inst->dst, bin_inst->src1, bin_inst->src2);
        }
        // displayBB(*bb);
        if (retval == -1) i--;
    }
}

int lvn::ProcessInst(vector<Inst> *bb, Inst *ist, BinOp op, SRC dst, SRC src1, SRC src2) {
    
    // to run this "find" function of stl vector, "==" needs to beoverloaded in "inst" class. 
    auto i = find(bb->begin(), bb->end(), *ist); // get current instruction
    
    // check the first operand is in the map or not.
    auto key1 = find(lvn_list.begin(), lvn_list.end(), lvnNode(src1)); 
    if (key1 != lvn_list.end()) {

    } else { // if not, insert it into the map with null tuple.
        cout << "insert " << src1.ToString() << " into map" << endl;
        lvn_list.push_back(lvnNode(src1));
    }

    // check the second operand is in the map or not.
    auto key2 = find(lvn_list.begin(), lvn_list.end(), lvnNode(src2));
    if(key2 != lvn_list.end()) {
        
    } else { // if not, insert it into the map with null tuple.
        cout << "insert " << src2.ToString() << " into map" << endl;
        lvn_list.push_back(lvnNode(src2));
    }

    auto key_tuple = make_tuple(GetIndex(src1), GetIndex(src2), op);
  
    /* in func.FindTuple, a singal "==" overload is needed. */
    pair<int, string> tuple_idx = FindTuple(key_tuple); // find tuple index in lvn_map
    if (tuple_idx.first != -1) { // if find it,
        // replace this instruction with a new assignment instruction
        if (dst == tuple_idx.second) {
            cout << "erase repeated instruction" << endl;
            cout << ">> " << dst << " <- " << src1 << " " << op << " " << src2 << " will be deleted." << endl;
            bb->erase(i);
            return -1;
        }
        cout << dst << " equals to " << tuple_idx.second << endl;
        lvn_list.push_back(lvnNode(dst, tuple_idx.first));
        bb->erase(i); // erase this instruction
        ProcessBB(bb, dst, tuple_idx.second, i); // replace all the instructions that use this variable with the new variable.
        return -1;
    } else {
        lvn_list.push_back(lvnNode(dst, GetIndex(src1), op, GetIndex(src2))); // if didnt find, insert it.
        return 0;
    }
    return 0;
}

void lvn::ProcessBB(vector<inst> *bb, string dst, string instead, auto i) {
    for (auto j = i; j != bb->end(); j++) {
        if (j->dst == dst) {
            j->dst = instead;
        }
        if (j->src1 == dst) {
            j->src1 = instead;
        }
        if (j->src2 == dst) {
            j->src2 = instead;
        }
    }
    
}

pair<int, string> lvn::FindTuple(tuple<int, int, string> t) {
    int cnt = 0;
    for (auto i: lvn_list) {
        if(i == t) {
            return make_pair(cnt, i.dst);
        } else {
            cnt++;
        }
    }
    return make_pair(-1, "");
}

int lvn::GetIndex(SRC dst) {
    int cnt = 0;
    bool flag = false;
    // in hash table, to find the latest element, use "end" as the iterator.
    for (auto i = lvn_list.end(); i != lvn_list.begin(); i--) {
        if (i->dst == dst) {
            flag = true;
        }
        if (flag) {
            cnt++;
        }
    }
    if (flag) {
        return cnt;
    } else {
        return -1;
    }
}

void lvn::display() {
    for (auto i: lvn_list) {
        cout << i.dst.ToString() << " <- " << i.src1 << " " << i.op << " " << i.src2 << endl;
    }
}