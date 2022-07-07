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
public:
    VirtReg(DeclType _type = TypeVoid) : reg_id(reg_id = reg_idx++), type(_type) { }
    VirtReg(int32_t tar_idx) : reg_id(tar_idx), type(TypeVoid) { }
    string ToString() {
        std::stringstream ss;
        ss << " %" << reg_id;
        return ss.str();
    }
    bool operator == (const VirtReg &r) { return reg_id == r.reg_id && type == r.type; }
};

const VirtReg NoRetReg = VirtReg(-1);
