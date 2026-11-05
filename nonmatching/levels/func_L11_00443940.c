/* func_L11_00443940 - 924 bytes, level 11 only. opus-2 tier (Claude Opus 5.5), third and last Opus attempt, 5 November 2026.
 * Best tools/check.py: 924/924 bytes, 0/1 places, first difference +0x10; 200 of 231 words in a normalized common
 * subsequence (relocations masked). Own -G8 unit, -O2 -G8 -ffunction-sections. Earlier bests were 908 and 912 bytes.
 *
 * Solved here, and kept in the body below:
 * - The loop: registers s1..s5, the alignment nop and the in-loop table address, without the +$s6 strength reduction
 *   of earlier drafts. Literal constants in scaled_y keep 0.5 in $f2 inside the draw instead of hoisted.
 * - The label in $a2 at all four draws: a real display-mode test after each conversion makes the label cross a block,
 *   so combine cannot fold it into (set a0 v0). func_L11_004456A0 is exact with the same helper.
 * - The frame, 0.5 in $f22, 1.0 in $f20, 0.0 in $f21, and 1.0/0.0 shared by draws 1, 2 and the pulse. These come from
 *   plain constant arguments: calls.c copies each const_double into a pseudo, and cse shares it.
 *
 * Still different:
 * 1. Draw 2's chain must end a block that sched2 sees, as in retail (its argument loads all follow the chain), and cse
 *    must also carry 1.0/0.0 across that point. The two helpers each give one of these:
 *    - `result != 0 && mode == 1` (scaled_half): the jump survives sched2, but its label has two uses, so cse stops
 *      there and draw 2 rematerializes 1.0/0.0 (228 words, 193 common). Explicit full/none locals do not help: the
 *      constants still reach draw 2 (gcse cprop), and copy chains of one and two links change nothing (measured; why
 *      was not established). Reassigning the constants in the arm does not help: the arm empties, apparently in cse1
 *      because the arm is on its path, the test then vanishes, and the chain interleaves.
 *    - `mode == 1` alone (scaled_one, used below): one use, so cse goes around it, but flow2 tidies the jump and merges
 *      the blocks, so sched2 interleaves the argument loads and $a0 takes the result (retail: $v0).
 *    Untested idea: an arm that stays non-empty at flow2's CFG cleanup and costs no word (a USE or a hard-register
 *    CLOBBER would do, and next_active_insn skips both after reload), but no C construct was found that emits one.
 * 2. Roles: state lands in $s0 and enabled in $s2 (retail object $s0, state $s2, enabled $s3). The draw-1 form with
 *    explicit `s` and `one` passed to the helper gave retail's roles and result $v0, but the label in $a0.
 * 3. Draws 3 and 4: retail loads the field into $v1 before `daddu $a2,$v0`, base $a0; here the copy comes first
 *    (base $v1, field $a0 in draw 3, field $v0 in draw 4). Loading the mode after the first test is worse (label $a0).
 *
 * Declarations: as below (standalone -G8 unit). Callees were checked against retail jal targets in an earlier attempt:
 * func_00336D50 = 11:0048DFD0, func_00300A00 = 11:00394D98, func_L00_0037B7B0 = 11:00390018, func_00283D38 = 11:00301748,
 * func_00288D58 = 11:00306870, func_002FC560 = 11:0038FD70.
 */
/* Draws the contextual selection help and locates the next available entry. */
#include "common.h"

typedef struct {
    u8 pad[0x1a0];
    int field1a0;
} InputView;
typedef struct {
    u8 pad[4];
    u8 enabled;
    u8 rest[0xdb];
} MenuRecord;
extern u8 *D_L11_001A904C;
extern float D_L11_001B3E04;
extern int D_L11_001B30C8;
extern unsigned char D_00138180[];
extern unsigned char D_00139568[], D_001395B8[];
extern unsigned char D_L11_00279BA0[];
/* The class pointer table is addressed through gp. */
extern int *D_L11_001B14F8[] SDATA(D_L11_001B14F8);
#define TABLE D_L11_001B14F8
extern void func_L00_0037B7B0(void);
extern int func_00336D50(u8 *, int);
extern void func_00300A00(char *, int, float, float, float, float, float);
extern unsigned int func_00283D38(unsigned int, unsigned int, float);
extern int func_L11_0043E108(unsigned int);
extern int func_00288D58(void);
extern void func_002FC560(unsigned int, unsigned long long);
extern float D_L11_001B161C __attribute__((sda));
extern int D_L11_001B1620 __attribute__((sda));
extern int D_L11_001B1624 __attribute__((sda));
extern int D_L11_001B1628 __attribute__((sda));
extern int D_L11_001B162C __attribute__((sda));
extern int D_L11_001B1630 __attribute__((sda));
extern int D_L11_001B1634 __attribute__((sda));
extern unsigned int D_L11_001B1638 __attribute__((sda));
extern unsigned int D_L11_001B163C __attribute__((sda));
extern unsigned int D_L11_001B1644 __attribute__((sda));
extern unsigned int D_L11_001B1648 __attribute__((sda));

extern int D_L11_001B3E00;

static __inline__ int scaled_half(int value, float half)
{
    int mode = D_L11_001B3E00;
    float scaled = (float)value * D_L11_001B3E04 + half;
    int result = (int)scaled;

    if (result != 0 && mode == 1)
        scaled = 1.0f;
    return result;
}

static __inline__ int scaled_one(int value, float half)
{
    int mode = D_L11_001B3E00;
    float scaled = (float)value * D_L11_001B3E04 + half;
    int result = (int)scaled;

    if (mode == 1)
        scaled = 1.0f;
    return result;
}

static __inline__ int scaled_y(int value)
{
    float scaled = (float)value * D_L11_001B3E04 + 0.5f;
    int result = (int)scaled;
    int mode = D_L11_001B3E00;

    if (result != 0 && mode == 1)
        scaled = 1.0f;
    return result;
}

void func_L11_00443940(u8 *object, int enabled)
{
    u8 *state;
    int label, phase, i, y, offset;
    unsigned int color;
    float pulse, x, half;
    if (D_L11_001A904C == 0) return;
    func_L00_0037B7B0();
    offset = 0x8710;
    half = 0.5f;
    state = *(u8 **)(object + 0x68);
    label = func_00336D50(D_L11_001A904C + offset, 0x49);
    x = (float)D_L11_001B1620;
    y = scaled_half(D_L11_001B1624, half);
    func_00300A00((char *)label, D_L11_001B163C, x, (float)y, D_L11_001B161C, 1.0f, 0.0f);
    label = func_00336D50(D_L11_001A904C + offset, 0x48);
    x = (float)D_L11_001B1620;
    y = scaled_one(D_L11_001B1624, half);
    func_00300A00((char *)label, D_L11_001B1638, x, (float)y, D_L11_001B161C, 1.0f, 0.0f);
    phase = D_L11_001B30C8 % 50;
    if (phase >= 25) pulse = 1.0f - (float)(phase - 25) * 0.04f;
    else pulse = (float)phase * 0.04f;
    color = func_00283D38(D_L11_001B1638, D_L11_001B163C, pulse);
    if (enabled) {
        if (*(short *)(state + 0x1e8) != 0) {
            label = func_00336D50(D_L11_001A904C + 0x8710, 0x4b);
            func_00300A00((char *)label, (((InputView *)D_00138180)->field1a0 & 0x8000) ? D_L11_001B1644 : color,
                (float)D_L11_001B1628, (float)scaled_y(D_L11_001B162C),
                D_L11_001B161C, 1.0f, 0.0f);
        }
        for (i = *(short *)(state + 0x1e8) + 1; i < 13; i++) {
            if (!D_001395B8[func_L11_0043E108(TABLE[i][2])]) continue;
            if (i >= 11 && !func_00288D58()) continue;
            if (i == 8) {
                MenuRecord *choice = (MenuRecord *)(D_L11_00279BA0 + D_00139568[0x1e] * 0xe0);
                if (!choice->enabled) continue;
            }
            label = func_00336D50(D_L11_001A904C + 0x8710, 0x4a);
            func_00300A00((char *)label, (((InputView *)D_00138180)->field1a0 & 0x2000) ? D_L11_001B1648 : color,
                (float)D_L11_001B1630, (float)scaled_y(D_L11_001B1634),
                D_L11_001B161C, 1.0f, 0.0f);
            break;
        }
    }
    func_002FC560(0x47, 0x5360b);
}
