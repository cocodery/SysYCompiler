#include "symtable.hh"

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

int32_t VarType::get_index(vector<int32_t>& arr_idx) {
    dbg(array_dims);
    int32_t step = 1, ret = 0;
    size_t next = array_dims.size() - 1;
    for (size_t i = arr_idx.size() - 1; i < arr_idx.size(); --i) {
        ret += arr_idx[i] * step;
        if (next < array_dims.size()) {
        step *= array_dims[next];
        --next;
        }
    }
    dbg(ret);
    return ret;
}

void VarType::printVarTypeForArg() {
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
    cout << ret;
}

FunctionInfo::FunctionInfo() : return_type(TypeVoid) { }

FunctionInfo::FunctionInfo(string _name, DeclType _type, vector<VarType> _args)
    : func_name(_name), return_type(_type), func_args(_args) { }

void Variable::printVariable(string var_name) {
    if (type.is_const == true) cout << "const ";
    cout << DeclTypeToStr(type.decl_type) << ' ' << var_name;
    if (type.is_array) {
        for (int i = 0; i < type.array_dims.size(); ++i) {
            cout << '[' << type.array_dims[i] << ']';
        }
    }
    cout << ' ';
    if (type.is_const) {
        cout << "= ";
        if (!type.is_array) {
            if (type.decl_type == TypeInt) cout << int_scalar;
            else if (type.decl_type == TypeFloat) cout << float_scalar;
        } else {
            cout << "{ ";
            if (type.decl_type == TypeInt) {
                for (int i = 0; i < int_list.size(); ++i) {
                    cout << int_list[i] << ' ';
                }
            } else if (type.decl_type == TypeFloat) {
                for (int i = 0; i < float_list.size(); ++i) {
                    cout << float_list[i] << ' ';
                }
            }
            cout << '}';
        }
    }
    cout << "; // index = " << var_idx <<  endl;
}

bool VariableTable::findInCurTable(string name) {
    for (auto one: var_table) {
        if (name == one.first) {
            return true;
        }
    }
    return false;
}

Variable *VariableTable::getInCurTable(string name) {
    for (auto one: var_table) {
        if (name == one.first) {
            return one.second;
        }
    }
    return nullptr;
}

void VariableTable::printVaribaleTable() {
    int size = var_table.size();
    for (int i = 0; i < size; ++i) {
        cout << get_tabs();
        var_table[i].second->printVariable(var_table[i].first);
    }
}

bool FunctionInfo::has_args() {
    return (func_args.size() != 0);
}

void FunctionInfo::printFunctionInfo() {
    cout << DeclTypeToStr(return_type) << ' ' << func_name << " (";
    if (has_args()) {
        int size = func_args.size();
        func_args[0].printVarTypeForArg();
        for (int i = 1; i < size; ++i) {
            cout << ", ";
            func_args[i].printVarTypeForArg();
        }
    }
    cout << ')' << endl;
    return;
}