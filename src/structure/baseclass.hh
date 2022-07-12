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
    virtual ~Inst() { }
};

class VirtReg: public Info {
public:
    int32_t reg_id;
    VarType type;
    bool global;
    bool assign;
public:
    VirtReg(int32_t _idx, VarType _type = VarType(TypeVoid), bool _glb = false, bool _assign = true) 
        : reg_id(_idx), type(_type), global(_glb), assign(_assign) { }
    string ToString() {
        std::stringstream ss;
        if (global) ss << "@_" << reg_id;
        else ss << "%r" << reg_id;
        return ss.str();
    }
};
