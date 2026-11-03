/* Opus 5.5, xhigh (v2384): -O2 -G8 -ffunction-sections, a *-g8.c unit. EXACT BYTES under check.py:
 * 644/644 bytes, 28 of 28 places (us); US2 and PAL do not have this function. Not linked.
 * Reported FAILED, not matched: the bytes need the `if` below, whose only effect is on scheduling
 * and allocation, and whose condition is a reconstruction. It needs a maintainer's ruling.
 *
 * The exact-bytes form, between the spacing conversion and the start conversion:
 *     half = (float)n * 0.5f;
 *     if (sp > 0 && sp * n > D_001A7344[0])
 *         top = 0.0f;
 *     top = (float)(D_001A7344[0] >> 1) - (float)sp * half;
 *     start = (int)top;
 *
 * Why retail must have had a vanished test here (each step measured):
 * - A block boundary after the spacing conversion is what orders the centring block: with
 *   identical arms there, every FP register and the whole schedule equal retail. Pinning count
 *   and spacing to a1/a0 without a boundary leaves the schedule wrong, so the 28 differing
 *   instructions of the earlier draft were not an allocation problem alone.
 * - count=a1 and spacing=a0 need two GPR temporaries live beside them, and retail has neither
 *   temporary. So the test's set-up was deleted after allocation.
 * - The arm is a float store overwritten before any read: flow1 empties it. An int store does
 *   not work, because jump1 turns it into a conditional move and the test goes before flow.
 * - The condition has to be `a && b`. flow2 tidies the inner jump and deletes its set-up (the
 *   temporaries) but keeps the outer jump, which is the boundary sched2 needs; jump2 then deletes
 *   it. A single test is merged by flow2, and the height load moves above the conversion.
 * - a must compare spacing with zero, so that it leaves no word and spacing outranks count:
 *   `n > 0 && ...` swaps a0 and a1. half must be computed before the test.
 *
 * What the bytes do not settle: the condition itself. These all give the same 28/28:
 *     sp > 0 && sp * n > D_001A7344[0]        sp > 0 && sp * n > 0x8000
 *     sp > 0 && sp * n > menu->unknown438     sp != 0 && sp + n > menu->unknown438
 *     sp != 0 && menu->selected > menu->unknown438
 * and these do not (their inner test needs only one temporary):
 *     sp > 0 && sp * n > 448                  sp != 0 && menu->selected >= n
 * The first is used here because it reads sensibly (the list is taller than the screen,
 * D_001A7344 being the screen height), not because anything shows the original said it. No RAC1
 * counterpart or sibling function with the test live was found. The stored 0.0f is equally
 * unconstrained. Evidence for the original test, or a ruling on this form, would unblock it.
 *
 * Earlier measurements below are historical where this note supersedes them.
 */
/* 3 November 2026: retained near miss, not linked.
 * Profile: -O2 -G8 -ffunction-sections.
 * Current size: 644/644 bytes; US 0/28, US2 absent, PAL absent.
 * Complete 72-byte text typing preserves the true 64-bit style. Reusing the descriptor address
 * shortened the body to 628 bytes; preserving its separately formed address retains the 27-word
 * centering/setup residual.
 * Reopen on a real additional lifetime or source/control-flow witness for the centering block;
 * owner typing and widget-address capture did not resolve it.
 * Earlier measurements below are historical where this note supersedes them.
 */
/* opus-1 (v1334): -O2 -G8 -ffunction-sections (a *-g8.c unit); 644/644 bytes, 0/28 places.
 * The whole loop body, the prologue/epilogue and the frame match retail. Two source facts
 * recovered that: the coordinate stores go through a struct view of the rectangle returned
 * by func_00335E10 (MEM_IN_STRUCT stores do not conflict with the scalar sda tuning loads, so
 * those loads schedule above the first store as retail does), and the highlight colour is
 * computed into a local before the func_00335E20 call.
 * Still different: the 28 instructions of the centring computation between the fifth
 * func_00336320 call and the count test (+0x8C..+0xF8). Retail allocates count=a1,
 * spacing=a0, height=v0, 5F0=f0, 23A4=f2, 0.5=f1, (float)count=f3; cc1 gives count=v1,
 * spacing=v0 (and f0/f3/f4 for the floats), so sched2 orders the block differently.
 * A local-alloc model of this block shows retail's GPR assignment needs one more GPR
 * quantity live across the spacing conversion than this dataflow has. Tried without
 * success: statement order/separate variables, float variable reuse, 0.5 hoisted to a
 * variable (takes f20), split-address D_001A7344 (count moves to a0), -g,
 * -fno-sched-interblock, -fno-sched-spec, -ffast-math, -fno-schedule-insns (breaks the
 * loop), a for loop nested in the count test (count becomes global, spacing v1, but a
 * second blez stays). Next: find what natural source adds a GPR value in that block.
 * Declarations below are standalone; the D_ addresses are the boot's (gp 0x001AEFF0).
 */
#include "common.h"

extern int func_002A9588(int, int, int, int, int);
extern void func_00336320(char *);
extern float *func_00335E10(char *);
extern int *func_00335E20(char *);
extern float D_001AE5F0 __attribute__((sda));
extern float D_001B23A4[] MACRO_ADDR;
extern int D_001A7344[] MACRO_ADDR;
extern float D_001AE5D0 __attribute__((sda));
extern float D_001AE5D4[] MACRO_ADDR;
extern float D_001AE5D8 __attribute__((sda));
extern float D_001AE5E0 __attribute__((sda));
extern float D_001AE5E4[] MACRO_ADDR;
extern float D_001AE5E8 __attribute__((sda));

typedef struct { float x, y, w, h; } ListRect;
typedef struct {
    char pad[0x434];
    ListRect *origin;
    int unknown438;
    int selected;
    int enabled;
    int count;
    int showSpecial;
} ListMenu;
typedef struct { short pad[4]; short offset; short pad2; void (*call)(char *); } ListVtable;
typedef struct {
    ListRect *position;
    ListRect *scale;
    void *parameter08;
    unsigned int *colors;
    float *visibility;
    char lifetime[0x1C];
    ListVtable *table;
    void *metrics;
    unsigned long style;
    char *text;
    int alignment;
} ListSlot;

void func_00348EC0(char *o)
{
    int color = func_002A9588(0x20FFFEED, 0x70FFFEED, 20, 0, 0);
    int start, i, n, sp;
    float half, top;
    ListMenu *menu = (ListMenu *)o;

    if (menu->enabled) {
        func_00336320(o);
        func_00336320(o + 0x130);
        func_00336320(o + 0x4C);
        func_00336320(o + 0x98);
        func_00336320(o + 0xE4);
        sp = (int)(D_001AE5F0 * D_001B23A4[0] + 0.5f);
        n = menu->count;
        half = (float)n * 0.5f;
        if (sp > 0 && sp * n > D_001A7344[0])
            top = 0.0f;
        top = (float)(D_001A7344[0] >> 1) - (float)sp * half;
        start = (int)top;
        if (n > 0) {
        i = 0;
        do {
            ListRect *p;
            char *slot;
            ListVtable *v;
            int c;

            if (i == 7 && !menu->showSpecial)
                continue;
            p = (ListRect *)func_00335E10(o + 0x17C);
            p->x = D_001AE5D0 + menu->origin->x;
            p->y = (D_001AE5D4[0] + (float)i * D_001AE5D8) + (float)start;
            c = i == menu->selected ? color : 0x55F0C070;
            *func_00335E20(o + 0x1C8) = c;
            func_00336320(o + 0x17C);
            p = (ListRect *)func_00335E10(o + 0x1C8);
            p->x = D_001AE5D0 + menu->origin->x;
            p->y = (D_001AE5D4[0] + (float)i * D_001AE5D8) + (float)start;
            func_00336320(o + 0x1C8);
            slot = (char *)&((ListSlot *)(o + 0x218))[i];
            p = (ListRect *)func_00335E10(slot);
            p->x = D_001AE5E0 + menu->origin->x;
            p->y = (D_001AE5E4[0] + (float)i * D_001AE5E8) + (float)start;
            v = *(ListVtable **)(o + i * 72 + 0x248);
            v->call(slot + v->offset);
        } while (++i < menu->count);
        }
    }
}
