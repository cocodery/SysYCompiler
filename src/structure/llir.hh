#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../common.hh"
#include "symtable.hh"
#include "baseclass.hh"
#include "value.hh"

class SRC {
public:
    CTValue *ctv;
    VirtReg *reg;
public:
    SRC(): ctv(nullptr), reg(nullptr) { }
    SRC(CTValue *_ctv): ctv(_ctv), reg(nullptr) { }
    SRC(VirtReg *_reg): ctv(nullptr), reg(_reg) { }
    string ToString();
    DeclType getType();
    CTValue *ToCTValue() {
        return (ctv == nullptr )? nullptr : ctv;
    };
    VirtReg *ToVirtReg() {
        return (reg == nullptr) ? nullptr : reg;
    }
};

class LLIR_RET: public Inst {
public:
    bool has_retvalue;
    SRC ret_value;
public:
    LLIR_RET(bool _hrv, SRC _vr): has_retvalue(_hrv), ret_value(_vr) { }
    string ToString();
};

class LLIR_BR: public Inst {
public:
    bool has_cond;
    SRC cond;
    int32_t tar_true;
    int32_t tar_false;
public:
    LLIR_BR(bool _hs, SRC _cond,int32_t _t, int32_t _f) : has_cond(_hs), cond(_cond), tar_true(_t), tar_false(_f) { }
    string ToString();
};

class LLIR_BIN: public Inst {
public:
    BinOp op;
    SRC dst;
    SRC src1, src2;
public:
    LLIR_BIN(BinOp _op, SRC _dst, SRC _src1, SRC _src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
    string ToString();
};

class LLIR_FBIN: public Inst {
public:
    BinOp op;
    SRC dst;
    SRC src1, src2;
public:
    LLIR_FBIN(BinOp _op, SRC _dst, SRC _src1, SRC _src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
    string ToString();
};

class LLIR_ALLOCA: public Inst {
public:
    SRC reg;
    Variable *var;
public:
    LLIR_ALLOCA(SRC _reg, Variable *_var) : reg(_reg), var(_var) { }
    string ToString();
};

class LLIR_LOAD: public Inst {
public:
    SRC dst;
    SRC src;
public:
    LLIR_LOAD(SRC _dst, SRC _src) : dst(_dst), src(_src) { }
    string ToString();
};

class LLIR_STORE: public Inst {
public:
    SRC dst;
    SRC src;
public:
    LLIR_STORE(SRC _dst, SRC _src) : dst(_dst), src(_src) { }
    string ToString();
};

class LLIR_GEP: public Inst {
public:
    SRC dst;
    SRC src;
    SRC off;
    VarType type;
public:
    LLIR_GEP(SRC _dst, SRC _src, SRC _off, VarType _type) : dst(_dst), src(_src), off(_off), type(_type) { }
    string ToString();
};

class LLIR_ICMP: public Inst {
public:
    RelOp op;
    SRC dst;
    SRC src1, src2;
public:
    LLIR_ICMP(RelOp _op, SRC _dst, SRC _src1, SRC _src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
    string ToString();
};

class LLIR_FCMP: public Inst {
public:
    RelOp op;
    SRC dst;
    SRC src1, src2;
public:
    LLIR_FCMP(RelOp _op, SRC _dst, SRC _src1, SRC _src2) : op(_op), dst(_dst), src1(_src1), src2(_src2) { }
    string ToString();
};

class LLIR_PHI: public Inst {
public:
};

class LLIR_CALL: public Inst {
public:
    SRC dst;
    vector<SRC> args;
    FunctionInfo *func_info;
public:
    LLIR_CALL(SRC _dst, vector<SRC> _args, FunctionInfo *_func_info) : dst(_dst), args(_args), func_info(_func_info) { }
    string ToString();
};

class LLIR_ZEXT: public Inst {
public:
    SRC dst, src;
public:
    LLIR_ZEXT(SRC _dst, SRC _src): dst(_dst), src(_src) { }
    string ToString();
};

class LLIR_FPEXT: public Inst {
public:
};

class LLIR_SITOFP: public Inst {
public:
    SRC dst, src;
public:
    LLIR_SITOFP(SRC _dst, SRC _src): dst(_dst), src(_src) { }
    string ToString();
};

class LLIR_FPTOSI: public Inst {
public:
    SRC dst, src;
public:
    LLIR_FPTOSI(SRC _dst, SRC _src): dst(_dst), src(_src) { }
    string ToString();
};

class LLIR_XOR : public Inst {
public:
    SRC dst;
    SRC src;
public:
    LLIR_XOR(SRC _dst, SRC _src): dst(_dst), src(_src) { }
    string ToString();
};

class LLIR_BC: public Inst {
public: 
    SRC dst;
    SRC src;
public:
    LLIR_BC(SRC _dst, SRC _src): dst(_dst), src(_src) { }
    string ToString();
};
