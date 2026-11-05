/* opus-3 xhigh (third and last Opus attempt), USA v1.01: FAILED under the source rules.
 * Best landable C (the body below, unchanged): 416 bytes / 412, 0 of 1 places, 96 of 104 rows.
 * Exact bytes exist (USA 1 of 1, v2.00 1 of 1, not in the PAL catalogue), but only with the form
 * at the end of this comment, which invents its test and is not landable.
 *
 * Measured cause, which replaces the priority estimate in the opus-1 note further down:
 * - Base and stride must both be global (read from local-alloc.c, then confirmed by the exact
 *   form). They cross calls and obj is live across those calls, so a local pseudo would take
 *   s0 in local-alloc before obj can. A global base also stops
 *   the tie to the %hi temp, which is why retail has `lui v0` then `addiu s1, v0`.
 * - Going global is not enough. Each constant carries REG_EQUIV, so update_equiv_regs doubles
 *   its REG_LIVE_LENGTH (local-alloc.c, `REG_LIVE_LENGTH (regno) *= 2`), and global-alloc sees
 *   base 46 and stride 54 against p 36: p ranks first and takes s1. This is the 34-row result of
 *   a block split alone (412/412, obj s0, p s1, base s2, stride s3).
 * - The order that reproduces retail is obj > base > stride > p. The only way found to skip the
 *   doubling from this source is an earlier insn in the chain calling no_equiv on the pseudo: a
 *   non-constant first set with REG_N_SETS == 2, which also gives two deaths, so the pseudo is
 *   global without any split. (A dest that is not a REG also skips it; no use for it was found.)
 *   CSE adds REG_EQUAL to every plain constant set, and a constant first set is doubled before
 *   a later mismatch cancels the equivalence, so the first set must be non-constant.
 *   Tried without effect on the allocation of this body, at -G0 and -G8: -g, -ffast-math,
 *   -fno-builtin, -frac2-keep-block-order, -frac2-gcse-unsplit-edges, -frac2-qcopy(-barrier),
 *   -frac2-hazard-labels, -fno-gcse, -fno-regmove, -fno-rerun-cse-after-loop,
 *   -fno-rerun-loop-opt, -fno-strength-reduce, -fno-expensive-optimizations (this one does
 *   change the size). Not tried: -mno-split-addresses, -frac2-zero-ti-por, -malign-loops=2,
 *   -G16, -G32. Also no help: a `||` or `&&` test (separate pseudos, 428-444 bytes) and
 *   explicit base/stride locals (still doubled).
 * - That first set has to disappear after allocation. With an arm whose only set is flow1-dead,
 *   the branch, the test and both loads are absent from the output (flow2 removes them,
 *   according to the v1989 measurement; not traced here). A plain `t = 1` arm is no use: its
 *   test is already gone after jump1, so no block remains. `t = obj[0x24]` survives.
 * - By the priority arithmetic it must not read p: an eighth reference gives p 24/37, above obj.
 *
 * Exact-bytes form, with `u32 t;` at function scope and `t = obj[0x23]; if (t < 2) ... else
 * obj[0x23] = t - 2;` in case 2. Declaring base before stride matters: the other order gives
 * base 14/23 over obj 45/74, and base takes s0.
 *
 *     case 0: {
 *         char *base = (char *)(int)obj[0x22];
 *         int stride = obj[0x23];
 *     #define R2 (base + *(s16 *)(p + 0x36) * stride)
 *         if (stride == (int)base) t = obj[0x24];
 *         base = D_L03_00259A30;
 *         stride = 0x150;
 *         func_L00_00322A98(obj, R2);
 *         func_L00_00322AC0(R2, 0x28, 2, 0.0f);
 *         func_L00_00322AC0(R2, 0x14, 4, 0.0f);
 *         func_L00_00322AC0(R2, 0xA, 6, 0.0f);
 *         func_002A7750(obj, 1, -1);
 *         break;
 *     }
 *
 * Why it does not land: nothing in retail shows those three reads or that comparison. The test
 * is a placeholder condition whose only effect is on allocation, and loading a byte into `base`
 * is a fake dependency. The form shows what the allocator needs, not what the source said.
 * What would unblock it: a real statement at the head of case 0 that gives base and stride a
 * non-constant first set and vanishes by flow2, or a maintainer ruling on this form.
 * The body below stays the behavioural C for the native port.
 */
/* 4 November 2026 current-RAC1 source-method trial; retained, not linked.
 * Two new real-operation group trials (the first append call and the state-pointer read) each
 * produce 420/412 bytes, 0/1 USA placements. Neither promotes table/stride allocation as
 * required; the older best C below is retained.
 */
/* opus-1 xhigh, USA v1.01: 416 bytes / 412 retail, 0 of 1 places match.
 * (The earlier form of the alpha test, >= 2 first, gives 412/412 with the wrong
 * final layout; the arm order below reproduces retail's layout.)
 * The remaining rows appear to follow from one allocation difference: retail has obj s0,
 * table base s1, stride s2, p s3; here base s0, stride s1, obj s2, p s3.
 * Base and stride are single-block, single-death pseudos in case 0, so local-alloc
 * takes them first and ties the %hi temp to the base. Retail's `lui v0` then
 * `addiu s1, v0` shows both were global pseudos there. The extra 4 bytes and both
 * dispatch delay slots come from this: `ld s0` can fill them only while s0 is dead
 * in case 0; with obj in s0 reorg must steal `lh a1` into a beql.
 * Priorities (floor_log2(refs) * refs / length) would need obj > base > stride > p:
 * obj 13 refs, length 71 = .549; base 5, 21 = .476; p 7, 34 = .412; stride 5, 25 = .400
 * (base and stride lengths are local-alloc qty lengths halved, an estimate).
 * So stride also needs length <= 24, or p length >= 36, and any added ref of p
 * (8 refs) lifts p above obj. A test between the AC0 calls does make both global
 * (obj s0, lui v0) but then gave p s1, base s2, stride s3 and leaves its own words.
 * The split probably lies after the first AC0 call, since mtc1 f20 sits above the A98
 * call in the same block. No allowed construct found that splits it and leaves no code.
 * No change from: -G8 unit, explicit base/stride locals, typed record array.
 * Declarations needed are below; compiled in the plain .c unit, not linked.
 */
#include "include_asm.h"
#include "common.h"

extern char D_L03_00259A30[];
extern void func_L00_00322A98(u8 *, char *);
extern void func_L00_00322AC0(char *, s32, s32, float);
extern void func_L00_00322BE0(char *, s32);
extern void func_L00_00322EC8(char *);
extern void func_002A7750(u8 *, s32, s32);
extern s32 func_00273B70(u8 *);
extern void func_0029F338(void *);

#define REC(o) (&D_L03_00259A30[*(s16 *)(p + 0x36) * 0x150])

void func_L03_00421930(u8 *obj)
{
    u8 *p = *(u8 **)(obj + 0x68);

    switch (obj[0x20]) {
    case 0:
        func_L00_00322A98(obj, REC(obj));
        func_L00_00322AC0(REC(obj), 0x28, 2, 0.0f);
        func_L00_00322AC0(REC(obj), 0x14, 4, 0.0f);
        func_L00_00322AC0(REC(obj), 0xA, 6, 0.0f);
        func_002A7750(obj, 1, -1);
        break;
    case 1:
        func_L00_00322BE0(REC(obj), 0);
        if (func_00273B70(obj) != 0) {
            func_002A7750(obj, 2, -1);
            *(u16 *)(obj + 0x34) |= 0x2800;
        }
        break;
    case 2:
        func_00273B70(obj);
        func_L00_00322EC8(REC(obj));
        if (obj[0x23] < 2) {
            func_0029F338(obj);
        } else {
            obj[0x23] -= 2;
        }
        break;
    }
}
