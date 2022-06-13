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

static string uop[] = { "!", "-", "+" };
static string bop[] = { "+", "-", "*", "/", "%", "<", "<=", "==", "!=", "&&", "||" };

class UnaryOp {
public:
    enum Type {
        NOT, NEG, POS,
    } unary_op;
    const string get_op() {
        return uop[(int32_t)unary_op];
    }
    Type get_op(string op) {
        int idx = 0;
        for (; idx < 3; ++idx) {
            if (op == uop[idx]) {
                break;
            }
        }
        return Type(idx);
    };
    UnaryOp(Type type) : unary_op(type) { }
    UnaryOp(string op) : unary_op(get_op(op)) { }
};

class BinaryOp {
public:
    enum Type {
        ADD, SUB, MUL, DIV, MOD, LTH, LEQ, EQU, NEQ, AND, ORR, 
    } bin_op;
    const string get_op() {
        return bop[(int32_t)bin_op];
    }
    Type get_op(string op) {
        int idx = 0;
        for (; idx < 11; ++idx) {
            if (op == bop[idx]) {
                break;
            }
        }
        return Type(idx);
    };
    BinaryOp(Type op) : bin_op(op) { }
    BinaryOp(string op): bin_op(get_op(op)) { }
};

class UnaryOpInst: public Inst {
public:
    UnaryOp op;
    VirtReg dst, src;
public:
    UnaryOpInst(UnaryOp _op, VirtReg _dst, VirtReg _src) : op(_op), dst(_dst), src(_src) { }
    void printuOpInst();
};

class BinaryOpInst: public Inst {
public:
    BinaryOp op;
    VirtReg dst, src1, src2;
public:
    BinaryOpInst(BinaryOp _op, VirtReg _dst, VirtReg _src1, VirtReg _src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
    void printbOpInst();
};

class ReturnInst: public Inst {
public:
    bool has_retvalue;
    VirtReg dst;
public:
    ReturnInst(bool _ret_v, VirtReg vg) : dst(vg), has_retvalue(_ret_v) { }
    void printRetInst();
};

class LoadNumber: public Inst {
public:
    VirtReg dst;
    CTValue src;
public:
    LoadNumber(VirtReg _dst, CTValue _src) : dst(_dst), src(_src) { }
    void printLdcInst();
};

class AssignInst: public Inst {
public:
    VirtReg dst, src;
public:
    AssignInst(VirtReg _dst, VirtReg _src): dst(_dst), src(_src) { }
    void printAssInst() { };
};
