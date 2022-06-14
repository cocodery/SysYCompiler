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
        if (!comment.empty())
            toReturn += " ; " + comment; 
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
    Segment extraSegment;
    Segment codeSegment;
public:
    CodeGenerator() : dataSegment("DSEG"), extraSegment("ESEG"), codeSegment("CSEG") {}
    void OutputToFile(string filePath)
    {
        // TODO
    }
    void AddInfoFromScope(const Scope& scope, bool iterateElems = true)
    {
        auto&& varTable = scope.local_table->var_table;
        auto&& elementPtrs = *scope.elements;

        for (auto&& var : varTable)
        {
            auto&& varName = var.first;
            auto&& varContent = *var.second;
            auto&& varType = varContent.type;
            if (!varType.is_array && varType.decl_type == TypeInt)
            {
                Instruction toInsert("DW",
                                    {std::to_string(varContent.int_scalar % 65536)});
                dataSegment.Insert(toInsert);
                toInsert.PrintInstruction();
            }
        }

        if (!iterateElems)
            return;

        for (auto&& elementPtr : elementPtrs)
        {
            if (Scope *scope_node = dynamic_cast<Scope *>(elementPtr); scope_node != nullptr)
            {
                AddInfoFromScope(*scope_node);
            }
            else
            {
                BasicBlock *bb_node = dynamic_cast<BasicBlock *>(elementPtr);
                AddInfoFromBasicBlock(*bb_node);
            }
        }
    }
    void AddInfoFromBasicBlock(const BasicBlock& basicBlock)
    {
        for (auto inst: basicBlock.basic_block)
        {
            vector<Instruction> insts;
            Case (ReturnInst, ret_inst, inst)
            {
                ret_inst->printRetInst();
            }
            Case (LoadNumber, ldc_inst, inst)
            {
                if (ldc_inst->src.type != TypeInt)
                    continue;
                insts.push_back(Instruction(
                    "MOV", {
                        "ES:[" + std::to_string(ldc_inst->dst.reg_id << 1) + "]",
                        "WORD PTR " + std::to_string(ldc_inst->src.int_value % 65536)},
                    "",
                    false,
                    "ld.c reg" + std::to_string(ldc_inst->dst.reg_id) + ", " + std::to_string(ldc_inst->src.int_value % 65536)));
            }
            Case (UnaryOpInst, uop_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AX",
                        "ES:[" + std::to_string(uop_inst->src.reg_id << 1) + "]"},
                    "",
                    false,
                    "uop" + uop_inst->op.get_op() + " reg" + std::to_string(uop_inst->dst.reg_id) + ", reg" + std::to_string(uop_inst->src.reg_id)));
                switch(uop_inst->op.unary_op)
                {
                case UnaryOp::Type::NOT:
                    insts.push_back(Instruction("NOT", {"AX"}));
                    break;
                case UnaryOp::Type::NEG:
                    insts.push_back(Instruction("NEG", {"AX"}));
                    break;
                case UnaryOp::Type::POS:
                    insts.push_back(Instruction("POS", {"AX"}));
                    break;
                }
                insts.push_back(Instruction("MOV", {"ES:[" + std::to_string(uop_inst->dst.reg_id << 1) + "]", "AX"}));
            }
            Case (BinaryOpInst, bop_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AX",
                        "ES:[" + std::to_string(bop_inst->src1.reg_id << 1) + "]"},
                    "",
                    false,
                    "bop" + bop_inst->op.get_op() + " reg" + std::to_string(bop_inst->dst.reg_id) + ", reg" + std::to_string(bop_inst->src1.reg_id) + ", reg" + std::to_string(bop_inst->src2.reg_id)));
                switch(bop_inst->op.bin_op)
                {
                case BinaryOp::Type::ADD:
                    insts.push_back(Instruction("ADD", {"AX", "ES:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"}));
                    break;
                case BinaryOp::Type::SUB:
                    insts.push_back(Instruction("SUB", {"AX", "ES:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"}));
                    break;
                case BinaryOp::Type::MUL:
                    break;
                case BinaryOp::Type::DIV:
                    break;
                case BinaryOp::Type::MOD:
                    break;
                case BinaryOp::Type::LTH:
                    break;
                case BinaryOp::Type::LEQ:
                    break;
                case BinaryOp::Type::EQU:
                    break;
                case BinaryOp::Type::NEQ:
                    break;
                case BinaryOp::Type::AND:
                    break;
                case BinaryOp::Type::ORR:
                    break;
                }
                insts.push_back(Instruction("MOV", {"ES:[" + std::to_string(bop_inst->dst.reg_id << 1) + "]", "AX"}));
            }
            Case (AssignInst, ass_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AX", 
                        "ES:[" + std::to_string(ass_inst->src.reg_id << 1) + "]"},
                    "",
                    false,
                    "ld.r reg" + std::to_string(ass_inst->dst.reg_id) + ", reg" + std::to_string(ass_inst->src.reg_id)));
                insts.push_back(Instruction("MOV", {"BX", "ES:[" + std::to_string(ass_inst->dst.reg_id << 1) + "]"}));
                insts.push_back(Instruction("MOV", {"DS:[BX]", "AX"}));
            }
            Case (LoadAddress, lad_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AX",
                        "DS:[" + std::to_string(lad_inst->variable->var_idx << 1) + "]"},
                    "",
                    false,
                    "ld.v reg" + std::to_string(lad_inst->dst.reg_id) + ", *" + std::to_string(lad_inst->variable->var_idx)));
                insts.push_back(Instruction("MOV", {"ES:[" + std::to_string(lad_inst->dst.reg_id << 1) + "]", "AX"}));
            }
            for (auto&& toInsert : insts)
            {
                codeSegment.Insert(toInsert);
                toInsert.PrintInstruction();
            }
        }
    }
    void Generate(const CompUnit &ir)
    {
        cout << " - Boilerplate\n";
        vector<Instruction> boilerplate{
            Instruction("ASSUME", {"CS:CSEG", "DS:DSEG", "ES:ESEG"}),
            Instruction("MOV", {"AX", "DSEG"}, "START:", true),
            Instruction("MOV", {"DS", "AX"}),
            Instruction("MOV", {"AX", "ESEG"}),
            Instruction("MOV", {"ES", "AX"})
        };
        for (auto&& toInsert : boilerplate)
        {
            codeSegment.Insert(toInsert);
            toInsert.PrintInstruction();
        }


        cout << " - Global Variable" << endl;
        auto&& globalScope = *ir.global_scope;
        AddInfoFromScope(globalScope, false);

        cout << " - User Functions" << endl;
        for (int i = 0, size = ir.functions.size(); i < size; ++i)
        {
            cout << "   - Funtion " << i << '\n';
            auto&& func = *ir.functions[i];
            auto&& funcInfo = func.func_info;
            auto&& funcName = funcInfo.func_name;

            auto&& mainScope = *func.main_scope;
            AddInfoFromScope(mainScope);
        }

        cout << " - Generated Code:\n";
        dataSegment.PrintSegment();
        extraSegment.PrintSegment();
        codeSegment.PrintSegment();
        cout << "    END START\n";
    }
};