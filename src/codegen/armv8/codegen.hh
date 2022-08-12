#ifndef CODEGEN_H
#define CODEGEN_H

#include "common.hh"
#include "ir.hh"
#include "liveness_info.hh"
#include "register_allocation.hh"

#include <sstream>

using std::stringstream;

static enum AsmBranchType{LT, GE, LE, GT, EQ, NE, AlwaysTrue, AlwaysFalse} b_type;
#define REVERSED_BRANCH_TYPE(_BT) ((AsmBranchType)(((char)_BT & 0xfe) | (~(char)_BT & 1)))

bool IsOperand2(int i)
{
    int mask = 0xff;
    for (unsigned d = 0; d <= 30; d += 2)
    {
        mask = (mask >> d)|(mask << (32 - d));
        if ((i & mask) == i)
            return true;
    }
    return false;
}

int DoubleToWord(double d)
{
    float f = d;
    return *(int*)(&f);
}

int IntToFloat(int i)
{
    float f = i;
    return *(int*)(&f);
}

bool IsRReg(REGs r) {return r < s0;}

bool IsSReg(REGs r) {return r >= s0 && r <= s31;}

const char *FunctionRename(FunctionInfo *func)
{
    if (func->func_name == "imemset" || func->func_name == "fmemset")
        return "memset";
    else if (func->func_name == "my_getint")
        return "getint";
    else return func->func_name.c_str();
}

#define CTV_TO_WORD(_CTV_PTR) ((_CTV_PTR->type == TypeFloat) ? DoubleToWord(_CTV_PTR->float_value) : _CTV_PTR->int_value)

#define GET_GLOBAL_PTR_NAME(_VAR_IDX) ((string(GLOB_PTR_PREFIX) + std::to_string(_VAR_IDX)).c_str())

#define GET_GLOBAL_VAR_NAME(_VAR_IDX) ((string(GLOB_VAR_PREFIX) + std::to_string(_VAR_IDX)).c_str())

#define GET_LOCAL_PTR_NAME(_FUNC_PTR, _VAR_IDX) \
    ((string(LOCA_PTR_PREFIX) + _FUNC_PTR->func_info.func_name + "_" + std::to_string(_VAR_IDX)).c_str())

#define GET_LOCAL_VAR_NAME(_FUNC_PTR, _VAR_IDX) \
    ((string(LOCA_VAR_PREFIX) + _FUNC_PTR->func_info.func_name + "_" + std::to_string(_VAR_IDX)).c_str())

#define GET_PTR_NAME(_FUNC_PTR, _REG_PTR) \
    (_REG_PTR->global ?\
        GET_GLOBAL_PTR_NAME(_REG_PTR->reg_id) :\
        GET_LOCAL_PTR_NAME(_FUNC_PTR, _REG_PTR->reg_id))

#define GET_VAR_NAME(_FUNC_PTR, _REG_PTR) \
    (_REG_PTR->global ?\
        GET_GLOBAL_VAR_NAME(_REG_PTR->reg_id) :\
        GET_LOCAL_VAR_NAME(_FUNC_PTR, _REG_PTR->reg_id))

#define GET_ALLOCATION_RESULT(_FUNC_PTR, _VIRTREG_ID) \
    ((_FUNC_PTR->AllocationResult.find(_VIRTREG_ID) == _FUNC_PTR->AllocationResult.end()) ? \
     NOALLOC : _FUNC_PTR->AllocationResult.at(_VIRTREG_ID))

#define CALL_INST_GET_ARGS_REGISTER_IDX(_ARG_ID) ((_ARG_ID > 3) ? _ARG_ID + 9 : _ARG_ID)

#define GET_ARGS_REGISTER(_ARG_ID) ((_ARG_ID > 3) ? REGs(_ARG_ID + 16) : REGs(_ARG_ID))

#define CONDITION_REGISTER_NOT_ALLOCATED(_FUNC_PTR, _REG_IDX) \
    (_FUNC_PTR->AllocationResult.find(_REG_IDX) == _FUNC_PTR->AllocationResult.end())

#define R_REGISTER_IN_BRACKETS(x) ((string("[r") + std::to_string(x) + "]").c_str())

#define R_REGISTER_IN_BRACES(x) ((string("{r") + std::to_string(x) + "}").c_str())

#define IF_BORROW_USE_X_OR_USE_FIRST_AVAIL_REG(_BORROW, _X, _INST_PTR) \
    (_BORROW ? _X : *_INST_PTR->availRegs.begin())

#define IF_BORROW_SECOND_USE_X_OR_USE_LAST_AVAIL_REG(_BORROW_SECOND, _X, _INST_PTR) \
    (_BORROW_SECOND ? _X : *_INST_PTR->availRegs.rbegin())

#define DECLEAR_BORROW_PUSH(_INST_PTR, _REG) \
    bool borrow = false;\
    if (_INST_PTR->availRegs.empty())\
    {\
        AddAsmCodePushRegisters(asm_insts, {_REG}, indent);\
        borrow = true;\
    }

#define DECLEAR_BORROW_SECOND_PUSH(_INST_PTR, _REG) \
    bool borrow_second = false;\
    if (_INST_PTR->availRegs.size() < 2)\
    {\
        AddAsmCodePushRegisters(asm_insts, {_REG}, indent);\
        borrow_second = true;\
    }

#define DECLEAR_BORROW_POP(_REG) \
    if (borrow)\
        AddAsmCodePopRegisters(asm_insts, {_REG}, indent);

#define DECLEAR_BORROW_SECOND_POP(_REG) \
    if (borrow_second)\
        AddAsmCodePopRegisters(asm_insts, {_REG}, indent);

#define GET_BB_NAME(_FUNC_PTR, _BB_IDX) \
    ((_FUNC_PTR->func_info.func_name+"_block_"+std::to_string(_BB_IDX)).c_str())

#define GET_FUNC_RETURN_NAME(_FUNC_PTR) \
    ((_FUNC_PTR->func_info.func_name+"_return").c_str())

#define GET_LOCAL_VARS_NAME(_FUNC_PTR) \
    (("local_vars_"+_FUNC_PTR->func_info.func_name).c_str())

class Param
{
public:
    enum ParamType {Imm_int, Imm_float, Reg, Addr, Str} p_typ;
    union Value
    {
        int i;
        double f;
        REGs r;
        char str[128];
        Value() {}
        Value(int _i): i(_i){}
        Value(double _f): f(_f){}
        Value(REGs _r): r(_r){}
        Value(const char *_s) {strcpy(str, _s);}
        ~Value(){}
    } val;
public:
    Param() {}
    Param(int _i): p_typ(Imm_int), val(_i) {}
    Param(double _f): p_typ(Imm_float), val(_f) {}
    Param(REGs _r): p_typ(Reg), val(_r) {}
    Param(ParamType _p_typ, const char *_s): p_typ(_p_typ), val(_s) {}
    string ToString()
    {
        stringstream ss;
        switch (p_typ)
        {
        case Imm_int:
            ss << '#' << val.i;
            break;
        case Imm_float:
            break;
        case Reg:
            if (val.r <= 12)
                ss << 'r' << val.r;
            else if (val.r >= 16)
                ss << 's' << val.r - 16;
            else switch (val.r)
            {
            case sp: ss << "sp"; break;
            case lr: ss << "lr"; break;
            case pc: ss << "pc"; break;
            default: break;
            }
            break;
        case Addr:
            ss << '=' << val.str;
            break;
        case Str:
            ss << val.str;
            break;
        }
        return ss.str();
    }
    void invert()
    {
        if (p_typ == Imm_int)
            val.i = -val.i;
        else if (p_typ == Imm_float)
            val.f = -val.f;
    }
};

class AsmInst
{
public:
    enum InstType {
        EMPTY, DOT_GLOBAL, DOT_DATA, DOT_TEXT, BX,
        MOV, MOVT, MOVW, STR, LDR,
        ADD, SUB, DOT_WORD, DOT_SPACE, MUL,
        PUSH, POP, BL, CMP, BLT,
        BLE, BEQ, BNE, B, BGT,
        BGE, MOVLT, MOVGE, MOVLE, MOVGT,
        MOVEQ, MOVNE, SDIV, MVN, RSB,
        DOT_LTORG, EORNE, VADD, VSUB, VMUL,
        VDIV, VMOV, VCMP, VCVT_ITOF, VCVT_FTOI,
        FMSTAT, VPUSH, VPOP, VLDR, VSTR,
        VMVN
    } i_typ; const vector<string> i_str {
        "", ".global", ".data", ".text", "bx",
        "mov", "movt", "movw", "str", "ldr",
        "add", "sub", ".word", ".space", "mul",
        "push", "pop", "bl", "cmp", "blt",
        "ble", "beq", "bne", "b", "bgt",
        "bge", "movlt", "movge", "movle", "movgt",
        "moveq", "movne", "sdiv", "mvn", "rsb",
        ".ltorg", "eorne", "vadd.f32", "vsub.f32", "vmul.f32",
        "vdiv.f32", "vmov", "vcmp.f32", "vcvt.f32.s32", "vcvt.s32.f32",
        "fmstat", "vpush", "vpop", "vldr", "vstr",
        "vmvn"
    };
public:
    AsmInst(InstType _i_typ):i_typ(_i_typ) {}
    const string &ToString()
    {
        return i_str[i_typ];
    }
    static InstType ConvertAsmBranchTypeToMoveInst(AsmBranchType b_typ)
    {
        switch(b_typ)
        {
            case AsmBranchType::LT: return MOVLT;
            case AsmBranchType::GE: return MOVGE;
            case AsmBranchType::LE: return MOVLE;
            case AsmBranchType::GT: return MOVGT;
            case AsmBranchType::EQ: return MOVEQ;
            case AsmBranchType::NE: return MOVNE;
            default: return MOV;
        }
    }
    static InstType ConvertAsmBranchTypeToBranchInst(AsmBranchType b_typ)
    {
        switch(b_typ)
        {
            case AsmBranchType::LT: return BLT;
            case AsmBranchType::GE: return BGE;
            case AsmBranchType::LE: return BLE;
            case AsmBranchType::GT: return BGT;
            case AsmBranchType::EQ: return BEQ;
            case AsmBranchType::NE: return BNE;
            default: return B;
        }
    }
};

class AsmCode
{
public:
    string label, comment;
    AsmInst inst;
    vector<Param> params;
    int indent = 0;
public:
    AsmCode(AsmInst::InstType _i_typ,
            const string &_label = "",
            const string &_comment = "",
            int _indent = 0):
                inst(_i_typ),
                label(_label),
                comment(_comment),
                indent(_indent),
                params(vector<Param>{}) {}
    AsmCode(AsmInst::InstType _i_typ,
            const string &_label,
            const vector<Param> &_params,
            const string &_comment,
            int _indent = 0):
            inst(_i_typ), 
            label(_label),
            params(_params),
            comment(_comment),
            indent(_indent) {}
    AsmCode(AsmInst::InstType _i_typ,
            const string &_label,
            const vector<Param> &_params,
            int _indent = 0):
            inst(_i_typ), 
            label(_label),
            params(_params),
            indent(_indent) {}
    AsmCode(AsmInst::InstType _i_typ,
            const vector<Param> &_params,
            int _indent = 0):
            inst(_i_typ), 
            params(_params),
            indent(_indent) {}
    string ToString()
    {
        stringstream ss;
        if (indent > 0)
            ss << get_tabs(indent);
        if (!label.empty())
            ss << label << ": ";
        ss << inst.ToString();
        if (!params.empty())
        {
            ss << ' ' << params.front().ToString();
            for (auto it = ++params.begin(); it != params.end(); ++it)
                if (inst.i_typ == AsmInst::STR || inst.i_typ == AsmInst::VSTR ||
                ((inst.i_typ == AsmInst::LDR || inst.i_typ == AsmInst::VLDR) && it->p_typ == Param::Reg))
                    ss << ", [" << it->ToString() << ']';
                else
                    ss << ", " << it->ToString();
        }
        if (!comment.empty())
            ss << " // " << comment;
        return ss.str();
    }
};

string GetRRegList(const vector<REGs> &regs)
{
    string reglist("{");
    for (auto &&reg : regs)
    {
        if (reg >= s0) continue; // skip s registers
        Param p(reg);
        reglist += p.ToString() + ", ";
    }
    if (reglist == "{") return "";
    reglist.pop_back();
    reglist.pop_back();
    reglist += "}";
    return reglist;
}

vector<string> GetSRegLists(const vector<REGs> &regs)
{
    vector<string> res;
    int idx = 0, siz = regs.size();
    if (siz == 0) return res;
    while(idx < siz)
    {
        string reglist("{");
        int cnt = 0;
        for (;cnt < 16 && idx < siz; ++idx)
        {
            if (regs[idx] < s0) continue; // skip r registers
            Param p(regs[idx]);
            reglist += p.ToString() + ", ";
            ++cnt;
        }
        if (cnt == 0) break;
        reglist.pop_back();
        reglist.pop_back();
        reglist += "}";
        res.push_back(reglist);
    }
    return res;
}

void AddAsmCodePushRegisters(vector<AsmCode> &asm_insts, const vector<REGs> &regs, int indent)
{
    auto &&cnt = regs.size();
    if (cnt == 0) return;
    string r_reg_list = GetRRegList(regs);
    vector<string> s_reg_lists = GetSRegLists(regs);
    if (!r_reg_list.empty())
        asm_insts.push_back(AsmCode(AsmInst::PUSH, {Param(Param::Str, r_reg_list.c_str())}, indent));
    for (auto &&s_reg_list : s_reg_lists)
        asm_insts.push_back(AsmCode(AsmInst::VPUSH, {Param(Param::Str, s_reg_list.c_str())}, indent));
    if (cnt % 2 != 0)
        asm_insts.push_back(AsmCode(AsmInst::SUB, {Param(sp), Param(sp), Param(4)}, indent));
}

void AddAsmCodePopRegisters(vector<AsmCode> &asm_insts, const vector<REGs> &regs, int indent)
{
    auto &&cnt = regs.size();
    if (cnt == 0) return;
    if (cnt % 2 != 0)
        asm_insts.push_back(AsmCode(AsmInst::ADD, {Param(sp), Param(sp), Param(4)}, indent));
    string r_reg_list = GetRRegList(regs);
    vector<string> s_reg_lists = GetSRegLists(regs);
    for (auto &&it = s_reg_lists.rbegin(); it != s_reg_lists.rend(); ++it)
        asm_insts.push_back(AsmCode(AsmInst::VPOP, {Param(Param::Str, it->c_str())}, indent));
    if (!r_reg_list.empty())
        asm_insts.push_back(AsmCode(AsmInst::POP, {Param(Param::Str, r_reg_list.c_str())}, indent));
}

void AddAsmCodeMoveIntToRegister(vector<AsmCode> &asm_insts, REGs r, int i, int indent, AsmInst::InstType conditional_move = AsmInst::MOV)
{
    if (IsSReg(r))
    {
        asm_insts.push_back(AsmCode(AsmInst::VLDR,
            {Param(r), Param(Param::Addr, std::to_string(i).c_str())},
            indent));
        return;
    }
    // conditional move will only move '1' or '0' into the register,
    // so it'll always fall this 'if'.
    if (i >= -257 && i <= 256)
        asm_insts.push_back(AsmCode(conditional_move,
            {Param(r), Param(i)},
            indent));
    else
    {
        // low 16 bits
        if ((i & 0xffff) <= 0xff)
            asm_insts.push_back(AsmCode(AsmInst::MOV,
                {Param(r), Param(i & 0xff)},
                indent));
        else
            asm_insts.push_back(AsmCode(AsmInst::MOVW,
                {Param(r), Param((uint16_t)(i))},
                indent));
        // high 16 bits
        if (i >> 16)
            asm_insts.push_back(AsmCode(AsmInst::MOVT,
                {Param(r), Param((uint16_t)(i >> 16))},
                indent));
    }
}

void AddAsmCodeMoveRegisterToRegister(vector<AsmCode> &asm_insts, REGs reg1, REGs reg2, int indent, bool negative = false)
{
    assert(reg1 != SPILL && reg2 != SPILL);
    if (reg1 == reg2) return;
    if (IsRReg(reg1) && IsRReg(reg2))
        asm_insts.push_back(AsmCode(negative ? AsmInst::MVN : AsmInst::MOV, {Param(reg1), Param(reg2)}, indent));
    else
        asm_insts.push_back(AsmCode(negative ? AsmInst::VMVN : AsmInst::VMOV, {Param(reg1), Param(reg2)}, indent));
}

void AddAsmCodeSwapRegisters(vector<AsmCode> &asm_insts, REGs reg1, REGs reg2, int indent)
{
    if (reg1 == reg2) return;
    if (IsRReg(reg1) && IsRReg(reg2))
    {
        asm_insts.push_back(AsmCode(AsmInst::CMP, {Param(reg1), Param(reg2)}, indent)); //if (x0 == x1) do not swap
        asm_insts.push_back(AsmCode(AsmInst::EORNE, {Param(reg1), Param(reg1), Param(reg2)}, indent)); //eor x0, x0, x1
        asm_insts.push_back(AsmCode(AsmInst::EORNE, {Param(reg2), Param(reg1), Param(reg2)}, indent)); //eor x1, x0, x1
        asm_insts.push_back(AsmCode(AsmInst::EORNE, {Param(reg1), Param(reg1), Param(reg2)}, indent)); //eor x0, x0, x1
    }
    else
    {
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(TEMP_S_REGISTER_1), Param(reg1)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(reg1), Param(reg2)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(reg2), Param(TEMP_S_REGISTER_1)}, indent));
    }
}

void AddAsmCodeAddSub(vector<AsmCode> &asm_insts, AsmInst::InstType _i_typ, REGs r, const Param &src1, const Param &src2, int indent)
{
    // 事实上，由于ssa的性质，bin r0,r0,#imm永远不会出现在ir中
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_int && src2.val.i == 0)
    {
        AddAsmCodeMoveRegisterToRegister(asm_insts, r, src1.val.r, indent);
        return;
    }
    if (src2.p_typ == Param::Reg) assert(IsRReg(src2.val.r));
    assert(IsRReg(r));
    // assumes src2 is smaller (when possible, e.g. add)
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Reg) // both reg
    {
        if (IsSReg(src1.val.r))
        {
            AddAsmCodeMoveRegisterToRegister(asm_insts, r, src1.val.r, indent);
            asm_insts.push_back(AsmCode(_i_typ, {Param(r), Param(r), src2}, indent));
        }
        else asm_insts.push_back(AsmCode(_i_typ, {Param(r), src1, src2}, indent));
    }
    else if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_int) // src1 is reg, src2 is ctv
    {
        Param p1;
        if (IsSReg(src1.val.r))
        {
            p1 = Param(r);
            AddAsmCodeMoveRegisterToRegister(asm_insts, r, src1.val.r, indent);
        }
        else p1 = src1;
        for (int i = 0, mask = 0xff; i < 3; ++i, mask <<= 8)
        {
            int chr = src2.val.i & mask;
            if (chr) asm_insts.push_back(AsmCode(_i_typ, {Param(r), p1, Param(chr)}, indent));
        }
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg) // src1 is ctv, src2 is reg, should be sub only
    {
        assert(_i_typ == AsmInst::SUB);
        for (int i = 0, mask = 0xff; i < 3; ++i, mask <<= 8)
        {
            int chr = src1.val.i & mask;
            if (chr) asm_insts.push_back(AsmCode(AsmInst::RSB, {Param(r), src2, Param(chr)}, indent));
        }
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Imm_int)// both ctv
    {
        if (_i_typ == AsmInst::ADD)
            AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i + src2.val.i, indent);
        else
            AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i - src2.val.i, indent);
    }
}

void AddAsmCodeMulDiv(vector<AsmCode> &asm_insts, AsmInst::InstType _i_typ, REGs r, const Param &src1, const Param &src2, int indent)
{
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_int && src2.val.i == 1
     || src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg && src1.val.i == 1 && _i_typ == AsmInst::MUL)
    {
        AddAsmCodeMoveRegisterToRegister(asm_insts, r, (src2.p_typ == Param::Imm_int) ? src1.val.r : src2.val.r, indent);
        return;
    }
    if (src2.p_typ == Param::Reg) assert(IsRReg(src2.val.r));
    assert(IsRReg(r));
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Reg) // both reg
    {
        if (IsSReg(src1.val.r))
        {
            AddAsmCodeMoveRegisterToRegister(asm_insts, r, src1.val.r, indent);
            asm_insts.push_back(AsmCode(_i_typ, {Param(r), Param(r), src2}, indent));
        }
        else asm_insts.push_back(AsmCode(_i_typ, {Param(r), src1, src2}, indent));
    }
    else if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_int) // src1 is reg, src2 is ctv
    {
        assert(IsRReg(src1.val.r));
        AddAsmCodeMoveIntToRegister(asm_insts, r, src2.val.i, indent);
        asm_insts.push_back(AsmCode(_i_typ, {Param(r), src1, Param(r)}, indent));
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg) // src1 is ctv, src2 is reg
    {
        AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i, indent);
        asm_insts.push_back(AsmCode(_i_typ, {Param(r), Param(r), src2}, indent));
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Imm_int)// both ctv
    {
        if (_i_typ == AsmInst::MUL)
            AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i * src2.val.i, indent);
        else AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i / src2.val.i, indent);
    }
}

void AddAsmCodeRem(vector<AsmCode> &asm_insts, Inst *instPtr, REGs r, const Param &src1, const Param &src2, int indent)
{
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Reg) // both reg
    {
        asm_insts.push_back(AsmCode(AsmInst::SDIV, {Param(r), src1, src2}, indent));
        asm_insts.push_back(AsmCode(AsmInst::MUL, {Param(r), Param(r), src2}, indent));
        asm_insts.push_back(AsmCode(AsmInst::SUB, {Param(r), src1, Param(r)}, indent));
    }
    else if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_int) // src1 is reg, src2 is ctv
    {
        auto &&imm = src2.val.i;
        // 该情况需要借用寄存器
        DECLEAR_BORROW_PUSH(instPtr, REM_REGISTER)
        AddAsmCodeMoveIntToRegister(asm_insts,
            IF_BORROW_USE_X_OR_USE_FIRST_AVAIL_REG(borrow,
            REM_REGISTER,
            instPtr), imm, indent);
        asm_insts.push_back(AsmCode(AsmInst::SDIV, {Param(r), src1,
            IF_BORROW_USE_X_OR_USE_FIRST_AVAIL_REG(borrow,
            REM_REGISTER,
            instPtr)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::MUL, {Param(r), Param(r), 
            IF_BORROW_USE_X_OR_USE_FIRST_AVAIL_REG(borrow,
            REM_REGISTER,
            instPtr)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::SUB, {Param(r), src1, Param(r)}, indent));
        DECLEAR_BORROW_POP(REM_REGISTER)
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg) // src1 is ctv, src2 is reg
    {
        auto &&imm = src1.val.i;
        AddAsmCodeMoveIntToRegister(asm_insts, r, imm, indent);
        asm_insts.push_back(AsmCode(AsmInst::SDIV, {Param(r), Param(r), src2}, indent));
        asm_insts.push_back(AsmCode(AsmInst::MUL, {src2, Param(r), src2}, indent));
        AddAsmCodeMoveIntToRegister(asm_insts, r, imm, indent);
        asm_insts.push_back(AsmCode(AsmInst::SUB, {Param(r), Param(r), src2}, indent));
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Imm_int)// both ctv
        AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i % src2.val.i, indent);
}

void AddAsmCodeComment(vector<AsmCode> &asm_insts, const string& comment, int indent)
{
    asm_insts.push_back(AsmCode(AsmInst::EMPTY, "", comment, indent));
}

void AddAsmCodeCmp(vector<AsmCode> &asm_insts, Inst *instPtr, RelOp op, const Param &src1, const Param &src2, int indent)
{
    bool reverse = false;
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Reg) // both reg
        asm_insts.push_back(AsmCode(AsmInst::CMP,
            {   src1,
                src2},
            indent));
    else if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_int) // src1 is reg, src2 is ctv
    {
        assert(IsOperand2(src2.val.i));
        asm_insts.push_back(AsmCode(AsmInst::CMP, {src1, src2}, indent));
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg) // src1 is ctv, src2 is reg
    {
        assert(IsOperand2(src1.val.i));
        asm_insts.push_back(AsmCode(AsmInst::CMP, {src2, src1}, indent));
        reverse = true;
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Imm_int)// both ctv
    {
        switch (op)
        {
        case RelOp::LTH: b_type = (src1.val.i < src2.val.i) ? AlwaysTrue : AlwaysFalse; return;
        case RelOp::LEQ: b_type = (src1.val.i <= src2.val.i) ? AlwaysTrue : AlwaysFalse; return;
        case RelOp::EQU: b_type = (src1.val.i == src2.val.i) ? AlwaysTrue : AlwaysFalse; return;
        case RelOp::NEQ: b_type = (src1.val.i != src2.val.i) ? AlwaysTrue : AlwaysFalse; return;
        }
    }
    switch (op)
    {
    case RelOp::LTH: b_type = LT; break;
    case RelOp::LEQ: b_type = LE; break;
    case RelOp::EQU: b_type = EQ; break;
    case RelOp::NEQ: b_type = NE; break;
    }
    if (reverse) b_type = REVERSED_BRANCH_TYPE(b_type);
}

void AddAsmCodeFloatBin(vector<AsmCode> &asm_insts, BinOp _bin_typ, REGs r, const Param &src1, const Param &src2, int indent)
{
    AsmInst::InstType _i_typ;
    switch (_bin_typ)
    {
    case BinOp::ADD: _i_typ = AsmInst::VADD; break;
    case BinOp::SUB: _i_typ = AsmInst::VSUB; break;
    case BinOp::MUL: _i_typ = AsmInst::VMUL; break;
    case BinOp::DIV: _i_typ = AsmInst::VDIV; break;
    default:
        assert(false && "codegen: invalid float bin op");
        return;
    }

    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Reg) // both reg
    {
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_1), src1}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_2), src2}, indent));
        asm_insts.push_back(AsmCode(_i_typ, {Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_2)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(r), Param(FLOAT_BINOP_REGISTER_1)}, indent));
    }
    else if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_float) // src1 is reg, src2 is ctv
    {
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_1), src1}, indent));
        AddAsmCodeMoveIntToRegister(asm_insts, r, DoubleToWord(src2.val.f), indent);
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_2), Param(r)}, indent));
        asm_insts.push_back(AsmCode(_i_typ, {Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_2)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(r), Param(FLOAT_BINOP_REGISTER_1)}, indent));
    }
    else if (src1.p_typ == Param::Imm_float && src2.p_typ == Param::Reg) // src1 is ctv, src2 is reg
    {
        AddAsmCodeMoveIntToRegister(asm_insts, r, DoubleToWord(src1.val.f), indent);
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_1), Param(r)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_2), src2}, indent));
        asm_insts.push_back(AsmCode(_i_typ, {Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_2)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(r), Param(FLOAT_BINOP_REGISTER_1)}, indent));
    }
    else if (src1.p_typ == Param::Imm_float && src2.p_typ == Param::Imm_float)// both ctv
    {
        switch (_bin_typ)
        {
        case BinOp::ADD: AddAsmCodeMoveIntToRegister(asm_insts, r, DoubleToWord(src1.val.f + src2.val.f), indent); break;
        case BinOp::SUB: AddAsmCodeMoveIntToRegister(asm_insts, r, DoubleToWord(src1.val.f - src2.val.f), indent); break;
        case BinOp::MUL: AddAsmCodeMoveIntToRegister(asm_insts, r, DoubleToWord(src1.val.f * src2.val.f), indent); break;
        case BinOp::DIV: AddAsmCodeMoveIntToRegister(asm_insts, r, DoubleToWord(src1.val.f / src2.val.f), indent); break;
        default:
            assert(false && "codegen: invalid float bin op");
            return;
        }
    }
}

void AddAsmCodeFloatCmp(vector<AsmCode> &asm_insts, RelOp op, const Param &src1, const Param &src2, int indent)
{
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Reg) // both reg
    {
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_1), src1}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_2), src2}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VCMP, {Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_2)}, indent));
    }
    else if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_float) // src1 is reg, src2 is ctv
    {
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_1), src1}, indent)); // 存src1原始值
        AddAsmCodeMoveIntToRegister(asm_insts, src1.val.r, DoubleToWord(src2.val.f), indent);        // 借用src1
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_2), src1}, indent)); // 借用src1
        asm_insts.push_back(AsmCode(AsmInst::VCMP, {Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_2)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {src1, Param(FLOAT_BINOP_REGISTER_1)}, indent)); // 取src1原始值
    }
    else if (src1.p_typ == Param::Imm_float && src2.p_typ == Param::Reg) // src1 is ctv, src2 is reg
    {
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_2), src2}, indent)); // 存src2原始值
        AddAsmCodeMoveIntToRegister(asm_insts, src2.val.r, DoubleToWord(src1.val.f), indent);        // 借用src2
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_1), src2}, indent)); // 借用src2
        asm_insts.push_back(AsmCode(AsmInst::VCMP, {Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_2)}, indent));
        asm_insts.push_back(AsmCode(AsmInst::VMOV, {src2, Param(FLOAT_BINOP_REGISTER_2)}, indent)); // 取src2原始值
    }
    else if (src1.p_typ == Param::Imm_float && src2.p_typ == Param::Imm_float)// both ctv
    {
        switch (op)
        {
        case RelOp::LTH: b_type = (src1.val.f < src2.val.f) ? AlwaysTrue : AlwaysFalse; return;
        case RelOp::LEQ: b_type = (src1.val.f <= src2.val.f) ? AlwaysTrue : AlwaysFalse; return;
        case RelOp::EQU: b_type = (src1.val.f == src2.val.f) ? AlwaysTrue : AlwaysFalse; return;
        case RelOp::NEQ: b_type = (src1.val.f != src2.val.f) ? AlwaysTrue : AlwaysFalse; return;
        }
    }
    switch (op)
    {
    case RelOp::LTH: b_type = LT; break;
    case RelOp::LEQ: b_type = LE; break;
    case RelOp::EQU: b_type = EQ; break;
    case RelOp::NEQ: b_type = NE; break;
    }
}

void AddAsmCodeLoadFromVarName(vector<AsmCode> &asm_insts, REGs r, const char *varname, int indent)
{
    assert(r != SPILL);
    if (IsRReg(r))
        asm_insts.push_back(AsmCode(AsmInst::LDR, {Param(r), Param(Param::Addr, varname)}, indent));
    else
        asm_insts.push_back(AsmCode(AsmInst::VLDR, {Param(r), Param(Param::Addr, varname)}, indent));
}

void AddAsmCodeStoreToRegisterPointer(vector<AsmCode> &asm_insts, REGs reg1, REGs reg2, int indent)
{
    assert(reg1 != SPILL && IsRReg(reg2));
    if (IsRReg(reg1))
        asm_insts.push_back(AsmCode(AsmInst::STR, {Param(reg1), Param(reg2)}, indent));
    else
        asm_insts.push_back(AsmCode(AsmInst::VSTR, {Param(reg1), Param(reg2)}, indent));
}

void AddAsmCodeLoadFromPointer(vector<AsmCode> &asm_insts, REGs reg1, REGs reg2, int indent)
{
    assert(reg1 != SPILL && IsRReg(reg2));
    if (IsRReg(reg1))
        asm_insts.push_back(AsmCode(AsmInst::LDR, {Param(reg1), Param(reg2)}, indent));
    else
        asm_insts.push_back(AsmCode(AsmInst::VLDR, {Param(reg1), Param(reg2)}, indent));
}

void AddAsmCodeFromLLIR(vector<AsmCode> &asm_insts, Function *funcPtr, Inst *instPtr, int indent)
{
    auto &&funcname = funcPtr->func_info.func_name;
    /**/Case (LLIR_RET, ret_inst, instPtr)
    {
        //cout << get_tabs() << ret_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, ret_inst->ToString(), indent);

        // return src

        // if has return value, move it to r0
        if (ret_inst->has_retvalue)
        {
            if (ret_inst->ret_value.ctv) // ctvalue
                AddAsmCodeMoveIntToRegister(asm_insts, r0, CTV_TO_WORD(ret_inst->ret_value.ctv), indent);
            else // virtreg
                if (r0 != GET_ALLOCATION_RESULT(funcPtr, ret_inst->ret_value.reg->reg_id))
                    AddAsmCodeMoveRegisterToRegister(asm_insts, r0, GET_ALLOCATION_RESULT(funcPtr, ret_inst->ret_value.reg->reg_id), indent);
        }

        asm_insts.push_back(AsmCode(AsmInst::B, {Param(Param::Str, GET_FUNC_RETURN_NAME(funcPtr))}, indent));
        
        // insert ltorg
        asm_insts.push_back(AsmCode(AsmInst::DOT_LTORG, "", "", indent));
    }
    /**/Case (LLIR_STORE, store_inst, instPtr)
    {
        //cout << get_tabs() << store_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, store_inst->ToString(), indent);

        // *dst = src ; str src [dst]
        // 解决SRC问题
        REGs src_reg, addr_reg;
        bool src_got_first = false, borrow_addr = false;

        if (store_inst->src.ctv) // 如果src是常量，它就没有被分配虚拟寄存器，需要借用
        {
            // 解决src_reg，这里肯定要借用一个r_reg或者s_reg
            if (instPtr->GetFirstUnusedRRegister() != SPILL) // 有空闲的r寄存器，用空闲的r寄存器
            {
                src_got_first = true;
                src_reg = instPtr->GetFirstUnusedRRegister();
            }
            else // 借用s2，不需要压栈
                src_reg = TEMP_S_REGISTER_1;
            AddAsmCodeMoveIntToRegister(asm_insts, src_reg, CTV_TO_WORD(store_inst->src.ctv), indent);

            // dst是从gep来的，是数组元素指针，dst有分配寄存器，但是如果是s_reg的话需要先mov进r_reg
            if (store_inst->dst.reg->is_from_gep || store_inst->dst.reg->type.is_args && store_inst->dst.reg->type.is_array)
            {
                // 先看看dst_reg是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
                if (IsSReg(GET_ALLOCATION_RESULT(funcPtr, store_inst->dst.reg->reg_id)))
                {
                    // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
                    REGs unused_reg = src_got_first ? instPtr->GetSecondUnusedRRegister() : instPtr->GetFirstUnusedRRegister();
                    if (unused_reg != SPILL) // 有空闲的r寄存器，用空闲的
                        addr_reg = unused_reg;
                    else // 借用其他r寄存器，需要压栈
                    {
                        borrow_addr = true;
                        // 不能跟src分配的寄存器冲突
                        if (src_reg == r0)
                            addr_reg = r1;
                        else addr_reg = r0;
                        AddAsmCodePushRegisters(asm_insts, {addr_reg}, indent);
                    }
                    AddAsmCodeMoveRegisterToRegister(asm_insts, addr_reg, GET_ALLOCATION_RESULT(funcPtr, store_inst->dst.reg->reg_id), indent);
                }
                else
                    addr_reg = GET_ALLOCATION_RESULT(funcPtr, store_inst->dst.reg->reg_id);
            }
            else // dst是从alloca来的变量（非数组），没有被分配寄存器，需要借用其他寄存器解除引用
            {
                // 给dst借用一个r_reg
                REGs unused_reg = src_got_first ? instPtr->GetSecondUnusedRRegister() : instPtr->GetFirstUnusedRRegister();
                if (unused_reg != SPILL) // 有空闲的r寄存器，用空闲的
                    addr_reg = unused_reg;
                else // 借用其他r寄存器，需要压栈
                {
                    borrow_addr = true;
                    // 不能跟src分配的寄存器冲突
                    if (src_reg == r0)
                        addr_reg = r1;
                    else addr_reg = r0;
                    AddAsmCodePushRegisters(asm_insts, {addr_reg}, indent);
                }
                AddAsmCodeLoadFromVarName(asm_insts, addr_reg, GET_VAR_NAME(funcPtr, store_inst->dst.reg), indent);
            }
        }
        else // src是虚拟寄存器
        {
            src_reg = GET_ALLOCATION_RESULT(funcPtr, store_inst->src.reg->reg_id);

            // dst是从gep来的，是数组元素指针，dst有分配寄存器
            if (store_inst->dst.reg->is_from_gep || store_inst->dst.reg->type.is_args && store_inst->dst.reg->type.is_array)
            {
                // 先看看dst_reg是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
                if (IsSReg(GET_ALLOCATION_RESULT(funcPtr, store_inst->dst.reg->reg_id)))
                {
                    // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
                    if (instPtr->GetFirstUnusedRRegister() != SPILL) // 有空闲的r寄存器，用空闲的
                        addr_reg = instPtr->GetFirstUnusedRRegister();
                    else // 借用其他r寄存器，需要压栈
                    {
                        borrow_addr = true;
                        // 不能跟src分配的寄存器冲突
                        if (src_reg == r0)
                            addr_reg = r1;
                        else addr_reg = r0;
                        AddAsmCodePushRegisters(asm_insts, {addr_reg}, indent);
                    }
                    AddAsmCodeMoveRegisterToRegister(asm_insts, addr_reg, GET_ALLOCATION_RESULT(funcPtr, store_inst->dst.reg->reg_id), indent);
                }
                else
                    addr_reg = GET_ALLOCATION_RESULT(funcPtr, store_inst->dst.reg->reg_id);
            }
            else // dst是从alloca来的变量（非数组），没有被分配寄存器，需要借用其他寄存器解除引用
            {
                // 给dst借用一个r_reg
                if (instPtr->GetFirstUnusedRRegister() != SPILL) // 有空闲的r寄存器，用空闲的
                    addr_reg = instPtr->GetFirstUnusedRRegister();
                else // 借用其他r寄存器，需要压栈
                {
                    borrow_addr = true;
                    // 不能跟src分配的寄存器冲突
                    if (src_reg == r0)
                        addr_reg = r1;
                    else addr_reg = r0;
                    AddAsmCodePushRegisters(asm_insts, {addr_reg}, indent);
                }
                AddAsmCodeLoadFromVarName(asm_insts, addr_reg, GET_VAR_NAME(funcPtr, store_inst->dst.reg), indent);
            }
        }
        AddAsmCodeStoreToRegisterPointer(asm_insts, src_reg, addr_reg, indent);
        if (borrow_addr) AddAsmCodePopRegisters(asm_insts, {addr_reg}, indent);
    }
    /**/Case (LLIR_LOAD, load_inst, instPtr)
    {
        // 跳过目的寄存器没有分配的语句（这种情况是load的结果没有被使用）
        if(CONDITION_REGISTER_NOT_ALLOCATED(funcPtr, load_inst->dst.reg->reg_id))
            return;

        //cout << get_tabs() << load_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, load_inst->ToString(), indent);
        
        REGs addr_reg;
        bool borrow = false;
        // 先看看dst_reg是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
        if (IsSReg(GET_ALLOCATION_RESULT(funcPtr, load_inst->dst.reg->reg_id)))
        {
            // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
            if (instPtr->GetFirstUnusedRRegister() != SPILL) // 有空闲的r寄存器，用空闲的
                addr_reg = instPtr->GetFirstUnusedRRegister();
            else // 借用其他r寄存器，需要压栈
            {
                borrow = true;
                // 不能跟src分配的寄存器冲突
                if (GET_ALLOCATION_RESULT(funcPtr, load_inst->src.reg->reg_id) == r0)
                    addr_reg = r1;
                else addr_reg = r0;
                AddAsmCodePushRegisters(asm_insts, {addr_reg}, indent);
            }
        }
        else
            addr_reg = GET_ALLOCATION_RESULT(funcPtr, load_inst->dst.reg->reg_id);
        // dst = *src
        // alloc来的数组首指针不会直接load，肯定要经过gep转换为数组元素指针
        // src是从gep来的，是数组元素指针，src有分配寄存器
        if (load_inst->src.reg->is_from_gep || load_inst->src.reg->type.is_args && load_inst->src.reg->type.is_array)
            AddAsmCodeMoveRegisterToRegister(asm_insts, addr_reg, GET_ALLOCATION_RESULT(funcPtr, load_inst->src.reg->reg_id), indent);
        else // src是从alloca来的变量（非数组），没有被分配寄存器
            AddAsmCodeLoadFromVarName(asm_insts, addr_reg, GET_VAR_NAME(funcPtr, load_inst->src.reg), indent);
        // 解除引用
        AddAsmCodeLoadFromPointer(asm_insts, GET_ALLOCATION_RESULT(funcPtr, load_inst->dst.reg->reg_id), addr_reg, indent);
        if (borrow) AddAsmCodePopRegisters(asm_insts, {addr_reg}, indent);
    }
    /*unfinished*/Case (LLIR_BIN, bin_inst, instPtr)
    {
        // 跳过目的寄存器没有分配的语句（这种情况是只二元运算的结果没有被使用）
        if(CONDITION_REGISTER_NOT_ALLOCATED(funcPtr, bin_inst->dst.reg->reg_id))
            return;

        //cout << get_tabs() << bin_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, bin_inst->ToString(), indent);

        // dst = src1 + src2
        Param src1, src2;
        if (bin_inst->src1.ctv)
            src1 = Param(bin_inst->src1.ctv->int_value);
        else
            src1 = Param(GET_ALLOCATION_RESULT(funcPtr, bin_inst->src1.reg->reg_id));
        if (bin_inst->src2.ctv)
            src2 = Param(bin_inst->src2.ctv->int_value);
        else
            src2 = Param(GET_ALLOCATION_RESULT(funcPtr, bin_inst->src2.reg->reg_id));
        if (bin_inst->op == BinOp::ADD && src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg)
            std::swap(src1, src2);
        if (bin_inst->op == BinOp::MUL && src1.p_typ == Param::Reg && IsSReg(src1.val.r) && src2.p_typ == Param::Imm_int)
            std::swap(src1, src2);
        
        bool borrow_dst = false, borrow_src2 = false, dst_got_first = false;
        REGs dst_reg, src2_reg;
        // 先看看dst是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
        if (IsSReg(GET_ALLOCATION_RESULT(funcPtr, bin_inst->dst.reg->reg_id)))
        {
            // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
            if (instPtr->GetFirstUnusedRRegister() != SPILL) // 有空闲的r寄存器，用空闲的
            {
                dst_got_first = true;
                dst_reg = instPtr->GetFirstUnusedRRegister();
            }
            else // 借用其他r寄存器，需要压栈
            {
                borrow_dst = true;
                dst_reg = r0;
                // 不能跟src分配的寄存器冲突
                while (bin_inst->src1.reg && GET_ALLOCATION_RESULT(funcPtr, bin_inst->src1.reg->reg_id) == dst_reg ||
                       bin_inst->src2.reg && GET_ALLOCATION_RESULT(funcPtr, bin_inst->src2.reg->reg_id) == dst_reg)
                    dst_reg = (REGs)(dst_reg + 1);
                AddAsmCodePushRegisters(asm_insts, {dst_reg}, indent);
            }
        }
        else
            dst_reg = GET_ALLOCATION_RESULT(funcPtr, bin_inst->dst.reg->reg_id);
        // 先看看src2是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
        if (src2.p_typ == Param::Reg && IsSReg(src2.val.r))
        {
            // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
            REGs unused_reg = dst_got_first ? instPtr->GetSecondUnusedRRegister() : instPtr->GetFirstUnusedRRegister();
            if (unused_reg != SPILL) // 有空闲的r寄存器，用空闲的
                src2_reg = unused_reg;
            else // 借用其他r寄存器，需要压栈
            {
                borrow_src2 = true;
                src2_reg = r0;
                // 不能跟src分配的寄存器冲突
                while (bin_inst->src1.reg && GET_ALLOCATION_RESULT(funcPtr, bin_inst->src1.reg->reg_id) == src2_reg ||
                       dst_reg == src2_reg)
                    src2_reg = (REGs)(src2_reg + 1);
                AddAsmCodePushRegisters(asm_insts, {src2_reg}, indent);
            }
            AddAsmCodeMoveRegisterToRegister(asm_insts, src2_reg, src2.val.r, indent);
            src2.val.r = src2_reg;
        }
        // 除法src1为s_reg且src2为imm时的特殊处理
        else if (bin_inst->op == BinOp::DIV && src1.p_typ == Param::Reg && IsSReg(src1.val.r) && src2.p_typ == Param::Imm_int)
        {
            // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
            REGs unused_reg = dst_got_first ? instPtr->GetSecondUnusedRRegister() : instPtr->GetFirstUnusedRRegister();
            if (unused_reg != SPILL) // 有空闲的r寄存器，用空闲的
                src2_reg = unused_reg;
            else // 借用其他r寄存器，需要压栈
            {
                borrow_src2 = true;
                src2_reg = r0;
                // 不能跟src分配的寄存器冲突
                while (dst_reg == src2_reg)
                    src2_reg = (REGs)(src2_reg + 1);
                AddAsmCodePushRegisters(asm_insts, {src2_reg}, indent);
            }
            AddAsmCodeMoveRegisterToRegister(asm_insts, src2_reg, src1.val.r, indent);
            src1.val.r = src2_reg;
        }

        switch(bin_inst->op)
        {
        case BinOp::ADD:
            AddAsmCodeAddSub(asm_insts, AsmInst::ADD, dst_reg, src1, src2, indent);
            break;
        case BinOp::SUB:
            AddAsmCodeAddSub(asm_insts, AsmInst::SUB, dst_reg, src1, src2, indent);
            break;
        case BinOp::MUL:
            AddAsmCodeMulDiv(asm_insts, AsmInst::MUL, dst_reg, src1, src2, indent);
            break;
        case BinOp::DIV:
            AddAsmCodeMulDiv(asm_insts, AsmInst::SDIV, dst_reg, src1, src2, indent);
            break;
        case BinOp::REM:
            AddAsmCodeRem(asm_insts, instPtr, dst_reg, src1, src2, indent);
            break;
        default:
            break;
        }

        if (borrow_src2)
            AddAsmCodePopRegisters(asm_insts, {src2_reg}, indent);

        if (borrow_dst)
        {
            AddAsmCodeMoveRegisterToRegister(asm_insts, GET_ALLOCATION_RESULT(funcPtr, bin_inst->dst.reg->reg_id), dst_reg, indent);
            AddAsmCodePopRegisters(asm_insts, {dst_reg}, indent);
        }
    }
    /**/Case (LLIR_GEP, gep_inst, instPtr)
    {
        //cout << get_tabs() << gep_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, gep_inst->ToString(), indent);
        
        // dst = *(src + off)
        REGs addr_reg, offs_reg;
        bool borrow_addr = false, borrow_offs = false, addr_got_first = false;
        // 先看看dst_reg是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
        if (IsSReg(GET_ALLOCATION_RESULT(funcPtr, gep_inst->dst.reg->reg_id)))
        {
            // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
            if (instPtr->GetFirstUnusedRRegister() != SPILL) // 有空闲的r寄存器，用空闲的
            {
                addr_reg = instPtr->GetFirstUnusedRRegister();
                addr_got_first = true;
            }
            else // 借用其他r寄存器，需要压栈
            {
                borrow_addr = true;
                // 不能跟src或者off分配的寄存器冲突
                addr_reg = r0;
                while (GET_ALLOCATION_RESULT(funcPtr, gep_inst->src.reg->reg_id) == addr_reg
                  || (gep_inst->off.reg && GET_ALLOCATION_RESULT(funcPtr, gep_inst->off.reg->reg_id) == addr_reg))
                    addr_reg = (REGs)(addr_reg + 1);
                AddAsmCodePushRegisters(asm_insts, {addr_reg}, indent);
            }
        }
        else
            addr_reg = GET_ALLOCATION_RESULT(funcPtr, gep_inst->dst.reg->reg_id);

        bool offset_is_zero = false;

        // src是从gep来的，直接是元素指针
        if (gep_inst->src.reg->is_from_gep || gep_inst->src.reg->type.is_args && gep_inst->src.reg->type.is_array)
            AddAsmCodeMoveRegisterToRegister(asm_insts, addr_reg, GET_ALLOCATION_RESULT(funcPtr, gep_inst->src.reg->reg_id), indent);
        else // src是从alloca来的，没有被分配寄存器，需要分配一下寄存器(直接用dst)，再把指针load进来，此时offset一定为0
        {
            AddAsmCodeLoadFromVarName(asm_insts, addr_reg, GET_PTR_NAME(funcPtr, gep_inst->src.reg), indent);
            AddAsmCodeLoadFromPointer(asm_insts, GET_ALLOCATION_RESULT(funcPtr, gep_inst->dst.reg->reg_id), addr_reg, indent);
            if (borrow_addr) AddAsmCodePopRegisters(asm_insts, {addr_reg}, indent);
            return;
        }

        if (gep_inst->off.ctv && gep_inst->off.ctv->int_value == 0)
            offset_is_zero = true;

        // 加上偏移量
        if (!offset_is_zero)
        {
            int32_t size_shift_bits = 2;
            if (gep_inst->off.ctv) // offset is ctv
            {
                if ((gep_inst->off.ctv->int_value << size_shift_bits) <= 256) // 偏移量<256，直接加
                {
                    AddAsmCodeAddSub(asm_insts, AsmInst::ADD, addr_reg, addr_reg, Param(gep_inst->off.ctv->int_value << size_shift_bits), indent);
                    if (borrow_addr) AddAsmCodePopRegisters(asm_insts, {addr_reg}, indent);
                    return;
                }
                // 偏移量大于256
                // 给off借用一个r_reg
                REGs unused_reg = addr_got_first ? instPtr->GetSecondUnusedRRegister() : instPtr->GetFirstUnusedRRegister();
                if (unused_reg != SPILL) // 有空闲的r寄存器，用空闲的
                    offs_reg = unused_reg;  
                else // 借用其他r寄存器，需要压栈
                {
                    borrow_offs = true;
                    // 不能跟src和addr分配的寄存器冲突
                    offs_reg = r0;
                    while (GET_ALLOCATION_RESULT(funcPtr, gep_inst->src.reg->reg_id) == offs_reg
                        ||addr_reg == offs_reg)
                        offs_reg = (REGs)(offs_reg + 1);
                    AddAsmCodePushRegisters(asm_insts, {offs_reg}, indent);
                }
                AddAsmCodeMoveIntToRegister(asm_insts, offs_reg, gep_inst->off.ctv->int_value << size_shift_bits, indent);
                AddAsmCodeAddSub(asm_insts, AsmInst::ADD, addr_reg, addr_reg, offs_reg, indent);
            }
            else // offset is reg
            {
                // 先看看off_reg是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
                if (IsSReg(GET_ALLOCATION_RESULT(funcPtr, gep_inst->off.reg->reg_id)))
                {
                    // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
                    REGs unused_reg = addr_got_first ? instPtr->GetSecondUnusedRRegister() : instPtr->GetFirstUnusedRRegister();
                    if (unused_reg != SPILL) // 有空闲的r寄存器，用空闲的
                        offs_reg = unused_reg;  
                    else // 借用其他r寄存器，需要压栈
                    {
                        borrow_offs = true;
                        // 不能跟src和addr分配的寄存器冲突
                        offs_reg = r0;
                        while (GET_ALLOCATION_RESULT(funcPtr, gep_inst->src.reg->reg_id) == offs_reg
                            ||addr_reg == offs_reg)
                            offs_reg = (REGs)(offs_reg + 1);
                        AddAsmCodePushRegisters(asm_insts, {offs_reg}, indent);
                    }
                    AddAsmCodeMoveRegisterToRegister(asm_insts, offs_reg, GET_ALLOCATION_RESULT(funcPtr, gep_inst->off.reg->reg_id), indent);
                }
                else
                    offs_reg = GET_ALLOCATION_RESULT(funcPtr, gep_inst->off.reg->reg_id);               
                asm_insts.push_back(AsmCode(AsmInst::ADD,
                {   addr_reg,
                    addr_reg,
                    offs_reg,
                    Param(Param::Str, (string("lsl #") + std::to_string(size_shift_bits)).c_str())},
                indent));
            }
        }

        AddAsmCodeMoveRegisterToRegister(asm_insts, GET_ALLOCATION_RESULT(funcPtr, gep_inst->dst.reg->reg_id), addr_reg, indent);
        if (borrow_offs) AddAsmCodePopRegisters(asm_insts, {offs_reg}, indent);
        if (borrow_addr) AddAsmCodePopRegisters(asm_insts, {addr_reg}, indent);
    }
    /**/Case (LLIR_CALL, call_inst, instPtr)
    {
        // cout << get_tabs() << call_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, call_inst->ToString(), indent);
    
        // [dst = ] func(arg1, arg2, ...)
        // 保存现场，只保存活跃的寄存器，不包括返回值所占用的寄存器
        vector<REGs> used_regs;
        for (REGs i = r0; i <= s31; i = (REGs)(i + 1))
        {
            // 跳过特殊寄存器
            if (i > r12 && i < s4)
                continue;
            // 跳过返回值的reg
            if (call_inst->func_info->return_type != TypeVoid && !CONDITION_REGISTER_NOT_ALLOCATED(funcPtr, call_inst->dst.reg->reg_id) && GET_ALLOCATION_RESULT(funcPtr, call_inst->dst.reg->reg_id) == i)
                continue;
            // 如果该寄存器在当前指令不是空闲的，就push
            if (instPtr->availRegs.find(i) == instPtr->availRegs.end())
                used_regs.push_back(i);
        }
        AddAsmCodePushRegisters(asm_insts, used_regs, indent);

        // 移动参数
            // TODO: more than 32 regs
        vector<REGs> registers AVAIL_REGS_INIT_LIST; // 每个“位置”上现在是哪个寄存器
        vector<REGs> should_be(registers); // 每个“位置”上应该是哪个寄存器
        vector<bool> skip_because_is_ctv(registers.size(), false); // 当前“位置”是常数，跳过检查
        vector<int32_t> ctv_to_move_in(registers.size(), 0); // 要移动进去的常数
        for (size_t i = 0, siz = std::min(call_inst->args.size(), (size_t)32); i < siz; ++i)
        {
            auto &&arg = call_inst->args[i];
            if (arg.ctv) // arg is const val
            {
                skip_because_is_ctv[CALL_INST_GET_ARGS_REGISTER_IDX(i)] = true;
                ctv_to_move_in[CALL_INST_GET_ARGS_REGISTER_IDX(i)] = CTV_TO_WORD(arg.ctv);
            }
            else // arg is virtreg
            {
                // 不需要移动
                if (GET_ARGS_REGISTER(i) == GET_ALLOCATION_RESULT(funcPtr, arg.reg->reg_id))
                    continue;
                // 需要移动
                should_be[CALL_INST_GET_ARGS_REGISTER_IDX(i)] = GET_ALLOCATION_RESULT(funcPtr, arg.reg->reg_id);
            }
        }
        // 移动（交换）寄存器
        for (size_t i = 0, siz = std::min(call_inst->args.size(), (size_t)32); i < siz; ++i)
        {
            while (!skip_because_is_ctv[CALL_INST_GET_ARGS_REGISTER_IDX(i)]
            && registers[CALL_INST_GET_ARGS_REGISTER_IDX(i)] != should_be[CALL_INST_GET_ARGS_REGISTER_IDX(i)])
            {
                int idx = 0;
                while (registers[idx] != should_be[CALL_INST_GET_ARGS_REGISTER_IDX(i)] && idx <= 41) // 找到should_be[i]的位置
                    ++idx;
                assert(idx != 41 && "codegen: error in function call generation inst when trying to move args!");
                                                    // 当前处理的位置       // 在idx位置的原来是哪个寄存器
                AddAsmCodeSwapRegisters(asm_insts, GET_ARGS_REGISTER(i), REGs((idx > 12) ? idx + 7 : idx), indent);
                std::swap(registers[idx], registers[CALL_INST_GET_ARGS_REGISTER_IDX(i)]);
            }
        }
        // 移动常数，应该在交换寄存器之后进行
        for (size_t i = 0, siz = std::min(call_inst->args.size(), (size_t)32); i < siz; ++i)
            if (skip_because_is_ctv[CALL_INST_GET_ARGS_REGISTER_IDX(i)])
                AddAsmCodeMoveIntToRegister(
                    asm_insts,
                    GET_ARGS_REGISTER(i),
                    ctv_to_move_in[CALL_INST_GET_ARGS_REGISTER_IDX(i)],
                    indent);
        
        // 特殊处理putfloat
        if (call_inst->func_info->func_name == "putfloat")
            AddAsmCodeMoveRegisterToRegister(asm_insts, s0, r0, indent);

        // 函数调用 
        asm_insts.push_back(AsmCode(AsmInst::BL, 
            {Param(Param::Str, FunctionRename(call_inst->func_info))},
            indent));
        
        // 特殊处理getfloat
        if (call_inst->func_info->func_name == "getfloat")
            AddAsmCodeMoveRegisterToRegister(asm_insts, r0, s0, indent);

        // 移动返回值
        if (call_inst->func_info->return_type != TypeVoid && !CONDITION_REGISTER_NOT_ALLOCATED(funcPtr, call_inst->dst.reg->reg_id))
            if (r0 != GET_ALLOCATION_RESULT(funcPtr, call_inst->dst.reg->reg_id))
                AddAsmCodeMoveRegisterToRegister(asm_insts, GET_ALLOCATION_RESULT(funcPtr, call_inst->dst.reg->reg_id), r0, indent);

        // 恢复现场
        AddAsmCodePopRegisters(asm_insts, used_regs, indent);
    }
    Case (LLIR_ICMP, icmp_inst, instPtr)
    {
        //cout << get_tabs() << icmp_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, icmp_inst->ToString(), indent);

        Param src1, src2;
        if (icmp_inst->src1.ctv)
            src1 = Param(icmp_inst->src1.ctv->int_value);
        else
            src1 = Param(GET_ALLOCATION_RESULT(funcPtr, icmp_inst->src1.reg->reg_id));
        if (icmp_inst->src2.ctv)
            src2 = Param(icmp_inst->src2.ctv->int_value);
        else
            src2 = Param(GET_ALLOCATION_RESULT(funcPtr, icmp_inst->src2.reg->reg_id));
        
        bool borrow_src1 = false, borrow_src2 = false, src1_got_first = false;
        REGs src1_reg, src2_reg;
        // 先看看src1是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
        if (src1.p_typ == Param::Reg && IsSReg(src1.val.r) || src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg && !IsOperand2(src1.val.i))
        {
            // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
            if (instPtr->GetFirstUnusedRRegister() != SPILL) // 有空闲的r寄存器，用空闲的
            {
                src1_got_first = true;
                src1_reg = instPtr->GetFirstUnusedRRegister();
            }
            else // 借用其他r寄存器，需要压栈
            {
                borrow_src1 = true;
                src1_reg = r0;
                // 不能跟src2分配的寄存器冲突
                while (icmp_inst->src2.reg && GET_ALLOCATION_RESULT(funcPtr, icmp_inst->src2.reg->reg_id) == src1_reg)
                    src1_reg = (REGs)(src1_reg + 1);
                AddAsmCodePushRegisters(asm_insts, {src1_reg}, indent);
            }

            // 由于不同的原因借用寄存器，对其初始化的操作也不同
            if (src1.p_typ == Param::Reg && IsSReg(src1.val.r))
                AddAsmCodeMoveRegisterToRegister(asm_insts, src1_reg, src1.val.r, indent);
            else AddAsmCodeMoveIntToRegister(asm_insts, src1_reg, src1.val.i, indent);
            src1.p_typ = Param::Reg;
            src1.val.r = src1_reg;
        }
        // 先看看src2是否是r_reg，如果是的话直接拿来用，如果不是的话需要借用
        if (src2.p_typ == Param::Reg && IsSReg(src2.val.r) || src2.p_typ == Param::Imm_int && src1.p_typ == Param::Reg && !IsOperand2(src2.val.i))
        {
            // 如果需要借用r_reg，先看看在这个指令的位置有没有空闲的r_reg
            REGs unused_reg = src1_got_first ? instPtr->GetSecondUnusedRRegister() : instPtr->GetFirstUnusedRRegister();
            if (unused_reg != SPILL) // 有空闲的r寄存器，用空闲的
                src2_reg = unused_reg;
            else // 借用其他r寄存器，需要压栈
            {
                borrow_src2 = true;
                src2_reg = r0;
                // 不能跟src分配的寄存器冲突
                while (src1.p_typ == Param::Reg && src1.val.r == src2_reg)
                    src2_reg = (REGs)(src2_reg + 1);
                AddAsmCodePushRegisters(asm_insts, {src2_reg}, indent);
            }
            if (src2.p_typ == Param::Reg && IsSReg(src2.val.r))
                AddAsmCodeMoveRegisterToRegister(asm_insts, src2_reg, src2.val.r, indent);
            else AddAsmCodeMoveIntToRegister(asm_insts, src2_reg, src2.val.i, indent);
            src2.p_typ = Param::Reg;
            src2.val.r = src2_reg;
        }
        // dst = src1 < src2
        // treat as sub, but without swapping
        // 在下面调用的函数中已经保存分支类型
        AddAsmCodeCmp(asm_insts, instPtr, icmp_inst->op, src1, src2, indent);
        if (borrow_src2) AddAsmCodePopRegisters(asm_insts, {src2_reg}, indent);
        if (borrow_src1) AddAsmCodePopRegisters(asm_insts, {src1_reg}, indent);
    }
    Case (LLIR_BR, br_inst, instPtr)
    {
        //cout << get_tabs() << br_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, br_inst->ToString(), indent);

        if (!br_inst->has_cond) // unconditional branch
        {
            asm_insts.push_back(AsmCode(AsmInst::B, 
            {   Param(Param::Str, GET_BB_NAME(funcPtr, br_inst->tar_true))},
                indent));
        }
        else // conditional branch
        {
            if (br_inst->cond.ctv) // branch condition is constant
            {
                if (br_inst->cond.ctv->int_value == 0)
                    b_type = AlwaysFalse;
                else b_type = AlwaysTrue;
            }
            if (b_type != AlwaysFalse)
                asm_insts.push_back(AsmCode(AsmInst::ConvertAsmBranchTypeToBranchInst(b_type), 
                {   Param(Param::Str, GET_BB_NAME(funcPtr, br_inst->tar_true))},
                    indent));
            if (b_type != AlwaysTrue)
                asm_insts.push_back(AsmCode(AsmInst::B, 
                {   Param(Param::Str, GET_BB_NAME(funcPtr, br_inst->tar_false))},
                    indent));
        }

        // insert ltorg
        asm_insts.push_back(AsmCode(AsmInst::DOT_LTORG, "", "", indent));
    }
    Case (LLIR_ZEXT, zext_inst, instPtr)
    {
        //cout << get_tabs() << zext_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, zext_inst->ToString(), indent);

        if (b_type != AlwaysFalse)
            AddAsmCodeMoveIntToRegister(
                asm_insts,
                GET_ALLOCATION_RESULT(funcPtr, zext_inst->dst.reg->reg_id),
                1,
                indent,
                AsmInst::ConvertAsmBranchTypeToMoveInst(b_type));
        if (b_type != AlwaysTrue)
            AddAsmCodeMoveIntToRegister(
                asm_insts,
                GET_ALLOCATION_RESULT(funcPtr, zext_inst->dst.reg->reg_id),
                0,
                indent,
                AsmInst::ConvertAsmBranchTypeToMoveInst(REVERSED_BRANCH_TYPE(b_type)));

        // dst = (int) src
    }
    Case (LLIR_XOR, xor_inst, instPtr)
    {
        //cout << get_tabs() << xor_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, xor_inst->ToString(), indent);

        // 给逻辑比较或逻辑运算结果取反，相当于给branch_type取反（每相邻的两个是相反的）
        b_type = REVERSED_BRANCH_TYPE(b_type);
        // dst = ~ src
    }
    Case (LLIR_FBIN, fbin_inst, instPtr)
    {
        //cout << get_tabs() << fbin_inst->ToString() << endl;
        // 跳过目的寄存器没有分配的语句（这种情况是只二元运算的结果没有被使用）
        if(CONDITION_REGISTER_NOT_ALLOCATED(funcPtr, fbin_inst->dst.reg->reg_id))
            return;

        //cout << get_tabs() << bin_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, fbin_inst->ToString(), indent);

        // dst = src1 + src2
        Param src1, src2;
        if (fbin_inst->src1.ctv)
            src1 = Param(fbin_inst->src1.ctv->float_value);
        else
            src1 = Param(GET_ALLOCATION_RESULT(funcPtr, fbin_inst->src1.reg->reg_id));
        if (fbin_inst->src2.ctv)
            src2 = Param(fbin_inst->src2.ctv->float_value);
        else
            src2 = Param(GET_ALLOCATION_RESULT(funcPtr, fbin_inst->src2.reg->reg_id));
        
        AddAsmCodeFloatBin(asm_insts, fbin_inst->op, GET_ALLOCATION_RESULT(funcPtr, fbin_inst->dst.reg->reg_id), src1, src2, indent); 
    }
    Case (LLIR_FCMP, fcmp_inst, instPtr)
    {
        //cout << get_tabs() << fcmp_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, fcmp_inst->ToString(), indent);

        Param src1, src2;
        if (fcmp_inst->src1.ctv)
            src1 = Param(fcmp_inst->src1.ctv->float_value);
        else
            src1 = Param(GET_ALLOCATION_RESULT(funcPtr, fcmp_inst->src1.reg->reg_id));
        if (fcmp_inst->src2.ctv)
            src2 = Param(fcmp_inst->src2.ctv->float_value);
        else
            src2 = Param(GET_ALLOCATION_RESULT(funcPtr, fcmp_inst->src2.reg->reg_id));
        // dst = src1 < src2
        // 在下面调用的函数中已经保存分支类型
        AddAsmCodeFloatCmp(asm_insts, fcmp_inst->op, src1, src2, indent);
        asm_insts.push_back(AsmCode(AsmInst::FMSTAT, "", "", indent));
    }
    Case (LLIR_SITOFP, itf_inst, instPtr)
    {
        // 跳过目的寄存器没有分配的语句（这种情况是只二元运算的结果没有被使用）
        if(CONDITION_REGISTER_NOT_ALLOCATED(funcPtr, itf_inst->dst.reg->reg_id))
            return;

        //cout << get_tabs() << itf_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, itf_inst->ToString(), indent);

        if (itf_inst->src.reg) // virtreg
        {
            asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_1), GET_ALLOCATION_RESULT(funcPtr, itf_inst->src.reg->reg_id)}, indent));
            asm_insts.push_back(AsmCode(AsmInst::VCVT_ITOF, {Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_1)}, indent));
            asm_insts.push_back(AsmCode(AsmInst::VMOV, {GET_ALLOCATION_RESULT(funcPtr, itf_inst->dst.reg->reg_id), Param(FLOAT_BINOP_REGISTER_1)}, indent));
        }
        else // ctv
            AddAsmCodeMoveIntToRegister(asm_insts, GET_ALLOCATION_RESULT(funcPtr, itf_inst->dst.reg->reg_id), IntToFloat(itf_inst->src.ctv->int_value), indent);
        
        // dst = (float) src
    }
    Case (LLIR_FPTOSI, fti_inst, instPtr)
    {
        // 跳过目的寄存器没有分配的语句（这种情况是只二元运算的结果没有被使用）
        if(CONDITION_REGISTER_NOT_ALLOCATED(funcPtr, fti_inst->dst.reg->reg_id))
            return;

        //cout << get_tabs() << fti_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, fti_inst->ToString(), indent);

        if (fti_inst->src.reg) // virtreg
        {
            asm_insts.push_back(AsmCode(AsmInst::VMOV, {Param(FLOAT_BINOP_REGISTER_1), GET_ALLOCATION_RESULT(funcPtr, fti_inst->src.reg->reg_id)}, indent));
            asm_insts.push_back(AsmCode(AsmInst::VCVT_FTOI, {Param(FLOAT_BINOP_REGISTER_1), Param(FLOAT_BINOP_REGISTER_1)}, indent));
            asm_insts.push_back(AsmCode(AsmInst::VMOV, {GET_ALLOCATION_RESULT(funcPtr, fti_inst->dst.reg->reg_id), Param(FLOAT_BINOP_REGISTER_1)}, indent));
        }
        else // ctv
            AddAsmCodeMoveIntToRegister(asm_insts, GET_ALLOCATION_RESULT(funcPtr, fti_inst->dst.reg->reg_id), (int)fti_inst->src.ctv->float_value, indent);

        // dst = (int) src
    }
}

vector<AsmCode> InitDotDataAndUnderscoreStart(const CompUnit &ir, vector<AsmCode> &data_underscore_init)
{
    vector<AsmCode> dot_data;
    dot_data.push_back(AsmCode(AsmInst::DOT_DATA));

    // 局部变量 (alloca)
    for (auto &&funcPtr : ir.functions)
    {
        // skip unused functions
        if (!funcPtr->func_info.is_used)
            continue;
        dot_data.push_back(AsmCode(AsmInst::DOT_WORD, GET_LOCAL_VARS_NAME(funcPtr), "", 0));
        for (auto &&bbPtr : funcPtr->all_blocks)
        {
            if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                continue;
            for (auto &&instPtr : bbPtr->basic_block)
            {
                Case (LLIR_ALLOCA, alloc_inst, instPtr)
                {
                    auto &&varPtr = alloc_inst->var;
                    if (!varPtr->type.is_array) // 标量
                    {
                        if (varPtr->type.is_const) // 常量标量，跳过
                            continue;
                        // 变量标量，直接声明
                        dot_data.push_back(AsmCode(AsmInst::DOT_WORD, GET_LOCAL_VAR_NAME(funcPtr, varPtr->var_idx), {Param(Param::Str, "0")}, "", 1));
                    }
                    else // 数组，在data区存指针
                    {
                        dot_data.push_back(AsmCode(AsmInst::DOT_WORD, GET_LOCAL_PTR_NAME(funcPtr, varPtr->var_idx), {Param(Param::Str, "0")}, "", 1));
                        if (!funcPtr->func_info.is_recursive) // 如果是非递归函数，把它的数组当成不需要初始化的全局数组
                        {
                            // 在栈上给数组分配空间，并赋值
                            AddAsmCodeComment(data_underscore_init, "allocating stack memory for " + string(GET_LOCAL_PTR_NAME(funcPtr, varPtr->var_idx)), 1);
                            // 计算指针的地址
                            int allocation_bytes = varPtr->type.elements_number() * 4;
                            AddAsmCodeAddSub(data_underscore_init, AsmInst::SUB, PRELLOC_REGISTER, Param(sp), Param(allocation_bytes), 1);
                            data_underscore_init.push_back(AsmCode(AsmInst::MOV, {Param(sp), Param(PRELLOC_REGISTER)}, 1));
                            // 储存指针的地址
                            data_underscore_init.push_back(AsmCode(AsmInst::LDR, {Param(PRELLOC_REGISTER), Param(Param::Addr, GET_LOCAL_PTR_NAME(funcPtr, varPtr->var_idx))}, 1));
                            data_underscore_init.push_back(AsmCode(AsmInst::STR, {Param(sp), Param(PRELLOC_REGISTER)}, 1));
                        }
                    }
                }
            }
        }
    }

    // 全局变量（不包括全局常量数组）
    {
    int bytes_allocated_for_global_non_const_arrays = 0;
    for (auto &&varPair : ir.global_scope->local_table->var_table)
    {
        auto &&varName = varPair.first;
        auto &&varPtr = varPair.second;
        string comment;
        if (!varPtr->type.is_array) // 标量
        {
            if (varPtr->type.is_const) // 常量标量，跳过
                continue;
            if (varPtr->type.decl_type == TypeInt) { // 变量标量，直接定义
                // 全局变量初始化被移到主函数里了
                // params.push_back(Param(Param::Str, std::to_string(varPtr->int_scalar).c_str()));
                comment = "int "; }
            else {
                comment = "float ";
            }
            comment += varName + ";";
            dot_data.push_back(AsmCode(AsmInst::DOT_WORD, GET_GLOBAL_VAR_NAME(varPtr->var_idx), {Param(Param::Str, "0")}, comment, 1));
        }
        else // 数组，在data区存指针
        {
            if(varPtr->type.is_const)
                continue; // 跳过全局常量数组
            if (varPtr->type.decl_type == TypeInt) {
                /*for (auto &&elem : varPtr->int_list)
                    params.push_back(Param(Param::Str, std::to_string(elem).c_str()));*/
                comment = "int *";
            }
            else {
                comment = "float *";
            }
            comment += varName + ";";
            dot_data.push_back(AsmCode(AsmInst::DOT_WORD, GET_GLOBAL_PTR_NAME(varPtr->var_idx), {Param(Param::Str, "0")}, comment, 1));
            // 在栈上或使用malloc给数组分配空间，并赋值
            AddAsmCodeComment(data_underscore_init, "allocating memory for " + string(GET_GLOBAL_PTR_NAME(varPtr->var_idx)), 1);
            int allocation_bytes = varPtr->type.elements_number() * 4;
            if (allocation_bytes > 4 * 1024 * 1024) // call malloc for arrays bigger than 4MB
            {
                // 给全局非const数组malloc + memset
                AddAsmCodeComment(data_underscore_init, "calling malloc & memset for big array", 1);
                AddAsmCodeMoveIntToRegister(data_underscore_init, r0, allocation_bytes, 1);
                data_underscore_init.push_back(AsmCode(AsmInst::BL, {Param(Param::Str, "malloc")}, 1));
                data_underscore_init.push_back(AsmCode(AsmInst::LDR, {Param(PRELLOC_REGISTER), Param(Param::Addr, GET_GLOBAL_PTR_NAME(varPtr->var_idx))}, 1));
                data_underscore_init.push_back(AsmCode(AsmInst::STR, {Param(r0), Param(PRELLOC_REGISTER)}, 1));
                AddAsmCodeMoveIntToRegister(data_underscore_init, r1, 0, 1);
                AddAsmCodeMoveIntToRegister(data_underscore_init, r2, allocation_bytes, 1);
                data_underscore_init.push_back(AsmCode(AsmInst::BL, {Param(Param::Str, "memset")}, 1));
                continue;
            }
            // 计算指针的地址
            bytes_allocated_for_global_non_const_arrays += allocation_bytes;
            AddAsmCodeAddSub(data_underscore_init, AsmInst::SUB, PRELLOC_REGISTER, Param(sp), Param(allocation_bytes), 1);
            data_underscore_init.push_back(AsmCode(AsmInst::MOV, {Param(sp), Param(PRELLOC_REGISTER)}, 1));
            // 储存指针的地址
            data_underscore_init.push_back(AsmCode(AsmInst::LDR, {Param(PRELLOC_REGISTER), Param(Param::Addr, GET_GLOBAL_PTR_NAME(varPtr->var_idx))}, 1));
            data_underscore_init.push_back(AsmCode(AsmInst::STR, {Param(sp), Param(PRELLOC_REGISTER)}, 1));
        }
    }

    if (bytes_allocated_for_global_non_const_arrays)
    {
        // 打印给全局非const数组分配的空间，并对齐到8字节
        AddAsmCodeComment(data_underscore_init, "bytes allocated for global non-const arrays on stack: " + std::to_string(bytes_allocated_for_global_non_const_arrays), 1);
        if (bytes_allocated_for_global_non_const_arrays % 8 != 0) {
            AddAsmCodeComment(data_underscore_init, "which is not aligned to 8 bytes, fixing...", 1);
            data_underscore_init.push_back(AsmCode(AsmInst::SUB, {Param(sp), Param(sp), Param(4)}, 1));
            bytes_allocated_for_global_non_const_arrays += 4;}

        // 给全局非const数组memset
        AddAsmCodeComment(data_underscore_init, "calling memset for global non-const arrays", 1);
        data_underscore_init.push_back(AsmCode(AsmInst::MOV, {Param(r0), Param(sp)}, 1));
        AddAsmCodeMoveIntToRegister(data_underscore_init, r1, 0, 1);
        AddAsmCodeMoveIntToRegister(data_underscore_init, r2, bytes_allocated_for_global_non_const_arrays, 1);
        data_underscore_init.push_back(AsmCode(AsmInst::BL, {Param(Param::Str, "memset")}, 1));
    }
    }

    // 全局const数组
    {
    int bytes_allocated_for_global_const_arrays = 0;
    for (auto &&varPair : ir.global_scope->local_table->var_table)
    {
        auto &&varName = varPair.first;
        auto &&varPtr = varPair.second;
        string comment;
        if (!varPtr->type.is_array) // 标量
            continue;
        else // 数组，在data区存指针
        {
            if(!varPtr->type.is_const)
                continue; // 跳过非常量数组
            AddAsmCodeComment(data_underscore_init, "allocating stack memory for " + string(GET_GLOBAL_PTR_NAME(varPtr->var_idx)), 1);
            if (varPtr->type.decl_type == TypeInt) {
                // 倒序push, 一次16个
                int pushed_bytes = 0;
                int actual_pushed_bytes = 0;
                for (auto &&it = varPtr->int_list.rbegin();it != varPtr->int_list.rend();)
                {
                    for (REGs i = s15;
                        i >= s0 && it != varPtr->int_list.rend();
                        i = (REGs)(i - 1), ++it, pushed_bytes += 4)
                        AddAsmCodeMoveIntToRegister(data_underscore_init, i, *it, 1);
                            AddAsmCodePushRegisters(data_underscore_init, CONST_ARRAY_INIT_REGISTERS, 1);
                            actual_pushed_bytes += 4*16;
                }
                data_underscore_init.push_back(AsmCode(AsmInst::ADD, {Param(sp), Param(sp), Param(actual_pushed_bytes - pushed_bytes)}, 1));
                bytes_allocated_for_global_const_arrays += pushed_bytes;
                comment = "const int *";
            }
            else {
                // 倒序push, 一次16个
                int pushed_bytes = 0;
                int actual_pushed_bytes = 0;
                for (auto &&it = varPtr->float_list.rbegin();it != varPtr->float_list.rend();)
                {
                    for (REGs i = s15;
                        i >= s0 && it != varPtr->float_list.rend();
                        i = (REGs)(i - 1), ++it, pushed_bytes += 4)
                        AddAsmCodeMoveIntToRegister(data_underscore_init, i, DoubleToWord(*it), 1);
                            AddAsmCodePushRegisters(data_underscore_init, CONST_ARRAY_INIT_REGISTERS, 1);
                            actual_pushed_bytes += 4*16;
                }
                data_underscore_init.push_back(AsmCode(AsmInst::ADD, {Param(sp), Param(sp), Param(actual_pushed_bytes - pushed_bytes)}, 1));
                bytes_allocated_for_global_const_arrays += pushed_bytes;
                comment = "const int *";
                comment = "const float *";
            }
            comment += varName + ";";
            dot_data.push_back(AsmCode(AsmInst::DOT_WORD, GET_GLOBAL_PTR_NAME(varPtr->var_idx), {Param(Param::Str, "0")}, comment, 1));
            // 储存指针的地址
            data_underscore_init.push_back(AsmCode(AsmInst::LDR, {Param(PRELLOC_REGISTER), Param(Param::Addr, GET_GLOBAL_PTR_NAME(varPtr->var_idx))}, 1));
            data_underscore_init.push_back(AsmCode(AsmInst::STR, {Param(sp), Param(PRELLOC_REGISTER)}, 1));
        }
    }

    if (bytes_allocated_for_global_const_arrays)
    {
        // 打印给全局非const数组分配的空间，并对齐到8字节
        AddAsmCodeComment(data_underscore_init, "bytes allocated for global const arrays on stack: " + std::to_string(bytes_allocated_for_global_const_arrays), 1);
        if (bytes_allocated_for_global_const_arrays % 8 != 0) {
            AddAsmCodeComment(data_underscore_init, "which is not aligned to 8 bytes, fixing...", 1);
            data_underscore_init.push_back(AsmCode(AsmInst::SUB, {Param(sp), Param(sp), Param(4)}, 1));
            bytes_allocated_for_global_const_arrays += 4;}
    }
    }

    // return
    return dot_data;
}

void GenerateAssembly(const string &asmfile, const CompUnit &ir)
{

    ofstream ofs(asmfile);
    ofs << ".cpu cortex-a72\n.arch armv8-a\n.fpu neon-fp-armv8\n.arch_extension crc\n";

    vector<AsmCode> data_init;
    vector<AsmCode> asm_insts(InitDotDataAndUnderscoreStart(ir, data_init));
    asm_insts.push_back(AsmCode(AsmInst::DOT_TEXT));
    asm_insts.push_back(AsmCode(AsmInst::DOT_GLOBAL, {Param(Param::Str, "main")}));

    // all functions
    for (auto &&funcPtr : ir.functions)
    {
        // skip unused functions
        if (!funcPtr->func_info.is_used)
            continue;

        int indent = 0;
        // function start
        asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name, "", indent));

        // insert global ptr alloc before start of main (DO NOT touch r0-r3)
        if (funcPtr->func_info.func_name == "main" && !data_init.empty())
        {
            asm_insts.push_back(AsmCode(AsmInst::EMPTY, "global_alloc", "", indent));
            AddAsmCodeMoveRegisterToRegister(asm_insts, GLOBAL_ALLOC_LR_TEMP, lr, 1);
            for (auto &&elem : data_init)
                asm_insts.push_back(elem);
            AddAsmCodeMoveRegisterToRegister(asm_insts, lr, GLOBAL_ALLOC_LR_TEMP, 1);
            asm_insts.push_back(AsmCode(AsmInst::B, {Param(Param::Str, "global_alloc_ltorg")}, 1));
            asm_insts.push_back(AsmCode(AsmInst::DOT_LTORG, "", "", 1));
            asm_insts.push_back(AsmCode(AsmInst::EMPTY, "global_alloc_ltorg", "", indent));
        }

        // if this function called other functions, push the lr register (DO NOT touch r0-r3)
        if (!funcPtr->func_info.called_funcs.empty() || funcPtr->func_info.func_name == "main" && !data_init.empty())
        {
            asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name + "_push_lr", "", indent));
            AddAsmCodePushRegisters(asm_insts, {lr}, 1);
        }

        // insert save local var & ptr for recursive funtions (DO NOT touch r0-r3)
        int local_vars_alloc_bytes = 0;
        if (funcPtr->func_info.is_recursive)
        {
            asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name + "_save", "", indent));
            for (auto &&bbPtr : funcPtr->all_blocks)
            {
                if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                    continue;
                for (auto &&instPtr : bbPtr->basic_block)
                    Case (LLIR_ALLOCA, alloc_inst, instPtr)
                        local_vars_alloc_bytes += 4;
            }
            if (local_vars_alloc_bytes % 8 != 0)
                local_vars_alloc_bytes += 4;
            // 移动堆栈指针，给局部变量创造空间
            AddAsmCodeAddSub(asm_insts, AsmInst::SUB, PRELLOC_REGISTER, Param(sp), Param(local_vars_alloc_bytes), 1);
            asm_insts.push_back(AsmCode(AsmInst::MOV, {Param(sp), Param(PRELLOC_REGISTER)}, 1));
            // 调用memcpy
            AddAsmCodeComment(asm_insts, "calling memcpy for saving local vars", 1);
            AddAsmCodePushRegisters(asm_insts, LOCAL_VARS_SAVE_MEMCPY_REGISTERS, 1);
            asm_insts.push_back(AsmCode(AsmInst::MOV, {Param(r0), Param(sp)}, 1));
            asm_insts.push_back(AsmCode(AsmInst::ADD, {Param(r0), Param(4 * 4)}, 1));
            asm_insts.push_back(AsmCode(AsmInst::LDR, {Param(r1), Param(Param::Addr, GET_LOCAL_VARS_NAME(funcPtr))}, 1));
            AddAsmCodeMoveIntToRegister(asm_insts, r2, local_vars_alloc_bytes, 1);
            asm_insts.push_back(AsmCode(AsmInst::BL, {Param(Param::Str, "memcpy")}, 1));
            AddAsmCodePopRegisters(asm_insts, LOCAL_VARS_SAVE_MEMCPY_REGISTERS, 1);
        }

        // insert local ptr alloc for recursive functions (DO NOT touch r0-r3)
        int bytes_allocated_for_local_arrays = 0;
        if (funcPtr->func_info.is_recursive)
        {
            asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name + "_alloc", "", indent));
            for (auto &&bbPtr : funcPtr->all_blocks)
            {
                if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                    continue;
                for (auto &&instPtr : bbPtr->basic_block)
                {
                    Case (LLIR_ALLOCA, alloc_inst, instPtr)
                    {
                        auto &&varPtr = alloc_inst->var;
                        if (!varPtr->type.is_array) // 跳过标量
                            continue;
                        // 在栈上给数组分配空间，并赋值
                        AddAsmCodeComment(asm_insts, "allocating stack memory for " + string(GET_LOCAL_PTR_NAME(funcPtr, varPtr->var_idx)), 1);
                        // 计算指针的地址
                        int allocation_bytes = varPtr->type.elements_number() * 4;
                        bytes_allocated_for_local_arrays += allocation_bytes;
                        AddAsmCodeAddSub(asm_insts, AsmInst::SUB, PRELLOC_REGISTER, Param(sp), Param(allocation_bytes), 1);
                        asm_insts.push_back(AsmCode(AsmInst::MOV, {Param(sp), Param(PRELLOC_REGISTER)}, 1));
                        // 储存指针的地址
                        asm_insts.push_back(AsmCode(AsmInst::LDR, {Param(PRELLOC_REGISTER), Param(Param::Addr, GET_LOCAL_PTR_NAME(funcPtr, varPtr->var_idx))}, 1));
                        asm_insts.push_back(AsmCode(AsmInst::STR, {Param(sp), Param(PRELLOC_REGISTER)}, 1));
                    }
                }
            }
            if (bytes_allocated_for_local_arrays)
            {
                // 打印给全局非const数组分配的空间，并对齐到8字节
                AddAsmCodeComment(asm_insts, "bytes allocated for local arrays on stack: " + std::to_string(bytes_allocated_for_local_arrays), 1);
                if (bytes_allocated_for_local_arrays % 8 != 0) {
                    AddAsmCodeComment(asm_insts, "which is not aligned to 8 bytes, fixing...", 1);
                    asm_insts.push_back(AsmCode(AsmInst::SUB, {Param(sp), Param(sp), Param(4)}, 1));
                    bytes_allocated_for_local_arrays += 4;}
            }
        }

        // basic blocks
        for (auto &&bbPtr : funcPtr->all_blocks)
        {
            if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                continue;
            asm_insts.push_back(AsmCode(AsmInst::EMPTY,
                GET_BB_NAME(funcPtr, bbPtr->bb_idx), "",
                indent));
            int first_ldr_idx;
            bool first_ldr_found = false;
            int dist_from_first_ldr_idx = 0;
            int ltorg_idx = 0;
            for (auto &&instPtr : bbPtr->basic_block)
            {
                ++indent;
                size_t siz_before, siz_after;
                siz_before = asm_insts.size();
                AddAsmCodeFromLLIR(asm_insts, funcPtr, instPtr, indent);
                siz_after = asm_insts.size();
                int new_insts_cnt = siz_after - siz_before;
                for (int i = siz_before; i < siz_after; ++i)
                {
                    if (!first_ldr_found && 
                    (asm_insts[i].inst.i_typ == AsmInst::LDR || asm_insts[i].inst.i_typ == AsmInst::VLDR))
                    {
                        first_ldr_found = true;
                        first_ldr_idx = i;
                        dist_from_first_ldr_idx = 0;
                        break;
                    }
                }
                if (first_ldr_found)
                {
                    dist_from_first_ldr_idx = siz_after - first_ldr_idx;
                
                    // insert ltorg, and reset counter
                    if (dist_from_first_ldr_idx > 300)
                    {
                        asm_insts.push_back(AsmCode(
                            AsmInst::B,
                            {Param(Param::Str, (string(GET_BB_NAME(funcPtr, bbPtr->bb_idx)) + "_ltorg_" + std::to_string(++ltorg_idx)).c_str())},
                            indent));
                        asm_insts.push_back(AsmCode(AsmInst::DOT_LTORG, "", "", indent));
                        asm_insts.push_back(AsmCode(
                            AsmInst::EMPTY,
                            string(GET_BB_NAME(funcPtr, bbPtr->bb_idx)) + "_ltorg_" + std::to_string(ltorg_idx),
                            "",
                            indent - 1));
                        first_ldr_found = false;
                        dist_from_first_ldr_idx = 0;
                    }
                }
                --indent;
            }
        }

        // function return
        asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name + "_return", "", indent));

        // insert local ptr dealloc for recursive functions (DO NOT touch r0)
        if (funcPtr->func_info.is_recursive)
        {
            asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name + "_dealloc", "", indent));
            for (auto &&bbPtr : funcPtr->all_blocks)
            {
                if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                    continue;
                for (auto &&instPtr : bbPtr->basic_block)
                {
                    Case (LLIR_ALLOCA, alloc_inst, instPtr)
                    {
                        auto &&varPtr = alloc_inst->var;
                        if (!varPtr->type.is_array) // 跳过标量
                            continue;
                        // 在栈上给数组分配空间，并赋值
                        AddAsmCodeComment(asm_insts, "deallocating stack memory for " + funcPtr->func_info.func_name, 1);
                        // 计算指针的地址
                        AddAsmCodeAddSub(asm_insts, AsmInst::ADD, DELLOC_REGISTER, Param(sp), Param(bytes_allocated_for_local_arrays), 1);
                        asm_insts.push_back(AsmCode(AsmInst::MOV, {Param(sp), Param(DELLOC_REGISTER)}, 1));
                    }
                }
            }
        }

        // insert load local var & ptr for recursive funtions (DO NOT touch r0)
        if (funcPtr->func_info.is_recursive)
        {
            asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name + "_load", "", indent));
            // 调用memcpy
            AddAsmCodeComment(asm_insts, "calling memcpy for loading local vars", 1);
            AddAsmCodePushRegisters(asm_insts, LOCAL_VARS_LOAD_MEMCPY_REGISTERS, 1);
            asm_insts.push_back(AsmCode(AsmInst::LDR, {Param(r0), Param(Param::Addr, GET_LOCAL_VARS_NAME(funcPtr))}, 1));
            asm_insts.push_back(AsmCode(AsmInst::MOV, {Param(r1), Param(sp)}, 1));
            asm_insts.push_back(AsmCode(AsmInst::ADD, {Param(r1), Param(2 * 4)}, 1));
            AddAsmCodeMoveIntToRegister(asm_insts, r2, local_vars_alloc_bytes, 1);
            asm_insts.push_back(AsmCode(AsmInst::BL, {Param(Param::Str, "memcpy")}, 1));
            AddAsmCodePopRegisters(asm_insts, LOCAL_VARS_LOAD_MEMCPY_REGISTERS, 1);
            // 移动堆栈指针，消除给局部变量创造的空间
            AddAsmCodeAddSub(asm_insts, AsmInst::ADD, DELLOC_REGISTER, Param(sp), Param(local_vars_alloc_bytes), 1);
            asm_insts.push_back(AsmCode(AsmInst::MOV, {Param(sp), Param(DELLOC_REGISTER)}, 1));
        }

        // return instructions
        asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name + "_end", "", indent));
        if (!funcPtr->func_info.called_funcs.empty() || funcPtr->func_info.func_name == "main" && !data_init.empty())
            AddAsmCodePopRegisters(asm_insts, {pc}, 1);
        else
            asm_insts.push_back(AsmCode(AsmInst::BX,
                {Param(lr)}, 1));

        // insert ltorg
        asm_insts.push_back(AsmCode(AsmInst::DOT_LTORG, "", "", 1));
    }


    for (auto &&asm_inst : asm_insts)
    {
        ofs << asm_inst.ToString() << '\n';
    }

}

#endif
