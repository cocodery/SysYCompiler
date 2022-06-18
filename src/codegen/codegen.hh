#include "../common.hh"
#include "../structure/ir.hh"

vector<int> firstAddress{0};

class Instruction
{
public:
    string name;
    vector<string> operands;
    string label;
    string comment;
    bool labelInSepLine;
    int tabs;
public:
    Instruction(const string& n, const vector<string>& oprnd = {},
                const string& l = "",
                bool _lisl = false,
                const string& c = "",
                int t = tab_num) :
        name(n), operands(oprnd), label(l), labelInSepLine(_lisl), comment(c), tabs(t)
    {

    }
    string ToString()
    {
        string toReturn;
        if (!label.empty())
            if (labelInSepLine)
                toReturn += label + '\n' + get_tabs(tabs);
            else
                toReturn += get_tabs(tabs) + label + " ";
        else
            toReturn += get_tabs(tabs);
        toReturn += name;
        if (!operands.empty())
        {
            for (auto&& operand : operands)
                toReturn += " " + operand + ",";
            toReturn.pop_back();
        }
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
    void OutputToFile(std::ostream& file)
    {
        // TODO
        file << dataSegment.ToString() << "\n";
        file << extraSegment.ToString() << "\n";
        file << codeSegment.ToString() << "\n";
        file << "    END START\n";
    }
    void AddInfoFromScope(const Scope& scope, bool iterateElems = true)
    {
        ++tab_num;

        auto&& varTable = scope.local_table->var_table;
        auto&& elementPtrs = *scope.elements;

        for (auto&& var : varTable)
        {
            auto&& varName = var.first;
            auto&& varContent = *var.second;
            auto&& varType = varContent.type;
            if (varType.decl_type == TypeInt)
            {
                Instruction* toInsert;
                if (varType.is_array)
                {
                    firstAddress.push_back(firstAddress.back() + (varType.elements_number() << 1));
                    if (varType.is_const)
                    {
                        string arrStr;
                        for (size_t i = 0, siz = varContent.int_list.size(); i < siz; ++i)
                            arrStr += std::to_string(varContent.int_list[i]) + ", ";
                        arrStr.pop_back();arrStr.pop_back();
                        toInsert = new Instruction("DW", {arrStr}, "", false, "", 1);
                    }
                    else
                        toInsert = new Instruction("DW", {std::to_string(varType.elements_number())+" DUP (?)"}, "", false, "", 1);
                }
                else
                {
                    firstAddress.push_back(2);
                    toInsert = new Instruction("DW", {varType.is_const?std::to_string(varContent.int_scalar):"?"}, "", false, "", 1);
                }
                extraSegment.Insert(*toInsert);
                //toInsert.PrintInstruction();
                delete toInsert;
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
                codeSegment.instructions.push_back(Instruction("", {}, "BB_"+std::to_string(bb_node->bb_idx)+":"));
                AddInfoFromBasicBlock(*bb_node);
            }
        }

        --tab_num;
    }
    void AddInfoFromBasicBlock(const BasicBlock& basicBlock)
    {
        for (auto inst: basicBlock.basic_block)
        {
            vector<Instruction> insts;

            Case (ReturnInst, ret_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AH",
                        "4CH"},
                    "",
                    false,
                    ret_inst->ToString()));
                insts.push_back(Instruction("MOV", {
                    "AL",
                    "DS:[" + std::to_string(ret_inst->dst.reg_id << 1) + "]"}));
                insts.push_back(Instruction("INT", {"21H"}));
            }
            Case (LoadNumber, ldc_inst, inst)
            {
                if (ldc_inst->src.type != TypeInt)
                    continue;
                insts.push_back(Instruction(
                    "MOV", {
                        "DS:[" + std::to_string(ldc_inst->dst.reg_id << 1) + "]",
                        "WORD PTR " + std::to_string(ldc_inst->src.int_value % 65536)},
                    "",
                    false,
                    ldc_inst->ToString()));
            }
            Case (UnaryOpInst, uop_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AX",
                        "DS:[" + std::to_string(uop_inst->src.reg_id << 1) + "]"},
                    "",
                    false,
                    uop_inst->ToString()));
                switch(uop_inst->op.unary_op)
                {
                case UnaryOp::Type::NOT:
                    insts.push_back(Instruction("NOT", {"AX"}));
                    insts.push_back(Instruction("AND", {"AX", "1"}));
                    break;
                case UnaryOp::Type::NEG:
                    insts.push_back(Instruction("NEG", {"AX"}));
                    break;
                case UnaryOp::Type::POS:
                    //insts.push_back(Instruction("POS", {"AX"}));
                    break;
                }
                insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(uop_inst->dst.reg_id << 1) + "]", "AX"}));
            }
            Case (BinaryOpInst, bop_inst, inst)
            {
                switch(bop_inst->op.bin_op)
                {
                case BinaryOp::Type::ADD:
                    insts.push_back(Instruction(
                        "MOV", {
                            "AX",
                            "DS:[" + std::to_string(bop_inst->src1.reg_id << 1) + "]"},
                        "",
                        false,
                        bop_inst->ToString()));
                    insts.push_back(Instruction("ADD", {"AX", "DS:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"}));
                    insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(bop_inst->dst.reg_id << 1) + "]", "AX"}));
                    break;
                case BinaryOp::Type::SUB:
                    insts.push_back(Instruction(
                        "MOV", {
                            "AX",
                            "DS:[" + std::to_string(bop_inst->src1.reg_id << 1) + "]"},
                        "",
                        false,
                        bop_inst->ToString()));
                    insts.push_back(Instruction("SUB", {"AX", "DS:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"}));
                    insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(bop_inst->dst.reg_id << 1) + "]", "AX"}));
                    break;
                case BinaryOp::Type::MUL:
                    insts.push_back(Instruction(
                        "MOV", {
                            "AX",
                            "DS:[" + std::to_string(bop_inst->src1.reg_id << 1) + "]"},
                        "",
                        false,
                        bop_inst->ToString()));
                    insts.push_back(Instruction("IMUL", {"WORD PTR DS:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"}));
                    insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(bop_inst->dst.reg_id << 1) + "]", "AX"}));
                    break;
                case BinaryOp::Type::DIV:
                    insts.push_back(Instruction(
                        "MOV", {
                            "AX",
                            "DS:[" + std::to_string(bop_inst->src1.reg_id << 1) + "]"},
                        "",
                        false,
                        bop_inst->ToString()));
                    insts.push_back(Instruction("CWD"));
                    insts.push_back(Instruction("IDIV", {"WORD PTR DS:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"}));
                    insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(bop_inst->dst.reg_id << 1) + "]", "AX"}));
                    break;
                case BinaryOp::Type::MOD:
                    insts.push_back(Instruction(
                        "MOV", {
                            "AX",
                            "DS:[" + std::to_string(bop_inst->src1.reg_id << 1) + "]"},
                        "",
                        false,
                        bop_inst->ToString()));
                    insts.push_back(Instruction("CWD"));
                    insts.push_back(Instruction("IDIV", {"WORD PTR DS:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"}));
                    insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(bop_inst->dst.reg_id << 1) + "]", "DX"}));
                    break;
                case BinaryOp::Type::LTH:
                    insts.push_back(Instruction(
                        "MOV", {
                            "AX",
                            "DS:[" + std::to_string(bop_inst->src1.reg_id << 1) + "]"},
                        "",
                        false,
                        bop_inst->ToString()));
                    insts.push_back(Instruction("SUB", {"AX", "DS:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"}));
                    insts.push_back(Instruction("MOV", {"AL", "AH"}));
                    insts.push_back(Instruction("XOR", {"AH", "AH"}));
                    insts.push_back(Instruction("MOV", {"CL", "7"}));
                    insts.push_back(Instruction("SHR", {"AL", "CL"}));
                    insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(bop_inst->dst.reg_id << 1) + "]", "AX"}));
                    break;
                case BinaryOp::Type::LEQ:
                    insts.push_back(Instruction(
                        "MOV", {
                            "AX",
                            "DS:[" + std::to_string(bop_inst->src2.reg_id << 1) + "]"},
                        "",
                        false,
                        bop_inst->ToString()));
                    insts.push_back(Instruction("SUB", {"AX", "DS:[" + std::to_string(bop_inst->src1.reg_id << 1) + "]"}));
                    insts.push_back(Instruction("MOV", {"AL", "AH"}));
                    insts.push_back(Instruction("XOR", {"AH", "AH"}));
                    insts.push_back(Instruction("MOV", {"CL", "7"}));
                    insts.push_back(Instruction("SHR", {"AL", "CL"}));
                    insts.push_back(Instruction("XOR", {"AL", "1"}));
                    insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(bop_inst->dst.reg_id << 1) + "]", "AX"}));
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
            }
            Case (StoreMem, stm_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AX", 
                        "DS:[" + std::to_string(stm_inst->src.reg_id << 1) + "]"},
                    "",
                    false,
                    stm_inst->ToString()));
                insts.push_back(Instruction("MOV", {"BX", "DS:[" + std::to_string(stm_inst->dst.reg_id << 1) + "]"}));
                insts.push_back(Instruction("MOV", {"ES:[BX]", "AX"}));
            }
            Case (LoadAddress, lad_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "DS:[" + std::to_string(lad_inst->dst.reg_id << 1) + "]",
                        "WORD PTR " + std::to_string(firstAddress[lad_inst->variable->var_idx])},
                    "",
                    false,
                    lad_inst->ToString()));
            }
            Case (LoadValue, ldv_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "BX", 
                        "DS:[" + std::to_string(ldv_inst->src.reg_id << 1) + "]"},
                    "",
                    false,
                    ldv_inst->ToString()));
                insts.push_back(Instruction("MOV", {"AX", "ES:[BX]"}));
                insts.push_back(Instruction("MOV", {"DS:[" + std::to_string(ldv_inst->dst.reg_id << 1) + "]", "AX"}));
            }
            Case (LoadOffset, ldo_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AX",
                        std::to_string(ldo_inst->size << 1)},
                    "",
                    false,
                    ldo_inst->ToString()));
                insts.push_back(Instruction(
                    "MUL", 
                    {"WORD PTR DS:[" + std::to_string(ldo_inst->off.reg_id << 1) + "]"}));
                insts.push_back(Instruction(
                    "ADD", {
                        "AX",
                        "DS:[" + std::to_string(ldo_inst->addr.reg_id << 1) + "]"}));
                insts.push_back(Instruction(
                    "MOV", {
                        "DS:[" + std::to_string(ldo_inst->dst.reg_id << 1) + "]",
                        "AX"}));
            }
            Case (JzeroInst, jzo_inst, inst)
            {
                insts.push_back(Instruction(
                    "MOV", {
                        "AX", 
                        "DS:[" + std::to_string(jzo_inst->cond.reg_id << 1) + "]"},
                    "",
                    false,
                    jzo_inst->ToString()));
                insts.push_back(Instruction("TEST", {"AL", "AL"}));
                insts.push_back(Instruction("JZ", {"BB_"+std::to_string(jzo_inst->bb_idx)}));
                jzo_inst->printJzoInst();
            }
            Case (JumpInst, jmp_inst, inst)
            {
                insts.push_back(Instruction(
                    "JMP", {"BB_"+std::to_string(jmp_inst->bb_idx)},
                    "",
                    false,
                    jmp_inst->ToString()));
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
            Instruction("ASSUME", {"CS:CSEG", "DS:DSEG", "ES:ESEG"}, "", false, "", 1),
            Instruction("MOV", {"AX", "DSEG"}, "START:", true, "", 1),
            Instruction("MOV", {"DS", "AX"}, "", false, "", 1),
            Instruction("MOV", {"AX", "ESEG"}, "", false, "", 1),
            Instruction("MOV", {"ES", "AX"}, "", false, "", 1)
        };
        for (auto&& toInsert : boilerplate)
        {
            codeSegment.Insert(toInsert);
            toInsert.PrintInstruction();
        }
        dataSegment.Insert(Instruction("DW", {"1024 DUP(0FFFFH)"}, "", false, "", 1));

        cout << " - Global Variable" << endl;
        auto&& globalScope = *ir.global_scope;
        AddInfoFromScope(globalScope, false);


        tab_num = 0;
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

        /*for (auto&& fAddr : firstAddress)
            cout << fAddr << " ";
        cout << "\n";*/
        /*cout << " - Generated Code:\n";
        dataSegment.PrintSegment();
        extraSegment.PrintSegment();
        codeSegment.PrintSegment();
        cout << "    END START\n";*/
    }
};
