#include "inst.hh"

void UnaryOpInst::printuOpInst() {
    cout << "\t" << "uop" << op.get_op() << " ";
    cout << "reg" << dst.reg_id << ", reg" << src.reg_id << ";\n";
}

void ReturnInst::printRetInst() {
    cout << "\t" << "return ";
    if (has_retvalue) {
        cout << "reg" << dst.reg_id;
    }
    cout << ";\n";
}

void LoadNumber::printLdcInst() {
    cout << "\t" << "load reg" << dst.reg_id << ", ";
    if (src.type == TypeInt) {
        cout << src.int_value;
    } else {
        cout << src.float_value;
    }
    cout << ";\n";
}
