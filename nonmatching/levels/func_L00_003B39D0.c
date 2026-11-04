/* Claude Opus 5.5 (xhigh), 2026-11-04 (v2417): FAILED, tie. Default -O2 -G0 -ffunction-sections:
 * 296/292 bytes, 0/27 places, only the entry block differs (object copied to $v1). With
 * -fno-schedule-insns in a unit of its own: 292/292, 0/27, 26 of 73 rows (s0/s1 swap of
 * projection/remaining, argument order of both func_00283028 calls). Those arms come from sched1's
 * register-weight tie-break (a1 = dying remaining first), so retail ran sched1.
 * Why the entry is fixed: the issue rate is 2 and the f12 constant (25), the hero-moby load (27)
 * and the data load (14) all need the one memory unit. 25/27 have priority 4; the data load only 3,
 * because its only link is the anti-dependence to the call (cost 1). At t=2, `pos = object+16`
 * (pri 4, 2 dependents) always beats 25, so `$a0 = pos` (21) is ready at t=3 and issues before the
 * data load (t=5). The load needs priority >= 4 (an in-block true consumer), or a block boundary
 * after `pos` that is gone before sched2. Pass order (toplev.c): cross-jumping runs after sched2,
 * so identical arms bind sched2 as well; the only sched1-only boundary is an arm reload turns into
 * a no-op move, whose jump flow2 then removes. Measured ties: true prototype (void *, int,
 * void *, float) with hero moby as arg 3 (used below), either declaration order, an inline getter
 * for object->data (copy folded before sched1); `if (data == 0) data = 0` and identical
 * position arms fold before sched1 (diagnostics only, not allowed).
 * Needs: a cast view of D_00189E20 (L00_003B3158.c declares it as Hero) or a unit of its own.
 * Reopen with: the real statement that ends a block after the data load in sched1.
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
typedef struct { char pad00[0xb0]; float origin[4]; char padc0[0x21d0]; void *moby; } CollisionState;
extern CollisionState D_00189E20;
extern Quad D_L00_001BF230[2];
extern int D_L00_001A8FF0[] __attribute__((sda));
extern int func_00277258(void *,int,void *,float);
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
    if(func_00277258(position,0,D_00189E20.moby,0.25f)) {
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
            float vertical=*(float *)(data+0x38);
            *(float *)(data+0x38)=0;
            func_00283028(data+0x30,data+0x30,&D_L00_001BF230[1]);
            *(float *)(data+0x38)=vertical;
        }
    }
}
