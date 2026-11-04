/* Opus-2 (v2438), 4 November 2026: GNU game -O2 -G0 -ffunction-sections.
 * US: 644/644 bytes, 0/2 places, one instruction misplaced ("instruction at +0x4 differs").
 * On the current compiler the opus-1 draft below also swapped the 0x1C/0x20 spill slots:
 * they are gcse PRE reaching registers (reg 192 = i + 1, reg 194 = o + 0x200), numbered in
 * hash-bucket order with a 61-bucket table (o+0x200 in bucket 49, i+1 in 36). `int r = 0;`
 * (one declaration initializer, to disclose) adds one insn to GCSE's count and restores
 * retail's order. Remaining: retail schedules the a1 spill `sw $a1,0x10($sp)` at +0x4, before
 * every save; here it follows `addiu $s0,$s6,0x10` (+0x20). sched2 (-fsched-verbose-5): the
 * spill (insn 6) has 5 dependents (14, 25, 31 stale from sched1, 33, 51) and priority 14, tying
 * the s0/s4/s6/s7/f21 saves, and loses on LUID (the prologue sits on the entry edge). It needs a
 * sixth dependent: a sched1-time reader of the a1 pseudo in block 0. An entry alias
 * `float *out = a1;` supplies it (the store goes to +0x4) but cprop returns the call and store
 * uses to a1 while the asm keeps out, so both live: 652 bytes. Retail's block 0 has no
 * instruction that could read a1, so the source shape is still unknown.
 * Earlier notes (opus-1, v1496): qcopy_nc for the a1 copy gives retail's `daddu $a0,$a3` call
 * argument; const parameters, -g and -fno-schedule-insns do not fix the spill.
 * Declarations: L03_002AB5C8.c already has D_L03_001B2A1C, D_L03_001B2A38, D_L03_001FB5A0,
 * func_L03_00419BD8 and func_00282E80; func_00277258, func_002A89A8, func_00282E48 and
 * L03SeekState must be added with the function.
 */
#include "common.h"

extern unsigned char *D_L03_001B2A1C __attribute__((sda));

extern int D_L03_001B2A38[] __attribute__((sda));

extern unsigned short *D_L03_001FB5A0[];

extern int func_L03_00419BD8(unsigned int, void *, void *);

extern float func_00282E80(void *, void *);

extern int func_00277258(void *, int, void *, float);

extern float func_002A89A8(void *, int, float);

extern float func_00282E48(void *, void *);

typedef struct {
    unsigned char pad0[0x200];
    int ids[8];
    unsigned int ref220;
    unsigned char pad1[0x354 - 0x224];
    unsigned char *target;
} L03SeekState;

void func_L03_003C5668(unsigned char *a0, float *a1, float *a2) {
    L03SeekState *o = *(L03SeekState **)(a0 + 0x68);
    unsigned short **tbl = D_L03_001FB5A0;
    unsigned char *cand;
    unsigned char *best = 0;
    unsigned char *e;
    unsigned short *q;
    float bestd;
    float d;
    float vec[4] __attribute__((aligned(16)));
    int r = 0;
    int i;

    bestd = func_00282E48(a0 + 0x10, o->target + 0x10);
    cand = o->target;
    if (func_L03_00419BD8(o->ref220, a0 + 0x10, cand + 0x10))
        best = o->target;
    for (i = 0; i < 8; i++) {
        for (q = tbl[o->ids[i]];
             (o->ids[i] & 0xFF) != 0xFF && o->ids[i] <= D_L03_001B2A38[0] && q != 0 &&
             ((short)q[-1] >= 0 || q == tbl[o->ids[i]]);
             q++) {
            e = D_L03_001B2A1C + ((*q & 0x7FFF) << 8);
            if (e == 0)
                break;
            if (*(short *)(e + 0xAA) == 0x972) {
                d = func_00282E80(a0 + 0x10, e + 0x10);
                if (d < bestd) {
                    qcopy(vec, e + 0x10);
                    vec[2] += 1.0f;
                    if (func_00277258(vec, 0, a0, 0.5f) == 0) {
                        cand = e;
                        r = func_L03_00419BD8(o->ref220, a0 + 0x10, e + 0x10);
                        bestd = d;
                        if (r)
                            best = cand;
                    }
                }
            }
        }
    }
    if (best)
        cand = best;
    qcopy(a2, cand + 0xF0);
    qcopy_nc(a1, cand + 0x10);
    a1[2] = func_002A89A8(a1, 0, 0.5f);
}

