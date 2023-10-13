//
// Created by huangyuyang on 10/13/23.
//

#ifndef REEDSOLOMON_C_NEON_GAL_NEON_H
#define REEDSOLOMON_C_NEON_GAL_NEON_H

#include "arm_neon.h"
void galMulXorNeon(unsigned char *mapTable, unsigned char *input, unsigned char *output, int len) {
    uint8_t *out = output;
    uint8_t *in = input;

    int sur = len & 15;
    int cnt = len - sur;

    if (cnt > 0) {
        register uint8_t *mapTable1 = mapTable;
        asm volatile(
                "ADD X2, %[out], %[cnt]\n\t"
                "LD1 {V0.16B - V3.16B}, [%[mapTable1]], 64\n\t"
                "LD1 {V4.16B - V7.16B}, [%[mapTable1]], 64\n\t"
                "LD1 {V8.16B - V11.16B}, [%[mapTable1]], 64\n\t"
                "LD1 {V12.16B - V15.16B}, [%[mapTable1]]\n\t"
                "movi v16.16B, 0X40\n\t"
                "movi v17.16B, 0X80\n\t"
                "movi v18.16B, 0Xc0\n\t"
                "1:\n\t"

                "LD1 {V21.16B}, [%[in]], 16\n\t"
                "LD1 {V20.16B}, [%[out]]\n\t"
                "SUB V22.16B, V21.16B, V16.16B\n\t"
                "SUB V23.16B, V21.16B, V17.16B\n\t"
                "SUB V24.16B, V21.16B, V18.16B\n\t"
                "TBL V28.16B, {V0.16B - V3.16B}, V21.16B\n\t"
                "TBL V29.16B, {V4.16B - V7.16B}, V22.16B\n\t"
                "TBL V30.16B, {V8.16B - V11.16B}, V23.16B\n\t"
                "TBL V31.16B, {V12.16B - V15.16B}, V24.16B\n\t"
                "ORR V22.16B, V29.16B, V28.16B\n\t"
                "ORR V23.16B, V30.16B, V31.16B\n\t"
                "ORR V24.16B, V22.16B, V23.16B\n\t"
                "EOR V30.16B, V24.16B, V20.16B\n\t"
                "ST1 {v30.16B}, [%[out]], 16\n\t"
                "CMP X2, %[out]\n\t"
                "BNE 1b\n\t"
        :[out] "+r"(out),[in] "+r"(in),[mapTable1] "+r"(mapTable1)
        :[cnt] "r"(cnt)
        :"cc", "memory", "x2","v0","v1","v2","v3","v4","v5","v6","v7","v8","v9",
                "v10","v11","v12","v13","v14","v15","v16","v17","v18","v19","v20","v21","v22","v23","v24","v25","v26","v27","v28","v29","v30","v31"
        );
    }

    for (int i = 0; i < sur; i++) {
        *(out++) ^= mapTable[*(in++)];
    }
}

#endif //REEDSOLOMON_C_NEON_GAL_NEON_H
