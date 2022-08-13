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

class BranchOptimization {
public:
    Function * function;

public:
    BranchOptimization(Function * func): function(func) { };
    void run() {
        cout << "----- Optimization Running: Branch Optimization -----" << endl;
        // 如果可能，需要判断函数是否为库函数
        // if (i->func_info.)
        cout << ("---> in function: " + function->func_info.func_name) << endl;
        runBranchOptimization(function);
    }

    bool runBranchOptimization(Function *f) {
        bool remove_phi;
        while(1) {
            bool completed;
            completed = removeUselessPhi(f);
            remove_phi = !completed;
            completed &= sameTargetBranch(f);
            completed &= onlyOndeUnconditionalBranch(f);
            // completed &= reduceConditionalBrach(f);

            if (completed) {
                break;
            }
        }
        return remove_phi;
    }

    bool removeUselessPhi(Function *f) {
        bool completed = true;
        for (auto bb : f->all_blocks) {
            auto bb_ = *bb;
            for (auto inst_iter = bb_.basic_block.begin(); inst_iter != bb_.basic_block.end(); ++inst_iter) {
                auto inst = *inst_iter;
                if (!dynamic_cast<LLIR_PHI *>(inst)) {
                    break;
                }
                auto phi_inst = dynamic_cast<LLIR_PHI *>(inst);
                if (phi_inst->srcs.size() == 1) {
                    // 这里可能有问题
                    f->replaceSRCs(&bb_, phi_inst->srcs[0].first.reg, phi_inst->dst);
                    inst_iter = bb_.basic_block.erase(inst_iter) - 1;
                    completed = false;
                }
            }
        }
        cout << ">> Remove useless phi: " << completed << endl;
        return completed;
    }


    void blockFinder(Scope *scope, vector<BasicBlock *>* all_blocks, int32_t bb_idx, int32_t pred_idx[], int32_t succ_idx) {
        // cout << " ----block finder---- " << endl;
        if (scope == nullptr || scope->elements->empty()) {
            return;
        }
        for (auto scope_iter = scope->elements->begin(); scope_iter != scope->elements->end(); ++scope_iter) {
            // cout << " # ";
            if (Scope *scope_node = dynamic_cast<Scope *>(*scope_iter); scope_node != nullptr) {
                // cout << "----> scope: " << scope_node->sp_idx << endl;
                blockFinder(scope_node, all_blocks, bb_idx, pred_idx, succ_idx);
                return;
            } else {
                BasicBlock *bb_node = dynamic_cast<BasicBlock *>(*scope_iter);
                if (bb_node->bb_idx == bb_idx) {
                    cout << endl << "    ######> find basic block: " << bb_idx << endl;
                    bb_node->valuable = false;
                    /* 
                        在scope中修改前驱后继
                     */
                    int32_t node_preds[bb_node->preds.size()];
                    int32_t cnt = 0;
                    for(auto j: bb_node->preds) {
                        node_preds[cnt++] = j.first;
                        j.second->succs.erase(j.first);
                        j.second->succs.insert({succ_idx, bb_node->succs[0]});
                        
                        (bb_node->succs.rbegin()->second)->preds.insert(j);
                    }
                    
                    (bb_node->succs.rbegin()->second)->preds.erase(bb_idx);
                    
                    bb_node->preds.clear();
                    
                    scope->elements->erase(scope_iter);
                    return;
                }
            }
        }
    }

    // onlyOneUncondBr：基本块内只有一个无条件跳转，且去掉该基本块不会导致后继中的 phi歧义，则可以去掉该基本块
    bool onlyOndeUnconditionalBranch(Function *f) {
        cout << ">> Only one unconditional branch: " << endl;
        bool completed = true;
        // cout << "    -> mark 1 in only one unconditional branch" << endl;
        if (f->all_blocks.empty()) { return completed; }
        for (auto i: f->all_blocks) {
            // cout << "    -> mark 2 in only one unconditional branch" << endl;
            cout << "    -> in basic block: " << i->bb_idx << endl;
            if (i->bb_idx <= 0) { 
                cout << "    ->>> continue: block index <= 0;" << endl;
                continue; 
            }
            Inst *inst = i->basic_block.back();
            // cout << "    -> mark 3-1 in only one unconditional branch" << endl;
            Case(LLIR_BR, br_inst, inst) {
                // dbg(br_inst->ToString());
                cout << "    $inst: " << br_inst->ToString() << endl;
                if (br_inst == i->basic_block.front()) {
                    cout << "    ->>> continue: first inst in block" << endl;
                    continue;
                }
                if (i->basic_block.size() == 1) {
                    if (br_inst->has_cond) {
                        cout << "    ->>> continue: inst has condition" << endl;
                        continue;
                    }
                    completed = false;
                    // cout << "    -> mark 3-2 in only one unconditional branch" << endl;
                    // 获取当前基本块的迭代器
                    auto bb_idx = find(f->all_blocks.begin(), f->all_blocks.end(), i);
                    // cout << "    -> mark 4 in only one unconditional branch" << endl;
                    BasicBlock cur_bb = **bb_idx;
                    // 对于只有一个无条件的跳转语句的基本块来说, 可能有很多个前驱, 但只能有一个后继.
                    // assert (cur_bb.succs.size() == 1);
                    // cout << "    -> mark 5-1 in only one unconditional branch" << endl;
                    int32_t pred_idx[cur_bb.preds.size()];
                    int32_t cnt = 0;
                    for (auto i : cur_bb.preds) {
                        pred_idx[cnt] = i.first;
                        cnt++;
                    }
                    // cout << "    -> mark 5-2 in only one unconditional branch" << endl;
                    int32_t succ_idx = cur_bb.succs.begin()->first;
                    int32_t cur_idx = cur_bb.bb_idx;
                    // cout << "    -> mark 5-3 in only one unconditional branch" << endl;
                    for (auto i : f->all_blocks) {
                        for (auto j : pred_idx) {
                            if (i->bb_idx == j) {
                                i->succs.erase(cur_idx);
                                i->succs.insert(make_pair(succ_idx, cur_bb.succs.begin()->second));
                                cout << "\t\tchange pred: " << j << " to " << succ_idx << endl;
                            }
                        }
                        if (i->bb_idx == succ_idx) {
                            i->preds.erase(cur_idx);
                            for (auto j : pred_idx) {
                                i->preds.insert(make_pair(j, cur_bb.preds.begin()->second));
                                cout << "\t\tchange succ: " << endl;
                            }
                        }
                        for (auto j : i->basic_block) {
                            Case(LLIR_BR, br_inst, j) {
                                if(br_inst->tar_false == cur_idx) {
                                    br_inst->tar_false = succ_idx;
                                    cout << "\t\tchange br tar_false: " << cur_idx << " to " << succ_idx << endl;
                                } else if (br_inst->tar_true == cur_idx) {
                                    br_inst->tar_true = succ_idx;
                                    cout << "\t\tchange br tar_true: " << cur_idx << " to " << succ_idx << endl;
                                }
                            }
                        }
                    }
                    
                    Scope *scope = f->main_scope;
                    blockFinder(scope, &f->all_blocks, cur_idx, pred_idx, succ_idx);

                    // if (scope_iter == f->main_scope->elements->end()) {
                    //     cout << "        @@22-> continue: connot find inst in scope" << endl;
                    // }

                    // 干掉当前基本块
                    auto iter = f->all_blocks.erase(bb_idx)-1;


                    if (iter == f->all_blocks.end()) {
                        cout << "        @@11-> continue: erase block in blocks failed" << endl;
                        continue;
                    }
                    
                    
                    cout << "    ######> remove basic block: " << cur_idx << endl;
                }
            }
        }
        return completed;
    }

    bool sameTargetBranch(Function *f) {
        cout << ">> Same target branch: " << endl;
        bool completed = true;
        if (f->all_blocks.empty()) { return completed; }
        for (auto i: f->all_blocks) {
            if (i->bb_idx <= 0) { continue; }
            Inst *inst = i->basic_block.back();
            Case(LLIR_BR, br_inst, inst) {
                if (br_inst->tar_false == br_inst->tar_true) {
                    cout << "\tsame Target branch:\n\t  " << br_inst->ToString() << endl;
                    br_inst->has_cond = false;
                    br_inst->tar_false = -1;
                    cout << "\tafter erase:" << br_inst->ToString() << endl;
                }
            }
        }
        return completed;
    }

    bool eraseUselessBB(Function *f, int32_t bb_idx) {
        cout << ">> Erase useless basic block: " << bb_idx << endl;
        bool completed = true;
        if (bb_idx <= 0) { return completed; }
        for (auto i : f->all_blocks) {
            if (i->bb_idx == bb_idx) {
                for (auto j: i->preds) {
                    j.second->succs.erase(bb_idx);
                }
                for (auto j : i->succs) {
                    j.second->preds.erase(bb_idx);
                }
                auto iter = f->all_blocks.erase(find(f->all_blocks.begin(), f->all_blocks.end(), i));
                if (iter == f->all_blocks.end()) {
                    cout << "        @@-> continue: erase block in blocks failed" << endl;
                    continue;
                }
                f->all_blocks.erase(iter);
            }
        }
        return completed;
    }

    void eraseUselessScope(Scope *scope, int32_t bb_idx) {
        cout << ">> Erase useless scope: " << scope->sp_idx << endl;
        bool completed = true;
        if (scope == nullptr || scope->elements->empty()) {
            return;
        }
        for (auto scope_iter = scope->elements->begin(); scope_iter != scope->elements->end(); ++scope_iter) {
            if (Scope *scope_node = dynamic_cast<Scope *>(*scope_iter); scope_node != nullptr) {
                // cout << "----> scope: " << scope_node->sp_idx << endl;
                eraseUselessScope(scope_node, bb_idx);
                return;
            } else {
                BasicBlock *bb_node = dynamic_cast<BasicBlock *>(*scope_iter);
                if (bb_node->bb_idx == bb_idx) {
                    for(auto j: bb_node->preds) {
                        j.second->succs.erase(bb_idx);
                    }
                    for(auto j: bb_node->succs) {
                        j.second->preds.erase(bb_idx);
                    }
                    scope->elements->erase(scope_iter);
                }
            }
        }
        return;
    }

    bool reduceConditionalBrach(Function *f) {
        cout << endl << ">> reduce Conditional branch: " << endl;
        bool completed = true;
        if (f->all_blocks.empty()) { return completed; }
        for (auto i: f->all_blocks) {
            if (i == nullptr) { return completed; }
            cout << "    -> in basic block: " << i->bb_idx << endl;
            if (i->bb_idx <= 0) { 
                cout << "    ->>> continue: block index <= 0;" << endl;
                continue; 
            }
            Inst *inst = i->basic_block.back();
            Case(LLIR_BR, br_inst, inst) {
                cout << "    $inst: " << br_inst->ToString() << endl;
                if (br_inst->has_cond) {
                    cout << "    ->>> has cond;" << endl;
                    SRC cur_cond = br_inst->cond;
                    int32_t t_tar = br_inst->tar_true;
                    int32_t f_tar = br_inst->tar_false;
                    cout << "everything is okay until now." <<endl;
                    /* 
                        在这里段错误了
                     */
                    if (cur_cond.ToCTValue() == nullptr) {
                        cout << "    ->>> continue: cond is VirtualReg;" << endl;
                        continue;
                    }
                    if (cur_cond.ToCTValue()->int_value!=0) {
                        // true condition
                        cout << "TRUE!!!!!!!!!" << endl;
                        br_inst->tar_false = -1;
                        br_inst->has_cond = false;
                        // 当前基本块的前驱有很多, 后继只有tar_true和tar_false。
                        // 在修改时，应将后续基本块中tar_false全删除
                        // 当前跳转语句应该修改成 br %tar_true
                        // 应在tar_false的前驱和后继中删除所有有关tar_false的前驱后继信息
                        // erase useless block完成 删除无用基本块 + 修改前驱后继
                        eraseUselessBB(f, f_tar);
                        eraseUselessScope(f->main_scope, f_tar);

                    } else {
                        cout << "FALSE!!!!!!!!!" << endl;
                        // false condition
                        br_inst->tar_true = -1;
                        br_inst->has_cond = false;
                        eraseUselessBB(f, t_tar);
                        eraseUselessScope(f->main_scope, t_tar);
                    }
                }
            }
        }
    }

    bool removePredBasicBlock(BasicBlock *pred, BasicBlock *succ, Function *f) {
        cout << "  >>>> removePredBasicBlock: " << endl;
        bool completed = true;
        pair<int32_t, BasicBlock*> pred_pair;
        cout << "    -> mark 1" << endl;
        for (auto i: pred->preds) {
            if (i.second == succ) {
                pred_pair = i;
                break;
            }
        }
        cout << "    -> mark 2" << endl;
        auto pred_index_itr = pred->preds.find(pred_pair.first);
        // dbg("pred_index_itr: " << pred_index_itr - pred->preds.begin());
        int32_t pred_idx = pred_pair.first;
        cout << "    -> mark 3" << endl;
        cout << "       Erase pred: " << pred_idx << " in BasicBlock " << succ->bb_idx << endl;
        succ->preds.erase(pred_index_itr);

        for (auto &&inst_iter = succ->basic_block.begin(); inst_iter != succ->basic_block.end(); ++inst_iter) { 
            auto inst = *inst_iter;
            if (!dynamic_cast<LLIR_PHI *>(inst)) {
                break;
            }
            
            auto phi_inst = dynamic_cast<LLIR_PHI *>(inst);
            if (phi_inst->srcs.size() == 1) {
                // 这里可能有问题
                f->replaceSRCs(succ, phi_inst->srcs[0].first.reg, phi_inst->dst);
                inst_iter = succ->basic_block.erase(inst_iter) - 1;
            } else {

            }
        }
        return true;
    }

    void dfs(BasicBlock *bb) {
        if (!bb->dirty) {
            bb->dirty = true;
            if (!bb->succs.empty()) {
                for (auto succ : bb->succs) {
                    dfs(succ.second);
                }
            }
        }
    }

    // removeDeadBB：去掉除了entry之外的没有前驱的基本块 
    bool removeDeadBB (Function *func) {
        cout << ">> removeDeadBB:" << endl;
        bool completed = true;
        for (auto bb: func->all_blocks) {
            bb->dirty = false;
        }
        dfs(func->all_blocks[0]);
        
        // auto &&inst_iter = data_bb.begin(); inst_iter != data_bb.end(); ++inst_iter
        for (auto &&bb = func->all_blocks.begin()+1; bb != func->all_blocks.end(); ++bb) {
            auto dead = *bb;
            if (!dead->dirty) {
                for (auto succ : (*bb)->succs) {
                    removePredBasicBlock(dead, succ.second, func);
                }
                for (auto inst : (*bb)->basic_block) {
                    // func->removeInst(inst);
                }
                bb = func->all_blocks.erase(bb) - 1;
                completed = false;
            }
        }
        return completed;
    }

};

