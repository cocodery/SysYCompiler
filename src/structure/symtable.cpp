#include "symtable.hh"

VarType::VarType() : is_const(false), is_array(false), is_args(false), decl_type(TypeVoid) { }

VarType::VarType(DeclType _type) 
    : is_const(false), is_array(false), is_args(false), decl_type(_type) { }

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
    // dbg(array_dims);
    int32_t step = 1, ret = 0;
    size_t next = array_dims.size() - 1;
    for (size_t i = arr_idx.size() - 1; i < arr_idx.size(); --i) {
        ret += arr_idx[i] * step;
        if (next < array_dims.size()) {
        step *= array_dims[next];
        --next;
        }
    }
    // dbg(ret);
    return ret;
}

vector<int32_t> VarType::get_dims() {
    vector<int32_t> ret;
    int32_t size = array_dims.size();
    int32_t dim = 1;
    for (int i = size - 1; i >= 0; --i) {
        ret.push_back(dim);
        dim *= array_dims[i];
    }
    std::sort(ret.rbegin(), ret.rend());
    return ret;
}

VarType VarType::move_down() {
    VarType ret = *this;
    ret.array_dims.erase(ret.array_dims.begin());
    ret.is_array = (ret.array_dims.size() == 0) ? false : true;
    ret.decl_type = ret.is_array ? ret.decl_type : DeclType(ret.decl_type - 3);
    return ret;
}

string VarType::printVarTypeForAlc() {
    std::stringstream ss;
    if (is_array && array_dims.size() > 0 && array_dims[0] != -1) {
        ss << "[" << elements_number() << " x " << DeclTypeToStr(decl_type) << "]";
    } else {
        ss << DeclTypeToStr(decl_type);
    }
    return ss.str();
}

string VarType::printVarTypeForArg() {
    std::stringstream ss;
    if (is_array) {
        ss << DeclTypeToStr(decl_type) << "*";
    } else {
        ss << DeclTypeToStr(decl_type);
    }
    return ss.str();
}

FunctionInfo::FunctionInfo() : return_type(TypeVoid), call_count(0) { }

FunctionInfo::FunctionInfo(string _name, DeclType _type, vector<pair<string, VarType>> _args)
    : is_used(false), func_name(_name), return_type(_type), func_args(_args), call_count(0) { }

void Variable::printVariable(string var_name) {
    if (type.is_const == true) // cout << "const ";
    // cout << DeclTypeToStr(type.decl_type) << ' ' << var_name;
    if (type.is_array) {
        for (int i = 0; i < type.array_dims.size(); ++i) {
            // cout << '[' << type.array_dims[i] << ']';
        }
    }
    // cout << ' ';
    if (type.is_const) {
        // cout << "= ";
        if (!type.is_array) {
            if (type.decl_type == TypeInt) { 
                // cout << int_scalar;
            }
            else if (type.decl_type == TypeFloat) {
                // cout << float_scalar;
            }
        } else {
            // cout << "{ ";
            if (type.decl_type == TypeInt) {
                for (int i = 0; i < int_list.size(); ++i) {
                    // cout << int_list[i] << ' ';
                }
            } else if (type.decl_type == TypeFloat) {
                for (int i = 0; i < float_list.size(); ++i) {
                    // cout << float_list[i] << ' ';
                }
            }
            // cout << '}';
        }
    }
    // cout << "; // index = " << var_idx <<  endl;
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
        // cout << get_tabs();
        var_table[i].second->printVariable(var_table[i].first);
    }
}

bool FunctionInfo::has_args() {
    return (func_args.size() != 0);
}

string FunctionInfo::printFunctionInfo(bool islib) {
    std::stringstream ss;
    if (islib) {
        ss << "declare "; 
    } else {
        ss << "define ";
    }
    ss << DeclTypeToStr(return_type) << " @" << func_name << "(";
    if (has_args()) {
        ss << func_args[0].second.printVarTypeForArg() << " %r0";
        for (int i = 1; i < func_args.size(); ++i) {
            ss << ", " << func_args[i].second.printVarTypeForArg() << " %r" << i;
        }
    }
    ss << ") ";
    if (!islib) {
        ss << "{";
    }
    return ss.str();
}