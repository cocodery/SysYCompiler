#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>

#include "frontend/SysYLexer.h"
#include "frontend/SysYParser.h"
#include "frontend/ASTVisitor.hh"
#include "optimizer/pass_manager.hh"
#include "codegen/armv8/liveness_info.hh"

// #include "optimizer/dag.hh"

#include "common.hh"

using namespace antlr4;
using namespace std;

int main(int argc, char *argv[]) {
    int opt = 0;
    bool print_usage = false;
    string input, output, irfile;

    for (int ch; (ch = getopt(argc, argv, "Sl:o:O:h")) != -1;) {
        switch (ch) {
            case 'l':
                irfile = strdup(optarg);
                break;
            case 'o':
                output = strdup(optarg);
                break;
            case 'O':
                opt = atoi(optarg);
                break;
            case 'h':
                print_usage = true; 
                break;
            default:
                break;
        }
    }

    if (optind <= argc) {
        input = argv[optind];
    }
    
    // dbg(input, output, irfile, opt, print_usage);
    ifstream src{input};
    if (!src.is_open()) {
        cerr << "cannot open input file" << endl;
        return EXIT_FAILURE;
    }
    
    ANTLRInputStream source(src);
    SysYLexer lexer(&source);
    CommonTokenStream tokens(&lexer);
    SysYParser parser(&tokens);
    parser.setErrorHandler(make_shared<BailErrorStrategy>());

    SysYParser::CompUnitContext *root = parser.compUnit();

    CompUnit ir;
    
    ASTVisitor visitor(ir);

    // cout << "Start Compilation" << endl;

    visitor.visitCompUnit(root);
    if (visitor.have_main_func == false) {
        dbg("No main Function Found");
        // exit(EXIT_FAILURE);
    }

    for (auto &&function : ir.functions) {
        if (function->func_info.is_used) {
            function->buildCFG();
        }
    }
    ir.GenerateLLIR("../main0.ll");

    // cout << "Start Optimization" << endl;

    PassManager pass_manager(ir.global_scope, ir.functions);
    pass_manager.excute_pass();

    // ----- CodeGen -----

    // GenerateLiveInfo(ir);

    // -------------------

    ir.GenerateLLIR("../main.ll");

    // cout << "Compilation Complete" << endl;

    return 0;
}