#include "common.hh"

string get_tabs() {
    string ret;
    for (int i = 0; i < tab_num; ++i) {
        ret += string(4, ' ');
    }
    return ret;
}

string get_tabs(int tabs) {
    string ret;
    for (int i = 0; i < tabs; ++i) {
        ret += string(4, ' ');
    }
    return ret;
}

int parseNum(const char *number_str) {
    int32_t ret = 0;
    if (number_str[0] == '0' ) {
        if (number_str[1] == 'x' || number_str[1] == 'X') {
            sscanf(number_str, "%x", &ret);
        } else {
            sscanf(number_str, "%o", &ret);
        }
    } else {
        sscanf(number_str, "%d", &ret);
    }
    return ret;
}

string BinOpToStr(BinOp op) {
    if (op == ADD) return "add";
    else if (op == SUB) return "sub";
    else if (op == MUL) return "mul";
    else if (op == DIV) return "sdiv";
    else if (op == REM) return "srem";
    dbg("Unknown BinOp");
    exit(EXIT_FAILURE);
}

BinOp StrToBinOp(string op) {
    if (op == "+") return ADD;
    else if (op == "-") return SUB;
    else if (op == "*") return MUL;
    else if (op == "/") return DIV;
    else if (op == "%") return REM;
    dbg("Unknown BinOp");
    exit(EXIT_FAILURE);
}

string RelOpToStr(RelOp op) {
    if (op == LTH) return "slt";
    else if (op == LEQ) return "sle";
    else if (op == EQU) return "eq";
    else if (op == NEQ) return "ne";
    dbg("Unknown RelOp");
    exit(EXIT_FAILURE);
}

RelOp StrToRelOp(string op) {
    if (op == "<") return LTH;
    else if (op == "<=") return LEQ;
    else if (op == "==") return EQU;
    else if (op == "!=") return NEQ;
    dbg("Unknown RelOp");
    exit(EXIT_FAILURE);
}

DeclType getDeclType(string type_str) {
    if (type_str == "int") return TypeInt;
    else if (type_str == "float") return TypeFloat;
    else if (type_str == "void") return TypeVoid;
    dbg("Unknown String DeclType");
    exit(EXIT_FAILURE);
}

string DeclTypeToStr(DeclType type) {
    switch (type) {
        case TypeInt: return "i32";
        case TypeFloat: return "float";
        case TypeBool: return "i1";
        case TypeVoid: return "void";
        case TypeByte: return "i8";
        default: break;
    }
    dbg("Unknown Declare Type");
    exit(EXIT_FAILURE);
}
