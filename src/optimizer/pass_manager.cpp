#include "pass_manager.hh"

void PassManager::compDomInfo() {
    // dbg("run CompDomInfo pass");
    for (auto &&function : functions) {
        if (function->func_info.is_used) {
            function->buildDom();
        }
    }
    for (auto &&function : functions) {
        if (function->func_info.is_used) {
            function->buildIDom();
        }
    } 
    for (auto &&function : functions) {
        if (function->func_info.is_used) {
            function->initBBDF();
        }
    }
}

void PassManager::mem2reg() {
    // dbg("run Mem2Reg pass");
    for (auto &&function : functions) {
        Mem2Reg mem2reg = Mem2Reg(function);
        mem2reg.runMem2Reg();
    }
}

void PassManager::lvn() {
    for (auto &&function : functions) {
        for (auto &&bb : function->all_blocks) {
            LVN lvn;
            lvn.ValueNumber(&bb->basic_block);
        }
    }
}