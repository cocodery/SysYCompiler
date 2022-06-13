#include "../common.hh"
#include "../structure/ir.hh"

class Instruction
{
public:
    string name;
    vector<string> operands;
    string label;
    string comment;
    bool labelInSepLine;
public:
    Instruction(const string& n, const vector<string>& oprnd = {},
                const string& l = "",
                bool _lisl = false,
                const string& c = "") :
        name(n), operands(oprnd), label(l), labelInSepLine(_lisl), comment(c)
    {

    }
    string ToString()
    {
        string toReturn;
        if (!label.empty())
            if (labelInSepLine)
                toReturn += label + '\n' + "    ";
            else
                toReturn += "    " + label + " ";
        else
            toReturn += "    ";
        toReturn += name;
        for (auto&& operand : operands)
            toReturn += " " + operand + ",";
        toReturn.pop_back();
        return toReturn;
    }
    void PrintInstruction()
    {
        cout << ToString() << '\n';
    }
};

class Segment
{
public:
    string name;
    vector<Instruction> instructions;
public:
    Segment(const string& n) : name(n) {}
    void Insert(const Instruction& instr)
    {
        instructions.push_back(instr);
    }
    string ToString()
    {
        string toReturn;
        toReturn += name + " SEGMENT\n";
        for (auto&& instr : instructions)
            toReturn += instr.ToString() + "\n";
        toReturn += name + " ENDS";
        return toReturn;
    }
    void PrintSegment()
    {
        cout << ToString() << '\n';
    }
};

class CodeGenerator
{
public:
    Segment dataSegment;
    Segment codeSegment;
public:
    CodeGenerator() : dataSegment("DSEG"), codeSegment("CSEG") {}
    void OutputToFile(string filePath)
    {
        // TODO
    }
    void Generate(CompUnit &ir)
    {
        cout << " - Generating DSEG:\n";
        
        cout << "   - Global Variable" << endl;
        auto&& globalScope = *ir.global_scope;
        auto&& varTable = globalScope.local_table->var_table;
        for (auto&& var : varTable)
        {
            auto&& varName = var.first;
            auto&& varContent = *var.second;
            auto&& varType = varContent.type;
            if (!varType.is_array && varType.decl_type == TypeInt)
            {
                Instruction toInsert("DW",
                                    {std::to_string(varContent.int_scalar)},
                                    (string)"GLOBAL_" + (varContent.type.is_const?"CONST_":"VAR_")+varName);
                dataSegment.Insert(toInsert);
                toInsert.PrintInstruction();
            }
        }

        cout << "   - User Functions" << endl;
        for (int i = 0, size = ir.functions.size(); i < size; ++i)
        {
            cout << "     - Funtion " << i << '\n';
            auto&& func = *ir.functions[i];
            auto&& funcInfo = func.func_info;
            auto&& funcName = funcInfo.func_name;
            
            ir.functions[i]->printFunction();
        }

        cout << " - Generating CSEG:\n";
        cout << "  - Boilerplate\n";
        vector<Instruction> boilerplate{
            Instruction("ASSUME", {"CS:CSEG", "DS:DSEG"}),
            Instruction("MOV", {"AX", "DSEG"}, "START:", true),
            Instruction("MOV", {"DS", "AX"})
        };
        for (auto&& toInsert : boilerplate)
        {
            codeSegment.Insert(toInsert);
            toInsert.PrintInstruction();
        }
        

        cout << " - Generated Code:\n";
        dataSegment.PrintSegment();
        codeSegment.PrintSegment();
        cout << "    END START\n";
    }
};