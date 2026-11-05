/* Claude Opus 5.5 xhigh, v2619, 2026-11-05: closest so far, not linked. Third and last Opus pass.
 * Unit: -O2 -G8 -ffunction-sections -fno-sched-interblock (L00_003B9CB8-nointerblock-g8.c
 * flags; tested as its own unit). USA v1.01: 708/708 bytes, 0/27 places, first difference
 * +0x130, 6 words differ (previous best 47). Everything else equals retail, including all
 * saved registers and the render loop.
 * Remaining differences:
 * - +0x130..+0x13C: this draft has (i*160 + (D_1 + 12)) + 16, retail (D + i*160) + 0x1C.
 * - +0x1FC..+0x204: retail `ori s1` comes before the D_29A0 la, here after it.
 * What fixed the rest (all measured):
 * - 2990/29A0/29B0 are MACRO_ADDR; retail's tied lui+addiu pairs are unsplit la's.
 * - The render loop is a plain index loop over a separate counter (`for (k=0;k<5;k++)`):
 *   loop.c reverses it and makes the pointers givs, which gives s0/s1/s2/s3 exactly. The
 *   conditional `continue` also keeps 29A0 from being anticipated at the function top.
 * - So no fadeBase local and no alias are needed: `D_L00_001B29A0[i]=...` in both arms makes
 *   PRE insert the la at the end of the if-block's first block, which is retail's lui fp.
 * The w store is the only blocker left. The `_1` alias here stands in for the real form and
 * must not land as such. Measured chain for retail's +0x1C: fold(16 + (M + X)) with X a
 * register gives t = M + X, s2 = t + 16; cse2 relates X to D + 12; combine then turns that
 * chain of three into (D + M) + 0x1C, reloading the spilled D a second time, not shared with
 * dest's sum because combine runs after cse2. This needs X = const(D+12) opaque at loop time,
 * so unrelated to D in cse1 although dest's D la sits earlier in the same body block.
 * With X related, add_val is ((M + 12) + D); fold gives M + (D + 12), force_operand gives
 * two feeders (M + 16) + (D + 12), and combine only tries linked chains. func_L00_003C4F78
 * has the same blocker: retail relates D+K to D only after loop.c.
 * Tried, worse: row = D[i] before the loop (merges with the [0][3] sum in cse1: 692),
 * row from an alias or alias [0][3] (PRE hoists the alias above loop 1 into s7: 716),
 * explicit `float *w` stepped by 4 from &D[i][1][3] (folds into the symbol: 700), w computed
 * before the call (combined as 0xC(s0): 684), function-top table for w (33-39 words),
 * -fno-rerun-loop-opt not retried here. Dropping MACRO_ADDR from 29A0: 87 words.
 */
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
extern short D_L00_001B2990[] MACRO_ADDR;
extern short D_L00_001B29A0[] MACRO_ADDR;
extern short D_L00_001B29B0[] MACRO_ADDR;
typedef float Vector[4] __attribute__((aligned(16)));
extern Vector D_L00_00240080[5][10];
extern Vector D_L00_00240080_1[5][10];
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
            for (j=1;j<10;j++) {
                func_00282CF0(vectors[j],D_L00_00240080[i][j],D_L00_00240070[i * 10 + j]);
            }
            qcopy(D_L00_00240080[i],pos);
            D_L00_00240080[i][0][3]=0.04f;
            for (j=1;j<10;j++) {
                func_00282CC0(D_L00_00240080[i][j],D_L00_00240070[i * 10 + j],vectors[j]);
                *(D_L00_00240080_1[i][j] + 3)=0.04f;
            }
            if (D_L00_001B2990[i]>0) {
                ratio=func_00283CE0(D_L00_001B2990[i])/func_00283CE0(D_L00_001B29B0[i]);
                D_L00_001B29A0[i]=func_00283CF0(D_L00_001AFF8C*ratio);
            } else D_L00_001B29A0[i]=0;
        }
    }
    for (k=0;k<5;k++) {
        unsigned int alpha,c,c0;
        if (!D_L00_001B2990[k]) continue;
        alpha=func_00283CF0(func_00283CE0(D_L00_001B29A0[k])*D_L00_001B29D4[0])<<24;
        c0=D_L00_001AFF24;
        c=D_L00_001AFF20;
        if (*(short *)(owner+0xAA)==0x1192) c=c0;
        func_L00_002EBE88(D_L00_00240080[k],23,14,10,alpha|0x7F7F7F,alpha|c,0.0f,4.0f,0.0f);
    }
}
