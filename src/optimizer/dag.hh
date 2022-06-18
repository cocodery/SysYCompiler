#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <map>

#include "../common.hh"
#include "../structure/ir.hh"
#include "../structure/inst.hh"
#include "../structure/value.hh"

using std::ostream;
using std::vector;
using std::string;
using std::map;

enum class operatorType {
    ADD, SUB, MUL, DIV, MOD, LTH, LEQ, NEQ, AND, ORR,
    NOT, NEG, POS,
};

class dagLeafConstNode {
public:
    int32_t dag_idx;
    CTValue value;
    vector<VirtReg> addi_tags;
public:
    void createConstNode(int32_t dag_idx, CTValue value, vector<VirtReg> addi_tags);
};
    
class dagLeafVarNode {
public:
    int32_t dag_idx;
    Variable *var;
    vector<VirtReg> addi_tags;
public:
    void createVarNode(int32_t dag_idx, Variable *var, vector<VirtReg> addi_tags);
};

class dagNode{
public:
    int32_t dag_idx;
    operatorType op;
    VirtReg reg;
    vector<VirtReg> addi_tags;
    dagLeafConstNode *cr;
    dagLeafConstNode *cl;
    dagLeafVarNode *vr;
    dagLeafVarNode *vl;
    dagNode *left;
    dagNode *right;
public:
    dagNode() : dag_idx(0), reg(VirtReg()), addi_tags(vector<VirtReg>()), cr(nullptr), cl(nullptr), vr(nullptr), vl(nullptr) {}
    int32_t getIdx() { return dag_idx; }
    bool isLeaf() { return ((right==nullptr)&&(left==nullptr)); }
    void createOpNode(int32_t dag_idx, VirtReg reg, operatorType op, vector<VirtReg> addi_tags, dagNode *right, dagNode *left);
    void createOpNode(int32_t dag_idx, VirtReg reg, operatorType op, vector<VirtReg> addi_tags, dagLeafConstNode *cr, dagLeafConstNode *cl);
    void createOpNode(int32_t dag_idx, VirtReg reg, operatorType op, vector<VirtReg> addi_tags, dagLeafVarNode *vr, dagLeafVarNode *vl);
};

class dag{
public:
    int32_t node_cnt;
    vector<dagNode> dag_nodes;
    vector<dagLeafConstNode> dag_leaf_const_nodes;
    vector<dagLeafVarNode> dag_leaf_var_nodes;
public:
    dag() : node_cnt(0), dag_nodes(vector<dagNode>()) {}
    void printDag();
    int32_t isExist(CTValue value);
    int32_t isExist(Variable *var);
    int32_t isExist(VirtReg reg, bool erase);
    int32_t isExprExist(VirtReg left, VirtReg right, operatorType op);
    int32_t isExprExist(VirtReg src, operatorType op);
    void printDagNode();
    operatorType getOp(BinaryOp bop) {
        switch(bop.bin_op){
            case BinaryOp::Type::ADD: return operatorType::ADD;
            case BinaryOp::Type::SUB: return operatorType::SUB;
            case BinaryOp::Type::MUL: return operatorType::MUL;
            case BinaryOp::Type::DIV: return operatorType::DIV;
            case BinaryOp::Type::MOD: return operatorType::MOD;
            case BinaryOp::Type::LTH: return operatorType::LTH;
            case BinaryOp::Type::LEQ: return operatorType::LEQ;
            case BinaryOp::Type::NEQ: return operatorType::NEQ;
            case BinaryOp::Type::AND: return operatorType::AND;
            case BinaryOp::Type::ORR: return operatorType::ORR;
        }
    }
    operatorType getOp(UnaryOp uop) {
        switch (uop.unary_op) {
            case UnaryOp::Type::NOT: return operatorType::NOT;
            case UnaryOp::Type::NEG: return operatorType::NEG;
            case UnaryOp::Type::POS: return operatorType::POS;
        }
    }
    string turnOp(operatorType op){
        switch(op){
            case operatorType::ADD: return "+";
            case operatorType::SUB: return "-";
            case operatorType::MUL: return "*";
            case operatorType::DIV: return "/";
            case operatorType::MOD: return "%";
            case operatorType::LTH: return "<";
            case operatorType::LEQ: return "<=";
            case operatorType::NEQ: return "!=";
            case operatorType::AND: return "&&";
            case operatorType::ORR: return "||";
            case operatorType::NOT: return "!";
            case operatorType::NEG: return "-";
            case operatorType::POS: return "+";
            default: return "";
        }
    }
};

int32_t dag::isExist(CTValue value){
    for (int32_t i = 0; i < dag_leaf_const_nodes.size(); ++i) {
        if (dag_leaf_const_nodes[i].value.type == value.type) {
            if (dag_leaf_const_nodes[i].value.type == TypeInt && (dag_leaf_const_nodes[i].value.int_value == value.int_value)) {
                return i;
            } else if (dag_leaf_const_nodes[i].value.type == TypeFloat && (dag_leaf_const_nodes[i].value.float_value == value.float_value)){
                return i;
            }
        }
    }
    return -1;
}

int32_t dag::isExist(Variable *var){
    for (int32_t i = 0; i < dag_leaf_var_nodes.size(); ++i) {
        if ((dag_leaf_var_nodes[i]).var->var_idx == var->var_idx) {
            return i;
        }
    }
    return -1;
}

// when checking the reg is existed or not, we just check the addi_tags and ignore the main reg
int32_t dag::isExist(VirtReg reg, bool erase = false){ 
    for (int32_t i = 0; i < node_cnt; ++i) {
        for (int32_t j = 0; j < dag_nodes[i].addi_tags.size(); ++j) {
            if (dag_nodes[i].addi_tags[j].reg_id == reg.reg_id) {
                // erase the existed reg, temporarily.
                if (erase) dag_nodes[i].addi_tags.erase(dag_nodes[i].addi_tags.begin() + j);
                return i;
            }
        }
    }
    return -1;
}

int32_t dag::isExprExist(VirtReg left, VirtReg right, operatorType op){
    for (int32_t i = 0; i < node_cnt; ++i) {
        if (dag_nodes[i].op == op) {
            if ((dag_nodes[i].left->reg == left)&&(dag_nodes[i].right->reg == right)) {
                return i;
            }
        }
    }
    return -1;
}


int32_t dag::isExprExist(VirtReg src, operatorType op){
    for (int32_t i = 0; i < node_cnt; ++i) {
        if (dag_nodes[i].op == op) {
            if (dag_nodes[i].left->reg == src) {
                return i;
            }
        }
    }
    return -1;
}


void dagLeafVarNode::createVarNode(int32_t dag_idx, Variable *var, vector<VirtReg> addi_tags) {
    this->dag_idx = dag_idx;
    this->var = var;
    this->addi_tags = addi_tags;
}

void dagLeafConstNode::createConstNode(int32_t dag_idx, CTValue value, vector<VirtReg> addi_tags) {
    this->dag_idx = dag_idx;
    this->value = value;
    this->addi_tags = addi_tags;
}

void dagNode::createOpNode(int32_t dag_idx, VirtReg reg, operatorType op, vector<VirtReg> addi_tags, dagNode *right, dagNode *left) {
    this->dag_idx = dag_idx;
    this->op = op;
    this->reg = reg;
    this->addi_tags = addi_tags;
    this->right = right;
    this->left = left;
}

void dag::printDagNode() {
    for (int32_t i = 0; i < dag_nodes.size(); ++i) {
        cout << "\n{ dag_idx: " << dag_nodes[i].dag_idx << endl;
        // cout << "  is_const: " << dag_nodes[i].is_const << endl;
        // cout << "CTValue" << dag_nodes[i].value.type << " i:" << dag_nodes[i].value.int_value << " f:" << dag_nodes[i].value.float_value << endl;
        // cout << "operatorType: " << turnOp(dag_nodes[i].op) << endl;
        // cout << " is_var: " << dag_nodes[i].is_var << endl;
        // cout << "Variable: " << dag_nodes[i].var.var_idx << endl;
        cout << "VirtReg: " << dag_nodes[i].reg.reg_id << endl;
        cout << "addi_tags: ";
        for (int32_t j = 0; j < dag_nodes[i].addi_tags.size(); ++j) {
            cout << dag_nodes[i].addi_tags[j].reg_id << " ";
        }
        cout << endl;
        if (dag_nodes[i].left!= nullptr) cout << "left: " << dag_nodes[i].left->dag_idx << endl;
        if (dag_nodes[i].right!= nullptr) cout << "right: " << dag_nodes[i].right->dag_idx << endl;
        cout << "}" << endl;
    }
}

void ReadFromBasicBlock(const BasicBlock& bb, dag &dag);
void reGenerateBB(dag &dag);


void ReadFromScope(const Scope& scope, bool interateElems = true) {
    auto&& elementPtrs = *scope.elements;
    for (auto&& elementPtr : elementPtrs) {
        if (Scope *scope_node = dynamic_cast<Scope *>(elementPtr); scope_node != nullptr){
            ReadFromScope(*scope_node);
        }
        else{
            BasicBlock *bb_node = dynamic_cast<BasicBlock *>(elementPtr);
            dag dag;
            ReadFromBasicBlock(*bb_node, dag);
            
            reGenerateBB(dag);
        }
    }
}

void ReadFromBasicBlock(const BasicBlock& bb, dag &dag) {
    for (auto inst: bb.basic_block) {
        Case (ReturnInst, ret_inst, inst) {
            continue; // if return, do nothing
        }
        Case (LoadNumber, ldc_inst, inst) {
            if (int32_t i = dag.isExist(ldc_inst->src); i != -1) {
                dag.dag_nodes[i].addi_tags.push_back(ldc_inst->dst);
            } else {
                vector<VirtReg> addi_tags;
                addi_tags.push_back(ldc_inst->dst); //put that dst reg to addi_tags
                dag.isExist(ldc_inst->dst, true);
                dagLeafConstNode dag_leaf_const_node;
                dag_leaf_const_node.createConstNode(dag.node_cnt, ldc_inst->src, addi_tags);
                dag.dag_leaf_const_nodes.push_back(dag_leaf_const_node);
                dag.node_cnt++;
            }
        }
        Case (LoadAddress, lda_inst, inst) {
            if (int32_t i = dag.isExist(lda_inst->variable); i != -1) {
                dag.dag_nodes[i].addi_tags.push_back(lda_inst->dst);
            } else {
                vector<VirtReg> addi_tags;
                addi_tags.push_back(lda_inst->dst); //put that dst reg to addi_tags
                dag.isExist(lda_inst->dst, true);
                dagLeafVarNode dag_leaf_var_node;
                dag_leaf_var_node.createVarNode(dag.node_cnt, lda_inst->variable, addi_tags);
                dag.dag_leaf_var_nodes.push_back(dag_leaf_var_node);
                dag.node_cnt++;
            }
        }
        Case (LoadValue, ldv_inst, inst) {
            dag.isExist(ldv_inst->dst, true);
            dag.dag_nodes[dag.isExist(ldv_inst->src)].addi_tags.push_back(ldv_inst->dst);
        }
        Case (StoreMem, stv_inst, inst) {
            dag.isExist(stv_inst->dst, true);
            dag.dag_nodes[dag.isExist(stv_inst->src)].addi_tags.push_back(stv_inst->dst);
        }
        Case (BinaryOpInst, bin_inst, inst) { // if return -1, that means the node is not exist.
            if (int32_t i = dag.isExprExist(bin_inst->src1, bin_inst->src2, dag.getOp(bin_inst->op)); i != -1) {
                dag.dag_nodes[i].addi_tags.push_back(bin_inst->dst);
            } else {
                vector<VirtReg> addi_tags;
                dag.isExist(bin_inst->dst, true);
                dagNode dag_node;
                dagNode *left;
                dagNode *right;
                left = &dag.dag_nodes[dag.isExist(bin_inst->src1)];
                right = &dag.dag_nodes[dag.isExist(bin_inst->src2)];
                dag_node.createOpNode(dag.node_cnt, bin_inst->dst, dag.getOp(bin_inst->op), addi_tags, left, right);
                dag.dag_nodes.push_back(dag_node);
                dag.node_cnt++;
            }
        }
        Case (UnaryOpInst, un_inst, inst) {
            if (int32_t i = dag.isExprExist(un_inst->src, dag.getOp(un_inst->op)); i != -1) {
                dag.dag_nodes[i].addi_tags.push_back(un_inst->dst);
            } else {
                vector<VirtReg> addi_tags;
                dag.isExist(un_inst->dst, true);
                dagNode dag_node;
                dagNode *left;
                dagNode *right;
                left = &dag.dag_nodes[dag.isExist(un_inst->src)];
                right = nullptr;
                dag_node.createOpNode(dag.node_cnt, un_inst->dst, dag.getOp(un_inst->op), addi_tags, left, right);
                dag.dag_nodes.push_back(dag_node);
                dag.node_cnt++;
            }
        }
        // add array offset to addr
        // cur_dim_size is `size`, `off` is offset num
        // ld.o reg.X, reg.Y, reg.Z * size
        /* VirtReg dst, addr, off;
        int32_t size; */
        // Case (LoadOffset, ldo_inst, inst) {
        //     if (int32_t i = dag.isExprExist)
        // }
        dag.printDagNode();
        cout << "-------------------------" << endl;
    }
}

void generateDAG(const CompUnit &ir) {
    for (int i = 0, size = ir.functions.size(); i < size; ++i) {
        cout << "   -ir in Funtion " << i << '\n';
        auto&& func = *ir.functions[i];
        auto&& mainScope = *func.main_scope;
        ReadFromScope(mainScope);
    }
}

void reGenerateBB(dag &dag) {
    BasicBlock bb;
    for (int32_t i = 0; i < dag.node_cnt; ++i) {
        cout << "   -node " << i << '\n';
        dagNode cur_node = dag.dag_nodes[i];
        if (cur_node.isLeaf()!=true) {
            if (cur_node.addi_tags.size() != 0) {
                switch (cur_node.op) {
                    case operatorType::ADD:
                        bb.basic_block.push_back((new BinaryOpInst(BinaryOp::ADD, cur_node.reg, cur_node.left->reg, cur_node.right->reg)));
                        break;
                    case operatorType::SUB:
                        bb.basic_block.push_back((new BinaryOpInst(BinaryOp::SUB, cur_node.reg, cur_node.left->reg, cur_node.right->reg)));
                        break;
                    case operatorType::MUL:
                        bb.basic_block.push_back((new BinaryOpInst(BinaryOp::MUL, cur_node.reg, cur_node.left->reg, cur_node.right->reg)));
                        break;
                    case operatorType::DIV:
                        bb.basic_block.push_back((new BinaryOpInst(BinaryOp::DIV, cur_node.reg, cur_node.left->reg, cur_node.right->reg)));
                        break;
                    case operatorType::MOD:
                        bb.basic_block.push_back((new BinaryOpInst(BinaryOp::MOD, cur_node.reg, cur_node.left->reg, cur_node.right->reg)));
                        break;
                    case operatorType::AND:
                        bb.basic_block.push_back((new BinaryOpInst(BinaryOp::AND, cur_node.reg, cur_node.left->reg, cur_node.right->reg)));
                        break;
                    case operatorType::ORR:
                        bb.basic_block.push_back((new BinaryOpInst(BinaryOp::ORR, cur_node.reg, cur_node.left->reg, cur_node.right->reg)));
                        break;
                    case operatorType::NOT:
                        bb.basic_block.push_back((new UnaryOpInst(UnaryOp::NOT, cur_node.reg, cur_node.left->reg)));
                        break;
                    case operatorType::NEG:
                        bb.basic_block.push_back((new UnaryOpInst(UnaryOp::NEG, cur_node.reg, cur_node.left->reg)));
                        break;
                    case operatorType::POS:
                        bb.basic_block.push_back((new UnaryOpInst(UnaryOp::POS, cur_node.reg, cur_node.left->reg)));
                        break;
                }
            }
        }
        // } else {
        //     if (cur_node.addi_tags.size() != 0) {
        //         for (int32_t j = 0; j < cur_node.addi_tags.size(); ++j) {
        //             if (int32_t k = dag.isExist(cur_node.addi_tags[j]); k != -1) {
        //                 bb.basic_block.push_back((new Load(cur_node.reg, cur_node.left->reg, cur_node.right->reg, dag.dag_nodes[k].reg)));
        //             }
        //         }
        //     }
        // }
        
    }
    cout << "#######################" << endl;
    bb.printBlock();
    cout << "#######################" << endl;
}
