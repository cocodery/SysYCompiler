#include "lvn.hh"

void LVN::ValueNumber(vector<Inst *> *bb) {
    int cnt = 0;
    for (auto i = bb->begin(); i != bb->end(); i++) {
        int retval = 0;
        cnt++;
        cout << "round " << cnt << endl;
        Case(LLIR_BIN, bin_inst, (*i)) {
            retval = ProcessInst(bb, bin_inst, bin_inst->op, bin_inst->dst, bin_inst->src1, bin_inst->src2);
        }
        // displayBB(*bb);
        if (retval == -1) i--;
    }
}

bool LVN::FindDst(lvnNode src) {
    cout << "mark in FindDst - 1" << endl;
    for (auto i = lvn_list.end(); i != lvn_list.begin(); i--) {
        cout << "mark in FindDst - 2" << endl;
        cout << " " << i->dst.ToString() << endl;
        if (i->dst == src.dst) {
            cout << "mark in FindDst - 4" << endl;
            return true;
        }
    }
    cout << "mark in FindDst - 3" << endl;
    return false;
}

int LVN::ProcessInst(vector<Inst *> *bb, Inst *ist, BinOp op, SRC dst, SRC src1, SRC src2) {
    
    // to run this "find" function of stl vector, "==" needs to beoverloaded in "inst" class. 
    auto i = find(bb->begin(), bb->end(), &(*ist)); // get current instruction
    
    cout << "mark 1" << endl;

    // check the first operand is in the map or not.
    lvnNode src1_node(src1);
    bool key1 = FindDst(src1_node); 
    if (key1) {

    } else { // if not, insert it into the map with null tuple.
        cout << "insert " << src1.ToString() << " into map" << endl;
        lvn_list.push_back(src1_node);
        display();
    }

    cout << "mark 2" << endl;

    // check the second operand is in the map or not.
    lvnNode src2_node(src2);

    cout << "mark 2-1" << endl;

    bool key2 = FindDst(src2_node);

    cout << "mark 2-2" << endl; 

    if(key2) {
        
    } else { // if not, insert it into the map with null tuple.
        cout << "insert " << src2.ToString() << " into map" << endl;
        lvn_list.push_back(src2_node);
    }

    auto key_tuple = make_tuple(GetIndex(src1), GetIndex(src2), op);
    
    cout << "mark 3" << endl;

    /* in func.FindTuple, a singal "==" overload is needed. */
    pair<int, SRC> tuple_idx = FindTuple(key_tuple); // find tuple index in lvn_map
    if (tuple_idx.first != -1) { // if find it,
        // replace this instruction with a new assignment instruction
        
        cout << "mark 4: replace this instruction with a new assignment instruction" << endl;
        
        if (dst == tuple_idx.second) {
            cout << "erase repeated instruction" << endl;
            cout << ">> " << dst.ToString() << " <- " << src1.ToString() << " " << op << " " << src2.ToString() << " will be deleted." << endl;
            bb->erase(i);
            return -1;
        }
        cout << dst.ToString() << " equals to " << tuple_idx.second.ToString() << endl;
        lvn_list.push_back(lvnNode(dst, tuple_idx.first));
        bb->erase(i); // erase this instruction
        ProcessBB(bb, dst, tuple_idx.second, ist); // replace all the instructions that use this variable with the new variable.
        return -1;
    } else {

        cout << "mark 5: list push_back" << endl;

        lvn_list.push_back(lvnNode(dst, GetIndex(src1), op, GetIndex(src2))); // if didnt find, insert it.
        
        cout << "mark 6" <<endl;
        
        return 0;
    }
    return 0;
}

void LVN::ProcessBB(vector<Inst *> *bb, SRC dst, SRC instead, Inst *i) {
    auto it = find(bb->begin(), bb->end(), &(*i));
    for (auto j = it; j != bb->end(); j++) {
        Case (LLIR_BIN, bin_inst, (*j)) {
            if ( bin_inst->dst == dst) {
                bin_inst->dst = instead;
            }
            if (bin_inst->src1 == dst) {
                bin_inst->src1 = instead;
            }
            if (bin_inst->src2 == dst) {
                bin_inst->src2 = instead;
            }
        }
        Case (LLIR_FBIN, fbin_inst, (*j)) {
            if ( fbin_inst->dst == dst) {
                fbin_inst->dst = instead;
            }
            if (fbin_inst->src1 == dst) {
                fbin_inst->src1 = instead;
            }
            if (fbin_inst->src2 == dst) {
                fbin_inst->src2 = instead;
            }
        }
        Case (LLIR_FCMP, fcmp_inst, (*j)) {
            if ( fcmp_inst->dst == dst) {
                fcmp_inst->dst = instead;
            }
            if (fcmp_inst->src1 == dst) {
                fcmp_inst->src1 = instead;
            }
            if (fcmp_inst->src2 == dst) {
                fcmp_inst->src2 = instead;
            }
        }
        Case (LLIR_ICMP, icmp_inst, (*j)) {
            if ( icmp_inst->dst == dst) {
                icmp_inst->dst = instead;
            }
            if (icmp_inst->src1 == dst) {
                icmp_inst->src1 = instead;
            }
            if (icmp_inst->src2 == dst) {
                icmp_inst->src2 = instead;
            }
        }
    }
    
}

pair<int, SRC> LVN::FindTuple(tuple<int, int, BinOp> t) {
    int cnt = 0;
    for (auto i: lvn_list) {
        if(i == t) {
            return make_pair(cnt, i.dst);
        } else {
            cnt++;
        }
    }
    return make_pair(-1, SRC());
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