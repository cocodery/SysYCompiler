#ifndef CODEGEN_H
#define CODEGEN_H

#include "common.hh"
#include "ir.hh"
#include "liveness_info.hh"

#include <sstream>

using std::stringstream;

#define FLOAT_TO_INT(x) (*((int *)(&(x))))

#define GET_GLOBAL_VAR_NAME(x) ((string(GLOB_VAR_PREFIX) + std::to_string(x)).c_str())

#define GET_LOCAL_VAR_NAME(x, y) ((string(LOCA_VAR_PREFIX) + x + "_" + std::to_string(y)).c_str())

#define R_REGISTER_IN_BRACKETS(x) ((string("[r") + std::to_string(x) + "]").c_str())

class Param
{
public:
    enum ParamType {Imm_int, Imm_float, Reg, Addr, Str} p_typ;
    union Value
    {
        int i;
        float f;
        REGs r;
        char str[128];
        Value() {}
        Value(int _i): i(_i){}
        Value(float _f): f(_f){}
        Value(REGs _r): r(_r){}
        Value(const char *_s) {strcpy(str, _s);}
        ~Value(){}
    } val;
public:
    Param() {}
    Param(int _i): p_typ(Imm_int), val(_i) {}
    Param(float _f): p_typ(Imm_float), val(_f) {}
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
        ADD, SUB, DOT_WORD, DOT_SPACE, MUL
    } i_typ; const vector<string> i_str {
        "", ".global", ".data", ".text", "bx",
        "mov", "movt", "movw", "str", "ldr",
        "add", "sub", ".word", ".space", "mul"
    };
public:
    AsmInst(InstType _i_typ):i_typ(_i_typ) {}
    const string &ToString()
    {
        return i_str[i_typ];
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
                if (inst.i_typ == AsmInst::STR ||
                (inst.i_typ == AsmInst::LDR && it->p_typ == Param::Reg))
                    ss << ", [" << it->ToString() << ']';
                else
                    ss << ", " << it->ToString();
        }
        if (!comment.empty())
            ss << " // " << comment;
        return ss.str();
    }
};

void AddAsmCodeMoveIntToRegister(vector<AsmCode> &asm_insts, REGs r, int i, int indent)
{
    if (i >= -257 && i <= 256)
        asm_insts.push_back(AsmCode(AsmInst::MOV,
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
        if (i < INT16_MIN || i > INT16_MAX)
            asm_insts.push_back(AsmCode(AsmInst::MOVT,
                {Param(r), Param((uint16_t)(i >> 16))},
                indent));
    }
}

void AddAsmCodeAddSub(vector<AsmCode> &asm_insts, AsmInst::InstType _i_typ, REGs r, const Param &src1, const Param &src2, int indent)
{
    // assumes src2 is smaller (when possible, e.g. add)
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Reg) // both reg
        asm_insts.push_back(AsmCode(_i_typ,
            {   Param(r),
                src1,
                src2},
            indent));
    else if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_int) // src1 is reg, src2 is ctv
    {
        if (abs(src2.val.i) <= 256) // src2 is small enough
            asm_insts.push_back(AsmCode(_i_typ,
                {   Param(r),
                    src1,
                    src2},
                indent));
        else // src2 is too big to be packed into a single instruction
        {
            AddAsmCodeMoveIntToRegister(asm_insts, r, src2.val.i, indent);
            asm_insts.push_back(AsmCode(_i_typ,
                {   Param(r),
                    Param(r),
                    src1},
                indent));
        }
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg) // src1 is ctv, src2 is reg, should be sub only
    {
        AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i, indent);
        if (abs(src2.val.i) <= 256) // src2 is small enough
            asm_insts.push_back(AsmCode(_i_typ,
                {   Param(r),
                    Param(r),
                    src2},
                indent));
        else // src2 is too big to be packed into a single instruction
        {
            AddAsmCodeMoveIntToRegister(asm_insts, r0, src2.val.i, indent);
            asm_insts.push_back(AsmCode(_i_typ,
                {   Param(r),
                    Param(r),
                    Param(r0)},
                indent));
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

void AddAsmCodeMul(vector<AsmCode> &asm_insts, AsmInst::InstType _i_typ, REGs r, const Param &src1, const Param &src2, int indent)
{
    // assumes src2 is smaller (when possible, e.g. mul)
    if (src1.p_typ == Param::Reg && src2.p_typ == Param::Reg) // both reg
        asm_insts.push_back(AsmCode(_i_typ,
            {   Param(r),
                src1,
                src2},
            indent));
    else if (src1.p_typ == Param::Reg && src2.p_typ == Param::Imm_int) // src1 is reg, src2 is ctv
    {
        AddAsmCodeMoveIntToRegister(asm_insts, r, src2.val.i, indent);
        asm_insts.push_back(AsmCode(_i_typ,
            {   Param(r),
                Param(r),
                src1},
            indent));
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg) // src1 is ctv, src2 is reg
    {
        AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i, indent);
        asm_insts.push_back(AsmCode(_i_typ,
            {   Param(r),
                Param(r),
                src2},
            indent));
    }
    else if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Imm_int)// both ctv
        AddAsmCodeMoveIntToRegister(asm_insts, r, src1.val.i * src2.val.i, indent);
}

void AddAsmCodeComment(vector<AsmCode> &asm_insts, const string& comment, int indent)
{
    asm_insts.push_back(AsmCode(AsmInst::EMPTY, "", comment, indent));
}

void AddAsmCodeFromLLIR(vector<AsmCode> &asm_insts, map<int32_t, REGs> &AllocationResult, const string& funcname, Inst *instPtr, int indent)
{
    Case (LLIR_RET, ret_inst, instPtr)
    {
        //cout << get_tabs() << ret_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, ret_inst->ToString(), indent);

        // return src
        if (ret_inst->has_retvalue)
        {
            if (ret_inst->ret_value.ctv) // ctvalue
                if (ret_inst->ret_value.ctv->type == TypeInt) // int
                    AddAsmCodeMoveIntToRegister(asm_insts, r0, ret_inst->ret_value.ctv->int_value, indent);
                else // float
                    AddAsmCodeMoveIntToRegister(asm_insts, r0, FLOAT_TO_INT(ret_inst->ret_value.ctv->float_value), indent);
            else // virtreg
                asm_insts.push_back(AsmCode(AsmInst::MOV,
                    {   Param(r0),
                        Param(AllocationResult.at(ret_inst->ret_value.reg->reg_id))},
                    indent));
        }
        asm_insts.push_back(AsmCode(AsmInst::BX,
            {Param(lr)},
            indent));
    }
    Case (LLIR_STORE, store_inst, instPtr)
    {
        //cout << get_tabs() << store_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, store_inst->ToString(), indent);

        // *dst = src
        if (store_inst->dst.reg->global) // if global add ldr inst
        {
            asm_insts.push_back(AsmCode(AsmInst::LDR,
                {   Param(AllocationResult.at(IF_GLOBAL_RETURN_NEG_ID(store_inst->dst.reg))),
                    Param(Param::Addr, GET_GLOBAL_VAR_NAME(store_inst->dst.reg->reg_id))},
                indent));
        }
        if (store_inst->src.ctv) // ctvalue
        {
            if (store_inst->src.ctv->type == TypeInt) // int
                AddAsmCodeMoveIntToRegister(asm_insts, r0, store_inst->src.ctv->int_value, indent);
            else // float
                AddAsmCodeMoveIntToRegister(asm_insts, r0, FLOAT_TO_INT(store_inst->src.ctv->float_value), indent);
            asm_insts.push_back(AsmCode(AsmInst::STR,
                {   Param(r0),
                    Param(AllocationResult.at(IF_GLOBAL_RETURN_NEG_ID(store_inst->dst.reg)))},
                indent));
        }
        else // virtreg
            asm_insts.push_back(AsmCode(AsmInst::STR,
                {   Param(AllocationResult.at(store_inst->src.reg->reg_id)),
                    Param(AllocationResult.at(IF_GLOBAL_RETURN_NEG_ID(store_inst->dst.reg)))},
                indent));
    }
    Case (LLIR_LOAD, load_inst, instPtr)
    {
        //cout << get_tabs() << load_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, load_inst->ToString(), indent);
        
        // dst = *src
        if (load_inst->src.reg->global) // if global use ldr rx, =y
        {
            asm_insts.push_back(AsmCode(AsmInst::LDR,
                {   Param(AllocationResult.at(load_inst->dst.reg->reg_id)),
                    Param(Param::Addr, GET_GLOBAL_VAR_NAME(load_inst->src.reg->reg_id))},
                indent));
            asm_insts.push_back(AsmCode(AsmInst::LDR,
                {   Param(AllocationResult.at(load_inst->dst.reg->reg_id)),
                    Param(Param::Str, R_REGISTER_IN_BRACKETS(AllocationResult.at(load_inst->dst.reg->reg_id)))},
                indent));
        }
        else // local
            asm_insts.push_back(AsmCode(AsmInst::LDR,
                {   Param(AllocationResult.at(load_inst->dst.reg->reg_id)),
                    Param(AllocationResult.at(load_inst->src.reg->reg_id))},
                indent));
    }
    Case (LLIR_BIN, bin_inst, instPtr)
    {
        //cout << get_tabs() << bin_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, bin_inst->ToString(), indent);

        // dst = src1 + src2
        Param src1, src2;
        if (bin_inst->src1.ctv)
            src1 = Param(bin_inst->src1.ctv->int_value);
        else
            src1 = Param(AllocationResult.at(bin_inst->src1.reg->reg_id));
        if (bin_inst->src2.ctv)
            src2 = Param(bin_inst->src2.ctv->int_value);
        else
            src2 = Param(AllocationResult.at(bin_inst->src2.reg->reg_id));
        
        switch(bin_inst->op)
        {
        case BinOp::ADD:
            if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg)
                std::swap(src1, src2);
            if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Imm_int && abs(src1.val.i) < abs(src2.val.i))
                std::swap(src1, src2);
            AddAsmCodeAddSub(asm_insts, AsmInst::ADD, AllocationResult.at(bin_inst->dst.reg->reg_id), src1, src2, indent);
            break;
        case BinOp::SUB:
            if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Imm_int && abs(src1.val.i) < abs(src2.val.i)) {
                std::swap(src1, src2);
                src1.invert();
                src2.invert(); }
            AddAsmCodeAddSub(asm_insts, AsmInst::SUB, AllocationResult.at(bin_inst->dst.reg->reg_id), src1, src2, indent);
            break;
        case BinOp::MUL:
            if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Reg)
                std::swap(src1, src2);
            if (src1.p_typ == Param::Imm_int && src2.p_typ == Param::Imm_int && abs(src1.val.i) < abs(src2.val.i))
                std::swap(src1, src2);
            AddAsmCodeMul(asm_insts, AsmInst::MUL, AllocationResult.at(bin_inst->dst.reg->reg_id), src1, src2, indent);
            break;
        case BinOp::DIV:
            break;
        default:
            break;
        }
    }
    Case (LLIR_ALLOCA, alloc_inst, instPtr)
    {
        // get_tabs() << alloc_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, alloc_inst->ToString(), indent);

        // dst = malloc(something);
        asm_insts.push_back(AsmCode(AsmInst::LDR,
            {   Param(AllocationResult.at(alloc_inst->reg.reg->reg_id)),
                Param(Param::Addr, GET_LOCAL_VAR_NAME(funcname, alloc_inst->reg.reg->reg_id))},
            indent));
    }
    Case (LLIR_GEP, gep_inst, instPtr)
    {
        //cout << get_tabs() << gep_inst->ToString() << endl;
        AddAsmCodeComment(asm_insts, gep_inst->ToString(), indent);
        
        // dst = *(src + off)
        if (gep_inst->src.reg->global) // if global use ldr rx, =y
            asm_insts.push_back(AsmCode(AsmInst::LDR,
                {   Param(AllocationResult.at(gep_inst->dst.reg->reg_id)),
                    Param(Param::Addr, GET_GLOBAL_VAR_NAME(gep_inst->src.reg->reg_id))},
                indent));
        else // local
            asm_insts.push_back(AsmCode(AsmInst::MOV,
            {   Param(AllocationResult.at(gep_inst->dst.reg->reg_id)),
                Param(AllocationResult.at(gep_inst->src.reg->reg_id))},
            indent));

        int32_t size_shift_bits = 2;

        if (gep_inst->off.ctv) // offset is ctv
            AddAsmCodeAddSub(
                asm_insts,
                AsmInst::ADD,
                AllocationResult.at(gep_inst->dst.reg->reg_id),
                Param(AllocationResult.at(gep_inst->dst.reg->reg_id)),
                Param(gep_inst->off.ctv->int_value << size_shift_bits),
                indent);
        else // offset is reg
            asm_insts.push_back(AsmCode(AsmInst::ADD,
            {   Param(AllocationResult.at(gep_inst->dst.reg->reg_id)),
                Param(AllocationResult.at(gep_inst->dst.reg->reg_id)),
                Param(AllocationResult.at(gep_inst->off.reg->reg_id)),
                Param(Param::Str, (string("lsl #") + std::to_string(size_shift_bits)).c_str())},
            indent));
        
        //IF_IS_REG_THEN_PUSH_BACK(src_regids, gep_inst->off.reg);
        //IF_IS_REG_THEN_PUSH_BACK(src_regids, gep_inst->src.reg);
        //dst_regid = IF_GLOBAL_RETURN_NEG_ID(gep_inst->dst.reg);
    }
    /*Case (LLIR_BR, br_inst, instPtr)
    {
        //cout << get_tabs() << br_inst->ToString() << endl;

        // if (src) goto label1;
        if (br_inst->has_cond)
            IF_IS_REG_THEN_PUSH_BACK(src_regids, br_inst->cond.reg);
    }
    Case (LLIR_FBIN, fbin_inst, instPtr)
    {
        //cout << get_tabs() << fbin_inst->ToString() << endl;

        // dst = src1 + src2
        IF_IS_REG_THEN_PUSH_BACK(src_regids, fbin_inst->src1.reg);
        IF_IS_REG_THEN_PUSH_BACK(src_regids, fbin_inst->src2.reg);
        dst_regid = IF_GLOBAL_RETURN_NEG_ID(fbin_inst->dst.reg);
    }
    Case (LLIR_ICMP, icmp_inst, instPtr)
    {
        //cout << get_tabs() << icmp_inst->ToString() << endl;
    
        // dst = src1 < src2
        if (icmp_inst->src1.reg)
            src_regids.push_back(IF_GLOBAL_RETURN_NEG_ID(icmp_inst->src1.reg));
        if (icmp_inst->src2.reg)
            src_regids.push_back(IF_GLOBAL_RETURN_NEG_ID(icmp_inst->src2.reg));
        dst_regid = IF_GLOBAL_RETURN_NEG_ID(icmp_inst->dst.reg);
    }
    Case (LLIR_FCMP, fcmp_inst, instPtr)
    {
        //cout << get_tabs() << fcmp_inst->ToString() << endl;
    
        // dst = src1 < src2
        IF_IS_REG_THEN_PUSH_BACK(src_regids, fcmp_inst->src1.reg);
        IF_IS_REG_THEN_PUSH_BACK(src_regids, fcmp_inst->src2.reg);
        dst_regid = IF_GLOBAL_RETURN_NEG_ID(fcmp_inst->dst.reg);
    }
    Case (LLIR_CALL, call_inst, instPtr)
    {
        //cout << get_tabs() << call_inst->ToString() << endl;
    
        // [dst = ] func(arg1, arg2, ...)
        for (auto &&arg : call_inst->args)
            IF_IS_REG_THEN_PUSH_BACK(src_regids, arg.reg);
        if (call_inst->func_info->return_type != TypeVoid)
            dst_regid = IF_GLOBAL_RETURN_NEG_ID(call_inst->dst.reg);
    }
    Case (LLIR_ZEXT, zext_inst, instPtr)
    {
        //cout << get_tabs() << zext_inst->ToString() << endl;
    
        // dst = (int) src
        IF_IS_REG_THEN_PUSH_BACK(src_regids, zext_inst->src.reg);
        dst_regid = IF_GLOBAL_RETURN_NEG_ID(zext_inst->dst.reg);
    }
    Case (LLIR_XOR, xor_inst, instPtr)
    {
        //cout << get_tabs() << xor_inst->ToString() << endl;
        
        // dst = ~ src
        IF_IS_REG_THEN_PUSH_BACK(src_regids, xor_inst->src.reg);
        dst_regid = IF_GLOBAL_RETURN_NEG_ID(xor_inst->dst.reg);
    }
    Case (LLIR_BC, bc_inst, instPtr)
    {
        //cout << get_tabs() << bc_inst->ToString() << endl; 

        // dst = (char)src
        IF_IS_REG_THEN_PUSH_BACK(src_regids, bc_inst->src.reg);
        dst_regid = IF_GLOBAL_RETURN_NEG_ID(bc_inst->dst.reg);
    }
    Case (LLIR_SITOFP, itf_inst, instPtr)
    {
        //cout << get_tabs() << itf_inst->ToString() << endl;
        
        // dst = (float) src
        IF_IS_REG_THEN_PUSH_BACK(src_regids, itf_inst->src.reg);
        dst_regid = IF_GLOBAL_RETURN_NEG_ID(itf_inst->dst.reg);
    }
    Case (LLIR_FPTOSI, fti_inst, instPtr)
    {
        //cout << get_tabs() << fti_inst->ToString() << endl;
        
        // dst = (int) src
        IF_IS_REG_THEN_PUSH_BACK(src_regids, fti_inst->src.reg);
        dst_regid = IF_GLOBAL_RETURN_NEG_ID(fti_inst->dst.reg);
    }*/
}

void GenerateAssembly(const string &asmfile, const CompUnit &ir)
{

    ofstream ofs(asmfile);

    vector<AsmCode> asm_insts;

    asm_insts.push_back(AsmCode(AsmInst::DOT_DATA));

    // global vars
    for (auto &&varPair : ir.global_scope->local_table->var_table)
    {
        auto &&varName = varPair.first;
        auto &&varPtr = varPair.second;
        vector<Param> params;
        string comment;
        if (!varPtr->type.is_array) // not array
        {
            if (varPtr->type.is_const)
                continue;
            if (varPtr->type.decl_type == TypeInt) {
                params.push_back(Param(Param::Str, std::to_string(varPtr->int_scalar).c_str()));
                comment = "int "; }
            comment += varName + ";";
            asm_insts.push_back(AsmCode(AsmInst::DOT_WORD, GET_GLOBAL_VAR_NAME(varPtr->var_idx), params, comment, 1));
        }
        else // array
        {
            if (varPtr->type.is_const) // const array
            {
                if (varPtr->type.decl_type == TypeInt)
                {
                    for (auto &&elem : varPtr->int_list)
                        params.push_back(Param(Param::Str, std::to_string(elem).c_str()));
                    comment = "int ";
                }
                comment += varName + "[];";
                asm_insts.push_back(AsmCode(AsmInst::DOT_WORD, GET_GLOBAL_VAR_NAME(varPtr->var_idx), params, comment, 1));
            }   
            else // non const array
            {
                if (varPtr->type.decl_type == TypeInt) {
                    params.push_back(Param(Param::Str, std::to_string(varPtr->type.elements_number()).c_str()));
                    comment = "int "; }
                comment += varName + "[];";
                asm_insts.push_back(AsmCode(AsmInst::DOT_SPACE, GET_GLOBAL_VAR_NAME(varPtr->var_idx), params, comment, 1));
            }
        }
        // TODO: float
    }

    // local arrays
    for (auto &&funcPtr : ir.functions)
    {
        int indent = 0;
        for (auto &&bbPtr : funcPtr->all_blocks)
        {
            if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                continue;
            for (auto &&instPtr : bbPtr->basic_block)
            {
                Case (LLIR_ALLOCA, alloc_inst, instPtr)
                {
                    //cout << get_tabs() << alloc_inst->ToString() << endl;

                    // dst = malloc(something);
                    auto &&varPtr = alloc_inst->var;
                    vector<Param> params;
                    params.push_back(Param(Param::Str, std::to_string(varPtr->type.elements_number() * 4).c_str()));
                    asm_insts.push_back(AsmCode(AsmInst::DOT_SPACE, GET_LOCAL_VAR_NAME(funcPtr->func_info.func_name, varPtr->var_idx), params, 1));
                }
            }
        }
    }

    asm_insts.push_back(AsmCode(AsmInst::DOT_TEXT));

    asm_insts.push_back(AsmCode(AsmInst::DOT_GLOBAL, {Param(Param::Str, "main")}));

    for (auto &&funcPtr : ir.functions)
    {
        int indent = 0;
        asm_insts.push_back(AsmCode(AsmInst::EMPTY, funcPtr->func_info.func_name, "", indent));
        for (auto &&bbPtr : funcPtr->all_blocks)
        {
            if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                continue;
            asm_insts.push_back(AsmCode(AsmInst::EMPTY,
                funcPtr->func_info.func_name+"_block_"+std::to_string(bbPtr->bb_idx), "",
                indent));
            for (auto &&instPtr : bbPtr->basic_block)
            {
                ++indent;
                AddAsmCodeFromLLIR(asm_insts, funcPtr->AllocationResult, funcPtr->func_info.func_name, instPtr, indent);
                --indent;
            }
        }
    }


    for (auto &&asm_inst : asm_insts)
    {
        ofs << asm_inst.ToString() << '\n';
    }

}

#endif