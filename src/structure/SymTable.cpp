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
        if (is_array) ret += "[]";
    }
    return ret;
}

void VarType::printVarTypeForSym() {
    cout << '(' << DeclTypeToStr(decl_type) << ')';
    cout << ' ';
    if (is_init) {
        if (decl_type == TypeInt) {
            if (is_array){
                for (int i = 0; i < int_list.size(); ++i) {
                    cout << int_list[i] << ' ';
                }
            }
            else {
                cout << int_scalar;
            }
        }
        else if (decl_type == TypeFloat) {
            if (is_array) {
                for (int i = 0; i < float_list.size(); ++i) {
                    cout << float_list[i] << ' ';
                }
            }
            else {
                cout << float_scalar;
            }
        }
    }
    cout << endl;
}

void Variable::printVariable() {
    type.printVarTypeForSym();
}

void VariableTable::printVariableTable() {
    int size = var_table.size();
    for (int i = 0; i < size; ++i) {
        cout << var_table[i]->first << " = ";
        var_table[i]->second.printVariable();
    }
}

FunctionInfo::FunctionInfo() : return_type(TypeVoid) { }

FunctionInfo::FunctionInfo(string _name, DeclType _type, vector<VarType> _args)
    : func_name(_name), return_type(_type), func_args(_args) { }

bool FunctionInfo::has_args() {
    return (func_args.size() != 0);
}

void FunctionInfo::printFunction() {
    cout << DeclTypeToStr(return_type) << ' ' << func_name << " (";
    if (has_args()) {
        int size = func_args.size();
        for (int i = 0; i < size; ++i) {
            if (i != 0) cout << ", ";
            cout << func_args[i].printVarTypeForArg();
        }
    }
    cout << ')' << endl;
    return;
}