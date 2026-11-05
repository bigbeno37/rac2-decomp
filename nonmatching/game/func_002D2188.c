/* v2585 (Claude Opus 5.5, 2026-11-05): EXACT BYTES, 592/592 and 28 of 28 places with cdiff.py, but in a form the worker
 * rules forbid, so reported FAILED and not placed in src/. The body below is one of two verified exact forms.
 * Build it as its own unit (for example src/game/002D2188-rc2.c), flags as func_002D1E70:
 * -O2 -G8 -fno-expensive-optimizations -ffunction-sections -fno-builtin. It needs nothing else.
 *
 * Corrections to earlier drafts: $gp is 0x1AEFF0, so the two small-data words are D_001ABA60 and D_001ABA64,
 * not D_001ABA68/6C. The older "compiler-side answer" and "reload" theories are superseded by what follows.
 *
 * What retail has, measured: ONE test, between the two rounding chains, with chain 2 written out in both arms.
 * Both arms compile alike once `adjust` folds to 0, so jump2 cross-jumps them and deletes the test. sched2 has by
 * then seen separate blocks, which alone gives retail's serial chains, the prologue, the third draw and the sltu
 * pair. An inline helper taking the value as a parameter does not work: it sets 332.0f ahead of the test (588
 * bytes). This form was only tried with scale and half as locals; with a two-condition test placed here, 0.5f
 * was loaded again in chain 2 when half was not a local.
 * A test at the end of chain 2 is not needed; a test right after func_002CE5D0 breaks the sltu pair.
 *
 * The tested value must already be in a live register, because jump2 leaves whatever set it: not a load.
 *
 * Form 1 (below): `mode` is never assigned. One construct explains everything: a pseudo live from entry that
 * crosses every call takes $s1, and the test leaves no word. Probably what retail did, but it is an uninitialised
 * read with identical arms. The result does not depend on `mode`, whichever arm runs.
 *
 * Form 2 (also 28 of 28, no uninitialised read), same body except:
 *     f32 w;  mode = D_001B23A0;  (first statement)
 *     if (mode == 1) w = 1.0f;    (after func_002CE5D0; w is overwritten at once)
 *     w = scale * 330.0f + half; y0 = (s32)w;
 *     if (y0 != 0) ...            (instead of mode != 0)
 * mode crosses the calls into $s1, then flow2 deletes the empty test and the load. y0 is a real, live value but has
 * no meaning as a condition here, and mode exists only to occupy $s1 and shift the frame, so this is invented
 * dead code, not a recovered source.
 *
 * Why neither landed: WORKER2 forbids identical arms or a placeholder condition whose only effect is on scheduling
 * or allocation, and unused locals that only shift the frame. RAC1's docs/AGENT_WORKFLOW.md and QUEUE.md also
 * reject a read of a never-assigned local. Against that, RAC1's src/overlays/shared/effect_particle_updates.c:368
 * has this exact construct merged (`unsigned char *q; if (q) X; else X;`, "q is never set and both arms are the
 * same: jump2 folds the branch away"). A maintainer ruling on that method would decide this function. No form with
 * a real tested value and a real $s1 was found: compiled away, the tested register's set must also go, and only
 * flow2 can delete it, which is before jump2 removes the test.
 */
#include "common.h"

extern void func_0027C540(s32);
extern void func_0027C660(void);
extern s32 func_0027FAA8(s32, s32, u64, s32, s32);
extern s32 func_00288FB8(s32);
extern void func_0028FC48(s32, s32, s32, s32, u64, s32);
extern void func_002CE5D0(s32, s32);
extern void func_00300A00(s32, s32, f32, f32, f32, f32, f32);
extern s32 func_00336D50(s32, s32);
extern s32 D_001A8CFC;
extern s32 D_001A7340;
extern f32 D_001B23A8;
extern f32 D_001B23A4;
extern s32 D_001ABA64 __attribute__((sda));
extern s32 D_001ABA60 __attribute__((sda));
extern struct { s16 text; s16 reserved; } D_001ABD68[] MACRO_ADDR;

s32 func_002D2188(void) {
    f32 z;
    f32 scale, half;
    s32 r;
    s32 center;
    s32 mode;
    s32 adjust = 0;
    s32 y0, y1;

    func_0027C540(0);
    z = 0.0f;
    r = func_00336D50(D_001A8CFC + 0x8710, 0xDE);
    func_00300A00(r, 0x60442D00, (f32)(D_001A7340 / 2), (f32)D_001ABA64, 1.0f, D_001B23A8, z);
    r = func_00336D50(D_001A8CFC + 0x8710, 0xDF);
    func_00300A00(r, 0x60241700, (f32)(D_001A7340 / 2), (f32)D_001ABA64, 1.0f, D_001B23A8, z);
    r = func_00336D50(D_001A8CFC + 0x8710, 0xE0);
    func_00300A00(r, 0x55F0C070, (f32)(D_001A7340 / 2), (f32)D_001ABA64, 1.0f, D_001B23A8, z);
    func_002CE5D0(D_001ABA60 != 0, D_001ABA60 != 5);
    scale = D_001B23A4;
    half = 0.5f;
    y0 = (s32)(scale * 330.0f + half);
    if (mode != 0)
        y1 = (s32)(scale * 332.0f + half) + adjust;
    else
        y1 = (s32)(scale * 332.0f + half);
    func_0028FC48(66, y0, 463, y1, 0x55F0C070, 0);
    center = D_001A7340 / 2;
    func_0027FAA8(center, 311, (u64)0x80F0F0F0, func_00288FB8(D_001ABD68[D_001ABA60].text), -1);
    func_0027FAA8(0x1B0, 0x177, (u64)0x80F0F0F0, func_00288FB8(0x2D4F), -1);
    func_0027C660();
    return 0;
}
