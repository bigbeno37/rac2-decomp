/* Claude Opus 5.5 (xhigh), tier opus-2, task v2557, 5 November 2026. FAILED; the body below is the v2124
 * draft unchanged, still the closest allowed form: 5856/5968 bytes, 0 of 2 places, frame 0x1C0, difflib
 * 1185/1492 opcodes, 590 identical words. It replaces the 3 November review draft, which measures
 * 5768 bytes, 1110 opcodes, 560 words here. That review's contract notes still hold: the sprite lookup
 * returns a 32-bit address, the x/y helper outputs are int pointers, m[4] and v90[4] are complete 64-byte
 * outputs. Earlier texts are in git history. Build as its own *-g8.c unit, no cflags line.
 *
 * Measurements (bytes, opcodes of 1492, identical words). Forbidden forms are diagnostics only:
 *   macro ((int)((v) * S + 0.5f))                              5852  1177  597
 *   -fno-schedule-insns on this body                           5856  1136  488
 *   `int one = 1;` across the marker loop: cse folds it back   same bytes as the body
 *   do-while(0) in the helper (forbidden)                      5916  1270  661
 *     combine still moves the fix past the loop notes into the argument move, so retail's early
 *     `mfc1 a0` is not reproduced: notes alone are not retail's split.
 *   `if (r != 0) return r; return r;` in the helper            5872  1191  588
 *     jump1 cross-jumps it, so no split: each arm must compute its own pseudos.
 *   `if (left != 0) top = SC; else top = SC;` at the first two draws, as upstream's func_0033C028
 *     (forbidden)                                              5904  1204  598
 *     first draw takes retail's exact order, but the join ends cse, so the second draw reloads
 *     112.0, 142.0 and 0.5 instead of keeping them in $f22/$f21/$f20.
 *   dead-store probe in the helper (forbidden, invented code)  5980  1216  700
 *       float y = v * S + 0.5f; int r = (int)y; if (D_L04_001B2D40 == 1) y = 1.0f; return r;
 *     gives retail's constant roles (112.0 $f22, 0.5 $f20, 142.0 $f21) and 7 saved FPRs. The split lasts
 *     through local-alloc and is gone by sched2, so argument sets float into the chains' stalls, and
 *     the owner stays in $s7.
 *   the probe with a named label (`goto done; ... done:`), in the helper or written in the function
 *     body: no sched2 split (5980 and 5864). Ruled out: a user label does not keep the block in this cc1.
 *
 * What retail shows:
 *  - A block boundary follows the rounding chain, seen in pairs and before later argument setup. It is
 *    real at combine (retail keeps chain 1 in `mfc1 a0` ahead of chain 2), at local-alloc and at sched2,
 *    yet cse sees through it, and it leaves no word. Same family as func_L11_00444400 and
 *    func_L02_003FE8B8. Without sched1 the chains still interleave, so sched1 is not the explanation.
 *  - The marker tail evaluates x - 2, SC(y - 1), x + 2, SC(y + 3) left to right, with the boundary after
 *    the first SC. That suggests named locals or an inline draw wrapper, not reversed call arguments.
 *  - Operands are S * k for constants and v * S for variables, which one (v) * S form gives after fold.
 *  - Constant 1 sits in $s2 at entry and is hoisted to $s7 for the marker loop though only one use
 *    survives. loop.c needs a long lifetime for that, so a second use of 1 existed lower in the loop at
 *    loop time. That fits the note that constant-1 registers mark vanished tests.
 *  - Inference, not measured (the one test folded back): that $s7 fills all nine saved GPRs, which spills
 *    the owner; the view matrix sp+0x50 is rebuilt per use, the shape of a spilled PRE pseudo.
 * Unblocks with: the original construct behind that boundary. Nothing allowed reproduces it yet.
 */
/* func_L04_00417308 (5,968 B; level 04 at 0x00417308 and level 11 at 0x00406E08): HUD draw.
 * Current retained result, 3 November 2026 (v2124, tier opus-1): 5856/5968 bytes, 0 of 2 places,
 * frame 0x1C0. Opcode difflib 1176 of 1492 equal (registers ignored), registers equal 580.
 * Earlier: c02 draft 5768 B, orch/main sol-high draft 5852 B / 1124 opcodes when rebuilt here.
 * Profile: -O2 -G8 -ffunction-sections, as its own unit named *-g8.c (e.g.
 * src/levels/shared/L04_00417308-hud-g8.c, with no cflags.txt line), and the INCLUDE_ASM in
 * src/levels/shared/L04_00417308.c replaced by a comment. The declarations it needs are below
 * (sda externs for the gp-relative loads, plain externs for the %hi/%lo ones).
 * Gains this round: the HUD rounding as a static inline hud_scale() (1124 -> 1157 opcodes);
 * the targets/reticle/position pointers set just before the tick loop (retail spills them in
 * that preheader, slots 0x114/0x118/0x11C/0x124) (-> 1173); a separate marker-loop counter
 * (retail: tick i = s2, marker slot = fp) and no `index` variable in the marker loop (retail
 * keeps targets[slot] in v0) (-> 1176 opcodes / 580 registers).
 * Still differs:
 *  - owner pointer (D_L04_001B0914) stays in $s7; retail spills it to sp+0x10C at entry. In the
 *    retail marker loop all nine saved GPRs are taken (s0 asset, s1 target_state, s2 target,
 *    s3 icon_matrix sp+0x90, s4 state, s5 marker colour, s6 -1, s7 constant 1, fp slot); ours
 *    does not hoist the constant 1 (used once, below the loop.c threshold) and the view matrix
 *    sp+0x50 gets a gcse PRE reaching reg (fp) across the tick and marker loops, where retail
 *    recomputes `addiu a2,sp,0x50` (its PRE reg must have lost allocation).
 *  - pairs of scale roundings interleave under sched1; retail runs them one after another.
 *  - state is $s3 here, $s4 in retail; spill slots about 3 words off; 8 saved FPRs vs retail 7.
 *  - a separate marker-loop colour variable made it worse (1160 opcodes).
 * Uses no do{}while(0); not a functional-port qualification.
 */
#include "include_asm.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern char *D_L04_001B0914 __attribute__((sda));
extern int D_L04_001B091C __attribute__((sda));
extern float D_L04_001B2D44;
extern int D_001A7340;
extern char D_001A63A8[];
extern int D_L04_001A8FE8 __attribute__((sda));
extern void func_L04_00416E38(void);
extern void func_L04_00417028(char *);
extern void func_00282948(int *);
extern void func_002E5CF8(int, int, char *);
extern float func_00282C48(float);
extern void func_0027EB88(int, int, int, int, int, unsigned int);
extern void func_0029B658(int, unsigned int, float, float, float, float);
extern char *func_00288FB8(int);
extern int func_001157AC(char *);
extern int func_0027FBD8(int, int, unsigned long, char *, int);
extern int func_002B6D28(void);
extern void func_L04_0040C880(char *);


typedef struct { int unused; float x; } HudTick;
typedef struct { int value[4]; } HudColors;
typedef struct { int value[5]; } HudCapacity;
extern HudColors D_L04_001B0B50;
extern HudColors D_L04_001B0B60;
extern HudCapacity D_L04_001B0B78;
extern char *D_L04_001A904C __attribute__((section(".sdata")));
extern char D_L04_001BDB80[];
extern int D_L04_001B0B70 __attribute__((sda));
extern int D_L04_001B0B74 __attribute__((sda));
extern int D_001A79F0;
extern int D_L04_001B090C __attribute__((sda));
extern char D_L04_002C63C0[];
extern int D_L04_002CA1E0[];
extern char D_L04_001B0B90[];
extern int D_L04_001A8FE4;
extern float D_L04_001B31A0[4] __attribute__((section(".sdata")));
extern float D_L04_001B3190[4] __attribute__((section(".sdata")));
extern float D_L04_001B31D0;
extern HudTick D_L04_002605D0[];
extern char *D_L04_001B24DC __attribute__((sda));
extern float func_00283198(float);
extern float func_00283180(float);
extern void func_0027AB40(void);
extern char *func_00336D50(char *, int);
extern void func_00300A00(char *, int, float, float, float, float, float);
extern int func_0028E588(int, int);
extern void func_0028EA58(int, int, int, int, int, int *);
extern void func_0027EAE8(int, int, int, int, int, int *);
extern int func_00283D38(int, int, float);
extern void func_0027F5B8(int, int, unsigned long, char *, int);
extern int func_00115DA8(char *, char *, ...);
extern int func_0027FB40(int, int, unsigned long, char *, int);
extern void func_00283388(float *);
extern void func_00283698(float *, float *);
extern void func_0028FC48(int, int, int, int, unsigned long, int);
extern void func_00282D30(void *, void *, float);
extern void func_002830C0(void *, void *, void *);
extern float func_00282DB0(void *, void *);
extern void func_00282CD8(void *, void *, void *);
extern float func_002AA2E8(float *, float, float);
extern void func_00282CF0(void *, void *, void *);
extern float func_00283248(float, float);
extern float func_00283BE0(float, float);
extern float func_002AAA00(void *, float, void *, int, float, float, float);
extern float func_002AA548(void *, void *, float, float, float, float);
extern void func_00283410(void *, void *);
extern void func_00300AA8(char *, float, float, int, float, float *);
extern void func_L04_0043AD48(int *, int *, int, int, void *, void *, void *);
extern void func_002FC560(int, unsigned long);

/* A HUD coordinate scaled for the display mode, rounded to whole pixels. */
static __inline__ int hud_scale(float value)
{
    return (int)(D_L04_001B2D44 * value + 0.5f);
}

static __inline__ int hud_scale_v(float value)
{
    return (int)(value * D_L04_001B2D44 + 0.5f);
}

int func_L04_00417308(void)
{
    HudColors weapon_colors = D_L04_001B0B50;
    HudColors label_colors = D_L04_001B0B60;
    HudCapacity weapon_capacity = D_L04_001B0B78;
    char number[16];
    float view[16] __attribute__((aligned(16)));
    float icon_matrix[16] __attribute__((aligned(16)));
    float direction[4] __attribute__((aligned(16)));
    float projected[4] __attribute__((aligned(16)));
    union { int colors[4]; float vector[4]; } quad;
    float temp, amount, angle, sine;
    char *asset, *target, *target_state;
    int i, next, texture, index, color, label_x, reticle_y;
    int slot;
    char *object = D_L04_001B0914;
    char *state = *(char **)(object + 0x68);
    char *text;
    int x, y;
    if (*(u8 *)(object + 0xBC) == 1)
        func_L04_00416E38();
    if (D_L04_001B091C > 0) {
        func_L04_00417028(object);
        func_00282948(&D_L04_001B091C);
        if (D_L04_001B091C == 173)
            func_002E5CF8(13, 0, object);
    } else if (func_00282C48(*(float *)(state + 0x4A4)) > 1.57079637f) {
        int left, top;
        left = hud_scale(112.0f);
        top = hud_scale(142.0f);
        func_0027EB88(left, top, 175, D_001A7340 - 175, 96, 0x60442D00);
        left = hud_scale(112.0f);
        top = hud_scale(142.0f);
        func_0029B658(96, 0x55F0C070, (float)left, (float)top,
                     175.0f, (float)(D_001A7340 - 175));
        text = func_00288FB8(0x2F95);
        x = D_001A7340 / 2;
        y = hud_scale(118.0f);
        func_0027FBD8(x, y, 0x700000E4, text, func_001157AC(text));
        if (*(int *)(state + 0x76C) == 0) {
            *(int *)(state + 0x76C) = 1;
            if ((*(int *)(D_001A63A8 + 0x68) != 0 ||
                 *(int *)(D_001A63A8 + 0x24) != -1) &&
                (unsigned int)(*(u16 *)(D_001A63A8 + 0x72) - 6) >= 2)
                *(u16 *)(D_001A63A8 + 0x72) = 5;
        }
        if (*(int *)(state + 0x76C) != 0 &&
            *(int *)(D_001A63A8 + 0x68) == 0 &&
            *(int *)(D_001A63A8 + 0x24) == -1) {
            *(int *)(D_001A63A8 + 0x28) = (int)object;
            *(int *)(D_001A63A8 + 0x24) = *(int *)(state + 0x760) + 3000;
        }
        func_002B6D28();
    }
    if (func_00282C48(*(float *)(state + 0x4A4)) < 1.57079637f &&
        *(int *)(state + 0x76C) != 0) {
        *(int *)(state + 0x76C) = 0;
        if ((unsigned int)(*(u16 *)(D_001A63A8 + 0x72) - 6) >= 2)
            *(u16 *)(D_001A63A8 + 0x72) = 5;
    }
    if ((unsigned int)(*(u8 *)(object + 0xBC) - 1) < 4 || D_L04_001A8FE8 < 60)
        func_L04_0040C880(object);
    if (D_L04_001A904C != 0) {
        int *targets;
        void *reticle_x_rate, *reticle_y_rate, *position;
        sine = func_00283198(0.5621705651283264f);
        angle = sine / func_00283180(0.5621705651283264f);
        *(float *)(D_L04_001BDB80 + 0xB0) = angle;
        func_0027AB40();
        asset = func_00336D50(D_L04_001A904C + 0x8710, 0x2D);
        func_00300A00(asset, 0x60442D00, 10.0f,
                     (float)hud_scale(10.0f), 1.0f, 1.0f, 0.0f);
        asset = func_00336D50(D_L04_001A904C + 0x8710, 0x2E);
        func_00300A00(asset, 0x55F0C070, 10.0f,
                     (float)hud_scale(10.0f), 1.0f, 1.0f, 0.0f);
        if (*(int *)(state + 0x360) != 0) {
            quad.colors[3] = quad.colors[0] = quad.colors[1] = quad.colors[2] =
                weapon_colors.value[*(int *)(state + 0x360) - 1];
            index = 0;
            switch (*(int *)(state + 0x360)) {
            case 2: index = 1; break;
            case 3: index = 2; break;
            }
            texture = func_0028E588(0x7568, index);
            func_0028EA58(texture, 62,
                         hud_scale(17.0f), 40,
                         hud_scale(35.0f), quad.colors);
            amount = (float)*(int *)(state + 0x364) /
                     (float)weapon_capacity.value[*(int *)(state + 0x360)];
            if (amount > 1.0f) amount = 1.0f;
            if (amount < 0.0f) amount = 0.0f;
            func_0027EAE8(111, hud_scale(28.0f),
                         (int)(amount * 91.0f) + 111,
                         hud_scale(41.0f), 0xFFFFF0, quad.colors);
        }
        if (*(int *)(state + 0x33C) > 0) {
            if (D_L04_001B0B70 != *(int *)(state + 0x33C)) {
                if (D_L04_001B0B70 < *(int *)(state + 0x33C))
                    D_L04_001B0B74 = 45;
                else D_L04_001B0B74 = 0;
                D_L04_001B0B70 = *(int *)(state + 0x33C);
            }
            if (D_L04_001B0B74 > 0) D_L04_001B0B74--;
            color = func_00283D38(0x603F00B2, 0x800000FF,
                                 (float)D_L04_001B0B74 * 0.02222222276031971f);
            asset = func_00336D50(D_L04_001A904C + 0x8710, 0x35);
            func_00300A00(asset, color, 11.0f,
                         (float)hud_scale(10.0f),
                         1.0f, 1.0f, (float)D_L04_001B0B70);
        } else D_L04_001B0B70 = 0;
        if (*(int *)(state + 0x278) != 0)
            amount = (float)*(int *)(state + 0x278) / (float)*(int *)(state + 0x650);
        else amount = (float)*(int *)(state + 0x338) * 0.004166666883975267f;
        if (amount > 1.0f) amount = 1.0f;
        if (amount < 0.0f) amount = 0.0f;
        quad.colors[3] = quad.colors[0] = quad.colors[1] = quad.colors[2] = 0x603F00B2;
        func_0027EAE8(26, hud_scale_v((float)(98 - (int)(amount * 74.0f))),
                     38, hud_scale(98.0f), 0xFFFFF0, quad.colors);
        asset = func_00336D50(D_L04_001A904C + 0x8710, 0x3F);
        func_00300A00(asset, 0x60442D00, 498.0f,
                     (float)hud_scale(10.0f), 1.0f, 1.0f, 0.0f);
        asset = func_00336D50(D_L04_001A904C + 0x8710, 0x40);
        func_00300A00(asset, 0x55F0C070, 498.0f,
                     (float)hud_scale(10.0f), 1.0f, 1.0f, 0.0f);
        asset = func_00336D50(D_L04_001A904C + 0x8710, 0x41);
        func_00300A00(asset, 0x55F0C070, 498.0f,
                     (float)hud_scale(10.0f), 1.0f, 1.0f, 0.0f);
        if (D_001A79F0 == 4) {
            asset = func_00336D50(D_L04_001A904C + 0x8710, 0x42);
            func_00300A00(asset, 0x55F0C070, 498.0f,
                         (float)hud_scale(10.0f), 1.0f, 1.0f, 0.0f);
        } else {
            asset = func_00336D50(D_L04_001A904C + 0x8710, 0x30);
            func_00300A00(asset, 0x55F0C070, 498.0f,
                         (float)hud_scale(10.0f), 1.0f, 1.0f, 0.0f);
        }
        if (D_001A79F0 == 4 &&
            (*(int *)(D_L04_002C63C0 + (D_L04_001B090C << 6) + 4) & 4)) {
            asset = func_00336D50(D_L04_001A904C + 0x8710, 0x43);
            func_00300A00(asset, 0x60FFDE8D, 498.0f,
                         (float)hud_scale(10.0f), 1.0f, 1.0f, 0.0f);
        }
        index = *(int *)(state + 0x320);
        if ((unsigned int)(index - 1) >= 20) index = 20;
        label_x = 291;
        if (index >= 10) label_x = 285;
        text = func_00288FB8(D_L04_002CA1E0[index - 1]);
        {
            int text_y = hud_scale(24.0f);
            i = 3;
            if (index < 4) i = index - 1;
            func_0027F5B8(label_x, text_y, label_colors.value[i], text, -1);
        }
        func_00115DA8(number, D_L04_001B0B90, D_L04_001A8FE4 + 1, 3);
        {
            int text_y = hud_scale(71.0f);
            func_0027FB40(469, text_y, 0x60F0F0F0, number, func_001157AC(number));
        }
        func_00283388(view);
        func_00283698(view, D_L04_001B31A0);
        {
            int top, bottom;
            top = hud_scale(36.0f);
            bottom = hud_scale(37.0f);
            func_0028FC48(343, top, 427, bottom, 0x25F0C070UL, 0);
        }
        {
            int top, bottom;
            top = hud_scale(28.0f);
            bottom = hud_scale(29.0f);
            func_0028FC48(337, top, 427, bottom, 0x25F0C070UL, 0);
        }
        {
            int top, bottom;
            top = hud_scale(45.0f);
            bottom = hud_scale(46.0f);
            func_0028FC48(352, top, 427, bottom, 0x25F0C070UL, 0);
        }
        func_00282D30(direction, object + 0xC0,
                     *(float *)(state + 0x30C) + *(float *)(state + 0x27C));
        func_002830C0(projected, direction, view);
        angle = func_00282DB0(projected, D_L04_001B31A0);
        func_00282D30(direction, D_L04_001B31A0, angle);
        func_00282CD8(quad.vector, projected, direction);
        func_002AA2E8(&D_L04_001B31D0, projected[0] * -1.25f, 0.05000000447034836f);
        position = object + 0x10;
        targets = (int *)(state + 0x1E4);
        reticle_x_rate = state + 0x494;
        reticle_y_rate = state + 0x498;
        i = 0;
        do {
            x = (int)(D_L04_002605D0[i].x + D_L04_001B31D0);
            if (x < 356 && D_L04_001B31D0 < 0.0f) {
                if (i == 0) {
                    D_L04_002605D0[0].x = D_L04_002605D0[8].x + 8.0f;
                    next = 1;
                } else {
                    next = i + 1;
                    D_L04_002605D0[i].x = D_L04_002605D0[i-1].x + 8.0f;
                }
            } else {
                next = i + 1;
                if (x >= 420 && D_L04_001B31D0 > 0.0f) {
                    if (i == 8) D_L04_002605D0[8].x = D_L04_002605D0[0].x - 8.0f;
                    else D_L04_002605D0[i].x = D_L04_002605D0[next].x - 8.0f;
                }
            }
            D_L04_002605D0[i].x += D_L04_001B31D0;
            x = (int)D_L04_002605D0[i].x;
            if (x < 371) amount = ((float)x - 355.0f) * 0.06666667014360428f;
            else if (x >= 405) amount = (420.0f - (float)x) * 0.06666667014360428f;
            else amount = 1.0f;
            if (amount > 1.0f) amount = 1.0f;
            if (amount < 0.0f) amount = 0.0f;
            color = func_00283D38(0x10F0C070, 0x25F0C070, amount);
            func_0028FC48(x, hud_scale(21.0f), x + 1,
                         hud_scale(55.0f), (unsigned long)color, 0);
            i = next;
        } while (i < 9);
        func_00282CF0(projected, position, D_L04_001B3190);
        func_002830C0(direction, projected, view);
        amount = direction[1] * 0.2f;
        if (amount > 1.0f) amount = 1.0f;
        if (amount < -1.0f) amount = -1.0f;
        reticle_y = (int)(amount * 13.0f) + 36;
        asset = func_00336D50(D_L04_001A904C + 0x8710, 0x3E);
        angle = func_00283248(D_L04_001B31A0[0], D_L04_001B31A0[1]);
        for (slot = 0; slot < 21; slot++) {
            if (targets[slot] == -1) continue;
            target = D_L04_001B24DC + (targets[slot] << 8);
            target_state = *(char **)(target + 0x68);
            if (*(u8 *)(target + 0x20) != 2 &&
                *(u8 *)(object + 0xBC) != 1 && *(u8 *)(object + 0xBC) != 3) continue;
            func_00282CF0(projected, target + 0x10, D_L04_001B3190);
            func_002830C0(direction, projected, view);
            x = (int)(direction[0] + direction[0]) + 387;
            y = (int)(direction[1] * 4.0f) + 36;
            if ((unsigned int)(y - 23) < 27 && x < 421 && x >= 360) {
                if (x < 371) amount = ((float)x - 360.0f) * 0.1f;
                else if (x >= 410) amount = (420.0f - (float)x) * 0.1f;
                else amount = 1.0f;
                if (amount == 1.0f) {
                    if (y < 29) amount = ((float)y - 23.0f) * 0.2f;
                    else if (y >= 44) amount = (49.0f - (float)y) * 0.2f;
                }
                color = func_00283D38(0x100040E4, 0x600040E4, amount);
                temp = func_00283BE0(angle, *(float *)(target + 0xF8));
                if (*(float *)(target_state + 0x1D4) == -1.0f)
                    *(float *)(target_state + 0x1D4) = temp;
                else func_002AAA00(target_state + 0x1D4, temp, target_state + 0x1E0, 0,
                                   0.002327105961740017f, 0.002327105961740017f, 4.71238899230957f);
                if (*(int *)(target_state + 0x1CC) == -1)
                    *(int *)(target_state + 0x1CC) = x;
                else {
                    temp = (float)*(int *)(target_state + 0x1CC);
                    func_002AA548(&temp, target_state + 0x1D8, (float)x,
                                 0.1388888955116272f, 0.1388888955116272f, 5.833333492279053f);
                    *(int *)(target_state + 0x1CC) = (int)temp;
                }
                if (*(int *)(target_state + 0x1D0) == -1)
                    *(int *)(target_state + 0x1D0) = y;
                else {
                    temp = (float)*(int *)(target_state + 0x1D0);
                    func_002AA548(&temp, target_state + 0x1DC, (float)y,
                                 0.1388888955116272f, 0.1388888955116272f, 5.833333492279053f);
                    *(int *)(target_state + 0x1D0) = (int)temp;
                }
                direction[0] = 0.0f;
                direction[1] = *(float *)(target_state + 0x1D4);
                direction[2] = 0.0f;
                func_00283410(icon_matrix, direction);
                func_00282D30(icon_matrix, icon_matrix, *(float *)(asset + 12));
                func_00282D30(icon_matrix + 8, icon_matrix + 8, *(float *)(asset + 12));
                func_00300AA8(asset, (float)*(int *)(target_state + 0x1CC),
                             (float)hud_scale_v((float)*(int *)(target_state + 0x1D0)),
                             color, 0.0f, icon_matrix);
            } else {
                *(int *)(target_state + 0x1D0) = -1;
                *(float *)(target_state + 0x1D4) = -1.0f;
                *(int *)(target_state + 0x1CC) = -1;
                *(int *)(target_state + 0x1E0) = 0;
                *(int *)(target_state + 0x1D8) = 0;
                *(int *)(target_state + 0x1DC) = 0;
            }
            func_L04_0043AD48(&x, &y, *(int *)(target_state + 0x120),
                              *(int *)(target_state + 0x124), target_state + 0x1C4,
                              target_state + 0x1C8, state);
            {
                int top, bottom;
                top = hud_scale_v((float)(y - 1));
                bottom = hud_scale_v((float)(y + 3));
                func_0028FC48(x - 2, top, x + 2, bottom, 0x600040E4UL, 0);
            }
        }
        temp = func_00283BE0(angle, *(float *)(object + 0xF8));
        if (*(float *)(state + 0x4A4) == -1.0f) *(float *)(state + 0x4A4) = temp;
        else func_002AAA00(state + 0x4A4, temp, state + 0x4B0, 0,
                           0.002327105961740017f, 0.002327105961740017f, 4.71238899230957f);
        *(int *)(state + 0x49C) = 387;
        if (*(int *)(state + 0x4A0) == -1) *(int *)(state + 0x4A0) = reticle_y;
        else {
            temp = (float)*(int *)(state + 0x4A0);
            func_002AA548(&temp, state + 0x4AC, (float)reticle_y,
                         0.1388888955116272f, 0.1388888955116272f, 5.833333492279053f);
            *(int *)(state + 0x4A0) = (int)temp;
        }
        direction[0] = 0.0f;
        direction[1] = *(float *)(state + 0x4A4);
        direction[2] = 0.0f;
        func_00283410(view, direction);
        func_00282D30(view, view, *(float *)(asset + 12));
        func_00282D30(view + 8, view + 8, *(float *)(asset + 12));
        func_00300AA8(asset, (float)*(int *)(state + 0x49C),
                     (float)hud_scale_v((float)*(int *)(state + 0x4A0)),
                     0x8000A6E4, 0.0f, view);
        func_L04_0043AD48(&x, &y, *(int *)(state + 0x314), *(int *)(state + 0x318),
                          reticle_x_rate, reticle_y_rate, state);
        func_0028FC48(x, hud_scale_v((float)y), x + 4,
                     hud_scale_v((float)(y + 4)), 0x8000A6E4UL, 0);
        func_002FC560(71, 0x5360BUL);
        if (*(u8 *)(object + 0x20) != 3) {
            amount = *(float *)(state + 0x28C) * 0.00872664712369442f;
            sine = func_00283198(amount);
            angle = sine / func_00283180(amount);
            *(float *)(D_L04_001BDB80 + 0xB0) = angle;
            func_0027AB40();
        }
    }
    return 1;
}
