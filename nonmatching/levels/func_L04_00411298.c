/*
 * Tier: opus-2 (Claude Opus 5.5 xhigh), task v2562, 5 November 2026. Retained near miss, USA v1.01.
 * NOT LANDABLE AS WRITTEN: the C below is a shape probe, kept because it is far closer than any earlier draft.
 * hud_y/hud_l/hud_yx hold invented tests and dead stores, and TXT is a statement expression. The admissible
 * opus-1 wording (3732/3732 bytes, about 154 rows off when aligned) is the previous git revision of this file.
 * Profile: -O2 -G8 -ffunction-sections, in a dedicated unit such as L04_0040F8F0-hud-g8.c (no cflags line needed).
 * Best check.py: 3736/3732 bytes, 0 of 2 places (04 Barlow, 11 Joba). Measured with relocations masked:
 * every row from the entry through 0x4119F4 equals retail (472 of 933); the first difference is 0x4119F8, in the
 * loop preheader; 890 of 933 retail rows align overall. PAL and v2.00 have no row. No linked build. Not a match.
 * About 40 compiles. D_L04_001B2D40 as the mode flag is by analogy with boot D_001B23A0/A4 (retail has no read).
 *
 * WHAT WAS LEARNED (retail's vanished per-rounding test; the same family as func_L04_00416E38, func_0033A520,
 * func_0033C028, func_002900B8 and the saved-but-unused registers). Each measured here:
 * 1. Sprite in $a2, lap in $s4 and every panel row need a test `D_mode == 1` in the block that ends after the fix.
 *    The load is block-local, so local-alloc gives it $a0 ($v0/$v1 hold x), and sprite cannot have $a0. cse replaces
 *    the constant 1 with the local known to be 1, so state->0x320 stays live in $s3 through the arm: this is why
 *    retail skips $s3 there. After the arm the constant is `li $s3,1`, which `D_L04_001B0A38 = 1` then reuses.
 *    The test dies at flow2: arm dead at flow1, jump tidied by flow.c, load and constant then dead in life_analysis.
 * 2. That test alone is not enough: with no block left, sched2 hoists 0.92, argument moves and strlen's a0.
 *    Retail also has a boundary at sched2, so a second test must live until jump2, and it must compare a live
 *    register or zero, because delete_computation leaves its inputs behind once sched2 runs.
 * 3. flow.c drops a jump into a single-predecessor join before sched2 if its arm is empty at flow2's CFG build, so
 *    the second test needs an arm that is live at flow1 and dead at flow2. Found: an arm insn read only by a THIRD
 *    test whose own arm is dead at flow1: `t = 0; if (r != 0) { t = 2; dead = 1; } if (t != 0) dead = 0;`.
 *    That third test is tidied at flow2, t dies in life_analysis, the arm empties after the CFG build, and the
 *    first jump survives sched2 and goes at jump2. It leaves no word. Two arm insns are required: jump1 if-converts
 *    a single set, and even `if (c) t = global` becomes load + movn.
 * 4. cse must stay transparent or 0.5/0.92/194 rematerialize: each jump needs a label with one use and a label-free
 *    arm (AROUND). `a && b`, `a || b` and if/else all flush (measured: 974 words). cse follows at most 9 branches
 *    per path (PATHLENGTH), which limits tests per site. Probes of an empty do { } while (0) after the test
 *    (retail order but wrong dependant counts, so ori $a1 moves) and -fno-schedule-insns2 (prologue wrong) are out.
 * 5. Text sites want x $s0, y $s1. A test on y gives y three refs and y wins, so the probe tests x there (hud_yx).
 *    Tie otherwise: a statement expression numbers its pseudo before x, which also swaps them. Use an inline.
 * 6. MACRO_ADDR on D_L04_001B2D44 and D_L04_001A8FE8 is right here: `lui $at` in flow, $gp in delay slots. row++ falls
 *    after the y rounding at the 220 site (`lw $v0,0x48($sp)` follows mfc1). Per-panel sprite locals are harmless.
 *
 * WHAT STILL DIFFERS (loop only, 43 rows):
 * - Retail hoists `li $s6,60` to the preheader and never reads $s6: the signature of `li $s3,1`, so a test comparing
 *   with 60 probably lives in the loop until jump2. At site 175 it took $s3 and shifted racer and y (inferred: cse
 *   merges it with the seconds block's 60); at site 220 loop.c did not hoist it. Not yet placed. Without it loop.c
 *   hoists 3600 into $fp, where retail has 60 in $s6, 10 in $s7 and 6 in $fp; 216000 stays in $s3 in both.
 * - state/row reloads differ ($v1/$a0/$a2 against ours): reload's round-robin, one rotation off, probably from the above.
 * - `lw $s4` / `lw $s2` swap at rs->0x1A4 and 0x1A0: probably the same cause.
 * Next: find the real source of the three tests. Candidates are a screen-y helper (compare with mode 1) and a
 * compare with 60; keep the ordering rules above when trying them. Do not retry placeholders: none of this is proof
 * of the original wording, only of the shape it compiles to.
 *
 * Corrections from opus-1 that still hold: 64-byte record rows with times at +8 indexed by lap - 1; time / 60 % 60 is
 * a block local reused by sprintf; alpha is one expression over (f32)bottom; the fix's arguments are direct.
 */
#include "common.h"

extern u8 *D_L04_001B0914 __attribute__((sda));
extern s32 D_L04_001B08F8 __attribute__((sda));
extern s32 D_L04_001B090C __attribute__((sda));
extern s32 D_L04_001B093C __attribute__((sda));
extern s32 D_L04_001B0A24 __attribute__((sda));
extern s32 D_L04_001B0A28 __attribute__((sda));
extern s32 D_L04_001B0A2C __attribute__((sda));
extern s32 D_L04_001B0A38 __attribute__((sda));
extern f32 D_L04_001B0A30 __attribute__((sda));
extern f32 D_L04_001B0A34 __attribute__((sda));
extern f32 D_L04_001B2D44 MACRO_ADDR;
extern s32 D_L04_001A8FE8 MACRO_ADDR;
extern s32 D_001A7340, D_001A7B90;
extern u8 D_001A7A0A;
extern s32 D_L04_001B2D40 __attribute__((sda));
static __inline__ s32 hud_y(f32 v, f32 *dead, s32 one) {
    s32 r = (s32)(v + 0.5f);
    if (D_L04_001B2D40 == one) *dead = 0.0f;
    {
        s32 t = 0;
        if (r != 0) { t = 2; *dead = 1.0f; }
        if (t != 0) *dead = 0.0f;
    }
    return r;
}
static __inline__ s32 hud_l(f32 v, f32 *dead) {
    s32 r = (s32)(v + 0.5f);
    {
        s32 t = 0;
        if (r != 0) { t = 2; *dead = 1.0f; }
        if (t != 0) *dead = 0.0f;
    }
    return r;
}
static __inline__ s32 hud_yx(f32 v, f32 *dead, s32 one, s32 k) {
    s32 r = (s32)(v + 0.5f);
    if (D_L04_001B2D40 == one) *dead = 0.0f;
    {
        s32 t = 0;
        if (k != 0) { t = 2; *dead = 1.0f; }
        if (t != 0) *dead = 0.0f;
    }
    return r;
}
#define HUD_Y(v) hud_y(v, &angle, flag)
#define HUD_L(v) hud_l(v, &angle)
#define TXT(v, c, s_, n_) ({ s32 x_ = D_001A7340 / 2; func_0027FAA8(x_, hud_yx(v, &angle, flag, x_), c, s_, n_); })
extern u8 *D_L04_001A904C;
typedef struct { u8 pad[0xB0]; f32 tangent; } HudCamera;
extern HudCamera D_L04_001BDB80;
typedef struct { s32 pad[2]; s32 times[14]; } RecordRow;
extern RecordRow D_L04_002C63C0[] NOT_SDA;
extern s32 D_L04_002CA1E0[] NOT_SDA;
extern char D_L04_001B0A40[] NOT_SDA, D_L04_001B0A50[] NOT_SDA;
extern char D_L04_001B0A58[] NOT_SDA, D_L04_001B0A70[] NOT_SDA;
extern char D_L04_001B0A88[] NOT_SDA, D_L04_001B0A98[] NOT_SDA;
extern char D_L04_001B0AA8[] NOT_SDA, D_L04_001B0AC0[] NOT_SDA;
extern f32 func_00283180(f32), func_00283198(f32);
extern void func_0027AB40(void);
extern s32 func_L04_0040A1D0(s32);
extern void *func_00336D50(void *, s32);
extern void func_00300A00(void *, s32, f32, f32, f32, f32, f32);
extern char *func_00288FB8(s32);
extern s32 func_001157AC(char *);
extern s32 func_00115DA8(char *, char *, ...);
extern s32 func_0027FAA8(s32, s32, u64, char *, s32);
extern void func_0027F5B8(s32, s32, u64, char *, s32);
extern s32 func_0027FB40(s32, s32, u64, char *, s32);
extern s32 func_00283D38(s32, s32, f32);
extern s32 func_002E5CF8(s32, s32, void *);
extern void func_002FC560(u32, u64);

s32 func_L04_00411298(void) {
    char text[64];
    u8 *object = D_L04_001B0914;
    u8 *state = *(u8 **)(object + 0x68);
    s32 row = 0, bottom, ahead, playerSeen = 0;
    s32 rank, time, y, lap, flag;
    u8 *racer;
    f32 alpha, fy, angle, fade;
    char *str, *str2, *str3;
    f32 fbottom;
    s32 color1, color2;
    s32 color;
    if (D_L04_001B08F8 < 10) D_L04_001B0A38 = 0;
    if (*(s32 *)(state + 0x320) >= 10) {
        if (D_L04_001B08F8 < 601) {
            D_L04_001B0A30 = 0;
            D_L04_001B0A34 = 0;
            D_L04_001B0A28 = 0;
            D_L04_001B0A2C = 0;
            goto tick;
        }
        goto animate;
    }
    if (D_L04_001B0A38) {
animate:
        if (!D_L04_001B0A28) D_L04_001B0A30 += 0.41666669f;
        if (!D_L04_001B0A2C) D_L04_001B0A34 += 0.41666669f;
    } else {
        D_L04_001B0A30 = 0;
        D_L04_001B0A34 = 0;
        D_L04_001B0A28 = 0;
        D_L04_001B0A2C = 0;
    }
tick:
    if (D_L04_001B08F8 % 15 == 0) {
        D_L04_001B0A24++;
        if (D_L04_001B0A24 >= 4) D_L04_001B0A24 = 0;
    }
    angle = 0.56217057f;
    D_L04_001BDB80.tangent = func_00283198(angle) / func_00283180(angle);
    func_0027AB40();
    flag = *(s32 *)(state + 0x320);
    if (flag == 1) {
        lap = func_L04_0040A1D0(D_L04_001B090C);
        if (lap > 2) lap = 2;
        {
            void *panel = func_00336D50(D_L04_001A904C + 0x8710, 0x39);
            func_00300A00(panel, 0x60442D00, (f32)(D_001A7340 / 2), (f32)HUD_Y(D_L04_001B2D44 * 194.0f), 0.92f, D_001A7B90 ? 0.9f : 0.92f, 0.0f);
        }
        {
            void *panel = func_00336D50(D_L04_001A904C + 0x8710, 0x38);
            func_00300A00(panel, 0x55F0C070, (f32)(D_001A7340 / 2), (f32)HUD_Y(D_L04_001B2D44 * 194.0f), 0.92f, D_001A7B90 ? 0.9f : 0.92f, 0.0f);
        }
        str = func_00288FB8(0x2F93);
        TXT(D_L04_001B2D44 * 32.0f, 0x80F0F0F0ULL, str, func_001157AC(str));
        if (D_L04_001B093C > 0) {
            TXT(D_L04_001B2D44 * 65.0f, 0x80F0F0F0ULL, func_00288FB8(0x1233), -1);
            D_L04_001B093C--;
        } else if (D_L04_002C63C0[D_L04_001B090C].times[lap - 1]) {
            s32 recordTime;
            char *recordFormat = func_00288FB8(0x2FC6);
            recordTime = D_L04_002C63C0[D_L04_001B090C].times[lap - 1];
            if (D_001A7A0A) recordTime *= 10;
            func_00115DA8(text, recordFormat, recordTime);
            TXT(D_L04_001B2D44 * 65.0f, 0x80F0F0F0ULL, text, func_001157AC(text));
        }
    }
    bottom = 315 - (s32)D_L04_001B0A34;
    if (bottom <= 156) bottom = 157;
    if (bottom == 157) D_L04_001B0A2C = 1;
    flag = 1;
    fbottom = (f32)bottom;
    alpha = (1.0f - (315.0f - fbottom) * 0.006329114f) * 0.72f + 0.08f;
    {
        void *panel = func_00336D50(D_L04_001A904C + 0x8710, 0x47);
        func_00300A00(panel, 0x60442D00, (f32)(D_001A7340 / 2), (f32)HUD_Y(D_L04_001B2D44 * 196.0f), 0.92f, D_001A7B90 ? 0.9f : 0.92f, alpha);
    }
    {
        void *panel = func_00336D50(D_L04_001A904C + 0x8710, 0x46);
        func_00300A00(panel, 0x55F0C070, (f32)(D_001A7340 / 2), (f32)HUD_Y(D_L04_001B2D44 * 196.0f), 0.92f, D_001A7B90 ? 0.9f : 0.92f, alpha);
    }
    func_00115DA8(text, D_L04_001B0A40, func_00288FB8(0x2F7F),
        func_00288FB8(D_L04_002CA1E0[*(s32 *)(state + 0x320) - 1]), func_00288FB8(0x2F80));
    TXT(D_L04_001B2D44 * 106.0f, 0x80F0F0F0ULL, text, func_001157AC(text));
    func_0027F5B8(339, HUD_Y(fbottom * D_L04_001B2D44), 0x80F0F0F0ULL, func_00288FB8(0x2F42), -1);
    y = 138;
    ahead = 0;
    racer = *(u8 **)(state + 0x39C);
    D_L04_001B0A38 = flag;
    while (racer) {
        s32 scroll = (s32)D_L04_001B0A30 - 138;
        fade = 0.1f;
        rank = 0;
        time = 0;
        y = row * 15 - scroll;
        if (*(s16 *)(racer + 0xAA) == 0xBBC) {
            if (y <= 138) { y = 138; D_L04_001B0A28 = 1; }
            rank = *(s32 *)(state + 0x320);
            time = D_L04_001A8FE8;
            playerSeen = 1;
            racer = *(u8 **)(state + 0x3A0);
        } else if (*(s16 *)(racer + 0xAA) == 0xB2D) {
            u8 *rs = *(u8 **)(racer + 0x68);
            rank = *(u8 *)(rs + 0xA2);
            racer = *(u8 **)(rs + 0x1A4);
            time = *(s32 *)(rs + 0x1A0);
            if (rank > 10) goto next;
            if (*(s32 *)(state + 0x320) < rank) {
                s32 projected = D_L04_001A8FE8 + D_L04_001B08F8;
                if (projected < time) { D_L04_001B0A38 = 0; ahead++; }
                if (time == projected) func_002E5CF8(22, 17, object);
            }
        }
        if (y < 128) { if (rank <= 10) row++; goto next; }
        if (y > bottom - 10) goto next;
        if (rank == *(s32 *)(state + 0x320)) { color2 = 0x8000C0C0; color1 = 0x8000C0C0; }
        else { color2 = 0x80F0F0F0; color1 = 0xF0F0F0; }
        if (y < 138) {
            color = func_00283D38(color1, color2, ((f32)y - 128.0f) * fade);
        }
        else if (*(s32 *)(state + 0x320) < rank && bottom - y <= 20)
        {
            color = func_00283D38(color1, color2, (f32)(bottom - y - 10) * fade);
        }
        else { color = color2; }
        fy = (f32)y;
        func_00115DA8(text, D_L04_001B0A50, func_00288FB8(D_L04_002CA1E0[rank - 1]), func_00288FB8(0x2F81));
        func_0027FB40(175, HUD_L(fy * D_L04_001B2D44), (s64)color, text, -1);
        if (ahead > 0) {
            if (D_L04_001B0A24 == 3 || ahead >= 2) func_00115DA8(text, D_L04_001B0A58, func_00288FB8(0x2FB0));
            else if (D_L04_001B0A24 == 2) func_00115DA8(text, D_L04_001B0A70, func_00288FB8(0x2FB0));
            else if (D_L04_001B0A24 == 1) func_00115DA8(text, D_L04_001B0A88, func_00288FB8(0x2FB0));
            else func_00115DA8(text, D_L04_001B0A98, func_00288FB8(0x2FB0));
        } else {
            s32 seconds = time / 60 % 60;
            if (seconds < 10)
                func_00115DA8(text, D_L04_001B0AA8, func_00288FB8(0x2F83), time / 216000, time / 3600 % 60, seconds, time / 6 % 10);
            else
                func_00115DA8(text, D_L04_001B0AC0, func_00288FB8(0x2F83), time / 216000, time / 3600 % 60, seconds, time / 6 % 10);
        }
        {
            s32 rowY = HUD_L(fy * D_L04_001B2D44);
            row++;
            func_0027F5B8(220, rowY, (s64)color, text, -1);
        }
next:
        if (row >= 10) break;
    }
    if (*(s32 *)(state + 0x320) > 10 && !D_L04_001B0A28 && !playerSeen) {
        func_00115DA8(text, D_L04_001B0A50, func_00288FB8(D_L04_002CA1E0[*(s32 *)(state + 0x320) - 1]), func_00288FB8(0x2F81));
        y += 15;
        func_0027FB40(175, HUD_L((f32)y * D_L04_001B2D44), 0x8000C0C0ULL, text, -1);
        if (D_L04_001A8FE8 / 60 % 60 < 10)
            func_00115DA8(text, D_L04_001B0AA8, func_00288FB8(0x2F83), D_L04_001A8FE8 / 216000, D_L04_001A8FE8 / 3600 % 60, D_L04_001A8FE8 / 60 % 60, D_L04_001A8FE8 / 6 % 10);
        else
            func_00115DA8(text, D_L04_001B0AC0, func_00288FB8(0x2F83), D_L04_001A8FE8 / 216000, D_L04_001A8FE8 / 3600 % 60, D_L04_001A8FE8 / 60 % 60, D_L04_001A8FE8 / 6 % 10);
        func_0027F5B8(220, HUD_L((f32)y * D_L04_001B2D44), 0x8000C0C0ULL, text, -1);
    }
    if (*(u8 *)(object + 0x20) != 3) {
        angle = *(f32 *)(state + 0x28C) * 0.008726647f;
        D_L04_001BDB80.tangent = func_00283198(angle) / func_00283180(angle);
        func_0027AB40();
    }
    func_002FC560(0x47, 0x5360B);
    return 1;
}
