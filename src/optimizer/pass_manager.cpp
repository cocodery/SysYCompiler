#include "pass_manager.hh"

void PassManager::compDomInfo() {
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
    for (auto &&function : functions) {
        Mem2Reg mem2reg = Mem2Reg(function);
        // mem2reg.runMem2Reg();
    }
}
