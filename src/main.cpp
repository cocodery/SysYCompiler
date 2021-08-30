#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>

#include "SysYLexer.h"
#include "SysYParser.h"

using namespace antlr4;
using namespace std;

int main(int argc, char *argv[]) {
    int opt = false;
    bool print_usage = false;
    string  input, output, irfile;

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
                opt = opt > 0 ? opt : 0;
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

    return 0;

}