#include "common.hh"

int parseNum(const char *number_str) {
    int64_t ret = 0;
    if (number_str[0] == '0' ) {
        if (number_str[1] == 'x' || number_str[1] == 'X')
            ret = strtoul(number_str, nullptr, 16);
        else ret = strtoul(number_str, nullptr, 8);
    }
    else ret = strtoul(number_str, nullptr, 10);

    if (ret > 2147483648ll) {
        dbg("interger is too big to fit in int32_t");
        exit(EXIT_FAILURE);
    }
    return static_cast<int32_t>(ret);
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
