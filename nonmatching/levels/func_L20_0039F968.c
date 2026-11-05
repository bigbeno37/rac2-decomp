/* Claude Opus 5.5 xhigh, task v2583, third and last Opus attempt, 2026-11-05; USA v1.01, level 20 only.
 * Best check.py: 996/996 bytes, 0/1 places, first difference +0xB0. 17 rows differ once
 * relocated values are ignored, all in +0xAC..+0x108: the block before the history copy
 * loop and the loop's registers. Every other instruction agrees.
 * Profile: -O2 -G8 -ffunction-sections -frac2-qcopy -frac2-qcopy-barrier, own unit
 * (L20_0039F968-g8.c). -fno-rerun-loop-opt is NOT needed and was the wrong lead: the default
 * loop passes hoist the 400 constant out of the outer loop, which gives retail's
 * mult v1,s4,a0 (result not tied to the constant) and fixes last/tail ($s0/$s2).
 * Fixed since the 996-byte draft of task v2165:
 *  - state in $fp and i+1 at sp+0x1E0: the history pseudo must lose to state. Priority is
 *    3*8/368 with three uses; head through the bare table (below) leaves 2 uses and it loses.
 *  - previous-row pointer `p = table - 1; p += i * 25;` is a two-insn (consec) loop movable
 *    with no REG_EQUAL, so the giv addend stays one register and the 4-operand flatten of
 *    fold's reduce_expression_tree_depth is avoided: L + (M + X), as retail.
 * Still differs. Retail's block after the blez, with pseudos named:
 *    addiu a0,0x190; mult v1(M),s4,a0; lui a0(hi); addiu v0(T),a0,%lo(table);
 *    addiu a1(X),v0,-0x10; sll a0(L),s0,4; addu v0(Q1),v1,v0; addu v1(Q2),v1,a1;
 *    addu a1(dest),a0,v0; addu a0(src),a0,v1
 * What that implies (inferred from retail; the pass behaviour quoted was measured here):
 *  1. T stays in this block and X is rewritten from it by cse2. The second loop pass moves T
 *     out of the outer loop (life 9 from duplicate giv inits; head's T matches it), so here
 *     X keeps its own lui and T is rematerialised.
 *  2. hi is a global spilled pseudo (reloaded into a0), T/Q1 and M/Q2 are local ties. Pointers
 *     built before a do-while inside `if (last > 0)` reach this shape (head and tail through
 *     `history`, the point loop through the bare table). Closest wording, 34 rows, most of them
 *     because last loses s0 to tail (24*4/30 = 3.2 against 3.3 in the nearest measured wording;
 *     this draft has 24*4/29 = 3.31 and wins):
 *       int end = last * 16, offset = i * 400, row = (int)D_L20_00234880, before = row - 16;
 *       int rowBefore = offset + before; Vector *source, *target; row = offset + row;
 *       source = (Vector *)(end + rowBefore); target = (Vector *)(end + row);
 *       do { target->q = source->q; --source; --historyIndex; --target; } while (historyIndex > 0);
 *     There M and X swap registers, and hi lands in v1 rather than a0.
 *  3. dest init runs before src init: original order must be src then dest, or sched1's
 *     register-weight tie picks src first. The .q copy's parallel records src first, so giv
 *     inits come out dest first; using qcopy() instead leaves the setup inside the loop.
 *  4. The speed load must end the pre-branch block (retail: sll, lui, addiu s0, lwc1). Read
 *     after the loop in the source it stays there (1004 bytes): gcse does not hoist it.
 * All declarations it needs follow.
 */
#include "common.h"
typedef int Quadword __attribute__((mode(TI)));
typedef union { Quadword q; float f[4]; } Vector;
extern void func_00282D30(void *, const void *, float);
extern void func_00282CC0(void *, const void *, const void *);
extern int func_L02_002F1658(void);
extern void func_002AB668(void *, int, float);
extern void func_002ACDE8(void *, const void *, const void *);
extern void func_002ACB48(float *, float);
extern float func_00282DF0(const void *);
extern unsigned char *func_002C36D0(void *, int, int, int);
extern void func_L00_003C5310(void *, int, int, int, unsigned int, unsigned int, unsigned int, unsigned int,
                             int, int, int, float, float, float);
extern Vector D_L20_00234880[][25];
extern Vector D_L20_00235500[];
extern int D_L20_001B3870[] __attribute__((sda));
extern float D_L20_001AED3C __attribute__((sda));
extern float D_L20_001AED40 __attribute__((sda));
extern float D_L20_001AED50 __attribute__((sda));
extern float D_L20_001AED58 __attribute__((sda));
extern unsigned int D_L20_001AED44 __attribute__((sda));
extern unsigned int D_L20_001AED48 __attribute__((sda));
extern unsigned int D_L20_001AED4C __attribute__((sda));
extern float D_L20_001AED54 __attribute__((sda));
extern unsigned char D_L20_001FFB40[];

void func_L20_0039F968(unsigned char *object) {
    Vector points[25];
    Vector origin, offset, rotation;
    int i = 0;
    int *counts = D_L20_001B3870;
    Vector (*history)[25] = D_L20_00234880;
    unsigned char *level = D_L20_001FFB40;
    unsigned char *state = *(unsigned char **)(object + 0x68);
    unsigned char *particle;
    int j, last;
    float length, limit;
    func_00282D30(&offset, object + 0xE0, D_L20_001AED3C);
    func_00282CC0(&origin, object + 0x10, &offset);
    for (; i < 8; ++i) {
        if (func_L02_002F1658()) {
            Vector *head;
            int historyIndex;
            float speed;
            ++counts[i];
            if (counts[i] > 24) counts[i] = 25;
            last = counts[i] - 1;
            speed = D_L20_001AED58;
            for (historyIndex = last; historyIndex > 0; --historyIndex) {
                Vector *target = &D_L20_00234880[i][historyIndex];
                Vector *previous = (Vector *)D_L20_00234880 - 1;
                previous += i * 25;
                target->q = previous[historyIndex].q;
            }
            head = D_L20_00234880[i];
            func_002AB668(&rotation, (int)&D_L20_00235500[i], speed * 0.017453294f);
            func_002ACDE8(head, head, &rotation);
            func_002ACB48(head->f, 0.125f);
            length = func_00282DF0(head);
            limit = D_L20_001AED40 * 1.1f;
            if (limit < length) func_00282D30(head, head, limit / length);
            else {
                limit = D_L20_001AED40 * 0.9f;
                if (length < limit) func_00282D30(head, head, limit / length);
            }
        }
        last = counts[i] - 1;
        if (last < 0) last = 0;
        {
            int n = 0;
            for (j = last; j > 0; --j) {
                func_00282CC0(&points[n], &origin, &history[i][j]);
                points[n].f[3] = 0.5f;
                ++n;
            }
        }
        func_00282CC0(&points[last], &origin, history[i]);
        points[last].f[3] = 0.05f;
        if (func_L02_002F1658()) {
            int top = counts[i] - 1;
            Vector *tip = &points[top];
            particle = func_002C36D0(tip, 10, 0, 1);
            if (particle) {
                float size = D_L20_001AED50 * 210000.0f;
                *(unsigned int *)(particle + 4) = D_L20_001AED44 | *(unsigned int *)(state + 4);
                *(float *)(particle + 0xC) = size;
                particle[2] = (*(unsigned char **)(level + 0xF8))[1];
            }
            particle = func_002C36D0(tip, 10, 0, 1);
            if (particle) {
                *(float *)(particle + 0xC) = D_L20_001AED50 * 63000.00390625f;
                *(unsigned int *)(particle + 4) = 0x407F7F7F;
            }
        }
        func_L00_003C5310(points, 25, 14, 14, D_L20_001AED48, D_L20_001AED48 & 0xFFFFFF,
                          D_L20_001AED4C | *(unsigned int *)(state + 4), D_L20_001AED4C,
                          0, counts[i] - 1, counts[i], 0.1f, 0.2f, D_L20_001AED54);
    }
}
