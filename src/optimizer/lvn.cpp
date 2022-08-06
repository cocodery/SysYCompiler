#include "lvn.hh"

void LVN::runLVN() {
    auto &&all_blocks = function->all_blocks;
    int32_t idx = 0;
    for (auto &&block : all_blocks) {
        bbIdxMapIdx.insert({block->bb_idx, idx++});
        map<SRC, lvnNode *> reg2LvnNode;
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
            SRC new_value = checkInMap(block, node);
            if (!new_value.ToCTValue() && !new_value.ToVirtReg()) {
                lvnMapNodes[mapIndex].insert({bin_inst->dst, node});
            } else {
                assert(bin_inst->dst.ToVirtReg());
                function->replaceSRCs(block, bin_inst->dst.ToVirtReg(), new_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        } else Case (LLIR_FBIN, fbin_inst, inst) {
            NodeOp op = NodeOp(fbin_inst->op + 5);
            lvnNode *node = new lvnNode(op, fbin_inst->src1, fbin_inst->src2);
            SRC new_value = checkInMap(block, node);
            if (!new_value.ToCTValue() && !new_value.ToVirtReg()) {
                lvnMapNodes[mapIndex].insert({fbin_inst->dst, node});
            } else {
                assert(fbin_inst->dst.ToVirtReg());
                function->replaceSRCs(block, fbin_inst->dst.ToVirtReg(), new_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        } else Case (LLIR_ICMP, icmp_inst, inst) {
            NodeOp op = iCmp;
            lvnNode *node = new lvnNode(op, icmp_inst->src1, icmp_inst->src2);
            SRC new_value = checkInMap(block, node);
            if (!new_value.ToCTValue() && !new_value.ToVirtReg()) {
                lvnMapNodes[mapIndex].insert({icmp_inst->dst, node});
            } else {
                assert(icmp_inst->dst.ToVirtReg());
                function->replaceSRCs(block, icmp_inst->dst.ToVirtReg(), new_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        } else Case (LLIR_FCMP, fcmp_inst, inst) {
            NodeOp op = fCmp;
            lvnNode *node = new lvnNode(op, fcmp_inst->src1, fcmp_inst->src2);
            SRC new_value = checkInMap(block, node);
            if (!new_value.ToCTValue() && !new_value.ToVirtReg()) {
                lvnMapNodes[mapIndex].insert({fcmp_inst->dst, node});
            } else {
                assert(fcmp_inst->dst.ToVirtReg());
                function->replaceSRCs(block, fcmp_inst->dst.ToVirtReg(), new_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        } else Case (LLIR_GEP, gep_inst, inst) {
            NodeOp op = Gep;
            lvnNode *node = new lvnNode(op, gep_inst->src, gep_inst->off);
            SRC new_value = checkInMap(block, node);
            if (!new_value.ToCTValue() && !new_value.ToVirtReg()) {
                lvnMapNodes[mapIndex].insert({gep_inst->dst, node});
            } else {
                assert(gep_inst->dst.ToVirtReg());
                function->replaceSRCs(block, gep_inst->dst.ToVirtReg(), new_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        }
    }
}

SRC LVN::checkInMap(BasicBlock *block, lvnNode *node) {
    int32_t mapIdx = bbIdxMapIdx[block->bb_idx];
    for (auto &&pair : lvnMapNodes[mapIdx]) {
        auto &&lvn_node = pair.second;
        if (lvn_node->op == node->op && lvn_node->src1 == node->src1 && lvn_node->src2 == node->src2) {
            return pair.first;
        }
    }
    return SRC();
}
