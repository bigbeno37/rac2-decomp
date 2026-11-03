/* func_L03_003F6BA8 - opus-2 (v2171), 2026-11-03.
 * Best: 736/736 bytes, 0/1 places; identical up to +0x1BC, 30 instructions differ after it
 * (62 cdiff rows incl. relocations). Needs its own unit with -O2 -G8 -ffunction-sections -g
 * (it reaches its data through $gp; -g gives retail's nop pair before div.s and the loop alignment nop).
 * Separate mirror-loop variable `n` fixes the +0xCC counter (v1); `next = k + 1` gives retail's
 * (k+1)<<2 colour index and separate colour givs.
 * Still differs, two causes:
 *  1. loop.c pass 1: `next<<2` is the 8th movable of the inner loop (49 real insns); threshold is
 *     T = 1 + n_non_fixed_regs = 61 (isa<4 fixes the 8 FP cc regs), minus 3 per move, so it needs
 *     61-21 >= 49: "not desirable". color[next] is reduced only in the rerun pass, its init lands last
 *     (shortest life) and it takes s0; retail reduces it in pass 1 (init first, s4; draw giv s0).
 *     Needs `next<<2` among the first 5 movables (171 const 144, 177 k<<4, 189 square copy,
 *     199 k*16+16, 214 k<<2, 222 colour-base copy, 237 0xFFFFFF) or life/savings > 1.
 *  2. gcse's PRE copy of the colour base sp+0xF0 (222: refs 5, live 54) is allocated before
 *     i+1 (304: refs 4, live 44), so the copy takes s6 and i+1 s7; retail has i+1 in s6 and the
 *     copy coalesced into s7 (needs 304 one insn shorter or 222 one insn longer).
 * -fmove-all-movables moves everything and permutes the givs differently (no match).
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
