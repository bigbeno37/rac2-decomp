/* func_L00_00411B30: near miss, 1044/1044 bytes, 0 of 27 places (USA v1.01, 3 November 2026).

   Tier: opus-1 (v2313), Claude Opus 5.5.
   Profile: its own unit (e.g. src/levels/shared/L00_00411B30-qcopy.c) with
   `-O2 -G0 -ffunction-sections -fno-gcse`; qcopy() is the common.h inline asm.
   check.py: first mismatch +0x8. Aligned for retail's one extra instruction, only 12 rows differ.

   This form fixes two things the earlier -frac2-qcopy -fno-gcse draft got wrong:
   - Reusing `first` for the 0x30 endpoint (`qcopy(p + 0x30, p + 0x50); first = p + 0x30;`) and
     recomputing `second = p + 0x20` for the lerp gives retail's `addiu a0,s0,0x30; lq; sq v0,0(a0);
     daddu s2,a0,zero` and the fresh `addiu s1,s0,0x20`.
   - Loading `amount = *(float *)(p + 0xc8)` before the owner +0xB8 store gives retail's hit setup
     order: 0xCC, then f12, then the store in the jal delay slot.

   What still differs:
   - Table address: retail keeps only `lui s6,%hi(D_L00_00256BC0)` outside the loop and forms
     `addiu a0,s6,%lo` in each iteration, which shifts every loop branch by one. `base` here hoists
     the whole address. That lui-only hoist is gcse PRE's (loop.c rejects high(D): savings 2,
     life 2). With gcse on, cprop/PRE instead keep long-lived copies of p+0x20/p+0x30 (an extra
     saved GPR, 1052 bytes; the sol-low gcse form is 1044 bytes with first mismatch +0x1c0).
   - After the second quad copy, retail puts `daddu s2,a0` right after the sq and branches with
     `beql a0,zero` on the +0xB0 load. Here the copy lands in the beqz delay slot.
   Might fix it: a source shape where only the high part is loop-invariant under -fno-gcse, or a
   gcse-on form that keeps cprop from propagating the endpoint copies.

   Declarations needed: Vector128 (TI mode), D_L00_00256BC0 (char[]), func_002AB1F0 returning
   char *, and func_002A8C00(void *, void *, int, void *, float), as below. */

#include "common.h"
typedef int Vector128 __attribute__((mode(TI)));
extern void func_002AFFD8(void *, void *, int);
extern void func_00282D30(void *, void *, float);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);

extern char D_L00_00256BC0[];
extern char *func_002AB1F0(char *);
extern float func_00283B98(float, float);
extern float func_00283180(float), func_00283198(float), func_00283CE0(int);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282D08(void *, void *, void *, float);
extern int func_00282948(void *);
extern void func_002A8C00(void *, void *, int, void *, float);
extern void func_002A8E20(void *, void *);
extern void func_L00_00412928(void *, int, int);
void func_L00_00411B30(char *object)
{
    Vector128 temp[1];
    char event[48];
    char *p, *node, *first, *second;
    char *base;
    int i;
    float fraction, amount;
    base = D_L00_00256BC0;
    for (i = 0; i < 48; i++) {
        p = base + i * 0xd0;
        if (*(char **)(p + 0xac) != object) continue;
        first = p + 0x20;
        qcopy(first, p + 0x40);
        if (*(char **)(p + 0xb4)) {
            node = func_002AB1F0(*(char **)(p + 0xb4));
            if (node) {
                func_002AFFD8(*(char **)(p + 0xb4) + 0x10, temp, 1);
                func_00282D30(temp, temp, *(float *)(node + 0x10));
                func_00282CC0(temp, temp, *(char **)(p + 0xb4) + 0x10);
                func_00282CC0(first, temp, first);
            } else qcopy(first, *(char **)(p + 0xb4) + 0x10);
        }
        if (*(int *)(p + 0xb8)) {
            *(float *)(p + 96) = func_00283B98(*(float *)(p + 96), *(float *)(p + 108) * 100.0f);
            *(float *)(p + 100) = func_00283B98(*(float *)(p + 100), *(float *)(p + 112) * 100.0f);
            *(float *)(p + 104) = func_00283B98(*(float *)(p + 104), *(float *)(p + 116) * 100.0f);
            *(float *)(p + 132) = func_00283B98(*(float *)(p + 132), *(float *)(p + 144) * 100.0f);
            *(float *)(p + 136) = func_00283B98(*(float *)(p + 136), *(float *)(p + 148) * 100.0f);
            *(float *)(p + 140) = func_00283B98(*(float *)(p + 140), *(float *)(p + 152) * 100.0f);
        } else {
            *(float *)(p + 96) = func_00283B98(*(float *)(p + 96), *(float *)(p + 108));
            *(float *)(p + 100) = func_00283B98(*(float *)(p + 100), *(float *)(p + 112));
            *(float *)(p + 104) = func_00283B98(*(float *)(p + 104), *(float *)(p + 116));
            *(float *)(p + 132) = func_00283B98(*(float *)(p + 132), *(float *)(p + 144));
            *(float *)(p + 136) = func_00283B98(*(float *)(p + 136), *(float *)(p + 148));
            *(float *)(p + 140) = func_00283B98(*(float *)(p + 140), *(float *)(p + 152));
        }
        *(float *)(p + 0) = func_00283180(*(float *)(p + 96)) * *(float *)(p + 120);
        *(float *)(p + 4) = func_00283198(*(float *)(p + 100)) * *(float *)(p + 124);
        *(float *)(p + 8) = -func_00283198(*(float *)(p + 104)) * *(float *)(p + 128);
        *(float *)(p + 16) = func_00283180(*(float *)(p + 132)) * *(float *)(p + 156);
        *(float *)(p + 20) = func_00283198(*(float *)(p + 136)) * *(float *)(p + 160);
        *(float *)(p + 24) = -func_00283198(*(float *)(p + 140)) * *(float *)(p + 164);
        qcopy(p + 0x30, p + 0x50);
        first = p + 0x30;
        if (*(char **)(p + 0xb0)) {
            node = func_002AB1F0(*(char **)(p + 0xb0));
            if (node) {
                func_002AFFD8(*(char **)(p + 0xb0) + 0x10, temp, 1);
                func_00282D30(temp, temp, *(float *)(node + 0x10));
                func_00282CC0(temp, temp, *(char **)(p + 0xb0) + 0x10);
                func_00282CC0(first, temp, first);
            } else qcopy(first, *(char **)(p + 0xb0) + 0x10);
        }
        fraction = func_00283CE0(*(int *)(p + 0xb8));
        fraction /= func_00283CE0(*(int *)(p + 0xbc));
        second = p + 0x20;
        func_00282D08(first, second, first, 1.0f - fraction);
        if (!func_00282948(p + 0xb8)) continue;
        if (*(int *)(*(char **)(p + 0xb0) + 0x98) && !*(int *)(p + 0xc0) && *(int *)(p + 0xc4)) {
            func_00282CF0(temp, first, second);
            func_00282F20(temp, temp, 0.033333335f);
            amount = *(float *)(p + 0xc8);
            *(int *)(object + 0xb8) = *(int *)(*(char **)(p + 0xcc) + 0x20);
            func_002A8C00(event, object, 0x210000, temp, amount);
            event[0x18] = 5;
            event[0x19] = 3;
            *(short *)(event + 0x1a) = *(short *)(object + 0xaa);
            func_002A8E20(*(char **)(p + 0xb0), event);
            *(int *)(p + 0xc0) = 1;
            func_L00_00412928(first, 10, 1);
        }
        if (func_00282948(p + 0xa8)) *(int *)(p + 0xac) = 0;
    }
}
