#include "SymTable.hh"

VarType::VarType() : is_const(false), is_array(false), is_args(false), is_init(false), decl_type(TypeVoid) { }

VarType::VarType(bool _const, bool _arr, bool _args, DeclType _type)
    : is_const(_const), is_array(_arr), is_args(_args), decl_type(_type) { }

int32_t VarType::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;
}

string VarType::printVarTypeForArg() {
    string ret;
    if (is_args) {
        ret += DeclTypeToStr(decl_type);
        if (is_array) {
            int size = array_dims.size();
            ret += "[]";
            for(int i = 1; i < size; ++i) {
                ret += '[' + std::to_string(array_dims[i]) + ']';
            }
        }
    }
    return ret;
}

FunctionInfo::FunctionInfo() : return_type(TypeVoid) { }

FunctionInfo::FunctionInfo(string _name, DeclType _type, vector<VarType> _args)
    : func_name(_name), return_type(_type), func_args(_args) { }

bool FunctionInfo::has_args() {
    return (func_args.size() != 0);
}

void FunctionInfo::printFunctionInfo() {
    cout << DeclTypeToStr(return_type) << ' ' << func_name << " (";
    if (has_args()) {
        int size = func_args.size();
        cout << func_args[0].printVarTypeForArg();
        for (int i = 1; i < size; ++i) {
            cout << ", " << func_args[i].printVarTypeForArg();
        }
    }
    cout << ')' << endl;
    return;
}