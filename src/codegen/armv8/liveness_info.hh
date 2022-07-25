#ifndef LIVENESS_INFO_H
#define LIVENESS_INFO_H

#include "common.hh"
#include "ir.hh"

#define IF_GLOBAL_RETURN_NEG_ID(regPtr)\
    ((regPtr)->global ? -(regPtr)->reg_id : (regPtr)->reg_id)

#define IF_IS_REG_THEN_PUSH_BACK(vec, regPtr)\
    {if (regPtr) vec.push_back(IF_GLOBAL_RETURN_NEG_ID(regPtr));}

void GenerateLiveInfo(const CompUnit &ir)
{
    printf("\n-----GenerateLiveInfo Start----\n");
    for (auto &&functionPtr : ir.functions)
    {
        printf("In Function \"%s\":\n", functionPtr->func_info.func_name.c_str());
        
        printf(" * Generate LiveUse & LiveDef\n");
        for (auto &&bbPtr : functionPtr->all_blocks)
        {
            if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                continue;

            printf(FOUR_SPACES"In BB %d:\n", bbPtr->bb_idx);

            /*if (!bbPtr->valuable)
            {
                printf(EIGHT_SPACES"skipping, reason: not valuable\n");
                continue;
            }
            if (bbPtr->basic_block.empty())
            {
                printf(EIGHT_SPACES"skipping, reason: no inst in this BB\n");
                continue;
            }*/

            for (auto &&instPtr : bbPtr->basic_block)
            {
                vector <int32_t> src_regids;
                int32_t dst_regid = INT32_MIN;
                Case (LLIR_RET, ret_inst, instPtr)
                {
                    //cout << get_tabs() << ret_inst->ToString() << endl;

                    // return src
                    if (ret_inst->has_retvalue)
                        IF_IS_REG_THEN_PUSH_BACK(src_regids, ret_inst->ret_value.reg);
                }
                Case (LLIR_BR, br_inst, instPtr)
                {
                    //cout << get_tabs() << br_inst->ToString() << endl;

                    // if (src) goto label1;
                    if (br_inst->has_cond)
                        IF_IS_REG_THEN_PUSH_BACK(src_regids, br_inst->cond.reg);
                }
                Case (LLIR_BIN, bin_inst, instPtr)
                {
                    //cout << get_tabs() << bin_inst->ToString() << endl;

                    // dst = src1 + src2
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, bin_inst->src1.reg);
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, bin_inst->src2.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(bin_inst->dst.reg);
                }
                Case (LLIR_FBIN, fbin_inst, instPtr)
                {
                    //cout << get_tabs() << fbin_inst->ToString() << endl;

                    // dst = src1 + src2
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fbin_inst->src1.reg);
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fbin_inst->src2.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(fbin_inst->dst.reg);
                }
                //Case (LLIR_ALLOCA, alloc_inst, instPtr) {}
                Case (LLIR_LOAD, load_inst, instPtr)
                {
                    //cout << get_tabs() << load_inst->ToString() << endl;
                    
                    // dst = *src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, load_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(load_inst->dst.reg);
                }
                Case (LLIR_STORE, store_inst, instPtr)
                {
                    //cout << get_tabs() << store_inst->ToString() << endl;

                    // *dst = src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, store_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(store_inst->dst.reg);
                }
                Case (LLIR_ICMP, icmp_inst, instPtr)
                {
                    //cout << get_tabs() << icmp_inst->ToString() << endl;
                
                    // dst = src1 < src2
                    if (icmp_inst->src1.reg)
                        src_regids.push_back(IF_GLOBAL_RETURN_NEG_ID(icmp_inst->src1.reg));
                    if (icmp_inst->src2.reg)
                        src_regids.push_back(IF_GLOBAL_RETURN_NEG_ID(icmp_inst->src2.reg));
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(icmp_inst->dst.reg);
                }
                Case (LLIR_FCMP, fcmp_inst, instPtr)
                {
                    //cout << get_tabs() << fcmp_inst->ToString() << endl;
                
                    // dst = src1 < src2
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fcmp_inst->src1.reg);
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fcmp_inst->src2.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(fcmp_inst->dst.reg);
                }
                Case (LLIR_CALL, call_inst, instPtr)
                {
                    //cout << get_tabs() << call_inst->ToString() << endl;
                
                    // [dst = ] func(arg1, arg2, ...)
                    for (auto &&arg : call_inst->args)
                        IF_IS_REG_THEN_PUSH_BACK(src_regids, arg.reg);
                    if (call_inst->func_info->return_type != TypeVoid)
                        dst_regid = IF_GLOBAL_RETURN_NEG_ID(call_inst->dst.reg);
                }
                Case (LLIR_ZEXT, zext_inst, instPtr)
                {
                    //cout << get_tabs() << zext_inst->ToString() << endl;
                
                    // dst = (int) src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, zext_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(zext_inst->dst.reg);
                }
                Case (LLIR_GEP, gep_inst, instPtr)
                {
                    //cout << get_tabs() << gep_inst->ToString() << endl;
                    
                    // dst = *(src + off)
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, gep_inst->off.reg);
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, gep_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(gep_inst->dst.reg);
                }
                Case (LLIR_XOR, xor_inst, instPtr)
                {
                    //cout << get_tabs() << xor_inst->ToString() << endl;
                    
                    // dst = ~ src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, xor_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(xor_inst->dst.reg);
                }
                Case (LLIR_BC, bc_inst, instPtr) {
                    //cout << get_tabs() << bc_inst->ToString() << endl; 

                    // dst = (char)src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, bc_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(bc_inst->dst.reg);
                }
                Case (LLIR_SITOFP, itf_inst, instPtr)
                {
                    //cout << get_tabs() << itf_inst->ToString() << endl;
                    
                    // dst = (float) src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, itf_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(itf_inst->dst.reg);
                }
                Case (LLIR_FPTOSI, fti_inst, instPtr)
                {
                    //cout << get_tabs() << fti_inst->ToString() << endl;
                    
                    // dst = (int) src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fti_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(fti_inst->dst.reg);
                }

                for (auto &&src_regid : src_regids)
                    if (bbPtr->LiveDef.find(src_regid) == bbPtr->LiveDef.end())
                        bbPtr->LiveUse.insert(src_regid);
                if (dst_regid != INT32_MIN)
                    bbPtr->LiveDef.insert(dst_regid);
            }
        
            printf(EIGHT_SPACES "LiveUse:");
            for (auto &&id : bbPtr->LiveUse)
                printf(" %d", id);
            printf("\n" EIGHT_SPACES "LiveDef:");
            for (auto &&id : bbPtr->LiveDef)
                printf(" %d", id);
            printf("\n");
        }
    
        printf(" * Generate LiveIn & LiveOut\n");       
        bool liveOutChanged;
        do
        {
            liveOutChanged = false;
            for (auto &&it = functionPtr->all_blocks.rbegin();
                it != functionPtr->all_blocks.rend();
                ++it)
            {
                auto &&bPtr = *it;
                if (bPtr->bb_idx == 0 || bPtr->bb_idx == -1)
                    continue;

                size_t oldSiz = bPtr->LiveOut.size();
                bPtr->LiveOut.clear();
                for (auto &&sPair : bPtr->succs)
                {
                    auto &&sPtr = sPair.second;
                    bPtr->LiveOut.insert(sPtr->LiveIn.begin(), sPtr->LiveIn.end());
                }
                if (!liveOutChanged && bPtr->LiveOut.size() != oldSiz)
                    liveOutChanged = true;

                bPtr->LiveIn.clear();
                set_difference(bPtr->LiveOut.begin(),  bPtr->LiveOut.end(),
                               bPtr->LiveDef.begin(),  bPtr->LiveDef.end(),
                               inserter(bPtr->LiveIn, bPtr->LiveIn.begin()));
                bPtr->LiveIn.insert(bPtr->LiveUse.begin(), bPtr->LiveUse.end());
            
                /*printf(FOUR_SPACES"In BB %d:\n", bPtr->bb_idx);
                printf(EIGHT_SPACES "LiveIn:");
                for (auto &&id : bPtr->LiveIn)
                    printf(" %d", id);
                printf("\n" EIGHT_SPACES "LiveOut:");
                for (auto &&id : bPtr->LiveOut)
                    printf(" %d", id);
                printf("\n");*/
            }
        } while (liveOutChanged);
        for (auto &&bbPtr : functionPtr->all_blocks)
        {
            if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
                continue;

            printf(FOUR_SPACES"In BB %d:\n", bbPtr->bb_idx);
            printf(EIGHT_SPACES "LiveIn:");
            for (auto &&id : bbPtr->LiveIn)
                printf(" %d", id);
            printf("\n" EIGHT_SPACES "LiveOut:");
            for (auto &&id : bbPtr->LiveOut)
                printf(" %d", id);
            printf("\n");
        }

        printf(" * Generate LiveInterval\n");
        // Instruction Indexing
        int instOffset = 0;
        for (auto &&bbPtr : functionPtr->all_blocks)
        {
            bbPtr->firstInstIndex = instOffset;
            instOffset += bbPtr->basic_block.size();
        }

        /*for (auto &&bbIt = functionPtr->all_blocks.rbegin();
            bbIt != functionPtr->all_blocks.rend();
            ++bbIt)
        {
            auto &&bbPtr = *bbIt;
            for (auto &&vReg : bbPtr->LiveOut)
                ExtendRangeOrAddRange();
            for (auto &&instIt = bbPtr->basic_block.rbegin();
                instIt != bbPtr->basic_block.rend();
                ++instIt)
            {
                auto &&instPtr = *instIt;
                Case (LLIR_RET, ret_inst, instPtr)
                {
                    //cout << get_tabs() << ret_inst->ToString() << endl;

                    // return src
                    if (ret_inst->has_retvalue)
                        IF_IS_REG_THEN_PUSH_BACK(src_regids, ret_inst->ret_value.reg);
                }
                Case (LLIR_BR, br_inst, instPtr)
                {
                    //cout << get_tabs() << br_inst->ToString() << endl;

                    // if (src) goto label1;
                    if (br_inst->has_cond)
                        IF_IS_REG_THEN_PUSH_BACK(src_regids, br_inst->cond.reg);
                }
                Case (LLIR_BIN, bin_inst, instPtr)
                {
                    //cout << get_tabs() << bin_inst->ToString() << endl;

                    // dst = src1 + src2
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, bin_inst->src1.reg);
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, bin_inst->src2.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(bin_inst->dst.reg);
                }
                Case (LLIR_FBIN, fbin_inst, instPtr)
                {
                    //cout << get_tabs() << fbin_inst->ToString() << endl;

                    // dst = src1 + src2
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fbin_inst->src1.reg);
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fbin_inst->src2.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(fbin_inst->dst.reg);
                }
                //Case (LLIR_ALLOCA, alloc_inst, instPtr) {}
                Case (LLIR_LOAD, load_inst, instPtr)
                {
                    //cout << get_tabs() << load_inst->ToString() << endl;
                    
                    // dst = *src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, load_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(load_inst->dst.reg);
                }
                Case (LLIR_STORE, store_inst, instPtr)
                {
                    //cout << get_tabs() << store_inst->ToString() << endl;

                    // *dst = src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, store_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(store_inst->dst.reg);
                }
                Case (LLIR_ICMP, icmp_inst, instPtr)
                {
                    //cout << get_tabs() << icmp_inst->ToString() << endl;
                
                    // dst = src1 < src2
                    if (icmp_inst->src1.reg)
                        src_regids.push_back(IF_GLOBAL_RETURN_NEG_ID(icmp_inst->src1.reg));
                    if (icmp_inst->src2.reg)
                        src_regids.push_back(IF_GLOBAL_RETURN_NEG_ID(icmp_inst->src2.reg));
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(icmp_inst->dst.reg);
                }
                Case (LLIR_FCMP, fcmp_inst, instPtr)
                {
                    //cout << get_tabs() << fcmp_inst->ToString() << endl;
                
                    // dst = src1 < src2
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fcmp_inst->src1.reg);
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fcmp_inst->src2.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(fcmp_inst->dst.reg);
                }
                Case (LLIR_CALL, call_inst, instPtr)
                {
                    //cout << get_tabs() << call_inst->ToString() << endl;
                
                    // [dst = ] func(arg1, arg2, ...)
                    for (auto &&arg : call_inst->args)
                        IF_IS_REG_THEN_PUSH_BACK(src_regids, arg.reg);
                    if (call_inst->func_info->return_type != TypeVoid)
                        dst_regid = IF_GLOBAL_RETURN_NEG_ID(call_inst->dst.reg);
                }
                Case (LLIR_ZEXT, zext_inst, instPtr)
                {
                    //cout << get_tabs() << zext_inst->ToString() << endl;
                
                    // dst = (int) src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, zext_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(zext_inst->dst.reg);
                }
                Case (LLIR_GEP, gep_inst, instPtr)
                {
                    //cout << get_tabs() << gep_inst->ToString() << endl;
                    
                    // dst = *(src + off)
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, gep_inst->off.reg);
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, gep_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(gep_inst->dst.reg);
                }
                Case (LLIR_XOR, xor_inst, instPtr)
                {
                    //cout << get_tabs() << xor_inst->ToString() << endl;
                    
                    // dst = ~ src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, xor_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(xor_inst->dst.reg);
                }
                Case (LLIR_BC, bc_inst, instPtr) {
                    //cout << get_tabs() << bc_inst->ToString() << endl; 

                    // dst = (char)src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, bc_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(bc_inst->dst.reg);
                }
                Case (LLIR_SITOFP, itf_inst, instPtr)
                {
                    //cout << get_tabs() << itf_inst->ToString() << endl;
                    
                    // dst = (float) src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, itf_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(itf_inst->dst.reg);
                }
                Case (LLIR_FPTOSI, fti_inst, instPtr)
                {
                    //cout << get_tabs() << fti_inst->ToString() << endl;
                    
                    // dst = (int) src
                    IF_IS_REG_THEN_PUSH_BACK(src_regids, fti_inst->src.reg);
                    dst_regid = IF_GLOBAL_RETURN_NEG_ID(fti_inst->dst.reg);
                }
            }
        }*/
    }

             
    printf("-----GenerateLiveInfo   End----\n\n");
}

#endif
