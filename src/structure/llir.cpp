#include "llir.hh"

// LLVM-IR RET
string LLIR_RET::ToString() {
    std::stringstream ss;
    ss << "ret ";
    if (has_retvalue) {
        if (CTValue *ctv = dynamic_cast<CTValue *>(ret_value); ctv != nullptr) {
            ss << DeclTypeToStr(ctv->type) << " ";
            if (ctv->type == TypeInt) {
                ss << ctv->int_value;
            } else {
                ss << ctv->float_value;
            }
        } else {
            VirtReg *reg = dynamic_cast<VirtReg *>(ret_value);
            ss << DeclTypeToStr(reg->type) << " ";
            ss << "%r" << reg->reg_id;
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

// LLVM-IR FBinaryOpInst
