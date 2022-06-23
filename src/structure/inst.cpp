#include "inst.hh"

string UnaryOpInst::ToString() {
    std::stringstream ss;
    ss << "uop" << op.get_op() << " ";
    ss << "reg" << dst.reg_id << ", reg" << src.reg_id;
    return ss.str();
}

string BinaryOpInst::ToString() {
    std::stringstream ss;
    ss << "bop" << op.get_op() << " ";
    ss << "reg" << dst.reg_id << ", reg" << src1.reg_id << ", reg" << src2.reg_id;
    return ss.str();
}

string ReturnInst::ToString() {
    std::stringstream ss;
    ss << "return ";
    if (has_retvalue) {
        ss << "reg" << dst.reg_id;
    }
    return ss.str();
}

string LoadNumber::ToString() {
    std::stringstream ss;
    ss << "ld.c reg" << dst.reg_id << ", ";
    if (src.type == TypeInt) {
        ss << src.int_value;
    } else {
        ss << src.float_value;
    }
    return ss.str();
}

string StoreMem::ToString() {
    std::stringstream ss;
    ss << "st [reg" << dst.reg_id << "], ";
    ss << "reg" << src.reg_id;
    return ss.str();
}

string LoadAddress::ToString() {
    std::stringstream ss;
    ss << "ld.a reg" << dst.reg_id << ", ";
    ss << "*" << variable->var_idx;
    return ss.str();
}

string LoadValue::ToString() {
    std::stringstream ss;
    ss << "ld.v reg" << dst.reg_id << ", ";
    ss << "[reg" << src.reg_id << "]";
    return ss.str();
}

string LoadOffset::ToString() {
    std::stringstream ss;
    ss << "ld.o reg" << dst.reg_id << ", ";
    ss << "reg" << addr.reg_id << ", ";
    ss << "reg" << off.reg_id << ", len = " << size;
    return ss.str();
}

string JzeroInst::ToString() {
    std::stringstream ss;
    ss << "jz reg" << cond.reg_id << ", ";
    ss << "bb_" << bb_idx;
    return ss.str();
}

string JnzroInst::ToString() {
    std::stringstream ss;
    ss << "jnz reg" << cond.reg_id << ", ";
    ss << "bb_" << bb_idx;
    return ss.str();
}

string JumpInst::ToString() {
    std::stringstream ss;
    ss << "jump bb_" << bb_idx;
    return ss.str();
}

string LoadParam::ToString() {
    std::stringstream ss;
    ss << "ld.p reg" << dst.reg_id << ", ";
    ss << "reg" << src.reg_id;
    return ss.str();
}

string CallFuntion::ToString() {
    std::stringstream ss;
    ss << "call ";
    if (has_ret) {
        ss << "reg" << ret_dst.reg_id << ", ";
    }
    ss << func_name;
    return ss.str();
}

void UnaryOpInst::printuOpInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void BinaryOpInst::printbOpInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void ReturnInst::printRetInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void LoadNumber::printLdcInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void StoreMem::printStmInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void LoadAddress::printLdaInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void LoadValue::printLdvInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void LoadOffset::printLdoInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void JzeroInst::printJzoInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void JnzroInst::printJnzInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void JumpInst::printJmpInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void LoadParam::printLdpInst() {
    cout << get_tabs() << ToString() << ";\n";
}

void CallFuntion::printCalInst() {
    cout << get_tabs() << ToString() << ";\n";
}
