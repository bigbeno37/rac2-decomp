/* Claude Opus 5.5 xhigh (opus-2), 5 November 2026, task v2489; retained near miss.
 * Best current-toolchain check: 1252/1248 bytes, 0/28 USA v1.01 places,
 * 144 of 313 cdiff rows differ. Profile: a unit of its own (common.h clashes
 * with 002B3868.c's s32/u8 typedefs) with
 *   -O2 -G0 -ffunction-sections -frac2-keep-block-order
 * i.e. config/cflags.txt line "src/game/002B44C0.c<TAB>-O2 -G0 -ffunction-sections
 * -frac2-keep-block-order". No linked match. Types and declarations are below;
 * common.h supplies qcopy and qcopy_nc.
 *
 * Fixed in this version (all of the branch layout up to 0x002B45A8 now
 * matches retail):
 * - Control flow is `if (target || (player.target == self && player.state >= 2))
 *   { if (target) T; else P; active } else decay`. cse1 threads the outer
 *   tests: bnez target -> T, the player tests branch to decay (addiu a1,a1,0x80
 *   in the second slot), then `b P`, as in retail.
 * - With the default profile cygnus_merge_blocks pulls P up and jump2
 *   cross-jumps the two func_00282CF0 calls (1236 bytes). With
 *   -frac2-keep-block-order P stays after T and falls into the active block,
 *   so flow's (use (const_int 0)) after P's call blocks the cross-jump.
 * - The clamps are written `x > limit` (angles[0], angles[1], roll): this
 *   gives retail's lwc1 0x44(sp) before lwc1 0x64(s2) in the second clamp.
 *
 * Remaining differences:
 * 1. Spill slots 0xC8..0xE8 are permuted (9 PRE reaching registers). The
 *    order is gcse's hash-bucket order (raw = 14029 + base regno + offset,
 *    fp = reg 1 "at"; bucket = raw % ((max_cuid/2)|1)); this compile has
 *    max_cuid 239 -> 119 buckets, self = reg 84, sway = reg 85, and its
 *    predicted order equals the measured one. An offline search over odd
 *    table sizes 61..399 finds retail's order only for 111 buckets (also
 *    217, 235, 239) with self's base pseudo 98..110 (mod 111) and sway's
 *    114..128 (mod 111). So retail had about 16-19 fewer insns entering gcse
 *    (220..223) AND later base pseudos for self/sway (e.g. late copies that
 *    cprop restores, see the b0523 note). None found.
 * 2. The delta-pointer giv (slot 0xEC) is reloaded a second time for its
 *    increment after the store (lw v1,0xEC(sp) at 0x002B47C8, the +4 bytes).
 *    The x.i.greg dump shows reload's inheritance not reusing v1 between the
 *    store (insn 504) and the in-out reload of the increment; the multiply
 *    feeding the store carries a REG_EQUIV (mem (reg P)) from
 *    update_equiv_regs. reload_cse cannot remove it (the SF store through v1
 *    may alias the slot). Cause not found.
 *
 * Earlier notes still valid: qcopy_nc for the first snapshot keeps self in a0
 * (retail's daddu v0,a0,zero); approved qcopy for the second snapshot and the
 * offset-to-work transfer; TI work-to-angles assignment; separate kick-loop
 * counter. -fno-thread-jumps 1268 bytes; -fno-gcse 948 bytes.
 */

#include "common.h"

typedef int Vec128_2B44C0 __attribute__((mode(TI)));

typedef struct {
    Vec128_2B44C0 anchor;
    Vec128_2B44C0 offset;
    Vec128_2B44C0 rotation;
    float position[3];
    float velocity[3];
    float spring[3];
    float damping[3];
    float limit[3];
    float scale[3];
    float kick[3];
    float phase[3];
    float speed[3];
    float amplitude[3];
    int unused;
    int timer;
} Sway_2B44C0;

typedef struct {
    char pad0[0x80];
    Vec128_2B44C0 position;
    char pad1[0x33C - 0x90];
    char *target;
    int state;
} Player_2B44C0;

extern Player_2B44C0 D_00189E20;
extern void func_00115484(void *, int, int);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282CC0(void *, void *, void *);
extern void func_002AFDD8(char *, void *, void *, void *);
extern int func_00282948(void *);
extern float func_00283B98(float, float);
extern float func_00283198(float);
extern void func_002833F0(void *, void *);
extern void func_00283738(void *, void *, void *);
extern void func_00283098(void *, void *, void *);
extern void func_002B0158(char *, void *, void *, float);
extern char *func_002AD0B0(char *);
extern void func_002AD6F0(char *, void *, void *, void *);

void func_002B44C0(char *self, Sway_2B44C0 *sway, float *target)
{
    float old_position[4];
    float old_rotation[4];
    float push[4];
    float delta[4];
    float angles[4];
    float work[4];
    float matrix[12];
    float moved[4];
    float shift[4];
    float step[4];
    char *body;
    float roll;
    int i;
    int kick_axis;

    qcopy_nc(old_position, self + 0x10);
    qcopy(old_rotation, self + 0xF0);
    func_00115484(push, 0, 16);
    if (target != 0 || (D_00189E20.target == self && D_00189E20.state >= 2)) {
        if (target != 0)
            func_00282CF0(delta, target, self + 0x10);
        else
            func_00282CF0(delta, &D_00189E20.position, self + 0x10);
        func_002AFDD8(self, delta, delta, 0);
        func_00282CF0(delta, delta, &sway->offset);
        push[0] = -delta[1] * sway->scale[0];
        push[1] = delta[0] * sway->scale[1];
        push[2] = -sway->scale[2];
        if (sway->timer == 0) {
            for (kick_axis = 0; kick_axis < 3; kick_axis++)
                sway->velocity[kick_axis] += push[kick_axis] * sway->kick[kick_axis];
            sway->timer = 10;
        }
    } else {
        func_00282948(&sway->timer);
    }
    for (i = 0; i < 3; i++) {
        sway->velocity[i] += sway->spring[i] * (push[i] - sway->position[i]);
        sway->velocity[i] -= sway->damping[i] * sway->velocity[i];
        sway->position[i] += sway->velocity[i];
        sway->phase[i] = func_00283B98(sway->phase[i], sway->speed[i]);
        delta[i] = sway->amplitude[i] * func_00283198(sway->phase[i]);
    }
    *(Vec128_2B44C0 *)work = 0;
    work[0] = sway->position[0] + delta[0];
    work[1] = sway->position[1] + delta[1];
    *(Vec128_2B44C0 *)angles = *(Vec128_2B44C0 *)work;
    if (angles[0] > sway->limit[0])
        angles[0] = sway->limit[0];
    else if (angles[0] < -sway->limit[0])
        angles[0] = -sway->limit[0];
    if (angles[1] > sway->limit[1])
        angles[1] = sway->limit[1];
    else if (angles[1] < -sway->limit[1])
        angles[1] = -sway->limit[1];
    func_002833F0(matrix, angles);
    func_002833F0(self + 0xC0, &sway->rotation);
    func_00283738(self + 0xC0, self + 0xC0, matrix);
    qcopy(work, &sway->offset);
    work[2] += delta[2];
    func_00283098(moved, work, self + 0xC0);
    func_00282CC0(moved, moved, self + 0x10);
    func_00282CF0(shift, &sway->anchor, moved);
    func_00282CC0(self + 0x10, self + 0x10, shift);
    roll = -sway->position[2];
    if (roll > sway->limit[2])
        roll = sway->limit[2];
    else if (roll < -sway->limit[2])
        roll = -sway->limit[2];
    func_002B0158(self, self + 0x10, self + 0x10, roll);
    body = func_002AD0B0(self);
    if (body != 0) {
        func_00282CF0(step, self + 0x10, old_position);
        func_002AD6F0(body, step, old_rotation, self + 0xF0);
    }
}
