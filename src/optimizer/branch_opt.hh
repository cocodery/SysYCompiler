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
    map<LLIR_PHI *, int32_t>* phi2AllocaMap;
    bool is_phi_map_exist = false;
    vector<int32_t> modi_bbs; 

public:
    BranchOptimization(Function * func): function(func) { 
        this->modi_bbs.clear();
    };
    void runBranchOpt(map<LLIR_PHI *, int32_t>* phi2AllocaMap) {
        this->is_phi_map_exist = true;
        this->phi2AllocaMap = phi2AllocaMap;
        // cout << "----- Optimization Running: Branch Optimization -----" << endl;
        // cout << ("\033[1m\033[45;33m---> in function: " + function->func_info.func_name + " \033[0m") << endl;
        runBranchOptimization(function);
    }

    void run() {
        // cout << "----- Optimization Running: Branch Optimization -----" << endl;
        // cout << ("\033[1m\033[45;33m---> in function: " + function->func_info.func_name + " \033[0m") << endl;
        runBranchOptimization(function);
    }

    void runBranchOptimization(Function *f) {
        
        while(1) {
            bool completed;
            completed = reduceConditionalBrach(f);
            completed &= onlyOnePredSuccBranch(f);
            completed &= sameTargetBranch(f);
            completed &= removeEmptyScope(f->main_scope);
            completed &= removeEmptyBasicBlock(f);
            completed &= removeUselessLoop(f);
            if (completed) {
                break;
            }
        }
        // cout << "----- Optimization Finished: Branch Optimization -----" << endl;
        
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
        // cout << ">> Remove useless phi: " << completed << endl;
        return completed;
    }


    void blockFinder(Scope *scope, vector<BasicBlock *>* all_blocks, int32_t bb_idx, int32_t pred_idx[], int32_t succ_idx) {
        // // cout << " ----block finder---- " << endl;
        if (scope == nullptr || scope->elements->empty()) {
            return;
        }
        for (auto scope_iter = scope->elements->begin(); scope_iter != scope->elements->end(); ++scope_iter) {
            // // cout << " # ";
            if (*scope_iter == nullptr) {
                continue;
            }
            if (Scope *scope_node = dynamic_cast<Scope *>(*scope_iter); scope_node != nullptr) {
                // // cout << "----> scope: " << scope_node->sp_idx << endl;
                blockFinder(scope_node, all_blocks, bb_idx, pred_idx, succ_idx);
                return;
            } else {
                BasicBlock *bb_node = dynamic_cast<BasicBlock *>(*scope_iter);
                if (bb_node->bb_idx == bb_idx) {
                    // cout << endl << "    ######> find basic block: " << bb_idx << endl;
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
        // cout << ">> Only one unconditional branch: " << endl;
        bool completed = true;
        // // cout << "    -> mark 1 in only one unconditional branch" << endl;
        if (f->all_blocks.empty()) { return completed; }
        for (auto i: f->all_blocks) {
            // // cout << "    -> mark 2 in only one unconditional branch" << endl;
            // cout << "    -> in basic block: " << i->bb_idx << endl;
            if (i->bb_idx <= 0) { 
                // cout << "    ->>> continue: block index <= 0;" << endl;
                continue; 
            }
            Inst *inst = i->basic_block.back();
            // // cout << "    -> mark 3-1 in only one unconditional branch" << endl;
            Case(LLIR_BR, br_inst, inst) {
                // dbg(br_inst->ToString());
                // cout << "    $inst: " << br_inst->ToString() << endl;
                if (br_inst == i->basic_block.front()) {
                    // cout << "    ->>> continue: first inst in block" << endl;
                    continue;
                }
                if (i->basic_block.size() == 1) {
                    if (br_inst->has_cond) {
                        // cout << "    ->>> continue: inst has condition" << endl;
                        continue;
                    }
                    completed = false;
                    // // cout << "    -> mark 3-2 in only one unconditional branch" << endl;
                    // 获取当前基本块的迭代器
                    auto bb_idx = find(f->all_blocks.begin(), f->all_blocks.end(), i);
                    // // cout << "    -> mark 4 in only one unconditional branch" << endl;
                    BasicBlock cur_bb = **bb_idx;
                    // 对于只有一个无条件的跳转语句的基本块来说, 可能有很多个前驱, 但只能有一个后继.
                    // assert (cur_bb.succs.size() == 1);
                    // // cout << "    -> mark 5-1 in only one unconditional branch" << endl;
                    int32_t pred_idx[cur_bb.preds.size()];
                    int32_t cnt = 0;
                    for (auto i : cur_bb.preds) {
                        pred_idx[cnt] = i.first;
                        cnt++;
                    }
                    // // cout << "    -> mark 5-2 in only one unconditional branch" << endl;
                    int32_t succ_idx = cur_bb.succs.begin()->first;
                    int32_t cur_idx = cur_bb.bb_idx;
                    // // cout << "    -> mark 5-3 in only one unconditional branch" << endl;
                    for (auto i : f->all_blocks) {
                        for (auto j : pred_idx) {
                            if (i->bb_idx == j) {
                                i->succs.erase(cur_idx);
                                i->succs.insert(make_pair(succ_idx, cur_bb.succs.begin()->second));
                                // cout << "\t\tchange pred: " << j << " to " << succ_idx << endl;
                            }
                        }
                        if (i->bb_idx == succ_idx) {
                            i->preds.erase(cur_idx);
                            for (auto j : pred_idx) {
                                i->preds.insert(make_pair(j, cur_bb.preds.begin()->second));
                                // cout << "\t\tchange succ: " << endl;
                            }
                        }
                        for (auto j : i->basic_block) {
                            Case(LLIR_BR, br_inst, j) {
                                if(br_inst->tar_false == cur_idx) {
                                    br_inst->tar_false = succ_idx;
                                    // cout << "\t\tchange br tar_false: " << cur_idx << " to " << succ_idx << endl;
                                } else if (br_inst->tar_true == cur_idx) {
                                    br_inst->tar_true = succ_idx;
                                    // cout << "\t\tchange br tar_true: " << cur_idx << " to " << succ_idx << endl;
                                }
                            }
                        }
                    }
                    
                    Scope *scope = f->main_scope;
                    blockFinder(scope, &f->all_blocks, cur_idx, pred_idx, succ_idx);

                    // if (scope_iter == f->main_scope->elements->end()) {
                    //     // cout << "        @@22-> continue: connot find inst in scope" << endl;
                    // }

                    // 干掉当前基本块
                    auto iter = f->all_blocks.erase(bb_idx)-1;


                    if (iter == f->all_blocks.end()) {
                        // cout << "        @@11-> continue: erase block in blocks failed" << endl;
                        continue;
                    }
                    
                    
                    // cout << "    ######> remove basic block: " << cur_idx << endl;
                }
            }
        }
        return completed;
    }

    
    void blockFinder(Scope *scope, int32_t bb_idx) {
        if (scope == nullptr || scope->elements->empty()) {
            return;
        }
        for (auto iter = scope->elements->begin(); iter != scope->elements->end(); ++iter) {
            if (Scope *scope_node = dynamic_cast<Scope *>(*iter); scope_node != nullptr) {
                blockFinder(scope_node, bb_idx);
            } else {
                BasicBlock *bb_node = dynamic_cast<BasicBlock *>(*iter);
                if (bb_node->bb_idx == bb_idx) {
                    // cout << "        @@-> erase basic block: " << bb_idx << endl;
                    
                    auto pred_node = bb_node->preds.begin()->second;
                    auto succ_node = bb_node->succs.begin()->second;
                    int32_t pred_idx = bb_node->preds.begin()->first;
                    int32_t succ_idx = bb_node->succs.begin()->first;
                    Case(LLIR_BR, br_inst, pred_node->basic_block.back()) {
                        if (br_inst->has_cond) {
                            if (br_inst->tar_false == bb_idx) {
                                br_inst->tar_false = succ_idx;
                                // cout << "\t\tchange br tar_false: " << bb_idx << " to " << succ_idx << endl;
                            } else if (br_inst->tar_true == bb_idx) {
                                br_inst->tar_true = succ_idx;
                                // cout << "\t\tchange br tar_true: " << bb_idx << " to " << succ_idx << endl;
                            }
                        } else {
                            br_inst->tar_true = succ_idx;
                            // cout << "\t\tchange br tar_true: " << bb_idx << " to " << succ_idx << endl;
                        }
                        pred_node->succs.erase(bb_idx);
                        pred_node->succs.insert(make_pair(succ_idx, succ_node));
                        succ_node->preds.erase(bb_idx);
                        succ_node->preds.insert(make_pair(pred_idx, pred_node));

                        if (is_phi_map_exist) fixPhiInst(bb_idx, pred_idx);

                    }

                    bb_node->preds.erase(bb_idx);
                    bb_node->succs.erase(bb_idx);

                    bb_node->valuable = false;
                    scope->elements->erase(iter);
                    break;
                }
            }
        }
    }

    void blockFinder(Scope *scope, int32_t bb_idx, bool is_head) {
        // dbg("in blockFinder "+std::to_string(bb_idx));
        if (is_head) {   
            if (scope == nullptr || scope->elements->empty()) {
                return;
            }
            for (auto iter = scope->elements->begin(); iter != scope->elements->end(); ++iter) {
                if (Scope *scope_node = dynamic_cast<Scope *>(*iter); scope_node != nullptr) {
                    blockFinder(scope_node, bb_idx, true);
                } else {
                    BasicBlock *bb_node = dynamic_cast<BasicBlock *>(*iter);
                    if (bb_node->bb_idx == bb_idx) {
                        bb_node->valuable = false;
                        scope->elements->erase(iter);
                        break;
                    }
                }
            }
        }
    }


    bool onlyOnePredSuccBranch (Function *f) {
        // cout << ">> Only one pred and succ branch: " << endl;
        bool completed = true;
        if (f->all_blocks.empty()) { return completed; }
        for (auto &&i: f->all_blocks) {
            
            if (i->bb_idx <= 0) { continue; }
            
            Inst *inst = i->basic_block.back();
            Case(LLIR_BR, br_inst, inst) {
                if (i->basic_block.size() == 1) {
                    // condition: only one LLIR_BR inst
                    if (br_inst->has_cond) {
                        // cout << "    ->>> continue: inst has condition" << endl;
                        continue;
                    } else {
                        
                        if (i->preds.size()==1 && i->succs.size()==1) {
                            // if this bb has only one preds & one succ, delete it.
                            int32_t pred_idx = i->preds.begin()->first;
                            int32_t succ_idx = i->succs.begin()->first;
                            if (pred_idx == succ_idx) {
                                // cout << "    ->>> continue: preds == succs" << endl;
                                continue;
                            } else if (pred_idx == 0 && succ_idx == -1) {
                                // cout << "    ->>> continue: only one basicblock" << endl;
                                continue;
                            } else if (pred_idx == 0) {
                                // cout << "    ->>> continue: preds == 0 (cannot opt the entry block)" << endl;
                                continue;
                            } else if (find(modi_bbs.begin(), modi_bbs.end(), succ_idx) != modi_bbs.end()) {
                                // cout << "    ->>> continue: succ has been modified" << endl;
                                continue;
                            }

                            
                            // cout << "    $inst: " << br_inst->ToString() << endl;
                            // cout << "    ######> remove basic block: " << i->bb_idx << endl;

                            blockFinder(f->main_scope, i->bb_idx);

                            auto cur_iter = find(f->all_blocks.begin(), f->all_blocks.end(), i);
                            auto iter = f->all_blocks.erase(cur_iter)-1;
                            if (iter == f->all_blocks.end()) {
                                // cout << "        @@11-> continue: erase block in blocks failed" << endl;
                                continue;
                            }
                        }
                    }
                } else {
                    // 把一些可以合并的基本块合并掉
                    if (i->preds.size()==1 && i->succs.size()==1) {
                        int32_t pred_idx = i->preds.begin()->first;
                        int32_t succ_idx = i->succs.begin()->first;
                        if (pred_idx == succ_idx) {
                            // cout << "    ->>> continue: preds == succs" << endl;
                            continue;
                        } else if (pred_idx == 0 && succ_idx == -1) {
                            // cout << "    ->>> continue: only one basicblock" << endl;
                            continue;
                        } else if (pred_idx == 0) {
                            // cout << "    ->>> continue: preds == 0 (cannot opt the entry block)" << endl;
                            continue;
                        }
                        if ( (i->preds.begin()->second->succs.size() == 1)&&(i->succs.begin()->second->preds.size() == 1)) {
                            auto succ_bb = i->succs.begin()->second;

                            // cout << "    in basic block " + std::to_string(i->bb_idx) + ": " + br_inst->ToString() << endl;
                            // cout << "    ->>> case: more than one inst." << endl;
                            
                            modi_bbs.push_back(succ_idx);

                            BasicBlock* pred_bb = f->getSpecificIdxBb(pred_idx);
                            pred_bb->succs.erase(i->bb_idx);
                            pred_bb->succs.insert(make_pair(succ_idx, i->succs.begin()->second));
                            auto inst = pred_bb->basic_block.back();
                            Case(LLIR_BR, br_inst, inst) {
                                if (br_inst->has_cond) {
                                    if (br_inst->tar_true == i->bb_idx) {
                                        br_inst->tar_true = succ_idx;
                                    } else if (br_inst->tar_false == i->bb_idx) {
                                        br_inst->tar_false = succ_idx;
                                    }
                                } else {
                                    br_inst->tar_true = succ_idx;
                                }
                            }
                            vector<Inst *> tmp_inst;
                            tmp_inst.assign(i->basic_block.begin(), i->basic_block.end()-1);
                            succ_bb->basic_block.insert(succ_bb->basic_block.begin(), tmp_inst.begin(), tmp_inst.end());
                            succ_bb->preds.erase(i->bb_idx);
                            succ_bb->preds.insert(make_pair(pred_idx, pred_bb));


                            if (is_phi_map_exist) fixPhiInst(i->bb_idx, pred_idx);
                            blockFinder(f->main_scope, i->bb_idx, true);
                            
                            auto cur_iter = find(f->all_blocks.begin(), f->all_blocks.end(), i);
                            auto iter = f->all_blocks.erase(cur_iter)-1;
                            
                        }
                    }
                }
            }
        }
        return completed;
    }

    bool sameTargetBranch(Function *f) {
        // cout << ">> Same target branch: " << endl;
        bool completed = true;
        if (f->all_blocks.empty()) { return completed; }
        for (auto i: f->all_blocks) {
            if (i->bb_idx <= 0) { continue; }
            Inst *inst = i->basic_block.back();
            Case(LLIR_BR, br_inst, inst) {
                if (br_inst->tar_false == br_inst->tar_true) {
                    // cout << "\tsame Target branch:\n\t  " << br_inst->ToString() << endl;
                    br_inst->has_cond = false;
                    br_inst->tar_false = -1;
                    // cout << "\tafter erase:" << br_inst->ToString() << endl;
                }
            }
        }
        return completed;
    }

    void eraseUselessScope(Scope *scope, int32_t bb_idx) {
        // cout << ">> Erase useless scope: " << scope->sp_idx << endl;
        bool completed = true;
        if (scope == nullptr || scope->elements->empty()) {
            return;
        }
        for (auto scope_iter = scope->elements->begin(); scope_iter != scope->elements->end(); ++scope_iter) {
            if (Scope *scope_node = dynamic_cast<Scope *>(*scope_iter); scope_node != nullptr) {
                // // cout << "----> scope: " << scope_node->sp_idx << endl;
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

    bool removeEmptyScope(Scope *scope) {
        bool completed = true;
        if (scope == nullptr) {
            return completed;
        }
        for (auto scope_iter = scope->elements->begin(); scope_iter != scope->elements->end(); ++scope_iter) {
            if (Scope *scope_node = dynamic_cast<Scope *>(*scope_iter); scope_node != nullptr) {
                // // cout << "----> scope: " << scope_node->sp_idx << endl;
                if (scope_node->elements->empty()) {
                    // cout << ">> Remove empty scope: " << scope_node->sp_idx << endl;
                    scope_iter = scope->elements->erase(scope_iter) - 1;
                    continue;
                } else {
                    removeEmptyScope(scope_node);
                }
            }
        }
        return completed;
    }

    bool removeEmptyBasicBlock(Function *f) {
        for (auto iter = f->all_blocks.begin(); iter != f->all_blocks.end(); ++iter) {
            auto i = *iter;
            // cout << "in bb " << i->bb_idx << ": " << endl;
            if (i->bb_idx <= 0) { continue; }
            else if (i->preds.empty()) {
                // cout << ">> Erase [empty] basic block: " << i->bb_idx << endl;
                blockFinder(f->main_scope, i->bb_idx, true);
                for(auto &&j: i->succs) {
                    // cout << "->>> succ: " << j.second->bb_idx << endl;
                    j.second->preds.erase(i->bb_idx);
                }
                iter = f->all_blocks.erase(find(f->all_blocks.begin(), f->all_blocks.end(), i)) - 1;
            }
        }
        return true;
    }

    void dfs(Function* f, int32_t bb_idx, set<int32_t> &visited) {
        auto bb_node = f->getSpecificIdxBb(bb_idx);
        visited.insert(bb_idx);
        for (auto &p : bb_node->succs) {
            if (p.second->bb_idx == -1) return;
            if (visited.find(p.second->bb_idx) == visited.end()) {
                if (p.second->bb_idx >= bb_idx) {
                    dfs(f, p.first, visited);
                }
            }
        }
    }

    bool removeUselessLoop(Function *f) {
        set<int32_t> visited;
        dfs(f, 1, visited);
        if (visited.size() < (f->all_blocks.size()-2)) {
            for(auto iter = f->all_blocks.begin(); iter != f->all_blocks.end(); ++iter) {
                BasicBlock* i = *iter;
                if (i->bb_idx <= 0) { continue; }
                else if (visited.find(i->bb_idx) == visited.end()) {
                    blockFinder(f->main_scope, i->bb_idx, true);
                    iter = f->all_blocks.erase(find(f->all_blocks.begin(), f->all_blocks.end(), i)) - 1;
                }
            }
        }

        return true;
    }



    bool isUsedInLoop(Function *f, int32_t bb_idx) {
        BasicBlock * cur_bb = f->getSpecificIdxBb(bb_idx);
        
        if (cur_bb == nullptr) {
            return true;
        } else if(cur_bb->preds.size()>1){ 
            // cout << std::to_string(bb_idx) << " is used in loop " << endl;
            return true;
        } else {
            for (auto &&i : cur_bb->preds) {
                if (i.first > cur_bb->bb_idx) {
                    // cout << std::to_string(bb_idx) << " is used in loop " << endl;
                    return true;
                }
            }
            return false;
        }
    }

    bool eraseUselessBB(Function *f, int32_t bb_idx) {

        bool completed = true;
        if (bb_idx <= 0) { return completed; }
        BasicBlock *i = f->getSpecificIdxBb(bb_idx);
        
        if (i->preds.empty()) {
            // cout << ">> Erase [empty] basic block: " << bb_idx << endl;
            f->all_blocks.erase(find(f->all_blocks.begin(), f->all_blocks.end(), i));
            return completed;
        } 

        Case (LLIR_BR, br_inst, i->basic_block.back()) {
            if (br_inst->has_cond) {
                BasicBlock* true_bb = f->getSpecificIdxBb(br_inst->tar_true);
                BasicBlock* false_bb = f->getSpecificIdxBb(br_inst->tar_false);
                true_bb->preds.erase(bb_idx);
                false_bb->preds.erase(bb_idx);
            } else {
                BasicBlock* true_bb = f->getSpecificIdxBb(br_inst->tar_true);
                true_bb->preds.erase(bb_idx);
            }
        
            
        }
        auto iter = f->all_blocks.erase(find(f->all_blocks.begin(), f->all_blocks.end(), i));
        if (iter == f->all_blocks.end()) {
            // cout << "        @@-> continue: erase block in blocks failed" << endl;
            return completed;
        }
        // cout << ">> Erase useless basic block: " << bb_idx << endl;
        return completed;
    }


    bool reduceConditionalBrach(Function *f) {
        // cout << endl << ">> reduce Conditional branch: " << endl;
        bool completed = true;
        if (f->all_blocks.empty()) { return completed; }
        for (auto &&i: f->all_blocks) {
            if (i == nullptr) { return completed; }
            // cout << "    -> in basic block: " << i->bb_idx << endl;
            if (i->bb_idx <= 0) { 
                // cout << "    ->>> continue: block index <= 0;" << endl;
                continue; 
            } 
            // bool result = true;
            // for (auto &&j: i->succs) {
            //     if (find(modi_bbs.begin(), modi_bbs.end(), j.first) != modi_bbs.end()) {
            //         result &= false;
            //     } else {
            //         result &= true;
            //     }
            // }
            // if (!result) {
            //     // cout << "    ->>> [continue: succ has been modified]" << endl;
            //     continue;
            // }
            Inst *inst = i->basic_block.back();
            Case(LLIR_BR, br_inst, inst) {
                // cout << "    $inst: " << br_inst->ToString() << endl;
                if (br_inst->has_cond) {
                    // cout << "    ->>> has cond: ";
                    SRC cur_cond = br_inst->cond;
                    int32_t t_tar = br_inst->tar_true;
                    int32_t f_tar = br_inst->tar_false;
                    if (cur_cond.ToCTValue() == nullptr) {
                        // cout << "    ->>> continue: cond is VirtualReg;" << endl;
                        continue;
                    }
                    if (cur_cond.ToCTValue()->int_value!=0) {
                        // true condition
                        // cout << "TRUE" << endl;
                        br_inst->has_cond = false;
                        br_inst->tar_true = t_tar;
                        br_inst->tar_false = -1;
                        i->succs.erase(f_tar);
                        BasicBlock* f_bb = f->getSpecificIdxBb(f_tar);
                        f_bb->preds.erase(i->bb_idx);
                        // cout << "      $ after fixed: " << br_inst->ToString() << endl;
                        // change current inst to unconditional branch inst
                        // erase useless block完成 删除无用基本块 + 修改前驱后继
                        // if (!isUsedInLoop(f, f_tar)) {
                        //     br_inst->tar_false = -1;
                        //     br_inst->has_cond = false;
                        //     eraseUselessBB(f, f_tar);
                        //     i->succs.erase(f_tar);
                        //     blockFinder(f->main_scope, f_tar, true);
                        // }
                    } else {
                        // cout << "FALSE" << endl;
                        br_inst->has_cond = false;
                        br_inst->tar_true = f_tar;
                        br_inst->tar_false = -1;
                        i->succs.erase(t_tar);
                        BasicBlock* t_bb = f->getSpecificIdxBb(t_tar);
                        t_bb->preds.erase(i->bb_idx);
                        // cout << "      $ after fixed: " << br_inst->ToString() << endl;
                        // false condition
                        // if (!isUsedInLoop(f, t_tar)) {
                        //     br_inst->tar_false = -1;
                        //     br_inst->has_cond = false;
                        //     eraseUselessBB(f, t_tar);
                        //     i->succs.erase(t_tar);
                        //     blockFinder(f->main_scope, t_tar, true);
                        // }
                    }
                    
                }
            }
        }
        return completed;
    }

    void fixPhiInst(int32_t bb_idx, int32_t chg_idx) {   
        if (phi2AllocaMap == nullptr) return;
        for (auto &&i: *phi2AllocaMap) {
            if (i.first == nullptr) return;
            Case (LLIR_PHI, phi_inst, i.first) {
                if (phi_inst == nullptr)
                    ;
                // dbg(phi_inst->ToString());
                for (auto &&j: phi_inst->srcs) {
                    
                    if (j.second == bb_idx) {
                        j.second = chg_idx;
                    }
                }
            }
        }
    }

    bool removePredBasicBlock(BasicBlock *pred, BasicBlock *succ, Function *f) {
        // cout << "  >>>> removePredBasicBlock: " << endl;
        bool completed = true;
        pair<int32_t, BasicBlock*> pred_pair;
        // cout << "    -> mark 1" << endl;
        for (auto i: pred->preds) {
            if (i.second == succ) {
                pred_pair = i;
                break;
            }
        }
        // cout << "    -> mark 2" << endl;
        auto pred_index_itr = pred->preds.find(pred_pair.first);
        // dbg("pred_index_itr: " << pred_index_itr - pred->preds.begin());
        int32_t pred_idx = pred_pair.first;
        // cout << "    -> mark 3" << endl;
        // cout << "       Erase pred: " << pred_idx << " in BasicBlock " << succ->bb_idx << endl;
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

    // removeDeadBB：去掉除了entry之外的没有前驱的基本块 
    bool removeDeadBB (Function *func) {
        // cout << ">> removeDeadBB:" << endl;
        bool completed = true;
        for (auto bb: func->all_blocks) {
            bb->dirty = false;
        }
        // dfs(func->all_blocks[0]);
        
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

