#include "llir.hh"

string SRC::ToString() {
    if (ctv != nullptr) {
        return ctv->ToString();
    } else if (reg != nullptr) {
        return reg->ToString();
    } else {
        dbg("Nothing in `SRC`");
    }
}

// LLVM-IR RET
string LLIR_RET::ToString() {
    std::stringstream ss;
    ss << "ret ";
    if (has_retvalue) {
        if (CTValue *ctv = ret_value.ToCTValue(); ctv != nullptr) {
            ss << DeclTypeToStr(ctv->type);
        } else {
            VirtReg *reg = ret_value.ToVirtReg();
            ss << DeclTypeToStr(reg->type);
        }
        ss << " " << ret_value.ToString();
    } else {
        ss << "void";
    }
    return ss.str();
}

// LLVM-IR BinaryOpInst
string LLIR_BIN::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    assert(dst_reg != nullptr); 
    ss << "%" << dst_reg->reg_id << " = ";
    ss << BinOpToStr(op) << " i32 ";
    ss << src1.ToString();
    ss << ", ";
    ss << src2.ToString();
    return ss.str();
}

// LLVM-IR FBinaryOpInst
string LLIR_FBIN::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    assert(dst_reg != nullptr); 
    ss << "%" << dst_reg->reg_id << " = f";
    ss << BinOpToStr(op) << " float ";
    ss << src1.ToString();
    ss << ", ";
    ss << src2.ToString();
    return ss.str();
}

// LLVM-IR Alloc Address
string LLIR_ALLOCA::ToString() {
    std::stringstream ss;
    VirtReg *var_reg = reg.ToVirtReg();
    assert(var_reg != nullptr); 
    ss << "%" << var_reg->reg_id << " = alloca ";
    ss << var->type.printVarTypeForAlc();
    ss << ", align 4";
    return ss.str();
}

// LLVM-IR Load
string LLIR_LOAD::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    VirtReg *src_reg = src.ToVirtReg();
    ss << "%" << dst_reg->reg_id << " = load ";
    ss << DeclTypeToStr(dst_reg->type) << ", ";
    ss << DeclTypeToStr(dst_reg->type) << "* ";
    if (src_reg->global) ss << "@_" << src_reg->reg_id;
    else  ss << "%" << src_reg->reg_id;
    ss << ", align 4";
    return ss.str();
}

// LLVM-IR Store
string LLIR_STORE::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    ss << "store " << DeclTypeToStr(dst_reg->type);
    ss << " " << src.ToString() << ", ";
    ss << DeclTypeToStr(dst_reg->type) << "* " << dst_reg->ToString();
    return ss.str();
}

// LLVM-IR Get-Element-Pointer
string LLIR_GEP::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    ss << "%" << dst_reg->reg_id << " = getelementptr inbounds ";
    ss << type.printVarTypeForAlc() << ", " << type.printVarTypeForAlc() << "* ";
    ss << src.ToString() << ", i32 0" << ", i32 " << off;
    return ss.str();
}

// LLVM-IR Int-Compare
string LLIR_ICMP::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    assert(dst_reg != nullptr); 
    ss << "%" << dst_reg->reg_id << " = icmp " << RelOpToStr(op) << " ";
    ss << src1.ToString();
    ss << ", ";
    ss << src2.ToString();
    return ss.str();
}

// LLVM-IR Signed-Int-TO-Float-Point
string LLIR_SITOFP::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg(), *src_reg = src.ToVirtReg();
    assert(dst_reg != nullptr && src_reg != nullptr);
    ss << "%" << dst_reg->reg_id << " = sitofp i32 ";
    ss << "%" << src_reg->reg_id << " to float";
    return ss.str();
}

// LLVM-IR Float-Point-TO-Signed-Int
string LLIR_FPTOSI::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg(), *src_reg = src.ToVirtReg();
    assert(dst_reg != nullptr && src_reg != nullptr);
    ss << "%" << dst_reg->reg_id << " = fptosi float ";
    ss << "%" << src_reg->reg_id << " to i32";
    return ss.str();
}
