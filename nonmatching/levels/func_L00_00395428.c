/* Opus 5.5 tier (v2521, 2026-11-05); default -O2 -G8 -ffunction-sections, so it goes in a
 * <file>-g8.c unit (tried as src/levels/shared/L00_0038F178-arc-g8.c).
 * US: 1400/1428 bytes, 0/27 placements, 246 of 357 rows differ (previous draft 1412, 253 rows).
 * PAL: not in its catalogue. v2.00: 0/27, same bytes.
 * This revision fixes:
 * - case 0: a count-up indexed loop with its own counter n gives retail's caller-saved
 *   v0/v1/a0/a1 registers and pointer order exactly.
 * - The table blocker the previous header named: &(D[i] + -1)[j] for the j-1 pointer.
 *   pointer_int_sum turns the -1 into a wrapped unsigned sizetype (4294967280 on this
 *   64-bit host), and plus_constant folds it into the symbol. cse use_related_value
 *   no longer ties D to D-16 (the offset is not a SMALL_INT), so every loop keeps
 *   separate D and D-16 pseudos. Reload rematerialises them per preheader, as retail
 *   does. The j pointer is now retail's (la D + i*80) + 16.
 *   The canonical &D[i][j - 1] (also j - 1u, (D[i] - 1)[j] and (&D[i][0])[j - 1],
 *   which fold to the same tree) gives D = reg(D-16) + 16 at cse1 and a j pointer of
 *   la D + (i*80 + 16). &D[i][j] + -1 gives 1412 (la D+16 + (i*80 - 16)).
 *   A row pointer `prev = D[i] - 1` before each loop gives 1420, but the else arm then
 *   shares prev across both loops and spills i+1.
 * Still differs:
 * - j-1 init order: ours (la(D-16) + i*80) + 16, retail (i*80 + la(D-16)) + 16.
 * - The D[i][0] store: retail does mtlo D / madd with a separate mult for the loop.
 *   Here cse2 shares the loop's mult, so the store is a plain addu. The canonical form
 *   kept the madd.
 * - Allocation: object $s7 (retail $fp), i/i*2 in s5/s4 (retail s4/s5), i+1 in $fp
 *   (retail s6), D_L00_001B8DC0's %hi in s6 (retail s7).
 * - bnez rather than bnel (with addiu v1,80 in the likely slot) at the else arm.
 * - The !emitting `continue` steals i+1 into the beqz delay slot and branches to
 *   the latch. Retail branches to a `b latch; addiu s6,s4,1` stub.
 * - beqz on the switch: delay slot copied from case 0 (retail fills from fallthrough).
 * Declarations: the externs below, plus include "common.h" (NOT_SDA, MACRO_ADDR).
 * The older comments below are kept as history, not as conclusions.
 */
/* Reconstructed on 2026-10-23, revised 2026-11-03 (v2138): NOT_SDA on D_L00_0025C64C (split
 * lui in the beq delay slot); MACRO_ADDR on the four short arrays (per-access lui/addiu);
 * separate scan index k (caller-saved scan loop); count-up indexed j-loops (loop.c
 * recomputes i*80 per preheader; the D[i][0] store uses mtlo/madd).
 * The MMI (SPECIAL2) words claimed by the 2026-10 note (0x72821800 at +0x030C, +0x049C)
 * are the madd instructions this C emits: they are not a blocker.
 */

#include "common.h"
typedef int Quadword __attribute__((mode(TI)));
typedef union { Quadword q; float f[4]; } Vector;
typedef struct { char pad[0x2290]; char *object; int mode; } WorldState;
extern WorldState D_00189E20;
extern float D_L00_0025C64C NOT_SDA;
extern short D_L00_001B2778[] MACRO_ADDR, D_L00_001B2790[] MACRO_ADDR, D_L00_001B27A8[] MACRO_ADDR, D_L00_001B27C0[] MACRO_ADDR;
extern int D_L00_001AF278 __attribute__((sda));
extern int D_L00_001AF27C __attribute__((sda));
extern float D_L00_001AF274 __attribute__((sda));
extern float D_L00_001B27DC __attribute__((sda));
extern int D_L00_001B27D4, D_L00_001B27D8;
extern Vector D_L00_00239EF0[][5];
extern Vector D_L00_001B8DC0;
extern void func_00282D30(void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern float func_0029F530(void *);
extern int func_00282948(int *);
extern float func_00283CE0(int);
extern void func_002A7470(void *, int, int, int);
extern void func_002A7988(void *, float, float);
extern float func_002A7878(float, float);
extern float func_002A78D8(float, float);
extern void func_002ACEC0(void *, void *, void *, float);
extern void func_L00_003959C0(void);
extern void func_L00_002E6EF8(void *, void *);

void func_L00_00395428(char *object)
{
    Vector origin, random, difference, offsets[5];
    int emitting = 0;
    int i, j, k, active;
    float distance;
    if (*(unsigned char *)(D_00189E20.object + 0x43) != 0x7B)
        D_L00_0025C64C = 0.1f;
    switch (*(unsigned char *)(object + 0x20)) {
    case 0:
        *(unsigned char *)(object + 0x20) = 2;
        {
            int n;
            for (n = 0; n < 10; n++) {
                D_L00_001B2778[n] = 0;
                D_L00_001B27A8[n] = 2;
                D_L00_001B27C0[n] = 1;
            }
        }
        D_L00_001B27D4 = D_L00_001AF278;
        D_L00_001B27D8 = D_L00_001AF27C;
        break;
    case 2:
        if (D_00189E20.mode == 31) {
            distance = func_0029F530(object);
            if (distance >= 6.0f) {
                emitting = 1;
                if (!func_00282948(&D_L00_001B27D4)) {
                    float elapsed = func_00283CE0(D_L00_001B27D4);
                    elapsed /= func_00283CE0(D_L00_001AF278);
                    D_L00_001B27DC = 1.0f - elapsed;
                } else if (distance >= 12.0f) {
                    if (!func_00282948(&D_L00_001B27D8)) {
                        float elapsed = func_00283CE0(D_L00_001B27D8);
                        elapsed /= func_00283CE0(D_L00_001AF27C);
                        D_L00_001B27DC = elapsed;
                    } else {
                        D_L00_001B27DC = 0.0f;
                    }
                } else {
                    D_L00_001B27DC = 1.0f;
                }
            }
        } else {
            D_L00_001B27D4 = D_L00_001AF278;
            D_L00_001B27D8 = D_L00_001AF27C;
            func_002A7470(object, 1, 0, 12);
        }
        break;
    }
    func_00282D30(&origin, object + 0xD0, -0.5f);
    func_00282CC0(&origin, &origin, object + 0x10);
    active = 0;
    for (k = 0; k < 10; k++) {
        if (D_L00_001B2778[k]) {
            active = 1;
            break;
        }
    }
    if (!emitting && !active)
        return;
    for (i = 0; i < 10; i++) {
        if (--D_L00_001B2778[i] < 0)
            D_L00_001B2778[i] = 0;
        if (!D_L00_001B2778[i]) {
            if (!emitting)
                continue;
            D_L00_001B2778[i] = 2;
            D_L00_001B2790[i] = 64;
            func_002A7988(&random, D_L00_001AF274, D_L00_001AF274);
            D_L00_00239EF0[i][0].q = origin.q;
            {
                float sign = 1.0f;
                for (j = 1; j < 5; j++) {
                    func_00282CF0(&difference, &(D_L00_00239EF0[i] + -1)[j], &D_L00_001B8DC0);
                    func_002ACEC0(&random, &random, &difference,
                                 func_002A7878(0.174532949924469f, 0.7853982448577881f) * sign);
                    sign = -sign;
                    func_00282CC0(&D_L00_00239EF0[i][j], &(D_L00_00239EF0[i] + -1)[j], &random);
                }
            }
        } else {
            for (j = 1; j < 5; j++) {
                func_00282CF0(&offsets[j], &D_L00_00239EF0[i][j], &(D_L00_00239EF0[i] + -1)[j]);
                offsets[j].f[0] += func_002A78D8(0.0f, 0.1f);
                offsets[j].f[1] += func_002A78D8(0.0f, 0.1f);
                offsets[j].f[2] += func_002A78D8(0.0f, 0.1f);
            }
            D_L00_00239EF0[i][0].q = origin.q;
            for (j = 1; j < 5; j++)
                func_00282CC0(&D_L00_00239EF0[i][j], &(D_L00_00239EF0[i] + -1)[j], &offsets[j]);
            if (D_L00_001B2778[i] > 0)
                D_L00_001B2790[i] = 32;
            else
                D_L00_001B2790[i] = 0;
        }
    }
    func_L00_002E6EF8(func_L00_003959C0, object);
}
