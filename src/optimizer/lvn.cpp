#include "lvn.hh"

void LVN::runLVN() {
    auto &&all_blocks = function->all_blocks;
    int32_t idx = 0;
    for (auto &&block : all_blocks) {
        bbIdxMapIdx.insert({block->bb_idx, idx++});
        map<VirtReg *, SRC> reg2Src;
        map<SRC, lvnNode *> reg2LvnNode;
        replaceMap.push_back(reg2Src);
        lvnMapNodes.push_back(reg2LvnNode);
    }

    for (auto &&block : all_blocks) {
        block->dirty = false;
        valueNumber(block);
    }
}

void LVN::valueNumber(BasicBlock *block) {
    int32_t mapIndex = bbIdxMapIdx[block->bb_idx];
    for (auto &&iter = block->basic_block.begin(); iter != block->basic_block.end(); ++iter) {
        auto &&inst = *iter;
        Case (LLIR_BIN, bin_inst, inst) {
            NodeOp op = NodeOp(bin_inst->op);
            lvnNode *node = new lvnNode(op, bin_inst->src1, bin_inst->src2);
            lvnMapNodes[mapIndex].insert({bin_inst->dst, node});
        } else Case (LLIR_FBIN, fbin_inst, inst) {
            NodeOp op = NodeOp(fbin_inst->op + 5);
            lvnNode *node = new lvnNode(op, fbin_inst->src1, fbin_inst->src2);
            lvnMapNodes[mapIndex].insert({fbin_inst->dst, node});
        } else Case (LLIR_ICMP, icmp_inst, inst) {
            NodeOp op = iCmp;
            lvnNode *node = new lvnNode(op, icmp_inst->src1, icmp_inst->src2);
            lvnMapNodes[mapIndex].insert({icmp_inst->dst, node});
        } else Case (LLIR_FCMP, fcmp_inst, inst) {
            NodeOp op = fCmp;
            lvnNode *node = new lvnNode(op, fcmp_inst->src1, fcmp_inst->src2);
            lvnMapNodes[mapIndex].insert({fcmp_inst->dst, node});
        } else Case (LLIR_GEP, gep_inst, inst) {
            NodeOp op = Gep;
            lvnNode *node = new lvnNode(op, gep_inst->src, gep_inst->off);
            lvnMapNodes[mapIndex].insert({gep_inst->dst, node});
        } 
    }
}

void LVN::ProcessInst(vector<Inst> *bb, Inst *ist, BinOp op, SRC dst, SRC src1, SRC src2) {
}

void LVN::FindTuple(tuple<int, int, string> t) {

}

void LVN::GetIndex(SRC dst) {
    
}

void LVN::display() {
    
}