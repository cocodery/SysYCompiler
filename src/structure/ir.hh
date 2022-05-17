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

class IRValue {
public:
    DeclType type;
    int32_t int_value;
    float float_value;
public:
    IRValue(): type(TypeVoid), int_value(0), float_value(0) { }
    IRValue(DeclType t = TypeVoid, int32_t iv = 0, float fv = 0);
};

class Info {
public:
    virtual ~Info() { }
};

class Reg: public Info {
public:
    int reg_id;
public:
    Reg(int id = 0) : reg_id(id) { }
    bool operator == (const Reg &r) { return reg_id == r.reg_id; }
};

class UnaryOp: public Info {
public:
    enum Type {
        NOT, NEG, POS,
    } unary_type;
    const string get_op() {
        string op[] = { "!", "-", "+" };
        return op[(int)unary_type];
    }
    UnaryOp(Type type) : unary_type(type) { }
};

class BinaryOP: public Info {
    enum Type {
        ADD, SUB, MUL, DIV, MOD, LTH, LEQ, EQU, NEQ, AND, ORR, LSH, RSH, 
    } bin_op;
    const string get_op() {
        string op[] = { "+", "-", "*", "/", "%", "<", "<=", "==", "!=", "&&", "||", "<<", ">>" };
        return op[(int)bin_op];
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
    Reg dst, src;
public:
    UnaryOpInst(UnaryOp _op, Reg _dst, Reg _src) : op(_op), dst(_dst), src(_src) { }
};

class BinaryOpInst: public Inst {
public:
    BinaryOP op;
    Reg dst, src1, src2;
public:
    BinaryOpInst(BinaryOP _op, Reg _dst, Reg _src1, Reg _src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
};

class ReturnInst: public Inst {
public:
    bool has_retvalue;
    Reg dst;
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
public:
    Scope() { }
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
    VariableTable *global_table;
    vector<Function *> functions;
    LibFunction lib_functions[11];
public:
    CompUnit();
    void DebugLibFuncs();
    void DebugUserFuncs();
    void DebugGlobalTable();
};

IRValue operator + (IRValue lhs, IRValue rhs);
IRValue operator - (IRValue lhs, IRValue rhs);
IRValue operator * (IRValue lhs, IRValue rhs);
IRValue operator / (IRValue lhs, IRValue rhs);
IRValue operator % (IRValue lhs, IRValue rhs);
