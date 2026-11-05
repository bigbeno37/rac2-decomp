/* func_002E7338 - 568 bytes, 28 places (boot and 27 levels).
 * Tier opus-5.5 (third Opus pass, 5 November 2026), game -O2 -G8
 * -ffunction-sections (append to src/game/002E7018-g8.c, or a unit of its own
 * named *-g8.c).
 * check.py: 568/568 bytes, 0/28 places; exactly 3 instructions differ, all by
 * register only (+0x100, +0x110, +0x114). Everything else, including frame,
 * spill slots and the loop-alignment nop, equals retail:
 *
 * Retail:  blez v0,exit / lui v1,0x20 / li.s f21,0.025 / ori v1,v1,0x58B0 /
 *          sw v1,0xA8(sp) / lh a0,0xB2(s7) / nop
 * Here:    blez v0,exit / lui v0,0x20 / li.s f21,0.025 / ori v0,v0,0x58B0 /
 *          sw v0,0xA8(sp) / lh a0,0xB2(s7) / nop
 *
 * Why this form has one store: `high` is set outside the loops, so cse cannot
 * fold `high | 0x58B0` and gcse cprop cannot either ((ior const const) is not
 * an insn). loop hoists the ior, so sched1 has no large constant to split and
 * the spilled baseColor has a single set. update_equiv_regs then sinks
 * `high = 0x200000` to just before the ior (local-alloc.c, the REG_EQUIV
 * single-use path), where it is a block-local pseudo, and combine_reloads
 * reuses its dying register for baseColor's output reload.
 *
 * Why it is $v0: local-alloc gives that local the first free register, and
 * nothing else lives in the preheader. Retail's $v1 is reload's round-robin
 * register after the $v0 of the count test, which needs `high` to have no hard
 * register at all, so that reload rematerializes it (in + out reload).
 * Blocking the sink needs REG_N_REFS(high) != 2, depth != 0 at the ior, or
 * REG_N_DEATHS != 1. Each way found needs something the rules forbid or the
 * retail shape lacks: a second use that vanishes, two sets on identical
 * arms, a set in the UV loop behind a label, or a once-only enclosing loop.
 * No per-file flag in config/cflags.txt helps: -fno-expensive-optimizations,
 * -frac2-gcse-unsplit-edges, -frac2-keep-block-order, -fmove-all-movables and
 * -fno-sched-interblock leave the 3 rows; -fno-gcse, -fno-rerun-loop-opt,
 * -mno-split-addresses, -ffast-math and -fno-schedule-insns are far worse.
 *
 * Toolchain route (reports v2028, v2505): tools/compiler/transformers/
 * reload_reg_used_guard.py cancels every inherited RELOAD_OTHER reload, so the
 * in-out reload of a split constant in a spilled pseudo never inherits. v2028
 * reported, on a private cc1 only, that testing the set saved right after
 * IOR_COMPL_HARD_REG_SET removes all 4 differences of the previous draft. That
 * draft, not this one, is the form to use then: drop `high`, and write
 *     baseColor = 0x2058B0;
 * before the `scale =` line, then color = (c << 24) | baseColor;
 * It compiles today to 568/568 with an extra store at +0x110..+0x11c.
 *
 * reload1.c paths checked this pass, none reachable from C: combine_reloads
 * always merges the ior's in and out (RELOAD_OTHER); the find_equiv_reg path
 * at emit time and the redirect-previous-insn path both call
 * reload_reg_free_for_value_p, which the guard refuses; the "previous store"
 * deletion needs reload_in_reg != the pseudo. A large constant never keeps a
 * REG_EQUIV: sched1 splits it first (two sets).
 *
 * The body corresponds to RAC1's DrawResidentEffectQuads (func_0022F4C8,
 * src/game/space.c) without its 0x215 colour case. RAC1 writes the colour as
 * (c << 24) | 0x2058B0, which here stays in the loop (560 bytes).
 */
#include "common.h"

typedef struct { float f[4]; } EffectRow;
typedef struct {
    float positions[4][4];
    unsigned int colors[4];
    float uv[4][2];
    unsigned long long reserved, texture, state, primitive;
} EffectQuad;
extern unsigned int D_001A7AF0 MACRO_ADDR;
extern int D_001AC040 __attribute__((sda));
extern EffectRow *D_001AC048 __attribute__((sda));
extern int D_001AC050 __attribute__((sda));
extern float D_001ABFF0 __attribute__((sda));
extern float D_001ABFF4[] MACRO_ADDR;
extern long func_0027C878(int);
extern void func_00282D30(void *, void *, float);
extern int func_002A77E0(int);
extern float func_00283CE0(int);
extern void func_00282CC0(void *, void *, void *);
extern void func_00283098(void *, void *, void *);
extern void func_00280B90(void *, int, int);

void func_002E7338(unsigned char *object)
{
    EffectQuad quad;
    EffectRow position;
    EffectRow *table;
    int count;
    int i, j;
    unsigned int baseColor;
    unsigned int high = 0x200000;
    int variant = (D_001A7AF0 >> 4) & 1;
    table = (&D_001AC048)[variant];
    count = (&D_001AC040)[variant];
    quad.texture = func_0027C878(5);
    quad.state = 0xFF9000000260ULL;
    quad.primitive = 0x8000000048ULL;
    quad.reserved = 0;
    {
        float *vSource = D_001ABFF4;
        unsigned char *vDest = (unsigned char *)&quad + 0x54;
        unsigned char *uDest = (unsigned char *)&quad + 0x50;
        float *uSource = &D_001ABFF0;
        int k;
        for (k = 0; k < 4; k++) {
            *(float *)uDest = *uSource;
            *(float *)vDest = *vSource;
            uDest += 8;
            vDest += 8;
            uSource += 2;
            vSource += 2;
        }
    }
    func_00282D30(&position, object, 0.0009765625f);
    for (i = 0; i < count; i++) {
        int c = object[0xBC];
        unsigned int color;
        float scale;
        if (*(short *)(object + 0xB2)) c += func_002A77E0(*(short *)(object + 0xB2));
        scale = func_00283CE0(c) * 0.025f * table[i].f[3];
        baseColor = high | 0x58B0;
        color = (c << 24) | baseColor;
        for (j = 0; j < 4; j++) {
            quad.colors[j] = color;
            func_00282D30(quad.positions[j], ((EffectRow *)&D_001AC050) + j, scale);
            func_00282CC0(quad.positions[j], quad.positions[j], &table[i]);
            func_00283098(quad.positions[j], quad.positions[j], object + 0xC0);
            func_00282CC0(quad.positions[j], quad.positions[j], &position);
        }
        func_00280B90(&quad, 0, 0);
    }
}
