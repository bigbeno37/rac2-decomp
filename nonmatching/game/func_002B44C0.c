/* Claude Opus 5.5 xhigh (opus-1), 3 November 2026, task v2270; retained near miss.
 * Best current-toolchain check: 1252/1248 bytes, 0/28 USA v1.01 places,
 * 145 of 313 cdiff rows differ. Profile: -O2 -G0 -ffunction-sections, isolated
 * source unit. No linked match. Required types and declarations are included
 * below; common.h supplies qcopy and qcopy_nc.
 *
 * Fixed in this version: the first snapshot uses qcopy_nc. The "memory"
 * clobber of qcopy made reload_cse forget that the spill slot at sp+0xC0
 * equals a0, so self was reloaded; with qcopy_nc self stays in a0 and the
 * prologue gives retail's daddu v0,a0,zero.
 *
 * Earlier work kept from the GPT-6.1 Sol-high draft (task v0788): approved
 * qcopy for the second snapshot and the offset-to-work transfer, an ordinary
 * TI work-to-angles assignment, a separate kick-loop counter (recovers push
 * in s4, the main-loop counter spill at sp+0xC4 and the loop's saved
 * registers), and explicit target/player call blocks.
 *
 * Remaining differences:
 * 1. Branch layout. Retail: bnez target -> T; state tests branch to decay
 *    (the second one with addiu a1,a1,0x80 in its delay slot); b P; T call;
 *    b active+4; P call; active; decay sits just before the update loop.
 *    This goto form inverts the player-state test (it branches to the player
 *    call). A structured if/else form gets the test direction right, but
 *    flow.c cygnus_merge_blocks (CYGNUS LOCAL, runs after jump1 and cse1)
 *    moves the single-predecessor P block before T, and jump2 then cross-jumps
 *    the two calls (1236 bytes). Retail probably needs P to have two
 *    predecessors at cse1 time, or the inner target test folded only in cse2.
 * 2. Spill slots 0xC8..0xEC are permuted. gcse PRE hashes
 *    (plus (reg R) (const_int X)) as 14029 + R + X, bucket = raw mod
 *    ((max_cuid/2)|1); reaching registers are numbered in bucket order and
 *    slots are given in that pseudo order (soft frame pointer = reg 1 with X
 *    = final sp offset). This compile has 119 buckets, self = reg 84,
 *    sway = reg 85. With those registers no table size gives retail's order.
 *    Retail's order is reachable with 111 buckets (max_cuid about 222) and a
 *    sway base pseudo numbered 114..128, i.e. a later copy of sway such as
 *    an inline helper parameter, plus either a self base = 98 (mod 111) or
 *    self + 0xC0 held in a non-PRE early pseudo.
 * 3. The delta-pointer update reloads its spill (0xEC) after the store at
 *    0x002B47C4, adding one word; explicit pointer induction does not fix it.
 *
 * Measured alternatives (this toolchain): structured if/else with shared
 * active block: 1236 bytes, P first, calls merged. Structured form with
 * distinct call arguments: P first, no merge. Goto form with decay placed
 * first: 1252 bytes, T moved up, test direction still wrong. Folding the
 * player address into a temporary (A) and a delta-pointer variant (Dv) were
 * worse. Sol-high notes: reversed inner target test reaches 1248 bytes only
 * with a shared counter and wrong allocation; -fno-thread-jumps 1268 bytes;
 * -fno-gcse 948 bytes; do not trust the -fstrict-aliasing 1248 result (TI
 * casts over float arrays propagate the earlier zero into the angle copy).
 * Reopen with source evidence for how sway is reached (a later pseudo) and
 * for the original target/player conditional.
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
    if (target != 0)
        goto target_delta;
    if (D_00189E20.target == self && D_00189E20.state >= 2)
        goto player_delta;
    goto decay;
target_delta:
    func_00282CF0(delta, target, self + 0x10);
    goto active;
player_delta:
    func_00282CF0(delta, &D_00189E20.position, self + 0x10);
active:
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
    goto update;
decay:
    func_00282948(&sway->timer);
update:
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
    if (sway->limit[0] < angles[0])
        angles[0] = sway->limit[0];
    else if (angles[0] < -sway->limit[0])
        angles[0] = -sway->limit[0];
    if (sway->limit[1] < angles[1])
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
    if (sway->limit[2] < roll)
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
