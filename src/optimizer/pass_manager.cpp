#include "pass_manager.hh"

void PassManager::buildCFGs() {
    for (auto &&function : functions) {
        function->buildCFG();
    }
}
