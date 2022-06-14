#include "common.hh"

string get_tabs() {
    string ret;
    for (int i = 0; i <= tab_num; ++i) {
        ret += "\t";
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

DeclType getDeclType(string type_str) {
    if (type_str == "int") return TypeInt;
    else if (type_str == "float") return TypeFloat;
    else if (type_str == "void") return TypeVoid;
    dbg("Unknown String DeclType");
    exit(EXIT_FAILURE);
}

string DeclTypeToStr(DeclType type) {
    switch (type) {
        case TypeInt: return "int";
        case TypeFloat: return "float";
        case TypeVoid: return "void";
        default: break;
    }
    dbg("Unknown Declare Type");
    exit(EXIT_FAILURE);
}
