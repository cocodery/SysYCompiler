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
            ss << reg->type.printVarTypeForAlc();
        }
        ss << " " << ret_value.ToString();
    } else {
        ss << "void";
    }
    return ss.str();
}

// LLVM_IR Branch
string LLIR_BR::ToString() {
    std::stringstream ss;
    ss << "br ";
    if (has_cond) {
        ss << "i1 " << cond.ToString() << ", ";
        ss << "label %Block" << tar_true << ", ";
        ss << "label %Block" << tar_false;
    } else {
        ss << "label %Block" << tar_true;
    }
    return ss.str();
}

// LLVM-IR BinaryOpInst
string LLIR_BIN::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    assert(dst_reg != nullptr); 
    ss << dst_reg->ToString() << " = ";
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
    ss << dst_reg->ToString() << " = f";
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
    ss << var_reg->ToString() << " = alloca ";
    ss << var->type.printVarTypeForAlc();
    ss << ", align 4";
    return ss.str();
}

// LLVM-IR Load
string LLIR_LOAD::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    VirtReg *src_reg = src.ToVirtReg();
    ss << dst_reg->ToString() << " = load ";
    ss << dst_reg->type.printVarTypeForAlc() << ", ";
    ss << dst_reg->type.printVarTypeForAlc() << "* ";
    ss << src_reg->ToString();
    ss << ", align 4";
    return ss.str();
}

// LLVM-IR Store
string LLIR_STORE::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    ss << "store " << dst_reg->type.printVarTypeForAlc();
    ss << " " << src.ToString() << ", ";
    ss << dst_reg->type.printVarTypeForAlc() << "* " << dst_reg->ToString();
    ss << ", align 4";
    return ss.str();
}

// LLVM-IR Get-Element-Pointer
string LLIR_GEP::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    ss << dst_reg->ToString() << " = getelementptr inbounds ";
    ss << type.printVarTypeForAlc() << ", " << type.printVarTypeForAlc() << "* ";
    ss << src.ToString() << ", i32 " << off.ToString();
    VarType var_type = dst_reg->type;
    if (var_type.is_array && var_type.array_dims.size() > 0 && var_type.array_dims[0] != -1) ss << ", i32 0";
    return ss.str();
}

// LLVM-IR Int-Compare
string LLIR_ICMP::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg();
    assert(dst_reg != nullptr); 
    ss << dst_reg->ToString() << " = icmp " << RelOpToStr(op) << " i32 ";
    ss << src1.ToString();
    ss << ", ";
    ss << src2.ToString();
    return ss.str();
}

string LLIR_CALL::ToString() {
    std::stringstream ss;
    DeclType ret_type = func_info->return_type;
    if (ret_type != TypeVoid) {
        VirtReg *dst_reg = dst.ToVirtReg();
        assert(dst_reg != nullptr); 
        ss << dst_reg->ToString() << " = ";
    }
    ss << "call " << DeclTypeToStr(func_info->return_type) << " ";
    ss << "@" << func_info->func_name << "(";
    int size = args.size();
    for (int i = 0; i < size; ++i) {
        if (i != 0) ss << ", ";
        if (CTValue *ctv = args[i].ToCTValue(); ctv != nullptr) {
            ss << DeclTypeToStr(ctv->type);
        } else {
            VirtReg *reg = args[i].ToVirtReg();
            ss << reg->type.printVarTypeForAlc();
            if (reg->type.is_array) ss << "*";
        }
        ss << " " << args[i].ToString();
    }
    ss << ")";
    return ss.str();
}

string LLIR_ZEXT::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg(), *src_reg = src.ToVirtReg();
    assert(dst_reg != nullptr && src_reg != nullptr);
    ss << dst_reg->ToString() << " = zext i1 ";
    ss << src_reg->ToString() << " to i32";
    return ss.str();
}

// LLVM-IR Signed-Int-TO-Float-Point
string LLIR_SITOFP::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg(), *src_reg = src.ToVirtReg();
    assert(dst_reg != nullptr && src_reg != nullptr);
    ss << dst_reg->ToString() << " = sitofp i32 ";
    ss << src_reg->ToString() << " to float";
    return ss.str();
}

// LLVM-IR Float-Point-TO-Signed-Int
string LLIR_FPTOSI::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg(), *src_reg = src.ToVirtReg();
    assert(dst_reg != nullptr && src_reg != nullptr);
    ss << dst_reg->ToString() << " = fptosi float ";
    ss << src_reg->ToString() << " to i32";
    return ss.str();
}

// LLVM-IR Xor
string LLIR_XOR::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg(), *src_reg = src.ToVirtReg();
    assert(dst_reg != nullptr && src_reg != nullptr);
    ss << dst_reg->ToString() << " = xor i1 ";
    ss << src_reg->ToString() << ", true";
    return ss.str();
}

// LLVM-IR Bit-Cast
string LLIR_BC::ToString() {
    std::stringstream ss;
    VirtReg *dst_reg = dst.ToVirtReg(), *src_reg = src.ToVirtReg();
    assert(dst_reg != nullptr && src_reg != nullptr);
    ss << dst_reg->ToString() << " = bitcast i32* ";
    ss << src_reg->ToString() << " to i8*";
    return ss.str();
}
