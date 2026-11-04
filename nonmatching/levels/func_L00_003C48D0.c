/* Claude Opus 5.5 xhigh, v2430, 2026-11-04; not linked.
 * Best check: USA v1.01, unit L00_003B9CB8-nointerblock-g8.c
 * (-O2 -G8 -ffunction-sections -fno-sched-interblock), 708/708 bytes,
 * 0/27 places, first difference +0x8C, 70 rows / 47 words differ beyond
 * relocation immediates (v2418's starting form: 72 rows).
 * Gains over v2418: the previous-point view as a flat D_L00_00240070[i*10 + j]
 * gives retail's (i*160 + D70) + 16 operand order in both loops (the
 * [i][j] form expands as i*160 + (j*16 + D), so its giv add is (D + i*160));
 * D_L00_001B29B0 MACRO_ADDR gives retail's adjacent lui/addiu.
 * Still differs:
 * - fadeBase: retail lui fp/addiu fp,fp (an unsplit la); ours lui a0/addiu fp,a0.
 *   MACRO_ADDR on D_L00_001B29A0 (or D_L00_001B2990) removes the fadeBase
 *   pseudo (uses get per-use la) and hoists the D80 la out of the outer loop into fp
 *   (156-157 rows); fadeBase set before the if is also 157.
 * - loop 2 setup: retail builds D80 three times (two adjacent la pairs = likely
 *   a spilled REG_EQUIV D80 pseudo that reload rematerializes) and the w store
 *   starts at (D80 + i*160) + 0x1C; ours folds +12 into the symbol
 *   ((i*160 + 16) + (D80 + 12)). A row pointer for loop 2 is 668 bytes.
 * - render loop: s0-s4 permuted; retail's D2990/D29A0 pointer inits are
 *   adjacent la pairs (MACRO_ADDR-like), ours split.
 * Earlier drafts here (2026-10-28 and 2026-11-03, 680-696 bytes with
 * -fno-rerun-loop-opt) are replaced; the integ draft (sol-high b1122) is on orch/main.
 */
#include "common.h"
extern short D_L00_001B2990[];
extern short D_L00_001B29A0[];
extern short D_L00_001B29B0[] MACRO_ADDR;
typedef float Vector[4] __attribute__((aligned(16)));
extern Vector D_L00_00240080[5][10];
extern Vector D_L00_00240070[];
extern float D_L00_001B29D4[] __attribute__((sda));
extern float D_L00_001AFF8C __attribute__((sda));
extern unsigned int D_L00_001AFF24 __attribute__((sda));
extern unsigned int D_L00_001AFF20 __attribute__((sda));
extern void func_00282CF0(void *, void *, void *);
extern void func_00282CC0(void *, void *, void *);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern void func_L00_002EBE88(void *, int, int, int, unsigned int, unsigned int, float, float, float);
void func_L00_003C48D0(char *owner)
{
    Vector pos, vectors[10];
    int i,j,k;
    char *a;
    float ratio;
    qcopy(pos, *(char **)(owner + 0x68) + 0x30);
    for (i=0;i<5;i++) {
        if (D_L00_001B2990[i]) {
            short *fadeBase = D_L00_001B29A0;
            for (j=1;j<10;j++) {
                func_00282CF0(vectors[j],D_L00_00240080[i][j],D_L00_00240070[i * 10 + j]);
            }
            qcopy(D_L00_00240080[i],pos);
            D_L00_00240080[i][0][3]=0.04f;
            for (j=1;j<10;j++) {
                func_00282CC0(D_L00_00240080[i][j],D_L00_00240070[i * 10 + j],vectors[j]);
                D_L00_00240080[i][j][3]=0.04f;
            }
            if (D_L00_001B2990[i]>0) {
                ratio=func_00283CE0(D_L00_001B2990[i])/func_00283CE0(D_L00_001B29B0[i]);
                fadeBase[i]=func_00283CF0(D_L00_001AFF8C*ratio);
            } else fadeBase[i]=0;
        }
    }
    a=(char *)D_L00_00240080;
    { short *life=D_L00_001B2990, *fade=D_L00_001B29A0;
    for (k=4;k>=0;a+=160,fade++,k--,life++) {
        unsigned int alpha,c,c0;
        if (!*life) continue;
        alpha=func_00283CF0(func_00283CE0(*fade)*D_L00_001B29D4[0])<<24;
        c0=D_L00_001AFF24;
        c=D_L00_001AFF20;
        if (*(short *)(owner+0xAA)==0x1192) c=c0;
        func_L00_002EBE88(a,23,14,10,alpha|0x7F7F7F,alpha|c,0.0f,4.0f,0.0f);
    }
    }
}
