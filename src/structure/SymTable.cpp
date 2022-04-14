#include "SymTable.hh"

VarType::VarType() : is_const(false), is_array(false), is_args(false), decl_type(TypeVoid) { }

VarType::VarType(bool _const, bool _arr, bool _args, DeclType _type)
    : is_const(_const), is_array(_arr), is_args(_args), decl_type(_type) { }

int32_t VarType::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;
}

string VarType::printVarType() {
    string ret;
    if (is_args) {
        ret += DeclTypeToStr(decl_type);
        if (is_array) ret += "[]";
    }
    return ret;
}

Variable::Variable(int vi, VarType vt) : var_idx(vi), type(vt) { }

FunctionInfo::FunctionInfo() : return_type(TypeVoid) { }

FunctionInfo::FunctionInfo(string _name, DeclType _type, vector<VarType> _args)
    : func_name(_name), return_type(_type), func_args(_args) { }

bool FunctionInfo::has_args() {
    return (func_args.size() != 0);
}

void FunctionInfo::printFunction() {
    cout << DeclTypeToStr(return_type) << ' ' << func_name << " args: ";
    if (has_args()) {
        int size = func_args.size();
        for (int i = 0; i < size; ++i) {
            if (i != 0) cout << ", ";
            cout << func_args[i].printVarType();
        }
        cout << endl;
    }
    else {
        cout << "no args" << endl;
    }
    return;
}