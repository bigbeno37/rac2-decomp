/* Opus-3 (v2693), 5 November 2026: last attempt. The C below is unchanged and gives the same
 * result: 644/644 bytes, 0/2 places, only the a1 spill misplaced. This pass measured why.
 * sched2 ranks by priority, then dependent count, then LUID, and the prologue always has the
 * lower LUIDs. The spill (insn 6) has priority 14 and 5 dependents: 54 (every insn), 36 (call
 * flush), 34 (stale sched1 anti on hard a1), 28 and 14 (loads). The s0/s4/s6/s7/f21 saves have
 * 54, 36, 28, 14 and their setter. -fno-gcse takes s7's setter out of block 0 and the spill then
 * passes `sd s7`, which confirms the model. Retail therefore has a sixth dependent or priority
 * 15. Ruled out: any retail word in block 0 reading the a1 pseudo; other hard a1 writers (anti
 * links reach only the first writer per pass); memory links (/u saves mean a /u load would
 * drop the spill's link, not theirs; s5 has no alias base); CLOBBER insns (reload deletes them);
 * USE insns (only under obey_regdecls); a jump2-deleted test (setters 41 and 38 show block 0
 * runs past the first call, and p is in memory there); loop notes (they link saves and spill
 * alike); an empty asm reader, forbidden anyway (no output makes it a volatile barrier: 20 or
 * more rows differ). Every flag in config/cflags.txt, tried singly: no effect on the spill except
 * -fno-gcse (+0x18, 172 insns) and -fno-schedule-insns. To unblock: find a no-code insn that
 * survives to sched2 in block 0 and reads the a1 pseudo or writes hard a1 before insn 28.
 */
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

