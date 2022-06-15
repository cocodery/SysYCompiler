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
    string ToString();
    void printuOpInst();
};

class BinaryOpInst: public Inst {
public:
    BinaryOp op;
    VirtReg dst, src1, src2;
    bool need_cast;
public:
    BinaryOpInst(BinaryOp _op, VirtReg _dst, VirtReg _src1, VirtReg _src2, bool _nc = false) : op(_op), dst(_dst), src1(_src1), src2(_src2), need_cast(_nc) { }
    string ToString();
    void printbOpInst();
};

class ReturnInst: public Inst {
public:
    bool has_retvalue;
    VirtReg dst;
public:
    ReturnInst(bool _ret_v, VirtReg vg) : dst(vg), has_retvalue(_ret_v) { }
    string ToString();
    void printRetInst();
};

// load a constant to register
// ld.c reg.X, Y
class LoadNumber: public Inst {
public:
    VirtReg dst;
    CTValue src;
public:
    LoadNumber(VirtReg _dst, CTValue _src) : dst(_dst), src(_src) { }
    string ToString();
    void printLdcInst();
};

// load value in register to address in register
// st [reg.X], reg.Y
class StoreMem: public Inst {
public:
    VirtReg dst, src;
public:
    StoreMem(VirtReg _dst, VirtReg _src): dst(_dst), src(_src) { }
    string ToString();
    void printStmInst();
};

// load an address of variable to register
// ld.a reg.X, addr
class LoadAddress: public Inst {
public:
    VirtReg dst;
    Variable *variable;
public:
    LoadAddress(VirtReg _dst, Variable *v): dst(_dst), variable(v) { }
    string ToString();
    void printLdaInst();
};

// load value in [reg.Y] to reg.X
// ld.v reg.X, [reg.Y]
class LoadValue: public Inst {
public:
    VirtReg dst, src;
public:
    LoadValue(VirtReg _dst, VirtReg _src): dst(_dst), src(_src) { }
    string ToString();
    void printLdvInst();
};

// add array offset to addr
// cur_dim_size is `size`, `off` is offset num
// ld.o reg.X, reg.Y, reg.Z * size
class LoadOffset: public Inst {
public:
    VirtReg dst, addr, off;
    int32_t size;
public:
    LoadOffset(VirtReg _dst, VirtReg _addr, VirtReg _off, int32_t _size): dst(_dst), addr(_addr), off(_off), size(_size) { }
    string ToString();
    void printLdoInst();
};
