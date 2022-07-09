#pragma once

#include "../common.hh"

static int32_t reg_idx = 0;

class Info {
public:
    virtual ~Info() { }
};

class Inst: public Info {
public:
    virtual ~Inst() { }
};

class VirtReg: public Info {
public:
    int32_t reg_id;
    DeclType type;
    bool global;
    bool assign;
public:
    VirtReg(int32_t _idx, DeclType _type = TypeVoid, bool _glb = false, bool _assign = true) 
        : reg_id(_idx), type(_type), global(_glb), assign(_assign) { }
    string ToString() {
        std::stringstream ss;
        if (global) ss << "@_" << reg_id;
        else ss << "%" << reg_id;
        return ss.str();
    }
    bool operator == (const VirtReg &r) { return reg_id == r.reg_id && type == r.type; }
};

const VirtReg NoRetReg = VirtReg(-1);
