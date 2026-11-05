/* Claude Opus 5.5 (xhigh), 2026-11-05. EXACT BYTES, NOT LANDED: needs a ruling on one statement.
 * The code below, default -O2 -G0 -ffunction-sections in a unit of its own: 292/292 bytes,
 * 27/27 places in each of us, pal and us2.
 * The statement in question is `if (data) vertical = *(float *)(data+0x38);`. Reported FAILED
 * because it does nothing at run time and is there only for its block boundary: retail has no
 * test of data and dereferences it unchecked, so the condition is a placeholder under WORKER2's
 * "forbidden for now" list. It is not the recovered source. Nothing else in the body is invented.
 * How it works: flow1 deletes the dead store, the arm stays empty with its label, so sched1 still
 * sees a block end after the data load; that puts the load before $a0 = position and object keeps
 * $a0. No jump pass runs between flow1 and flow2. flow2's find_basic_blocks merges the blocks and
 * deletes the jump (merge_blocks_nomove), so sched2 (which runs before jump2) sees one block.
 * Measured variants: `if (data) collided = *(int *)data;`, and position assigned after the test,
 * are exact too. `if (data) collided = 1;` is gone by sched1 (296 bytes), probably jump's
 * store-flag conversion, so the arm needs a load or something else that stays a branch.
 * Why a boundary is likely real (inference, not proof): -fno-schedule-insns gives the retail entry
 * but 26 of 73 rows differ, including argument order at both func_00283028 calls, so retail ran
 * sched1; a one-pass group gets the body right but its loop notes bind sched2 too, and retail's
 * `a1 = 0` sits third in the entry, before any such note could fall.
 * Sched1 at the entry, without the test: 25 ($f12) and 27 ($a2 load) take the one memory unit at
 * t=3 and t=4, data load 11 waits for t=5 (priority 3), $a0 = position issues at t=3. 25 beating
 * position 14 at t=2 would also fix it, but 14 has the lower LUID and an equal or lower weight.
 * Tried, no help: inline accessor for object->data (296/292, 0/27). Reasoned, not compiled: a
 * surviving `v = data` copy would give the load a true consumer, but cse/cprop rewrite its uses and
 * flow1 deletes it, or combine folds it into the load.
 * To land: new unit src/levels/shared/L00_003B39D0.c (default flags, no cflags line), because this
 * D_00189E20 type conflicts with Hero in L00_003B3158.c, and remove the marker from that file.
 * Reopen with: a ruling on this form, or a real statement with the same effect.
 */
/* 4 November 2026 current-RAC1 source-method trial; retained, not linked.
 * Grouping the actual data read or actual collision-result call gives 292/292 bytes, 0/27 USA
 * placements, first +0x4, with the default G0 profile. The entry schedule and subsequent
 * allocation remain different; no empty barrier or invented access was used.
 */
/* Claude Opus 5.5 (xhigh), 2026-10-31: code below unchanged, remeasured.
 * -fno-schedule-insns: 292/292 bytes, 0/27 places, 11 register/order lines in the arms.
 * Default -O2 -G0 -ffunction-sections: 296/292, 0/27; only the entry block differs, and it
 * causes the extra word. So sched1 was on in retail (the arms need it); the entry is the blocker.
 * sched1 priorities there (-fsched-verbose-5): position 4, f12 4, a2 load 4, data load 3,
 * a0 = position 3. f12 and both loads share the one memory unit, so a0 = position issues before
 * the data load, the object pseudo overlaps $a0 and takes $v1. Retail needs the load first, so
 * the load needs priority >= 4 or a boundary. Ties: float local for 0.25f, true prototype
 * (void *, int, void *, float), -g, -frac2-qcopy, -fno-gcse, -fno-expensive-optimizations,
 * -fno-strength-reduce, -fno-rerun-loop-opt, -fno-regmove, -fno-rerun-cse-after-loop.
 * Diagnostic only, forbidden wrapper: do { } while (0) just before the call gives 292/292 on the
 * default profile with the whole body right, but the note also binds sched2, so the prologue
 * order still differs: not exact. `if (hero)` keeps a real test.
 * Reopen with: an independent reason for the data load to precede a0 = position.
 */
/* GPT-6.1 Sol (high), 2026-10-29. Retained near miss; not linked.
 * Profile: -O2 -G0 -ffunction-sections -fno-schedule-insns.
 * USA v1.01: 292/292 bytes, 0/27 places. The first scheduling pass caused
 * the extra object-pointer copy in the older 296-byte draft. Disabling it
 * reproduces the complete retail prologue and first vector operations.
 * Remaining: projection/remaining use s0/s1 rather than s1/s0, and both
 * func_00283028 calls put the third argument in the delay slot instead of
 * retail's first/second argument. Reusing the position variable regresses
 * allocation; aggregate temporary storage ties. FP-first collision arguments,
 * -fno-force-mem, -fno-sched-spec and -fno-sched-interblock retain 296 bytes.
 * The declarations below are complete; the source needs the profile above.
 */
#include "common.h"

typedef int Quad __attribute__((mode(TI)));
typedef struct { char pad00[0xb0]; float origin[4]; char padc0[0x21d0]; int mask; } CollisionState;
extern CollisionState D_00189E20;
extern Quad D_L00_001BF230[2];
extern int D_L00_001A8FF0[] __attribute__((sda));
extern int func_00277258(void *,int,float,void *);
extern void func_00282CF0(float *,void *,void *);
extern void func_00282F20(float *,float *,float);
extern float func_00282DB0(float *,float *);
extern void func_00282D30(float *,float *,float);
extern void func_00283028(void *,void *,void *);
extern void func_00282CC0(void *,void *,void *);

typedef struct { char pad[0x10]; Quad position; char pad20[0x48]; char *data; } CollisionObject;

void func_L00_003B39D0(CollisionObject *object)
{
    float normal[4],projection[4],remaining[4];
    char *data=object->data;
    void *position=&object->position;
    int collided;
    float vertical;
    if (data) vertical = *(float *)(data+0x38);
    collided = func_00277258(position,0,0.25f,(void *)D_00189E20.mask);
    if (collided) {
        qcopy(position,D_L00_001BF230);
        if(D_L00_001A8FF0[0]) {
            float dot;
            data+=0x30;
            func_00282CF0(normal,D_00189E20.origin,position);
            func_00282F20(normal,normal,1.0f);
            dot=func_00282DB0(normal,(float *)data);
            func_00282D30(projection,normal,dot);
            func_00282CF0(remaining,data,projection);
            func_00283028(data,remaining,&D_L00_001BF230[1]);
            func_00282CC0(data,data,projection);
        } else {
            vertical=*(float *)(data+0x38);
            *(float *)(data+0x38)=0;
            func_00283028(data+0x30,data+0x30,&D_L00_001BF230[1]);
            *(float *)(data+0x38)=vertical;
        }
    }
}
