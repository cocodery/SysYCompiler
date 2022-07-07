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
        ss << DeclTypeToStr(ret_value.reg->type) << " ";
        ss << ret_value.ToString();
    } else {
        ss << "void";
    }
    return ss.str();
}

void LLIR_RET::printRetInst() {
    cout << get_tabs() << ToString() << "\n";
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

void LLIR_BIN::printBinInst() {
    cout << get_tabs() << ToString() << "\n";
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

void LLIR_FBIN::printFBinInst() {
    cout << get_tabs() << ToString() << "\n";
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

void LLIR_ICMP::printIcmpInst() {
    cout << get_tabs() << ToString() << "\n";
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

void LLIR_SITOFP::printItofInst() {
    cout << get_tabs() << ToString() << "\n";
}
