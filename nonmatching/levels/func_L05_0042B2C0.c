/* Claude Opus 5.5 (opus-1), worker v2392, USA v1.01.
 * Profile: -O2 -G8 -ffunction-sections -frac2-zero-ti-por.
 * Best: 2104/2128 bytes, frame 0x150 (retail 0x150), 502 rows differ, 0/4
 * places, branches at 108,152,347,445,501 (retail 108,152,350,451,507).
 * Not a verified behavioral match. All declarations are below; retry in a
 * dedicated -g8.c unit. Replaces the v1347 draft (2120 bytes, frame 0x1A0):
 * this one is 16 bytes further in size but has the right frame and nearly
 * retail's inner-loop instruction order.
 *
 * NOT LANDABLE AS WRITTEN: the frame depends on the volatile w field below,
 * which is a compiler-state model, not a proven fact about the object. Plain
 * VBC gives 2272 bytes, frame 0x180, 563 rows. If volatile is the answer it
 * belongs on the AE element type alone, and it needs review under the volatile
 * rule; every AE w access here is a store of 0.
 *
 * Why volatile: retail's inner loop keeps both increments of each induction
 * variable (s7 +1, s5/s2 +12, s4 +4, s0 +8, each twice), so loop.c's biv
 * increment conversion ran in neither pass. It runs if n_extra_increment &&
 * !loop_has_volatile. Ruled out: a test (both `addiu s7` sit before the two
 * calls, so no block boundary at sched time; jump2 runs after sched2, and no
 * test folded by cse2 was found); an inline &out helper (ADDRESSOF leaves out
 * no biv, so mult stays in the loop; 2096, 0x140); a volatile color parameter
 * (takes the first stack slot; locals shift 0x10). Volatile on the other
 * objects: AE whole 2096/498 rows, CE 2096/507, radial 2096/514. POS plus
 * startU gives 2128 bytes, frame 0x160, 473 rows, the best size and row count,
 * but it contradicts retail's forwarded z, so it is diagnostic only.
 *
 * Recovered structure: startU is reused as the next row's U (f22), no nextU;
 * first = out + 1 before the row test and out = first in the i == 0 arm, so
 * out is no biv of the outer loop; the second half of the inner body keeps the
 * i == 0 statement order, whose UV[out][1] read gives retail's -4/0 uv+4 base;
 * the draw loop uses return rather than break (break leaves a mid-loop b).
 *
 * Remaining: (1) roles: retail a3 = &radial, s6 = CE ptr, s7 = out; here a3 =
 * CE ptr, s7 = &radial, s6 = out, s0-s3 rotated, which costs one lq position
 * per iteration and dominates the row count. (2) i == 0 arm is 3 short: retail
 * y is la sym+4; addu; swc1 0(t1), here la sym; addu; swc1 4(t0) through cse's
 * related value. (3) tail: retail's pg has a fresh mult/madd and rereads z, x,
 * y from t0; that needs out's qty to change between pf and pg with i-based
 * entries kept (unsolved; reusing pf/q gives 2112 bytes, 504 rows). (4) slots:
 * retail first 0x90, first<<2 0x94, &rotation2 0x98, &inverse2 0x9C, row+1
 * 0xA0, against first 0x8C, &rotation2 0x94, row+1 0x98, &inverse2 0x9C,
 * first<<2 0xA0 here.
 *
 * Next: fix the roles first; only then do the row counts mean much. Stopped at
 * the effort bound, about 90 tool calls.
 */
#include "common.h"
typedef unsigned int QBC __attribute__((mode(TI)));
typedef union { QBC q; float v[4]; struct { float xyz[3]; volatile float w; } s; } VBC;
extern VBC D_L05_00270AE0[32];
extern float D_L05_0026ECE0[][3], D_L05_002700E0[][2], D_L05_00270D60[][2];
extern unsigned D_L05_0026FBE0[];
extern unsigned D_L05_00270CE0[];
extern int D_L05_001B1724 __attribute__((sda));
extern void ref_L05_0042B2C0_00(void *, void *, float);
extern void ref_L05_0042B2C0_01(void *, void *, float);
extern void ref_L05_0042B2C0_02(void *, void *, void *);
extern void ref_L05_0042B2C0_03(void *, void *, void *);
extern unsigned long long ref_L05_0042B2C0_04(int);
extern void ref_L05_0042B2C0_05(int, unsigned long long);
extern void ref_L05_0042B2C0_06(void);
extern void ref_L05_0042B2C0_07(int, void *, void *, void *, int);
void func_L05_0042B2C0(VBC *origin, VBC *axis, int texture, unsigned color,
 float advance, float radius, float stepU, float stepV, float startV, float startU)
{
    VBC unit, center, delta, radial, rotation, inverse, rotation2, inverse2;
    float u = startU, v;
    int i, row, out = 0, remaining, base, first;
    unsigned c; int next;
    float *pa, *pb, *pc, *pd, *pe, *pf, *pg, *q;
    unit.q = 0;
    unit.v[3] = unit.v[2] = 1.0f;
    qcopy(&center, origin);
    ref_L05_0042B2C0_00(&delta, axis, advance);
    ref_L05_0042B2C0_00(&radial, &unit, radius);
    radial.v[3] = 0;
    ref_L05_0042B2C0_01(&rotation, axis, 0.202683404f);
    ref_L05_0042B2C0_00(&inverse, &rotation, -1.0f);
    inverse.v[3] = rotation.v[3];
    ref_L05_0042B2C0_02(&D_L05_00270AE0[0], &center, &radial);
    D_L05_00270D60[0][0] = u;
    D_L05_00270D60[0][1] = startV;
    D_L05_00270CE0[0] = color;
    v = startV + stepU;
    for (i = 1; i < 31; i++) {
        ref_L05_0042B2C0_03(&radial, &rotation, &radial);
        ref_L05_0042B2C0_03(&radial, &radial, &inverse);
        ref_L05_0042B2C0_02(&D_L05_00270AE0[i], &center, &radial);
        D_L05_00270D60[i][1] = v;
        v += stepU;
        D_L05_00270CE0[i] = color;
        D_L05_00270D60[i][0] = startU;
    }
    qcopy(&D_L05_00270AE0[31], &D_L05_00270AE0[0]);
    D_L05_00270CE0[31] = D_L05_00270CE0[0];
    D_L05_00270D60[31][1] = v;
    D_L05_00270D60[31][0] = startU;
    ref_L05_0042B2C0_02(&center, &center, &delta);
    u += stepV;
    row = 1;
    do {
        ref_L05_0042B2C0_00(&radial, &unit, radius);
        v = startV;
        ref_L05_0042B2C0_01(&rotation2, axis, 0.202683404f);
        radial.v[3] = 0;
        ref_L05_0042B2C0_00(&inverse2, &rotation2, -1.0f);
        inverse2.v[3] = rotation2.v[3];
        first = out + 1;
        if (row == 5) color &= 0xFFFFFF;
        i = 0;
        pa = D_L05_0026ECE0[out];
        pa[0] = D_L05_00270AE0[i].v[0];
        pa[1] = D_L05_00270AE0[i].v[1];
        pa[2] = D_L05_00270AE0[i].v[2];
        D_L05_002700E0[out][0] = D_L05_00270D60[i][0];
        D_L05_002700E0[out][1] = D_L05_00270D60[i][1];
        D_L05_0026FBE0[out] = D_L05_00270CE0[i];
        out = first;
        D_L05_0026ECE0[out][0] = center.v[0] + radial.v[0];
        D_L05_0026ECE0[out][1] = center.v[1] + radial.v[1];
        D_L05_0026ECE0[out][2] = center.v[2] + radial.v[2];
        D_L05_002700E0[out][1] = v;
        D_L05_002700E0[out][0] = u;
        D_L05_0026FBE0[out] = color;
        pb = D_L05_0026ECE0[out];
        D_L05_00270AE0[i].v[0] = pb[0];
        D_L05_00270AE0[i].v[1] = pb[1];
        D_L05_00270AE0[i].v[2] = pb[2];
        D_L05_00270AE0[i].s.w = 0;
        D_L05_00270D60[i][0] = u;
        D_L05_00270D60[i][1] = D_L05_002700E0[out][1];
        D_L05_00270CE0[i] = color;
        out++;
        startU = u + stepV;
        v += stepU;
        i = 1;
        do {
            pc = D_L05_0026ECE0[out];
            pc[0] = D_L05_00270AE0[i].v[0];
            pc[1] = D_L05_00270AE0[i].v[1];
            pc[2] = D_L05_00270AE0[i].v[2];
            D_L05_002700E0[out][0] = D_L05_00270D60[i][0];
            D_L05_002700E0[out][1] = D_L05_00270D60[i][1];
            D_L05_0026FBE0[out] = D_L05_00270CE0[i];
            out++;
            ref_L05_0042B2C0_03(&radial, &rotation2, &radial);
            ref_L05_0042B2C0_03(&radial, &radial, &inverse2);
            D_L05_0026ECE0[out][0] = center.v[0] + radial.v[0];
            D_L05_0026ECE0[out][1] = center.v[1] + radial.v[1];
            D_L05_0026ECE0[out][2] = center.v[2] + radial.v[2];
            D_L05_002700E0[out][1] = v;
            D_L05_0026FBE0[out] = color;
            D_L05_002700E0[out][0] = u;
            pd = D_L05_0026ECE0[out];
            D_L05_00270AE0[i].v[0] = pd[0];
            D_L05_00270AE0[i].v[1] = pd[1];
            D_L05_00270AE0[i].v[2] = pd[2];
            D_L05_00270AE0[i].s.w = 0;
            D_L05_00270CE0[i] = color;
            D_L05_00270D60[i][0] = u;
            D_L05_00270D60[i][1] = D_L05_002700E0[out][1];
            out++;
            v += stepU;
        } while (++i < 31);
        pe = D_L05_0026ECE0[out];
        pe[0] = D_L05_00270AE0[i].v[0];
        pe[1] = D_L05_00270AE0[i].v[1];
        pe[2] = D_L05_00270AE0[i].v[2];
        D_L05_002700E0[out][0] = D_L05_00270D60[i][0];
        D_L05_002700E0[out][1] = D_L05_00270D60[i][1];
        D_L05_0026FBE0[out] = D_L05_00270CE0[i];
        next = out + 1;
        pf = D_L05_0026ECE0[next];
        q = D_L05_0026ECE0[first];
        pf[0] = q[0];
        pf[1] = q[1];
        pf[2] = q[2];
        out = next;
        D_L05_002700E0[out][1] = v;
        D_L05_002700E0[out][0] = u;
        c = D_L05_0026FBE0[first];
        D_L05_00270D60[i][0] = u;
        D_L05_00270CE0[i] = c;
        D_L05_00270D60[i][1] = D_L05_002700E0[out][1];
        D_L05_0026FBE0[out] = c;
        pg = D_L05_0026ECE0[out];
        D_L05_00270AE0[i].v[0] = pg[0];
        D_L05_00270AE0[i].v[1] = pg[1];
        D_L05_00270AE0[i].v[2] = pg[2];
        D_L05_00270AE0[i].s.w = 0;
        out++;
        u = startU;
        ref_L05_0042B2C0_02(&center, &center, &delta);
    } while (++row < 6);
    ref_L05_0042B2C0_05(6, ref_L05_0042B2C0_04(texture));
    ref_L05_0042B2C0_05(0x42, 0x8000000040ULL | ((long long)D_L05_001B1724 << 2));
    ref_L05_0042B2C0_05(8, 0);
    ref_L05_0042B2C0_05(0x14, 0xFF9000000260ULL);
    ref_L05_0042B2C0_05(0x47, 0x53001);
    ref_L05_0042B2C0_05(0x4A, 0);
    ref_L05_0042B2C0_06();
    remaining = 320; base = 0;
    while (1) {
        int n = remaining < 149 ? remaining : 148;
        ref_L05_0042B2C0_07(n, D_L05_0026ECE0[base], &D_L05_0026FBE0[base], D_L05_002700E0[base], 1);
        remaining -= n;
        if (remaining <= 0) return;
        remaining += 2;
        base += n - 2;
    }
}
