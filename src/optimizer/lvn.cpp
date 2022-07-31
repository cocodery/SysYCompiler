#include "lvn.hh"

void LVN::runLVN() {
    
}

void LVN::ValueNumber(vector<Inst> *bb) {
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

int LVN::ProcessInst(vector<Inst> *bb, Inst *ist, BinOp op, SRC dst, SRC src1, SRC src2) {
    
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

void LVN::ProcessBB(vector<inst> *bb, string dst, string instead, auto i) {
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

pair<int, string> LVN::FindTuple(tuple<int, int, string> t) {
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

int LVN::GetIndex(SRC dst) {
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

void LVN::display() {
    for (auto i: lvn_list) {
        cout << i.dst.ToString() << " <- " << i.src1 << " " << i.op << " " << i.src2 << endl;
    }
}