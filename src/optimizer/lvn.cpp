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
        dbg(block->bb_idx);
        valueNumber(block);
    }
}

void LVN::valueNumber(BasicBlock *block) {
    int32_t mapIndex = bbIdxMapIdx[block->bb_idx];
    for (auto &&iter = block->basic_block.begin(); iter != block->basic_block.end(); ++iter) {
        auto &&inst = *iter;
        Case (LLIR_BIN, bin_inst, inst) {
            dbg(bin_inst->ToString());
            NodeOp op = NodeOp(bin_inst->op);
            lvnNode *node = new lvnNode(op, bin_inst->src1, bin_inst->src2);
            if (!checkInMap(block, node)) {
                dbg("insert src-node");
                dbg(bin_inst->dst.ToString());
                lvnMapNodes[mapIndex].insert({bin_inst->dst, node});
            } else {
                iter = block->basic_block.erase(iter) - 1;
            }
        } else Case (LLIR_FBIN, fbin_inst, inst) {
            dbg(fbin_inst->ToString());
            NodeOp op = NodeOp(fbin_inst->op + 5);
            lvnNode *node = new lvnNode(op, fbin_inst->src1, fbin_inst->src2);
            if (!checkInMap(block, node)) {
                lvnMapNodes[mapIndex].insert({fbin_inst->dst, node});
            } else {
                iter = block->basic_block.erase(iter) - 1;
            }
        } else Case (LLIR_ICMP, icmp_inst, inst) {
            dbg(icmp_inst->ToString());
            NodeOp op = iCmp;
            lvnNode *node = new lvnNode(op, icmp_inst->src1, icmp_inst->src2);
            if (!checkInMap(block, node)) {
                lvnMapNodes[mapIndex].insert({icmp_inst->dst, node});
            } else {
                iter = block->basic_block.erase(iter) - 1;
            }
        } else Case (LLIR_FCMP, fcmp_inst, inst) {
            dbg(fcmp_inst->ToString());
            NodeOp op = fCmp;
            lvnNode *node = new lvnNode(op, fcmp_inst->src1, fcmp_inst->src2);
            if (!checkInMap(block, node)) {
                lvnMapNodes[mapIndex].insert({fcmp_inst->dst, node});
            } else {
                iter = block->basic_block.erase(iter) - 1;
            }
        } else Case (LLIR_GEP, gep_inst, inst) {
            dbg(gep_inst->ToString());
            NodeOp op = Gep;
            lvnNode *node = new lvnNode(op, gep_inst->src, gep_inst->off);
            if (!checkInMap(block, node)) {
                lvnMapNodes[mapIndex].insert({gep_inst->dst, node});
            } else {
                iter = block->basic_block.erase(iter) - 1;
            }
        }
        dbg("check lvnMapNodes");
        for (auto &&pair : lvnMapNodes[mapIndex]) {
            cout << pair.first.reg->ToString() << ' ' << pair.second->op << ' ' << pair.second->src1.ToString() << ' ' << pair.second->src2.ToString() << endl;
        }
    }
}

bool LVN::checkInMap(BasicBlock *block, lvnNode *node) {
    int32_t mapIdx = bbIdxMapIdx[block->bb_idx];
    
    return false;
}
