#include "memaccess_opt.hh"

void MemAccessOpt::runMemAccessOpt() {
    auto &&all_blocks = function->all_blocks;
    int32_t idx = 0;
    for (auto &&block : all_blocks) {
        bbIdx2MapIdx.insert({block->bb_idx, idx++});
        Addr2Value.push_back(map<pair<int32_t, bool>, SRC>());
    }

    for (auto &&block : all_blocks) {
        auto &&localMap = Addr2Value[bbIdx2MapIdx[block->bb_idx]];
        auto &&bb_list = list<Inst *>(block->basic_block.begin(), block->basic_block.end());
        for (auto &&iter = bb_list.begin(); iter != bb_list.end(); ) {
            auto &&inst = *iter;
            Case (LLIR_STORE, store_inst, inst) {
                auto &&store_dst = store_inst->dst.reg;
                auto &&store_src = store_inst->src;
                assert(store_dst != nullptr);
                localMap[{store_dst->reg_id, store_dst->global}] = store_src;
            }
            else Case (LLIR_LOAD, load_inst, inst) {
                auto &&load_src = load_inst->src.reg;
                assert(load_src != nullptr);
                if (localMap.find({load_src->reg_id, load_src->global}) != localMap.end()) {
                    block->replaceSRC(load_inst->dst.reg, localMap[{load_src->reg_id, load_src->global}]);
                    iter = bb_list.erase(iter);
                    continue;
                }
            }
            ++iter;
        }
        block->basic_block = vector<Inst *>(bb_list.begin(), bb_list.end());
    }
}
