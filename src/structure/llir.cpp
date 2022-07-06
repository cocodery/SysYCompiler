#include "llir.hh"

// LLVM-IR RET
string LLIR_RET::ToString() {
    std::stringstream ss;
    ss << "ret ";
    if (has_retvalue) {
        if (CTValue *ctv = dynamic_cast<CTValue *>(ret_value); ctv != nullptr) {
            ss << ctv->ToString();
        } else {
            VirtReg *reg = dynamic_cast<VirtReg *>(ret_value);
            ss << reg->ToString();
        }
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
    ss << "%" << dst->reg_id << " = " << BinOpToStr(op) << " ";
    if (CTValue *ctv1 = dynamic_cast<CTValue *>(src1); ctv1 != nullptr) {
        ss << ctv1->ToString();
    } else {
        VirtReg *reg1 = dynamic_cast<VirtReg *>(src1);
        ss << reg1->ToString();
    }
    if (CTValue *ctv2 = dynamic_cast<CTValue *>(src2); ctv2 != nullptr) {
        ss << ctv2->ToString();
    } else {
        VirtReg *reg2 = dynamic_cast<VirtReg *>(src2);
        ss << reg2->ToString();
    }
    return ss.str();
}

void LLIR_BIN::printBinInst() {
    cout << get_tabs() << ToString() << "\n";
}

// LLVM-IR FBinaryOpInst
string LLIR_FBIN::ToString() {
    std::stringstream ss;
    ss << "%" << dst->reg_id << " = f" << BinOpToStr(op) << " ";
    if (CTValue *ctv1 = dynamic_cast<CTValue *>(src1); ctv1 != nullptr) {
        ss << ctv1->ToString();
    } else {
        VirtReg *reg1 = dynamic_cast<VirtReg *>(src1);
        ss << reg1->ToString();
    }
    if (CTValue *ctv2 = dynamic_cast<CTValue *>(src2); ctv2 != nullptr) {
        ss << ctv2->ToString();
    } else {
        VirtReg *reg2 = dynamic_cast<VirtReg *>(src2);
        ss << reg2->ToString();
    }
    return ss.str();
}

void LLIR_FBIN::printFBinInst() {
    cout << get_tabs() << ToString() << "\n";
}
