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
#include "frontend/ASTVisitor.h"

#include "common.h"

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
    
    dbg(input, output, irfile, opt, print_usage);

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

    ASTVisitor visitor;

    cout << "Start Compiler" << endl;

    if (!visitor.visitCompUnit(root)) {
        dbg("No main Function Found");
        exit(EXIT_FAILURE);
    }

    cout << "Compiler Complete" << endl;

    return 0;

}