/* opus-1 (Claude Opus 5.5, xhigh), 3 November 2026, this checkout's compiler (upstream + #348):
 * 1240/1240 bytes, 0/28 places, 7 instructions differ in every place.
 *
 * Unit: a new sibling src/game/00345358-list-g8.c with
 *   -O2 -G8 -ffunction-sections -frac2-hazard-labels -frac2-gcse-unsplit-edges
 * (gp-relative D_001AE5EC needs -G8; unsplit edges is what the matched sibling func_00347F80 in
 * 00345358-wrap.c uses for the same wrap loop).
 *
 * Fixed in this round (the 2 November draft, recompiled here, had the whole do-loop wrong):
 * - wrap loop: retail keeps -1 and a `wrapped = last` copy inside the loop (late cmove,
 *   `movn a1,v0,a0`). The sibling's `WrapLow(&idx, lim)` pointer helper plus
 *   -frac2-gcse-unsplit-edges gives it exactly; the plain `if (v < 0) v = lim;` hoists the -1.
 * - `addu v0,a3,v0` (base first) needs the flags as an array member (`flags->enabled[idx]`);
 *   `int *flags` gives index first, `*(int *)(m + 0x418 + idx * 4)` folds into lw 0x418.
 *
 * Still differs:
 * 1. Spill slots swapped (+0x1B0, +0x1E0, +0x3B8, +0x478): retail has the final call's
 *    `m + 0xE4` at 4($sp) and `pad & 0x1000` at 8($sp). Slots follow pseudo order: `pressed`
 *    is a user var (97), the E4 value is gcse PRE reaching reg ~291 (copy of the fifth
 *    func_00335E10 argument, hoisted next to it by sched1). Retail therefore needs the mask
 *    pseudo above the PRE copy. Making the mask a PRE copy too (`if (pad & 0x1000)` with the early
 *    `pressed = pad & 0x1000;` left dead) puts it in bucket 29 vs E4 bucket 100 of 141; raw
 *    hashes solved from the 139- and 141-bucket tables: E4 (plus r84 228) = 14341,
 *    mask (and r85 4096) = 18218, so table sizes 129/133/135/137/143/147/149 would flip them. But
 *    the dead store is not landable, and declaration inits (`start = 0, lim = 0, pressed = 0`)
 *    never reached gcse: the table stayed at 141 buckets. `i`/`idx`/`v` inits also change the loop.
 *    A `wE4` user variable (declared first, set after the fifth call) is copy-propagated away by
 *    gcse cprop: the fifth call's pseudo (193) is then spilled and that call loads from the stack.
 *    The original likely has a second real `pad & 0x1000` occurrence or another reason the mask
 *    pseudo is created after gcse; not found.
 * 2. For-loop preheader order (+0x300..+0x308): retail `ori s4,0x8080` before the
 *    `addiu s2,s3,0x418` / `daddu s1,zero,zero` giv inits; here after them. Not investigated.
 */
#include "include_asm.h"

typedef unsigned char u8;
typedef struct {
    int enabled[1];
} MenuFlags;

extern float D_001AE5EC __attribute__((sda));
extern const float D_001AE760[10] __attribute__((section(".sdata")));

extern void func_00335E68(void *, int);
extern void func_003363C0(void *, float);
extern float *func_00335E10(void *);
extern unsigned int *func_00335E20(void *);
extern void func_00335F88(void *, float, float, float, float);
extern void func_002A9588(int, int, int, int, int);
extern int func_002E5E58(int, int, int);

static inline void WrapLow(int *index, int last)
{
    if (*index < 0)
        *index = last;
}

#define MODE (*(int *)(m + 0x44C))
#define ORG (*(float **)(m + 0x434))

int func_003489E8(u8 *m, int pad)
{
    u8 *w98 = m + 0x98;
    u8 *w4C = m + 0x4C;
    u8 *w130 = m + 0x130;
    u8 *w17C = m + 0x17C;
    u8 *w1C8 = m + 0x1C8;
    u8 *a, *b, *c;
    float *p;
    MenuFlags *flags;
    int pressed, step, i, idx, v, start, lim;

    func_00335E68(m, MODE == 0);
    func_00335E68(w98, MODE == 0);
    func_00335E68(w4C, MODE == 0);
    func_00335E68(m + 0xE4, MODE == 0);
    func_00335E68(w130, MODE == 0);
    func_00335E68(w17C, MODE == 1);
    func_00335E68(w1C8, MODE == 1);
    func_003363C0(w17C, D_001AE5EC);
    func_003363C0(w1C8, D_001AE5EC);

    if (MODE == 0 && *(int *)(m + 0x448) != 0) {
        func_003363C0(m, 1.0f);
        func_003363C0(m + 0x98, 1.0f);
        func_003363C0(m + 0x4C, 1.0f);
        func_003363C0(m + 0x130, 1.0f);
        c = m + 0x98;
        b = m + 0x4C;
        a = m + 0x130;
        ORG[1] = 170.0f;
    } else {
        func_003363C0(m, 0.0f);
        func_003363C0(m + 0x98, 0.0f);
        func_003363C0(m + 0x4C, 0.0f);
        func_003363C0(m + 0x130, 0.0f);
        c = m + 0x98;
        b = m + 0x4C;
        a = m + 0x130;
        ORG[1] = 190.0f;
    }
    pressed = pad & 0x1000;

    p = func_00335E10(m);
    p[0] = D_001AE760[0] + ORG[0];
    p[1] = D_001AE760[1] + ORG[1];
    p = func_00335E10(a);
    p[0] = D_001AE760[2] + ORG[0];
    p[1] = D_001AE760[3] + ORG[1];
    p = func_00335E10(b);
    p[0] = D_001AE760[4] + ORG[0];
    p[1] = D_001AE760[5] + ORG[1];
    p = func_00335E10(c);
    p[0] = D_001AE760[6] + ORG[0];
    p[1] = D_001AE760[7] + ORG[1];
    p = func_00335E10(m + 0xE4);
    p[0] = D_001AE760[8] + ORG[0];
    p[1] = D_001AE760[9] + ORG[1];

    for (i = 0; i < *(int *)(m + 0x444); i++) {
        u8 *row = m + 0x218 + i * 0x48;

        p = func_00335E10(row);
        p[0] = ((float *)(*(int *)(m + 0x414) + i * 12))[0] + ORG[0];
        p[1] = ((float *)(*(int *)(m + 0x414) + i * 12))[1] + ORG[1];
        func_00335F88(row, ((float *)(*(int *)(m + 0x414) + i * 12))[2], 0.0f, 0.0f, 0.0f);
        if (*(int *)(m + 0x418 + i * 4) != 0)
            *func_00335E20(row) = 0x80F0F0F0;
        else
            *func_00335E20(row) = 0x80808080;
    }

    step = 0;
    if (pressed) {
        func_002E5E58(3, 0, 0);
        step = -1;
    } else if (pad & 0x4000) {
        func_002E5E58(3, 0, 0);
        step = 1;
    }
    if (step != 0) {
        start = *(int *)(m + 0x43C);
        func_002A9588(0, 0, 1, 0, 1);
        lim = *(int *)(m + 0x444) - 1;
        flags = (MenuFlags *)(m + 0x418);
        do {
            v = *(int *)(m + 0x43C) + step;
            if (lim < v)
                v = 0;
            idx = v;
            WrapLow(&idx, lim);
            *(int *)(m + 0x43C) = idx;
            if (flags->enabled[idx] != 0)
                break;
        } while (start != idx);
    }
    func_003363C0(m + 0xE4, (float)*(int *)(m + 0x43C));
    *func_00335E20(m + (*(int *)(m + 0x43C) * 0x48 + 0x218)) = 0x70FFFEED;
    return *(int *)(m + 0x43C);
}
