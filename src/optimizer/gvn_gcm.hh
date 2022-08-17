#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

#include "dead_code_elim.hh"

using twoSrcs = pair<BinOp, pair<SRC, SRC>>;
using phiSrcs = vector<pair<SRC, int32_t>>;
using funcRel = pair<string, vector<SRC>>;

struct localValueTable {
    map<twoSrcs, SRC> bin2src;
    map<phiSrcs, SRC> phi2src;
    map<funcRel, SRC> fun2src;
};

class GvnGcm {
public:
    Function *function;

    map<int32_t, int32_t> idxMap;
    vector<localValueTable> globalValueTable;
public:
    GvnGcm(Function *func) : function(func) {
        auto &&all_blocks = function->all_blocks;
        for (int32_t idx = 0; idx < function->all_blocks.size(); ++idx) {
            idxMap.insert({all_blocks[idx]->bb_idx, idx});
            globalValueTable.push_back(localValueTable());
        }
    }

    void moveCtv2Rhs();
    void runGVN();
    void runGvnOnBlock(BasicBlock *block);

    SRC lookupOrAdd(int32_t idx, LLIR_BIN *bin_inst);
    SRC lookupOrAdd(int32_t idx, LLIR_FBIN *fbin_inst);
    SRC lookupOrAdd(int32_t idx, LLIR_GEP *gep_inst);
    SRC lookupOrAdd(int32_t idx, LLIR_PHI *bin_inst);
    SRC lookupOrAdd(int32_t idx, LLIR_CALL *bin_inst);

    void runGCM();
    void runGvnGcm();
};
