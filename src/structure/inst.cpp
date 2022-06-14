#include "inst.hh"

void UnaryOpInst::printuOpInst() {
    cout << "\t" << "uop" << op.get_op() << " ";
    cout << "reg" << dst.reg_id << ", reg" << src.reg_id << ";\n";
}

void BinaryOpInst::printbOpInst() {
    cout << "\t" << "bop" << op.get_op() << " ";
    cout << "reg" << dst.reg_id << ", reg" << src1.reg_id << ", reg" << src2.reg_id << ";\n";
}

void ReturnInst::printRetInst() {
    cout << "\t" << "return ";
    if (has_retvalue) {
        cout << "reg" << dst.reg_id;
    }
    cout << ";\n";
}

void LoadNumber::printLdcInst() {
    cout << "\t" << "ld.c reg" << dst.reg_id << ", ";
    if (src.type == TypeInt) {
        cout << src.int_value;
    } else {
        cout << src.float_value;
    }
    cout << ";\n";
}

void AssignInst::printAssInst() {
    cout << "\t" << "ld.r reg" << dst.reg_id << ", ";
    cout << "reg" << src.reg_id;
    cout << ";\n";
}

void LoadAddress::printLadInst() {
    cout << "\t" << "ld.v reg" << dst.reg_id << ", ";
    cout << "*" << variable->var_idx;
    cout << ";\n";
}
