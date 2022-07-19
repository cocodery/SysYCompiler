#include "pass_manager.hh"

void PassManager::buildCFGs() {
    for (auto &&function : functions) {
        if (function->func_info.is_used){
            function->buildCFG();
        }
    }
}
