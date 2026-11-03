/* Tier: opus-1 (v2313), Claude Opus 5.5, USA v1.01, -O2 -G8 -ffunction-sections.
 * Best check.py: 1184/1184 bytes, 0/27 places; first mismatch +0x68.
 * Three instructions differ: the first timer address (+0x68 lui, +0x6C addiu,
 * +0x70 addu into s1) uses v0 where retail uses v1. Everything else agrees
 * apart from relocation addends.
 * Fixed since the sol-high draft: the row pointer `row = D_L00_002563A0[g]`
 * fixes the reset destination setup at +0x160..+0x16C, and the flat
 * `[g * 10 + j - 1]` previous-node index fixes the block-0 HIGH register and
 * drops the D_L00_00256390 alias.
 * Cause of the rest: reload round-robin. All reaching regs that gcse inserts
 * at the end of bb 0 (high(D_L00_002563A0), la D_L00_001B2D40,
 * high(D_0018BE30)) are spilled and reloaded per use. Retail has one more
 * round-robin reload before the timer la, most likely because base's lo_sum
 * (insn 14) reads the shared spilled high(D_L00_002563A0) pseudo. Here gcse
 * inserts 339 = high(D) after insn 14 and cse2 rewrites it as a copy of the
 * local 93, so insn 14 keeps its own lui and the timer reload takes v0.
 * Removing `base`, declaring it in the loop, or using D directly in the else
 * branch all regressed badly (210-240 differing rows).
 * Next: a source form in which base's lo_sum follows the bb-0 insertion
 * (or reads the PRE reaching reg) without a barrier.
 * No barriers, pinned registers or volatile accesses were used.
 * Required declarations are below; compile as a -G8 unit.
 */
#include "common.h"

typedef float Vector[4] __attribute__((aligned(16)));
extern int func_00282978(void *);
extern float func_002A7878(float, float);
extern float func_002A78D8(float, float);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern void func_L00_0031AA28(void *, void *, void *, void *, void *, float);
extern short D_L00_001B2D40[] __attribute__((sda));
extern short D_L00_001B2D48[] __attribute__((sda));
extern short D_L00_001B2D50[] __attribute__((sda));
extern float D_0018BE30[4];
extern float D_L00_001B100C __attribute__((sda));
extern float D_L00_001B1004 __attribute__((sda));
extern Vector D_L00_002563A0[][10];

void func_L00_0040B398(char *object)
{
    Vector p, delta, out, diffs[10];
    char *state = *(char **)(object + 0x68);
    int g, i, j, k, l, n;
    float (*base)[4] = (float (*)[4])D_L00_002563A0;
    Vector *row;

    for (g = 0; g < 4; g++) {
        if (func_00282978(&D_L00_001B2D40[g])) {
            D_L00_001B2D48[g] = 0;
            D_L00_001B2D40[g] = D_L00_001B2D50[g];
            qcopy(p, state + 0x30);
            p[0] += func_002A7878(-0.5f, 0.5f);
            p[1] += func_002A7878(-0.5f, 0.5f);
            p[2] += func_002A7878(-0.5f, 0.5f);
            func_00282CF0(delta, p, D_0018BE30);
            func_00282F20(delta, delta, D_L00_001B100C);
            func_00282CC0(delta, p, delta);
            row = D_L00_002563A0[g];
            for (i = 0; i < 10; i++) {
                float t = i * 0.1f;
                if (i == 9)
                    t = 1.0f;
                func_L00_0031AA28(out, D_0018BE30, p, state + 0x40, delta, t);
                qcopy(row[i], out);
            }
        } else {
            for (j = 1; j < 10; j++) {
                func_00282CF0(diffs[j], ((Vector (*)[10])base)[g][j], ((Vector *)D_L00_002563A0)[g * 10 + j - 1]);
                diffs[j][0] += func_002A78D8(0.0f, 0.05f);
                diffs[j][1] += func_002A78D8(0.0f, 0.05f);
                diffs[j][2] += func_002A78D8(0.0f, 0.05f);
            }
            qcopy(base[g * 10], D_0018BE30);
            for (k = 1; k < 10; k++)
                func_00282CC0(((Vector (*)[10])base)[g][k], ((Vector *)D_L00_002563A0)[g * 10 + k - 1], diffs[k]);
        }
        for (l = 2; l < 9; l++) {
            qcopy(p, base[l]);
            p[0] += func_002A78D8(0.0f, D_L00_001B1004);
            p[1] += func_002A78D8(0.0f, D_L00_001B1004);
            p[2] += func_002A78D8(0.0f, D_L00_001B1004);
            qcopy(base[l + 10], p);
        }
        for (n = 2; n < 9; n++) {
            qcopy(p, base[n]);
            p[0] += func_002A78D8(0.0f, D_L00_001B1004);
            p[1] += func_002A78D8(0.0f, D_L00_001B1004);
            p[2] += func_002A78D8(0.0f, D_L00_001B1004);
            qcopy(base[n + 20], p);
        }
        for (i = 2; i < 9; i++) {
            qcopy(p, base[i]);
            p[0] += func_002A78D8(0.0f, D_L00_001B1004);
            p[1] += func_002A78D8(0.0f, D_L00_001B1004);
            p[2] += func_002A78D8(0.0f, D_L00_001B1004);
            qcopy(base[i + 30], p);
        }
    }
}
