/* func_L04_0040DF90 (696 bytes, levels 04 and 11). Tier opus-2, 4 November 2026.
 * Unit: a file of its own with -O2 -G0 -ffunction-sections -g (-fno-gcse
 * gives the same bytes).
 * Best check.py: 696/696 bytes, 0/2 places, first mismatch +0x1d4, 17 words
 * differ (cdiff, call targets masked), all in the second envelope.
 * New against the 3 November draft: the then-arm computes its value before
 * the copy (`value = amount*k + 0.2f; original = amount; amount = value;`),
 * so its mul reads amount (f20) as retail does.
 * What still differs:
 *  - sched1 issues the 0.85 load at t2, ahead of div.s (t3, after the 0.45
 *    load's two cycles). Nothing in the block depends on it, so it goes as
 *    soon as the memory unit is free. Global alloc then gives it f1 (retail:
 *    f0, reused after the div, so retail's 0.85 load came after the div in
 *    sched1: a dependence on the div, or one pseudo for 0.45 and 0.85).
 *  - else arm: `original = amount; amount -= 0.85f;` subtracts from the copy
 *    (sub.s f20,f21,f1; retail sub.s f20,f20,f0). cse.c make_regs_eqv
 *    (lines 866-885) makes the copy canonical when it outlives the block and
 *    its LAST_UID is later than amount's. original is read last, at
 *    (1 - original)*-2816. Retail's arm reads amount, so either amount is
 *    used after original in the original source, or the copy was made after
 *    cse.
 * Tried without gain: value-first else arm (`value = (1-(amount-0.85f))*0.8f
 * + 0.2f; original = amount; amount = value;`, 692 bytes, sub into f3), the
 * final argument as (1 - elapsed/0.45f) so PRE would make the copy (no
 * effect: div.s may trap, so gcse leaves it and a second div appears),
 * -fno-schedule-insns on the unit (fixes the 0.85 order but the first
 * phase's schedule breaks: 138 rows differ).
 */
#include "common.h"
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern unsigned char D_00188660[];
extern void func_002E59F8(int);
extern int func_00282948(void *);
extern int func_002E59B0(void *, int);
extern int func_002E5CF8(int, int, void *);
extern int func_L01_00355BA0(int, int);
extern int func_002E5F40(int, int);

void func_L04_0040DF90(unsigned char *m, unsigned char *p)
{
    float elapsed, amount, original;
    if (*(int *)(p + 0x32c) && func_00282948(p + 0x324)) {
        int handle = *(int *)(p + 0x328);
        if (handle != -1) {
            unsigned char *record = D_00188660 + handle * 0x70;
            if (*(void **)(record + 0x88) == m && record[0x74])
                func_002E59F8(handle);
        }
        *(int *)(p + 0x32c) = 0;
        *(int *)(p + 0x328) = -1;
        *(int *)(p + 0x324) = 0;
    }
    elapsed = *(float *)(p + 0x30c) + *(float *)(p + 0x27c) - 0.050000004f;
    if (elapsed < 0.0f) elapsed = 0.0f;
    if (!func_002E59B0(m, *(int *)(p + 0x330))) {
        if (m[0x20] == 3)
            *(int *)(p + 0x330) = func_002E5CF8(20, 5, m);
        else
            *(int *)(p + 0x330) = func_002E5CF8(0, 5, m);
    }
    if (!func_002E59B0(m, *(int *)(p + 0x334))) {
        if (m[0x20] == 3)
            *(int *)(p + 0x334) = func_002E5CF8(21, 5, m);
        else
            *(int *)(p + 0x334) = func_002E5CF8(1, 5, m);
    }
    if (func_002E59B0(m, *(int *)(p + 0x330))) {
        amount = elapsed * 3.0f;
        if (amount > 1.0f) amount = 1.0f;
        amount = 1.0f - amount;
        func_L01_00355BA0(*(int *)(p + 0x330), func_00283CF0(func_00283CE0(1024) * amount));
        amount = elapsed * 3.0f;
        if (amount > 1.0f) amount = 1.0f;
        func_002E5F40(*(int *)(p + 0x330), func_00283CF0(amount * 2816.0f));
    }
    if (func_002E59B0(m, *(int *)(p + 0x334))) {
        amount = elapsed / 0.45f;
        if (amount < 0.85f) {
            float value = amount * 0.9411764f + 0.2f;
            original = amount;
            amount = value;
        } else {
            original = amount;
            amount -= 0.85f;
            amount = (1.0f - amount) * 0.8f + 0.2f;
        }
        func_L01_00355BA0(*(int *)(p + 0x334), func_00283CF0(func_00283CE0(1024) * amount));
        func_002E5F40(*(int *)(p + 0x334), func_00283CF0((1.0f - original) * -2816.0f));
    }
}
