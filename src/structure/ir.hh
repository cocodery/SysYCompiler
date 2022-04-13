#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <map>

#include "../common.hh"
#include "SymTable.hh"

using std::ostream;
using std::vector;
using std::variant;
using std::string;
using std::map;

class Info {
public:
    // virtual void print(ostream &os) const = 0;
    // virtual ~Info() { }
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
};

class UnaryOpInst: public Inst {
public:
    UnaryOp op;
    Reg dst, src;
    UnaryOpInst(UnaryOp _op, Reg _dst, Reg _src) : op(_op), dst(_dst), src(_src) { }
};

class BinaryOpInst: public Inst {
public:
    BinaryOP op;
    Reg dst, src1, src2;
    BinaryOpInst(BinaryOP _op, Reg _dst, Reg _src1, Reg _src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
};

class Scope: public Info {
public:
    VariableTable local_table;
    vector<variant<Inst *, Scope *>> elements;
};

class Function {
public:
    FunctionInfo func_info;
    Scope main_scope;
};

class LibFunction {
public:
    FunctionInfo libfunc_info;
    bool is_used;
};

class CompUnit: public Info {
public:
    VariableTable global_table;
    vector<Function> functions;
    vector<LibFunction> lib_functions;
};
