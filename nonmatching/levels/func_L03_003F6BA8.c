/* func_L03_003F6BA8 - opus-3 (v2575), third and last Opus pass, 2026-11-05.
 * Needs its own unit with -O2 -G8 -ffunction-sections -g.
 * Body below: the closest wording within the rules, unchanged from opus-2 (v2171):
 * 736/736 bytes, 0/1 places, 62 cdiff rows including relocations (30 instructions per opus-2).
 *
 * EXACT BYTES, NOT LANDABLE (1 of 1 places on USA, PAL and v2.00): it needs a pasted instruction. Against the
 * body below: add `s32 mask` with `mask = 0xFFFFFF;` just before `fade = ...`, use `& mask`
 * for both colours, drop the outer `next`, and replace the outer loop with:
 *     for (i = 0; i < 32; i++) {
 *         s32 t;
 *         s32 c = i & 1;
 *
 *         __asm__("addiu %0,$0,2" : "=r"(t) : "r"(i));
 *         k = c ? t : 0;
 *         for (j = 0; j < 4; j++) {
 *             func_00283098(draw[j].position[k], draw[j].position[k], square);
 *             func_00283098(draw[j].position[k + 1], draw[j].position[k + 1], square);
 *             draw[j].color[k] = func_00283D38(draw[j].color[k], draw[j].color[k] & mask, fade);
 *             {
 *                 s32 next = k + 1;
 *
 *                 draw[j].color[next] = func_00283D38(draw[j].color[next], draw[j].color[next] & mask, fade);
 *             }
 *             func_00280B90(&draw[j], transform, 1);
 *         }
 *     }
 * Everything except the asm is plain C, and the same selector also drops the `k = 2 - k` step.
 *
 * Why it works, measured from the dumps:
 *  - Both opus-2 causes are fixed. `mask` removes one insn and one movable from the inner loop.
 *    `next` set inside it is a movable pair (savings 2, life 2), so its shift moves free through
 *    `forces`, in pass 1, and colour[next]'s giv gets its init first.
 *  - The block needs one more insn at sched1 cycle 1: the `2` loaded after `i & 1`, feeding the
 *    movz. sched1 issues two per cycle. Without it the draw giv's init (a free copy) fills the
 *    slot beside the movz and lives longest; with it that init issues last (s0), i+1 issues
 *    after the movz (live 42, so it beats the colour-base copy, which coalesces into s7), and
 *    `c` spans the extra insn, so `k` outranks it in local-alloc (k=v0, c=v1).
 *
 * The one open problem: an in-block `2` that combine does not fold.
 *  - Any in-block constant with `c` known to be one bit folds to `sll k,c,1`
 *    (simplify_if_then_else). Tried: long/short/char `c`, a reused multi-set `c`, `k = m + 1`.
 *  - regmove's copy_src_to_dest emits that insn itself (`k = r` before the cmove) for the
 *    form (if_then_else (eq c 0) 0 r) with r a constant set in another block. The form
 *    (ne c 0) r 0 is skipped.
 *  - Literal `k = c ? 2 : 0` INSIDE the inner loop gives that form and retail's order, but `r`
 *    sits between k's two sets, so consec only moves the pair in the rerun pass and the givs
 *    are not reduced (72 real diffs). With `two` set outside, the pair moves in pass 1 but the
 *    form is (ne c 0), and k<<2 misses the budget (33 real): `forces` needs k set once.
 *  - In the outer block, `k = two; if (c == 0) k = 0;` converts after cprop, so it folds;
 *    ?: and if/else spellings, and 2 - (c == 0 ? 2 : 0), stay (ne) or fold (40-62 real).
 *  - Not used: empty do/while(0) after the selector (barrier) stops at 11 real diffs, all the
 *    k/c v0-v1 swap, because i+1 is pinned after the movz and `c` spans nothing.
 *  - A short-lived pointer local for `square` takes a saved register and spills fp.
 */
#include "common.h"

typedef struct {
    f32 position[4][4];
    s32 color[4];
    u8 pad[0x40];
} DrawQuad;

extern void func_0029FC20(void *entity, s32 id, void *out);
extern void func_L02_002EF968(void *quad, s32 a, s32 b, void *c, void *d, s32 e);
extern void func_00282C88(void *v);
extern void func_002833F0(void *m, void *v);
extern void func_00283098(void *dst, void *src, void *m);
extern void func_00283738(void *dst, void *a, void *b);
extern s32 func_00283D38(s32 a, s32 b, f32 t);
extern void func_00280B90(void *packet, void *m, s32 a);

extern s32 D_L03_001B1108 __attribute__((sda));
extern s32 D_L03_001B1138 __attribute__((sda));
extern f32 D_L03_001B1118 __attribute__((sda));
extern f32 D_L03_001B1150 __attribute__((sda));
extern f32 D_L03_001B1190 __attribute__((sda));
extern f32 D_L03_001B11D0 __attribute__((sda));
extern s32 D_L03_001B110C[] __attribute__((sda));
extern s32 D_L03_001B1140[] __attribute__((sda));
extern s32 D_L03_001B113C[] __attribute__((sda));
extern s32 D_L03_001B1144[] __attribute__((sda));

void func_L03_003F6BA8(u8 *entity) {
    f32 rotation[4], matrix[12], square[12], transform[16];
    DrawQuad draw[4];
    s32 i, j, k, n;
    f32 fade;

    func_0029FC20(entity, 7, transform);
    func_L02_002EF968(&draw[0], D_L03_001B1108, D_L03_001B1138, &D_L03_001B1118, &D_L03_001B1150, 1);
    func_L02_002EF968(&draw[1], D_L03_001B1108, D_L03_001B1138, &D_L03_001B1118, &D_L03_001B1150, 1);
    func_L02_002EF968(&draw[2], D_L03_001B110C[0], D_L03_001B1140[0], &D_L03_001B1118, &D_L03_001B1190, 1);
    func_L02_002EF968(&draw[3], D_L03_001B110C[0], D_L03_001B1140[0], &D_L03_001B1118, &D_L03_001B1190, 1);
    for (n = 0; n < 4; n++) {
        f32 *p = draw[0].position[n];

        p[36] = -p[36];
        p[37] = -p[37];
        p[108] = -p[108];
        p[109] = -p[109];
    }
    draw[1].color[1] = draw[1].color[3] = draw[0].color[1] = draw[0].color[3] = D_L03_001B113C[0];
    draw[3].color[1] = draw[3].color[3] = draw[2].color[1] = draw[2].color[3] = D_L03_001B1144[0];
    func_00282C88(rotation);
    rotation[2] = 0.09817477f;
    func_002833F0(matrix, rotation);
    for (j = 0; j < 2; j++) {
        func_00283098(draw[j].position[2], draw[j].position[2], matrix);
        func_00283098(draw[j].position[3], draw[j].position[3], matrix);
    }
    func_00283738(square, matrix, matrix);
    fade = 1.0f / D_L03_001B11D0;
    for (i = 0; i < 32; i++) {
        s32 next;

        k = (i & 1) ? 0 : 2;
        k = 2 - k;
        next = k + 1;
        for (j = 0; j < 4; j++) {
            func_00283098(draw[j].position[k], draw[j].position[k], square);
            func_00283098(draw[j].position[k + 1], draw[j].position[k + 1], square);
            draw[j].color[k] = func_00283D38(draw[j].color[k], draw[j].color[k] & 0xFFFFFF, fade);
            draw[j].color[next] = func_00283D38(draw[j].color[next], draw[j].color[next] & 0xFFFFFF, fade);
            func_00280B90(&draw[j], transform, 1);
        }
    }
}
