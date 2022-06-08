#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <map>

#include "../common.hh"
#include "SymTable.hh"

#define Case(Type, dst, src) if (auto dst = dynamic_cast<Type *>(src))

using std::ostream;
using std::vector;
using std::variant;
using std::string;
using std::map;

static int32_t reg_idx = 0;

class Info {
public:
    virtual ~Info() { }
};

class VirtReg: public Info {
public:
    int32_t reg_id;
public:
    VirtReg() : reg_id(reg_id = reg_idx++) { }
    bool operator == (const VirtReg &r) { return reg_id == r.reg_id; }
};

class UnaryOp: public Info {
public:
    enum Type {
        NOT, NEG, POS,
    } unary_type;
    const string get_op() {
        string op[] = { "!", "-", "+" };
        return op[(int32_t)unary_type];
    }
    UnaryOp(Type type) : unary_type(type) { }
};

class BinaryOP: public Info {
    enum Type {
        ADD, SUB, MUL, DIV, MOD, LTH, LEQ, EQU, NEQ, AND, ORR, LSH, RSH, 
    } bin_op;
    const string get_op() {
        string op[] = { "+", "-", "*", "/", "%", "<", "<=", "==", "!=", "&&", "||", "<<", ">>" };
        return op[(int32_t)bin_op];
    }
    BinaryOP(Type op) : bin_op(op) { }
};

class Inst: public Info {
public:
    Inst() { }
};

class UnaryOpInst: public Inst {
public:
    UnaryOp op;
    VirtReg dst, src;
public:
    UnaryOpInst(UnaryOp _op, VirtReg _dst, VirtReg _src) : op(_op), dst(_dst), src(_src) { }
};

class BinaryOpInst: public Inst {
public:
    BinaryOP op;
    VirtReg dst, src1, src2;
public:
    BinaryOpInst(BinaryOP _op, VirtReg _dst, VirtReg _src1, VirtReg _src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
};

class ReturnInst: public Inst {
public:
    bool has_retvalue;
    VirtReg dst;
public:
    ReturnInst(bool _ret_v = false) : has_retvalue(_ret_v) { }
    void printRetInst();
};

class BasicBlock: public Info {
public:
    vector<Inst *> basic_block; 
public:
    BasicBlock() { }
    void printBlock();
};

class Scope: public Info {
public:
    VariableTable  *local_table;
    vector<Info *> *elements; // `Info` -> `Scope` or `BasicBlock`
    Scope* parent;
public:
    Scope() { local_table = nullptr; elements = nullptr; parent = nullptr; }
    Variable *resolve(string var_name);
    void printElements();
    void printScope();
};

class Function {
public:
    FunctionInfo func_info;
    Scope *main_scope;
public:
    void printFunction();
};

class LibFunction {
public:
    FunctionInfo libfunc_info;
    bool is_used;
public:
    void printFunction();
};

class CompUnit: public Info {
public:
    Scope *global_scope;
    vector<Function *> functions;
    LibFunction lib_functions[11];
public:
    CompUnit();
    void DebugLibFuncs();
    void DebugUserFuncs();
    void DebugGlobalTable();
};
