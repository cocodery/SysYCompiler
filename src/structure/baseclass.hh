#pragma once

#include "../common.hh"
#include "symtable.hh"

static int32_t reg_idx = 0;

class Info {
public:
    virtual ~Info() { }
};

class Inst: public Info {
public:
    CLAIM_AVAIL_REGS
public:
    REGs GetNthUnusedRRegister(int n) const {
        int skipped_cnt = 0;
        for (auto &&r : availRegs) {
            if (skipped_cnt < n - 1) {
                ++skipped_cnt;
                continue;
            }
            if (r < s0) return r;
            else return SPILL;
        }
        return SPILL;
    }
    REGs GetFirstUnusedRRegister() const {
        return GetNthUnusedRRegister(1);
    }
    REGs GetSecondUnusedRRegister() const {
        return GetNthUnusedRRegister(2);
    }
    REGs GetThirdUnusedRRegister() const {
        return GetNthUnusedRRegister(3);
    }
    virtual ~Inst() { }
};

class VirtReg: public Info {
public:
    int32_t reg_id;
    VarType type;
    bool global;
    bool assign;

    // codegen
    bool is_from_gep = false;
public:
    VirtReg(int32_t _idx, VarType _type = VarType(TypeVoid), bool _glb = false, bool _assign = true) 
        : reg_id(_idx), type(_type), global(_glb), assign(_assign) { }
    string ToString() {
        std::stringstream ss;
        if (global) ss << "@_" << reg_id;
        else ss << "%r" << reg_id;
        return ss.str();
    }
    bool operator == (const VirtReg &rhs) const {
        return (reg_id == rhs.reg_id) && (global == rhs.global) && (type == rhs.type);
    }
};
