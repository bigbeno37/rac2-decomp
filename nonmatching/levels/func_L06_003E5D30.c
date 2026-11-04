/* Claude Opus 5.5 (xhigh), task v2441. Best: 976/976 bytes, 0/3 places, 3 instructions differ
 * (first at +0x2A0). Own unit L06_003E5D30-g8.c, -O2 -G8 -ffunction-sections (-fmove-all-movables
 * changes nothing here). No removed orch/main switch is used.
 *
 * Compared with the v1673 draft (968 bytes, cse2 merged the col*16+sp sums): temporary[k] goes
 * through a second pointer, middle = &temporary[1][col], whose start is sp + (col*16 + 0xA0), so
 * cse2 no longer folds it into the column base; temporary[k-1] is written directly. Frame, saved
 * registers, spill slots and the col/col*16 registers ($s0/$s1) now all follow retail.
 *
 * What still differs, smoothing-loop preheader only:
 *     retail: addu v1,s1,sp ; addu a0,sp,s1 ; ... addiu s2,a0,0xA0
 *     here:   addu v1,sp,s1 ; addiu a0,s1,0xA0 ; ... addu s2,sp,a0
 * Retail's temporary[k] start (sp + col*16) + 0xA0 cannot exist at cse2 next to the column base
 * (cse2 merges it: 968 bytes), so it is probably made later by combine splitting
 * sp + (col*16 + 0xA0); here combine has no third insn to do that split.
 * Measured: column = (L06GridVector (*)[10])(col * 16 + (int)temporary) gives retail's
 * addu v1,s1,sp but then the temporary[k-1] start is addu s1,sp,s1 instead of a copy of v1
 * (still 3 instructions). column[k-1] for temporary[k-1] swaps $s0/$s1 (50 rows); direct
 * temporary[k+1] gives 984 bytes; middle as &temporary[0][col] + 10 or column-based sums merge
 * again (968). temporary[k-1] spelled as temporary[k - 1] + col or &temporary[k][col] - 10 ties.
 * Next: a source form for temporary[k] whose sp + (col*16 + 0xA0) chain has a third insn for
 * combine to split, or the col*16 + sp column base together with a temporary[k-1] start that
 * reload_cse can see as the same sum.
 * Required declarations are retained below.
 */
#include "common.h"
typedef struct { float f[4]; } __attribute__((aligned(16))) L06GridVector;
typedef struct {
    L06GridVector point[4][30][20];
    L06GridVector velocity[4][30][20];
    int timer[4][30];
    float phase;
} L06Grid;
extern int D_001A79F0;
extern u8 D_0018C0D4[] NOT_SDA;
extern int D_L06_001B31B0;
extern int D_L06_001B31B4;
extern float D_L06_001AF718 __attribute__((sda));
extern float D_L06_001AF714 __attribute__((sda));
extern float D_L06_001AF710 __attribute__((sda));
extern void func_0029F338(void *);
extern int func_00282948(int *);
extern void func_00282C88(void *);
extern void func_002A7A00(void *, float, float);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282D30(void *, void *, float);

void func_L06_003E5D30(u8 *moby)
{
    L06Grid *st = *(L06Grid **)(moby + 0x68);
    L06GridVector temporary[20][10];

    switch (moby[0x20]) {
    case 0:
        if (D_001A79F0 == 10 && D_0018C0D4[0] == 1) {
            func_0029F338(moby);
            break;
        }
        D_L06_001B31B4 = 22;
        D_L06_001B31B0 = 45;
        {
            int i;
            for (i = 0; i < 10; i++) {
                int delay = i * D_L06_001B31B0 / 10;
                st->timer[0][i] = delay;
                st->timer[1][i] = delay + 11;
                st->timer[2][i] = delay + 23;
                st->timer[3][i] = delay + 34;
            }
        }
        moby[0x20] = 1;
        break;
    case 1: {
        int group, col, row;

        st->phase += D_L06_001AF718 * (1.0f/60.0f);
        if (st->phase > 1.0f) st->phase -= 1.0f;
        for (group = 0; group < 4; group++) {
            for (col = 0; col < 10; col++) {
                int hit = func_00282948(&st->timer[group][col]);
                int base = group * 0x2580;
                L06GridVector (*points)[20] = (L06GridVector (*)[20])((char *)st + base);
                L06GridVector (*velocities)[20] = (L06GridVector (*)[20])((char *)st + (base + 0x9600));
                if (hit) {
                    for (row = 0; row < 20; row++) {
                        func_00282C88(&points[row][col]);
                        ((L06GridVector *)((char *)st + (row * 0x140 + (col * 16 + base))))->f[1] = row * 0.05f * D_L06_001AF714 - D_L06_001AF714 * 0.5f;
                        ((L06GridVector *)((char *)st + (row * 0x140 + (col * 16 + base))))->f[3] = 1.0f;
                        func_002A7A00(&temporary[row][col], 0.0f, D_L06_001AF710 * (1.0f/60.0f));
                    }
                    {
                        int k;
                        L06GridVector (*column)[10] = (L06GridVector (*)[10])&temporary[0][col];
                        L06GridVector (*middle)[10] = (L06GridVector (*)[10])&temporary[1][col];
                        for (k = 1; k < 19; k++) {
                            func_00282CC0(&velocities[k][col], &temporary[k - 1][col], middle[k - 1]);
                            func_00282CC0(&velocities[k][col], &velocities[k][col], column[k + 1]);
                            func_00282D30(&velocities[k][col], &velocities[k][col], 0.333f);
                        }
                    }
                    st->timer[group][col] = D_L06_001B31B0;
                } else {
                    int k;
                    for (k = 1; k < 19; k++) {
                        func_00282CC0(&points[k][col], &points[k][col], &velocities[k][col]);
                    }
                }
            }
        }
        break;
    }
    }
}
