#include "pass_manager.hh"

void PassManager::buildCFGs() {
    for (auto &&function : functions) {
        if (function->func_info.is_used){
            function->buildCFG();
        }
    }
}

void PassManager::buildDoms() {
    for (auto &&function : functions) {
        if (function->func_info.is_used) {
            function->buildDom();
        }
    }
}

void PassManager::buildIDoms() {
    for (auto &&function : functions) {
        if (function->func_info.is_used) {
            function->buildIDom();
        }
    }
}
