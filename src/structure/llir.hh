#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../common.hh"
#include "symtable.hh"
#include "baseclass.hh"
#include "value.hh"

static string bop[] = { "+", "-", "*", "/", "%", "<", "<=", "==", "!=" };
static string bops[] = { ""};

class BinaryOp {
public:
    enum Type {
        ADD, SUB, MUL, DIV, MOD, LTH, LEQ, EQU, NEQ, 
    } bin_op;
    const string get_op() {
        return bop[(int32_t)bin_op];
    }
    Type get_op(string op) {
        int32_t idx = 0;
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

class LLIR_RET: public Inst {
public:
    bool has_retvalue;
    Info *ret_value;
public:
    LLIR_RET(bool _hrv, Info *_vr): has_retvalue(_hrv), ret_value(_vr) { }
    string ToString();
    void printRetInst();
};

class LLIR_BR: public Inst {
public:
};

class LLIR_BIN: public Inst {
public:
    BinaryOp op;
    VirtReg *dst;
    Info *src1, *src2;
public:
    LLIR_BIN(BinaryOp _op, VirtReg *_dst, Info *_src1, Info *_src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
    string ToString();
    void printBinInst();
};

class LLIR_FBIN: public Inst {
public:
    BinaryOp op;
    VirtReg *dst;
    Info *src1, *src2;
public:
    LLIR_FBIN(BinaryOp _op, VirtReg* _dst, Info *_src1, Info *_src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
    string ToString();
    void printFBinInst();
};

class LLIR_ALLOCA: public Inst {
public:
};

class LLIR_LOAD: public Inst {
public:
};

class LLIR_STORE: public Inst {
public:
};

class LLIR_GEP: public Inst {
public:
};

class LLIR_ICMP: public Inst {
public:
};

class LLIR_FCMP: public Inst {
public:
};

class LLIR_PHI: public Inst {
public:
};

class LLIR_CALL: public Inst {
public:
};

class LLIR_ZEXT: public Inst {
public:
};

class LLIR_FPEXT: public Inst {
public:
};

class LLIR_SITOFP: public Inst {
public:
};

class LLIR_FPTOSI: public Inst {
public:
};
