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
    else return TypeVoid;
}

