#include "inst.hh"

void UnaryOpInst::printuOpInst() {
    cout << get_tabs() << "uop" << op.get_op() << " ";
    cout << "reg" << dst.reg_id << ", reg" << src.reg_id << ";\n";
}

void BinaryOpInst::printbOpInst() {
    cout << get_tabs() << "bop" << op.get_op() << " ";
    cout << "reg" << dst.reg_id << ", reg" << src1.reg_id << ", reg" << src2.reg_id << ";\n";
}

void ReturnInst::printRetInst() {
    cout << get_tabs() << "return ";
    if (has_retvalue) {
        cout << "reg" << dst.reg_id;
    }
    cout << ";\n";
}

void LoadNumber::printLdcInst() {
    cout << get_tabs() << "ld.c reg" << dst.reg_id << ", ";
    if (src.type == TypeInt) {
        cout << src.int_value;
    } else {
        cout << src.float_value;
    }
    cout << ";\n";
}

void StoreMem::printStmInst() {
    cout << get_tabs() << "st [reg" << dst.reg_id << "], ";
    cout << "reg" << src.reg_id;
    cout << ";\n";
}

void LoadAddress::printLdaInst() {
    cout << get_tabs() << "ld.a reg" << dst.reg_id << ", ";
    cout << "*" << variable->var_idx;
    cout << ";\n";
}

void LoadValue::printLdvInst() {
    cout << get_tabs() << "ld.v reg" << dst.reg_id << ", ";
    cout << "[reg" << src.reg_id << "]";
    cout << ";\n";
}
