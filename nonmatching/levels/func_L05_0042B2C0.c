/* Claude Opus 5.5 (opus-2), worker v2564, USA v1.01.
 * Profile: -O2 -G8 -ffunction-sections -frac2-zero-ti-por; retry in a dedicated
 * -g8.c unit. All declarations are below. Not a verified behavioral match.
 *
 * Stock compiler (the only one that counts): 2312/2128 bytes, frame 0x180
 * (retail 0x150), 573 rows, 0/4 places. Not landable.
 * Private cc1 with extra-increment conversion off (see below), this plain C,
 * no volatile: 2088/2128 bytes, frame 0x150, 460 rows, 0/4 places.
 * Same source with `next = out + 1 ... out = next` in the tail and `i = 0;` /
 * AE[i] in the row-start arm: 2104 bytes, 432 rows, but the y address and the
 * D60 setup go wrong (see "pass 2 threshold"); this form is structurally ahead.
 *
 * BLOCKER, this function is a retail witness for docs/COMPILER-NOTES.md "loop:
 * no giv recombination": "the same release-branch change also turned off the
 * biv-to-giv conversion of extra biv increments (n_extra_increment); that has
 * no retail witness yet". Retail's inner loop keeps both increments of every
 * biv (s7 +1, s5/s2 +12, s4 +4, s0 +8, each twice). Measured: disabling the
 * `if (n_extra_increment && ! loop_has_volatile)` branch in strength_reduce
 * gives this plain C frame 0x150 and the same size, frame and diff-row count as
 * the old `volatile float w` stand-in (2104 bytes, 502 rows, on the opus-1 draft;
 * compared by listing, not by object), so the volatile looks like a proxy for
 * that branch and has been dropped. A no_extra_increment_givs
 * transformer, next to no_recombine_givs.py, is what this function needs; the
 * docs say turning it off changes no matched function (re-verify).
 * Volatile stays forbidden under the worker rules.
 *
 * Solved here, on the private compiler (all stack slots now equal retail):
 * - Slots 0x8C copy of &rotation2, 0x90 first, 0x94 first<<2, 0x98 &rotation2,
 *   0x9C &inverse2, 0xA0 row+1: `first` in a scoped block (later pseudo than
 *   the &rotation2 copy, and first<<2 hashes before at+96), `row` in a scoped
 *   block opened before D60[0][0] (hash = regno + 80, needs regno >= 113 to
 *   sort after at+112 in the 225-bucket table).
 * - qcopy source computed first (`src`) gives addiu a0,v1,0x1F0 and puts the
 *   AE base in s1 / the giv start in s5 as in retail.
 * - CE0 statement before the D60 statement in the first half of the inner loop
 *   (retail's store order), so CE0's giv reduces in pass 1 (a pass 2 giv gets a
 *   constant REG_EQUAL, then update_equiv_regs doubles its live length).
 * - `ae = AE + i` (index first) makes the AE lo_sum life 1, not hoisted in pass
 *   1, so AE is the pass 2 giv: i keeps a use, no dbra reversal (retail counts
 *   up with slti fp,31), AE's start comes last, and s1, s4-s7, fp, a3 all take
 *   retail's roles in the inner loop.
 * - Pass 2 threshold: the outer loop's sym+4 movable (life 2, savings 2) needs
 *   T*4 >= real insns, T == 61, so 244 insns or fewer. Dropping `next` and the
 *   dead `i = 0` reach exactly 244: it moves, the arm's y becomes la sym+4 /
 *   addu / swc1 0(t1) as in retail, T then falls below the D60 pair's bar, so
 *   the D60 lo_sum stays in the inner preheader as in retail, and the loop's
 *   AE loads take f3,f4,f2 as in retail.
 *
 * Remaining:
 * (1) s0/s2/s3 rotated in both loops: retail UV s0, ECE0 s2, AE s3, first loop
 *     hi(D60) s0, D60 ptr s1, CE ptr s2, hi(CE0) s3; row in s2, here s0.
 *     Retail's `addiu s0,s0,0xD60` before the first ref_02: the la D60 pseudo
 *     (the lo_sum of D60 after the first loop) must cross that call, so local-alloc gives it s0 and
 *     T(hi D60) gains a preference for s0, which makes the D60 pointer avoid
 *     it. It crosses only if its copy to the outer-loop hoist stays after the
 *     call in sched1, i.e. has no dependents in the block. A reused local
 *     (q = D60[0]) does reach a saved reg, but costs the alias base (alias.c
 *     record_set), so the D60 loads fall behind the ECE0 stores: rejected.
 * (2) tail is 10 instructions short: retail's AE[i] = ECE0[next] reads come from
 *     memory through a second madd t0,s7,a1, so cse1 and cse2 both failed to
 *     merge the two next*12 although they sit in one sched block. That needs a
 *     cse block boundary (label or cse flush) between the seam stores and the
 *     read-back, gone before sched1. No construct found: a test folded in cse1
 *     alone would not stop cse2. `if (i != 0)` in the arm with i = 0 (forbidden
 *     anyway) changed nothing useful.
 * (3) pre-loop schedule follows from (1) and (2): retail sets the ECE0/UV/FBE0
 *     pointers early and D60/CE/AE last; the D60 hi must share the arm's pseudo
 *     (retail daddu v0,a0 + addiu v0,v0,0xD60), which suggests the arm's D60
 *     hi is not hoisted in pass 1 (short life), again a block boundary in the
 *     arm between the old-vertex copy and the new-vertex stores.
 *
 * Recovered structure: 6 rings of 32 vertices swept along axis, rotation step
 * 0.2026834 = 2pi/31, seam vertex copied from the row's first vertex; six
 * contiguous arrays: positions 320*12, colors 320*4, uvs 320*8, then the
 * previous ring: 32 quadwords, 32 colors, 32 uvs. Drawn as strips of at most
 * 148 vertices, overlapping by 2.
 */
#include "common.h"
typedef unsigned int QBC __attribute__((mode(TI)));
typedef union { QBC q; float v[4]; struct { float xyz[3]; float w; } s; } VBC;
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
    int i, out = 0, remaining, base;
    unsigned c;
    float *pa, *pb, *pc, *pd, *pe, *pf, *pg, *q;
    VBC *ae;
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
    {
    int row;
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
    {
        VBC *src = D_L05_00270AE0;
        qcopy(&D_L05_00270AE0[31], src);
    }
    D_L05_00270CE0[31] = D_L05_00270CE0[0];
    D_L05_00270D60[31][1] = v;
    D_L05_00270D60[31][0] = startU;
    ref_L05_0042B2C0_02(&center, &center, &delta);
    u += stepV;
    row = 1;
    do {
        ref_L05_0042B2C0_00(&radial, &unit, radius);
        v = startV;
        radial.v[3] = 0;
        ref_L05_0042B2C0_01(&rotation2, axis, 0.202683404f);
        ref_L05_0042B2C0_00(&inverse2, &rotation2, -1.0f);
        inverse2.v[3] = rotation2.v[3];
        {
        int first = out + 1;
        if (row == 5) color &= 0xFFFFFF;
        pa = D_L05_0026ECE0[out];
        pa[0] = D_L05_00270AE0[0].v[0];
        pa[1] = D_L05_00270AE0[0].v[1];
        pa[2] = D_L05_00270AE0[0].v[2];
        D_L05_002700E0[out][0] = D_L05_00270D60[0][0];
        D_L05_002700E0[out][1] = D_L05_00270D60[0][1];
        D_L05_0026FBE0[out] = D_L05_00270CE0[0];
        out = first;
        D_L05_0026ECE0[out][0] = center.v[0] + radial.v[0];
        D_L05_0026ECE0[out][1] = center.v[1] + radial.v[1];
        D_L05_0026ECE0[out][2] = center.v[2] + radial.v[2];
        D_L05_002700E0[out][1] = v;
        D_L05_002700E0[out][0] = u;
        D_L05_0026FBE0[out] = color;
        pb = D_L05_0026ECE0[out];
        D_L05_00270AE0[0].v[0] = pb[0];
        D_L05_00270AE0[0].v[1] = pb[1];
        D_L05_00270AE0[0].v[2] = pb[2];
        D_L05_00270AE0[0].s.w = 0;
        D_L05_00270D60[0][0] = u;
        D_L05_00270D60[0][1] = D_L05_002700E0[out][1];
        D_L05_00270CE0[0] = color;
        out++;
        startU = u + stepV;
        v += stepU;
        i = 1;
        do {
            pc = D_L05_0026ECE0[out];
            ae = D_L05_00270AE0 + i;
            pc[0] = ae->v[0];
            pc[1] = ae->v[1];
            pc[2] = ae->v[2];
            D_L05_0026FBE0[out] = D_L05_00270CE0[i];
            D_L05_002700E0[out][0] = D_L05_00270D60[i][0];
            D_L05_002700E0[out][1] = D_L05_00270D60[i][1];
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
        out++;
        pf = D_L05_0026ECE0[out];
        q = D_L05_0026ECE0[first];
        pf[0] = q[0];
        pf[1] = q[1];
        pf[2] = q[2];
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
        }
    } while (++row < 6);
    }
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
