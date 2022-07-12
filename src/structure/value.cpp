#include "value.hh"

IRValue::IRValue(VarType t, VirtReg vg, bool ass) : type(t), reg(vg), is_left_value(ass) { }

CTValue::CTValue(DeclType t, int32_t iv, float fv): type(t), int_value(iv), float_value(fv) { }

string CTValue::ToString() {
    std::stringstream ss;
    if (type == TypeInt) {
        ss << int_value;
    } else if (type == TypeFloat) {
        ss << float_value;
    } else {
        dbg("UnExpected DeclType");
    }
    return ss.str();
}

// override operators for `CTValue`
CTValue operator + (CTValue lhs, CTValue rhs) {
    if (lhs.type == rhs.type) {
        if (lhs.type == TypeInt) {
            return CTValue(TypeInt, lhs.int_value + rhs.int_value, 0);
        } else if (lhs.type == TypeFloat) {
            return CTValue(TypeFloat, 0, lhs.float_value + rhs.float_value);
        }
    } else {
        if (lhs.type == TypeInt) {
            return CTValue(TypeFloat, 0, lhs.int_value + rhs.float_value);
        } else if (lhs.type == TypeFloat) {
            return CTValue(TypeFloat, 0, lhs.float_value + rhs.int_value);
        }
    }
}

CTValue operator - (CTValue lhs, CTValue rhs) {
    if (lhs.type == rhs.type) {
        if (lhs.type == TypeInt) {
            return CTValue(TypeInt, lhs.int_value - rhs.int_value, 0);
        } else if (lhs.type == TypeFloat) {
            return CTValue(TypeFloat, 0, lhs.float_value - rhs.float_value);
        }
    } else {
        if (lhs.type == TypeInt) {
            return CTValue(TypeFloat, 0, lhs.int_value - rhs.float_value);
        }  else if (lhs.type == TypeFloat) {
            return CTValue(TypeFloat, 0, lhs.float_value - rhs.int_value);
        }
    }
}

CTValue operator * (CTValue lhs, CTValue rhs) {
    if (lhs.type == rhs.type) {
        if (lhs.type == TypeInt) {
            return CTValue(TypeInt, lhs.int_value * rhs.int_value, 0);
        } else if (lhs.type == TypeFloat) {
            return CTValue(TypeFloat, 0, lhs.float_value * rhs.float_value);
        }
    } else {
        if (lhs.type == TypeInt) {
            return CTValue(TypeFloat, 0, lhs.int_value * rhs.float_value);
        } else if (lhs.type == TypeFloat) {
            return CTValue(TypeFloat, 0, lhs.float_value * rhs.int_value);
        }
    }
}

CTValue operator / (CTValue lhs, CTValue rhs) {
    if (lhs.type == rhs.type) {
        if (lhs.type == TypeInt) {
            return CTValue(TypeInt, lhs.int_value - rhs.int_value, 0);
        } else if (lhs.type == TypeFloat) {
            return CTValue(TypeFloat, 0, lhs.float_value - rhs.float_value);
        }
    } else {
        if (lhs.type == TypeInt) {
            return CTValue(TypeFloat, 0, lhs.int_value - rhs.float_value);
        } else if (lhs.type == TypeFloat) {
            return CTValue(TypeFloat, 0, lhs.float_value - rhs.int_value);
        }
    }
}

CTValue operator % (CTValue lhs, CTValue rhs) {
    if (lhs.type == TypeInt && rhs.type == TypeInt) {
        return CTValue(TypeInt, lhs.int_value % rhs.int_value, 0);
    } else {
        dbg("invalid operands of types to binary 'operator%'");
        exit(EXIT_FAILURE);
    }
}

CTValue operator - (CTValue rhs) {
    if (rhs.type == TypeInt) {
        return CTValue(TypeInt, -rhs.int_value, -rhs.float_value);
    } else {
        return CTValue(TypeFloat, -rhs.int_value, -rhs.float_value);
    }
}

CTValue operator ! (CTValue rhs) {
    if (rhs.type == TypeInt) {
        return CTValue(TypeInt, !rhs.int_value, !rhs.float_value);
    } else {
        return CTValue(TypeFloat, !rhs.int_value, !rhs.float_value);
    }
}