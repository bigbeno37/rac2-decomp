/* Claude Opus 5.5 xhigh, v2619, 2026-11-05: closest so far, not linked.
 * Unit: any -O2 -G8 -ffunction-sections file (tested as L00_003B9CB8-trail-g8.c).
 * USA v1.01: 916/916 bytes, 0/27 places, first difference +0xB0, 8 words differ, all
 * in the inner-loop setup (+0xAC..+0xDC): ours has M=v0 D=a0 X=v1 and from-sum before
 * to-sum, retail M=v1 D=v0 X=a0 and to-sum before from-sum. Everything else is equal
 * except relocation immediates. Previous best: 11 words.
 * Measured this round (use these, they cost about 30 compiles):
 * - A movable is opaque to simplify_giv_expr when its SET_SRC is not PLUS/MULT/ASHIFT/
 *   CONST_INT/SYMBOL_REF/sym+int, or when simplifying it returns 0. So retail's
 *   last*16 + (i*320 + X) comes from a LO_SUM X, a PRE copy (set X R), or a
 *   X = (plus R -16) with REG_EQUAL const whose R is not invariant.
 * - The D-16 / D relation must not fire in cse1 and must fire in cse2. It does when D is
 *   set in a block before the loop (a real la, or `to = D[i]`) and D-16 is written in
 *   the body: the body's label stops cse1, and after loop.c moves X into the same
 *   extended block cse2 relates it (addiu X,D,-16). This is the form below.
 * - Fall-through critical edges give no PRE block: split_edge's empty block vanishes in
 *   the second find_basic_blocks, so a statement between guard and loop needs a real
 *   block. `j = last; if (j > 0) { ...; do ... while (j > 0); }` keeps j=last in the
 *   blez delay slot (`if (last > 0) for (j = last...` loses it to the i+1 spill).
 * - In the outer loop a la in that block is movable unless used across blocks; it matches
 *   the later D[i][0] loads (savings 3) and is hoisted, so those go through `table`.
 *   A last use in a movable adds its savings via force_movables (D forced by X).
 *   Rerun pass: hoisted when threshold(63)*savings*life >= 165, so life <= 2.
 * - r5900_madd_profitable_p: a D local set in the block of D[i][0] loses mtlo/madd.
 * - Local-alloc flips dst/src of the first qcopy (a0/v1) in forms with no function-top
 *   la of D (+0x8 onward); function-top `table` keeps retail's order.
 * - A sum in that block sorts before loop-emitted giv inits only by luid: sched1 takes
 *   the later luid on a priority tie, so retail's to-then-from means both are giv inits,
 *   i.e. `to` is in the body, as is retail's fresh lui v0 (a loop.c move-insn HIGH; a
 *   la in the block uses PRE's spilled HIGH and reloads it into another register).
 * Other forms, all 916 bytes: row = D multi-set in the block plus body &row[i][j] gives
 * retail's to/from order and registers but needs a second set of row in v0 and a local
 * HIGH (lui a1; addiu v0,a1). Reopen: a form with D and D-16 both in the body but in
 * different cse1 blocks (a real vanishing test, or a bare volatile asm between them).
 */
/* Claude Opus 5.5 xhigh, v2430, 2026-11-04: no closer (916/916, 0/27, same 11 words).
 * Mechanism, from -ds/-dL dumps: retail's source start last*16 + (i*320 + X)
 * needs the giv add_val (plus (mult i 320) X) with X opaque when the giv is
 * reduced. Ours is (plus (plus (mult i 320) -16) D): cse1 relates
 * const(D-16) to the destination's D register (use_related_value), so X is a
 * PLUS movable that simplify_giv_expr substitutes; expand_mult_add's fold
 * then pulls the -16 out. A LO_SUM movable, or a register already outside the loop
 * (reduction only in the rerun pass), would stay opaque. In one cse block,
 * source-first makes the destination X+16 instead (952). Same bytes: struct-row type
 * D[i].p[j]. Worse: dest via a row pointer (924, computed before blez),
 * u128 copy (908, -16 folds into lq). X has savings 1/life 1, so pass 1 always moves it.
 */
/* Complete-owner follow-up, 2026-11-03; no linked match.
 * One new check under -O2 -G8 -ffunction-sections: int32 byte-offset cursors
 * over the actual four 20-point rows stay 916/916 bytes, 0/27 US placements.
 * The first residual moves to +0x4C (stack/cursor capture); equal extent and
 * frame 0x1F0 do not mean equal code. The earlier indexed body below is kept.
 * The offset probe formed pointers only inside j>0, so it never needed a
 * D-16 pointer outside the 1280-byte owner. Both forms use complete 16-byte
 * points, the 320-byte local vertex array and the measured draw/particle ABI.
 * No new source witness resolves the existing inner-loop source lifetime.
 * Reopen with independently grounded ownership/capture evidence, not an
 * invented alias, spill or before-owner pointer.
 */
/* Claude Opus 5.5 xhigh, v2208, 2026-11-03; not linked.
 * Best check: USA v1.01, -O2 -G8 -ffunction-sections (the L00_003B9CB8-g8.c unit),
 * 916/916 bytes, 0/27 places; declarations are included below. Improved from 912:
 * the texture table is nosda and read through a block-local `tables` pointer,
 * which gives retail's lui/addiu + lw 0xF8 and also fixes the i+1 temporary,
 * the first effect's context/scale order and the final context register.
 * Residual: only the inner copy loop's setup (+0xB8..+0x10C). Retail starts the
 * source pointer at last*16 + (i*320 + X), X = D-16, like the destination's
 * last*16 + (i*320 + D). Here cse1's related value rewrites D-16 as D_reg + -16
 * (the destination already loaded D in the same block), loop.c substitutes that
 * movable into the giv, and the start comes out as (last*16 + i*320) + (D-16);
 * a0/a1 and s1/s2 swap as a result. X must stay opaque at loop time (a LO_SUM
 * movable, related only in cse2). Tied: &D[i][j] - 1, D[i] + j - 1,
 * D[i] + (j - 1), flat (UiHistoryPoint *)D + i*20 + j - 1. Worse: source
 * pointer local computed first (952), count-up loop with reversed indices (936),
 * struct assignment (932, ld/sd pairs), separate prev = j - 1 index (924, no
 * strength reduction). Reopen with a form that keeps the source's D-16 out of
 * the destination's cse1 block.
 */
#include "common.h"


typedef struct { float f[4]; } __attribute__((aligned(16))) UiHistoryPoint;
extern int D_L00_001B2940[] MACRO_ADDR;
extern UiHistoryPoint D_L00_0023FB80[][20];
extern UiHistoryPoint D_L00_001B2900[] MACRO_ADDR;
extern float D_L00_001AFF4C __attribute__((sda));
extern int D_L00_001AFF50 __attribute__((sda));
extern int D_L00_001AFF54 __attribute__((sda));
extern int D_L00_001AFF58 __attribute__((sda));
extern float D_L00_001AFF5C __attribute__((sda));
extern float D_L00_001AFF60 __attribute__((sda));
extern char D_L00_001FE940[] __attribute__((nosda));
extern void func_00282CC0(void *, void *, void *);
/* The second integer-bank input is forwarded as the axis vector pointer. */
extern void func_002AB668(void *, void *, float);
extern void func_002ACDE8(void *, void *, void *);
extern void func_002ACB48(float *, float);
extern float func_00282DF0(void *);
extern void func_00282D30(void *, void *, float);
typedef struct {
    char prefix[8];
    int flags;
    char pad[0x24];
    UiHistoryPoint origin;
} UiTrailState;
typedef struct {
    unsigned char prefix[2];
    unsigned char texture;
    unsigned char flags;
    int color;
    int unused;
    float size;
} UiTrailParticle;
extern UiTrailParticle *func_002C36D0(void *, int, int, int);
extern void func_L00_003C5310(void *, int, int, int, int, int, int, int,
                              int, int, int, float, float, float);

void func_L00_003C4F78(char *owner)
{
    UiHistoryPoint vertices[20], origin, change;
    UiTrailState *state = *(UiTrailState **)(owner + 0x68);
    int i;
    UiHistoryPoint (*table)[20] = D_L00_0023FB80;
    qcopy(&origin, &state->origin);
    for (i = 0; i < 4; i++) {
        int last, j;
        UiHistoryPoint *head, *tip;
        float length, limit;
        UiTrailParticle *effect;
        D_L00_001B2940[i]++;
        if (D_L00_001B2940[i] >= 20)
            D_L00_001B2940[i] = 20;
        last = D_L00_001B2940[i] - 1;
        j = last;
        if (j > 0) {
            int offset = i * 320;
            UiHistoryPoint *to = (UiHistoryPoint *)((char *)D_L00_0023FB80 + offset);
            do {
                int index = last - j;
                qcopy(&to[j], (UiHistoryPoint *)((char *)&D_L00_0023FB80[0][j - 1] + offset));
                func_00282CC0(&vertices[index], &origin, &to[j]);
                vertices[index].f[3] = 0.05f;
                j--;
            } while (j > 0);
        }
        head = &table[i][0];
        func_002AB668(&change, &D_L00_001B2900[i], 0.10471976548433304f);
        func_002ACDE8(head, head, &change);
        func_002ACB48(head->f, 0.025f);
        length = func_00282DF0(head);
        limit = D_L00_001AFF4C * 1.1f;
        if (length > limit)
            func_00282D30(head, head, limit / length);
        else {
            limit = D_L00_001AFF4C * 0.9f;
            if (length < limit)
                func_00282D30(head, head, limit / length);
        }
        func_00282CC0(&vertices[last], &origin, &table[i][0]);
        vertices[last].f[3] = 0.05f;
        {
            int endpoint = D_L00_001B2940[i] - 1;
            tip = vertices + endpoint;
        }
        effect = func_002C36D0(tip, 2, 0, 1);
        if (effect != 0) {
            char *tables = D_L00_001FE940;
            effect->color = D_L00_001AFF50 | state->flags;
            effect->size = D_L00_001AFF5C * 210000.0f;
            effect->texture = (*(char **)(tables + 0xF8))[1];
        }
        effect = func_002C36D0(tip, 2, 0, 1);
        if (effect != 0) {
            effect->color = 0x407F7F7F;
            effect->size = D_L00_001AFF5C * 31500.001953125f;
        }
        func_L00_003C5310(vertices, 20, 14, 14,
                          D_L00_001AFF54, D_L00_001AFF54 & 0xFFFFFF,
                          D_L00_001AFF58 | state->flags, D_L00_001AFF58,
                          0, D_L00_001B2940[i] - 1, D_L00_001B2940[i],
                          0.02f, 0.07f, D_L00_001AFF60);
    }
}
