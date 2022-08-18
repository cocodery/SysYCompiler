#include "pass_manager.hh"

int PassManager::get_depth(Scope *now_scope, int now_depth)
{
    int max_depth = now_depth;
    for (auto iter = now_scope->elements->begin(); iter != now_scope->elements->end(); ++iter) {
        if (Scope *scope_node = dynamic_cast<Scope *>(*iter); scope_node != nullptr) {
            max_depth = std::max(max_depth, get_depth(scope_node, now_depth + 1));
        }
    }
    return max_depth;
}

bool PassManager::do_not_run_m2r_r2m(Function *funcPtr)
{
    return (get_depth(funcPtr->main_scope, 1) > 8
         || funcPtr->func_info.func_name == "pseudo_sha1");
}
