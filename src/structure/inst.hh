#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <map>

#include "../common.hh"
#include "symtable.hh"
#include "baseclass.hh"
#include "value.hh"

class UnaryOp {
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

class BinaryOP {
    enum Type {
        ADD, SUB, MUL, DIV, MOD, LTH, LEQ, EQU, NEQ, AND, ORR, LSH, RSH, 
    } bin_op;
    const string get_op() {
        string op[] = { "+", "-", "*", "/", "%", "<", "<=", "==", "!=", "&&", "||", "<<", ">>" };
        return op[(int32_t)bin_op];
    }
    BinaryOP(Type op) : bin_op(op) { }
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

class LoadNumber: public Inst {
public:
    CTValue src;
    VirtReg dst;
public:
    LoadNumber(CTValue _src, VirtReg _dst) : src(_src), dst(_dst) { }
    void printLdcInst();
};
