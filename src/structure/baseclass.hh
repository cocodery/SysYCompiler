#pragma once

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
public:
    VirtReg() : reg_id(reg_id = reg_idx++) { }
    VirtReg(int32_t tar_idx) : reg_id(tar_idx) { }
    bool operator == (const VirtReg &r) { return reg_id == r.reg_id; }
};

const VirtReg NoRetReg = VirtReg(-1);
