/* func_L00_002CA4E0: no linked match. Claude Opus 5.5 (opus-2, xhigh), v2408, 4 November 2026.
 * Best: 940/948 bytes, 0/27 places (USA v1.01 and v2.00; absent from PAL), 100 of 237 rows
 * differ, first difference +0xC4. Profile: its own unit, -O2 -G0 -ffunction-sections
 * -frac2-hazard-labels (for example src/levels/shared/L00_002C2DE8-choice.c plus a cflags line);
 * D_00189E20 is declared as a struct here, unlike the char array in L00_002C2DE8.c.
 *
 * Solved here, relative to the 944-byte opus-1 draft:
 * - Candidate loop in RAC1 pick_idle_fidget's shape (records = table inside the loop, separate
 *   inactive and `weight == 0.0f` tests, scale_ticks-style `50 < count`, `state >= 2`), and
 *   the byte_22bb test read into an int local first (one RTL insn instead of a QImode load plus
 *   extend). The rerun loop pass then sees 58 insns: 0x70 moves at threshold 61, 0.0f at 58,
 *   -1 stays (55): retail's $f20 = 0.0f, $fp = origin, -1 set in the loop and `move a0,v0`.
 * - ChoiceRecord starts with Vector pad0[5] (16-byte alignment), so the sum loop keeps
 *   `lwc1 0x58(base)` instead of folding +0x58 into the base; the class-table byte is
 *   ClassRecord (0xE0 bytes, byte04) indexed by D_00139568[30], giving retail's `lbu 0x4(v1)`.
 *
 * Still differs (both named by tools/regalloc.py dumps):
 * 1. First-loop preheader order: retail sets $s1 (records base) right after $fp; here it is
 *    last. sched1 ties at priority 1; the records lo_sum (giv init from strength reduction)
 *    has register weight 1 because gcse's PRE register for %hi(D_L00_0025BC00) stays live into
 *    the sampling block (`table = D_L00_0025BC00` there is a fully redundant HIGH). With
 *    -fno-gcse (diagnostic only) the order matches retail, but the prologue's D_00189E20 base
 *    copy moves. The sum loop's copy is harmless: loop.c hoists it as a constant move.
 * 2. Sampling: retail has `mov.s $f13,$f21` before the blez and divides by $f13 in the merged
 *    loop. Here `one` is a copy of the 1.0f argument pseudo after cse1, and gcse's cprop
 *    replaces it everywhere. A literal 1.0f in the loop gives the copy via loop.c + cse2, but
 *    the duplicated first iteration (jump.c, before cse1) then uses $f21 and the cross-jump
 *    merge fails. A `one` that is still a constant at gcse (cprop cannot put a CONST_DOUBLE
 *    into div.s) and becomes a copy only in cse2 would give retail's form.
 * The size gap is the missing mov.s plus the .p2align 3 padding nop, which moves with length.
 * Tried and worse: records set before the loop (-1 hoisted again); entry = &D_L00_0025BC00[i]
 * (204 rows); sampling `table = records` (178 rows); RAC1's sampling loop with the address
 * and 1.0f inside (dup not merged, 1020 bytes).
 */
#include "common.h"
typedef int Vector __attribute__((mode(TI)));
typedef union { Vector q; float f[4]; } ChoiceVector;
typedef struct {
    unsigned char pad0[0xE0]; float origin[4];
    unsigned char pad1[0x1B8 - 0xF0]; int count;
    unsigned char pad2[0x208 - 0x1BC]; short gate;
    unsigned char pad3[0xC28 - 0x20A]; int flags;
    unsigned char pad4[0x2290 - 0xC2C]; void *object;
    unsigned char pad5[0x22A4 - 0x2294]; int mode;
    unsigned char pad6[0x22BB - 0x22A8]; unsigned char byte_22bb;
    unsigned char pad7[0x24CC - 0x22BC]; int state;
} HeroChoiceState;
extern HeroChoiceState D_00189E20;
extern int D_001A79F0 __attribute__((sda));
extern int D_001A7398 __attribute__((sda));
extern unsigned char D_00139568[];
typedef struct {
    int word00;
    unsigned char byte04;
    unsigned char pad05[0xE0 - 5];
} ClassRecord;
extern ClassRecord D_L00_00262570[];
typedef struct {
    Vector pad0[5]; int selector;
    unsigned char pad1[4]; float weight;
    unsigned char pad2[4]; int inactive;
    unsigned char pad3[12];
} ChoiceRecord;
extern ChoiceRecord D_L00_0025BC00[4];
extern int D_L00_0025BDB0;
extern int func_L00_002ADEA0(int);
extern int func_002761F8(void *, void *, int, void *, int);
extern float func_002A7878(float, float);

int func_L00_002CA4E0(int *selected)
{
    int choices[20], count = 0, index;
    ChoiceRecord *entry, *records;
    ChoiceVector raised, origin;
    ChoiceVector *startp, *endp;
    int *next;
    float sum, draw;
    if (D_00189E20.gate != 0) return 0;
    if (D_00189E20.state == 1) {
        *selected = 3;
        if (D_L00_0025BDB0 != 0) return 0;
        if ((D_00189E20.flags & 2) == 0) return 0;
        return func_002A7878(0.0f, 1.0f) < 0.33f;
    }
    index = 0;
    startp = &origin;
    endp = &raised;
    next = choices;
    for (; index < 4; index++) {
        int area, carried;
        records = D_L00_0025BC00;
        entry = &records[index];
        if (entry->inactive != 0) continue;
        if (entry->weight == 0.0f) continue;
        area = func_L00_002ADEA0(0);
        if (entry->selector != -1 && entry->selector != area) continue;
        if (index == 2) {
            if (D_001A79F0 != 7 && D_001A79F0 != 0x13) continue;
            carried = D_00189E20.byte_22bb;
            if (carried == 2) continue;
            qcopy(startp, D_00189E20.origin);
            qcopy(endp, startp);
            raised.f[2] += 8.0f;
            if (func_002761F8(startp, endp, 2, D_00189E20.object, 0)) continue;
        }
        if (index == 3) {
            if (D_00189E20.mode != 1) continue;
            if (50 < D_00189E20.count) continue;
            if (D_00189E20.state >= 2) continue;
        }
        *next = index;
        count++;
        next++;
    }
    if (!count) return 0;
    sum = 0.0f;
    for (index = 0; index < count; index++) {
        sum += 1.0f / (float)(int)(D_L00_0025BC00[choices[index]].weight * 60.0f);
    }
    draw = func_002A7878(0.0f, 1.0f);
    if (draw < sum) {
        float one = 1.0f;
        ChoiceRecord *table = D_L00_0025BC00;
        int size = sizeof(ChoiceRecord);
        float rate = 60.0f;
        int *cursor = choices;
        for (index = 0; index < count; index++, cursor++) {
            int choice = *cursor;
            *selected = choice;
            sum -= one / (float)(int)(*(float *)((char *)table + choice * size + 0x58) * rate);
            if (sum < draw) break;
        }
        if (D_001A7398 == 30 && D_L00_00262570[D_00139568[30]].byte04 == 0) {
            if (*selected == 1) *selected = 0x91;
            else if (*selected == 2) *selected = 0x92;
        }
        return 1;
    }
    return 0;
}
