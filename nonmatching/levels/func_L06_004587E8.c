/* opus-2 (Claude Opus 5.5, xhigh), v2457: NOT A MATCH. Lawful form only.
 * check.py: 1596/1572 bytes, 0/1 USA v1.01 places, frame 0x100 vs retail 0xF0.
 * Aligned instruction diff: 137 of 393 retail instructions differ (the
 * previous lawful draft: 141; opus-1's forbidden `int regen = 1; if (regen)`
 * form in git 1713ac2cd: 77, 1564 bytes).
 *
 * New here: the collision buffer and velocity/axis are block-scoped locals
 * (collision[112] in case 0's block, velocity[4]/axis[4] in the trail loop's
 * body). gcc 2.95's expand_decl gives block locals assign_stack_temp(keep=1)
 * slots that are freed at block end and split on reuse, so velocity lands at
 * sp+0x10 and axis at sp+0x20 inside the freed collision slot: retail's layout
 * without the inferred scratch union, with identical code.
 *
 * Still differs: gcse PRE hoists sp+0x10 (velocity) and sp+0x20 (axis) out of
 * the trail loop (inserted on the default arm, available from case 0), so both
 * become spilled loop-invariant pseudos (sp+0x84, sp+0x88; i+1 moves to 0x8C).
 * Retail computes `addiu s0,sp,0x10` per outer iteration and `addiu s1,sp,0x20`
 * in the inner preheader, i.e. neither address was anticipatable at the outer
 * loop entry during PRE: some path through the loop body skipped the trail
 * regeneration at gcse time and vanished afterwards (the regen test does this,
 * but it is an always-true test). Also the case-1 dispatch constant (retail
 * `addiu v1,zero,1` at +4, a pseudo set before the switch), scan registers,
 * clear-loop `mult zero; mflo`. Tried here, worse: the regeneration as a static
 * inline helper (frame base const-equiv is substituted per use: 180 differ)
 * and only the segment loop inline (frame 0xF0 but velocity still hoisted:
 * 156 differ). Next: find the real source of a gcse-time path around the
 * regeneration (a loop exit/continue that later folds), not a placeholder test.
 *
 * Declarations below. L06_00458138.c declares func_002761F8's fifth argument
 * as int: pass `(int)collision` there (same code) or give this its own unit.
 */
#include "common.h"
extern short D_L06_001B3790[] __attribute__((sda));
extern short D_L06_001B37A0[] __attribute__((sda));
extern char D_L06_0025FFC0[];
extern float func_00283B98(float, float);
typedef unsigned int NotakQuad __attribute__((mode(TI)));
extern char D_00188660[];
extern char D_L06_001A8F20[] __attribute__((sda));
extern void func_002E59F8(int);
extern int func_00282948(void *);
extern char D_L06_001B9840[];
extern void func_00282CF0(float *, const float *, const float *);
extern void func_00282F20(char *, char *, float);
extern int func_002A77E0(int);
extern void func_002ACEC0(void *, void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern char *func_002C2210(void *, int, float, float, unsigned int, int, int, void *, int, float);
extern void func_002A7750(char *, int, int);
extern float func_002A7878(float, float);
extern void func_00350388(char *);
typedef struct {
    char prefix[0x18];
    char *hit;
    int count;
    float point[4];
} NotakCollision;
extern NotakCollision D_L06_001BFC80;
extern char D_L06_001C79C0[];
extern int D_L06_001B3778[], D_L06_001B3760[];
extern void func_002A8C00(void *, char *, int, void *, float);
extern int func_002761F8(void *, void *, int, void *, void *);
extern int func_0031EBF8(float, void *, int, void *);
extern int func_002A8C28(void *, void *, int, int, int, int, float, float, float);
extern void func_L06_00459828(float *, int);
extern void func_00291450(int);
extern void func_002A7988(void *, float, float);
extern float func_00282DF0(void *);
extern int func_002E59B0(void *, int);
extern int func_002E5DA0(int, int, void *, int);
extern void func_L00_002E6EF8(void *, void *);
extern void func_L06_00458F30(char *);

void func_L06_004587E8(char *o) {
    float pos[4];
    char *st = *(char **)(o + 0x68);
    int i, j, sign, snd;
    typedef NotakQuad Trail[6][4];
    float direction;
    float size;
    switch (*(unsigned char *)(o + 0x20)) {
    case 0: {
        char collision[112];
        *(float *)(o + 0xF0) = func_00283B98(*(float *)(o + 0xF0), 0.104719773f);
        func_00282CC0(pos, o + 0x10, st);
        func_002A8C00(collision, o, 1, st, *(float *)(st + 0x40));
        *(unsigned short *)(collision + 0x1A) = *(unsigned short *)(o + 0xAA);
        if (func_002761F8(o + 0x10, pos, 0, o, collision) ||
            func_0031EBF8(0.3f, o + 0x10, 0, o) || func_00282948(st + 0x38)) {
            char *hit = D_L06_001BFC80.hit;
            float *impact = D_L06_001BFC80.point;
            if (hit) {
                if (hit == *(char **)(st + 0x3C)) goto update_position;
                if (*(short *)(hit + 0xAA) != 0x4E)
                    func_002A8C28(o, o + 0x10, 0x10001, 0, 1, 0, 0.5f, *(float *)(st + 0x40), 1.0f);
                impact = (float *)(o + 0x10);
            }
            func_L06_00459828(impact, 20);
            snd = *(int *)(st + 0x4C);
            if (snd != -1) {
                char *s = D_00188660 + snd * 0x70;
                if (*(char **)(s + 0x88) == o && *(unsigned char *)(s + 0x74)) func_002E59F8(snd);
            }
            *(int *)(st + 0x4C) = -1;
            func_002A7750(o, 1, -1);
            return;
        }
    update_position:
        qcopy(o + 0x10, pos);
        break;
    }
    case 1: {
        int index, row, col;
        index = D_L06_001B3778[*(int *)(st + 0x48)];
        if (index >= 0) {
            char *p = D_L06_001C79C0 + index * 32;
            *(float *)(p + 0x1C) -= 0.166666672f;
            if (!(*(float *)(p + 0x1C) <= 0)) return;
            func_00291450(D_L06_001B3778[*(int *)(st + 0x48)]);
            D_L06_001B3778[*(int *)(st + 0x48)] = -1;
            D_L06_001B3760[*(int *)(st + 0x48)] = 0;
            for (row = 0; row < 6; row++) {
                int off = *(int *)(st + 0x48) * 0x180 + (row << 6);
                for (col = 1; col < 4; col++)
                    qzero_nc(&((NotakQuad *)(D_L06_0025FFC0 + off))[col]);
            }
            func_00350388(o);
            return;
        }
        D_L06_001B3760[*(int *)(st + 0x48)] = 0;
        func_00350388(o);
        return;
    }
    }
    qcopy(pos, o + 0x10);
    {
        int k = 0;
        if (D_L06_001B3790[k]) goto scanned;
    next:
        k++;
        while (k < 6) {
            if (!D_L06_001B3790[k]) goto next;
            break;
        }
    scanned:;
    }
    for (i = 0; i < 6; i++) {
        float velocity[4], axis[4];
        if (--D_L06_001B3790[i] < 0) D_L06_001B3790[i] = 0;
        D_L06_001B3790[i] = 2;
        D_L06_001B37A0[i] = 64;
        func_002A7988(velocity, 0.2f, 0.2f);
        if (func_00282DF0(velocity) > 0.2f) func_00282F20((char *)velocity, (char *)velocity, 0.2f);
        qcopy(&((Trail *)D_L06_0025FFC0)[*(int *)(st + 0x48)][i][0], pos);
        direction = 1.0f;
        for (j = 1; j < 4; j++) {
            func_00282CF0(axis, (float *)(&((Trail *)D_L06_0025FFC0)[*(int *)(st + 0x48)][i][j - 1]), (float *)D_L06_001B9840);
            func_002ACEC0(velocity, velocity, axis, func_002A7878(0.17453295f, 0.7853982448577881f) * direction);
            direction = -direction;
            func_00282CC0(&((Trail *)D_L06_0025FFC0)[*(int *)(st + 0x48)][i][j], &((Trail *)D_L06_0025FFC0)[*(int *)(st + 0x48)][i][j - 1], velocity);
        }
    }
    if (!func_002A77E0(2)) {
        size = func_002A7878(0.2f, 0.9f);
        sign = func_002A77E0(2);
        if (!sign) sign--;
        func_002C2210(o + 0x10, 2, size * 0.1f, size, 0x7F7F2020, 0, sign, D_L06_001A8F20, 0, 0);
        func_002C2210(o + 0x10, 2, size * 0.07f, size * 0.7f, 0x7F7F7F7F, 1, -sign, D_L06_001A8F20, 0, 0);
    }
    if (!func_002E59B0(o, *(int *)(st + 0x4C)))
        *(int *)(st + 0x4C) = func_002E5DA0(7, 4, o, *(short *)(*(char **)(st + 0x3C) + 0xAA));
    func_L00_002E6EF8(func_L06_00458F30, o);
}
