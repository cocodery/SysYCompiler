#include "value.hh"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"

CTValue::CTValue(DeclType t, int32_t iv, float fv): type(t), int_value(iv), float_value(fv) { }

string CTValue::ToString() {
    std::stringstream ss;
    if (type == TypeInt) {
        ss << int_value;
    } else if (type == TypeFloat) {
        uint64_t uint64_value = reinterpret_cast<uint64_t&>(float_value);
        ss << "0x" << std::hex << uint64_value;
    } else if (type == TypeBool) {
        ss << int_value;
    } else if (type == TypeByte) {
        ss << int_value;
    } else {
        // dbg("UnExpected DeclType");
    }
    return ss.str();
}

#pragma clang diagnostic pop
