/*
 * Opus 5.5, v2341, 2026-11-04: retained near miss, no linked credit.
 * Profile: -O2 -G8 -ffunction-sections (default for a -g8.c unit; tried as
 * src/levels/shared/L18_002C4000-groups-g8.c because L18_002C4000-g8.c declares
 * D_L18_001FB320 and D_L18_001B281C with other types).
 * US: 1596/1596 bytes, 0/3 places; first mismatch at +0x38; 88 diff rows,
 * 41 of them non-address (same distance as the v1181 draft).
 * Change from v1181: the do { } while (0) wrapper is gone. Each walk is a
 * plain block with `goto test; for (;;) { ... test: if (!cond) goto end; }`.
 * The for(;;) still emits NOTE_INSN_LOOP_BEG, so the body label keeps its
 * 8-byte alignment. loop.c sees a jump from outside into the loop, so the
 * loop is invalid and nothing is hoisted (retail keeps lui 0x1B, 0xB69 and
 * 0xFF inside). expand_end_loop does not roll anything, because the exits go
 * to user labels.
 * Remaining differences, all in the walk inits:
 * - The table-high PRE copy (daddu t3,v1) issues next to the lo_sum at t=2
 *   in sched1 and sched2. lw t0 is then the last insn before `b` and reorg
 *   puts it in the delay slot. Retail has every copy last, in the slot.
 * - In chained walks the group-id copy issues at t=3 and overlaps the lo_sum
 *   pseudo, so it gets a1. Retail's copy comes after the addu and reuses a0.
 * Both copies are scheduled last in retail, in sched1 as well as sched2.
 * That suggests a barrier between the init's lookup and the copies: a loop
 * note attached to the copy, or the copies being PRE end-of-block insertions
 * behind such a note. Plain PRE copies sit right after their computing insn.
 * Tried and worse: an ordinary while (1876, test duplicated or constants
 * hoisted); a do-while with goto entry (1624, the member test is rolled); a
 * plain block without the for (1576, no LOOP_BEG alignment).
 * Declarations required by this standalone draft follow below.
 */
#include "common.h"
extern unsigned short *D_L18_001FB320[];
extern char *D_L18_001B281C[] __attribute__((sda));
extern int D_L18_001B2838 __attribute__((sda));

#define GROUP_ID(field) (*(int *)(state + (field)))

/* Each group is a terminated stream of 15-bit actor indices. */
#define DISABLE_GROUP(field) { \
    unsigned short *entry = D_L18_001FB320[GROUP_ID(field)]; \
    goto test_##field; \
    for (;;) { \
        char *member = D_L18_001B281C[0] + (*entry & 0x7fff) * 0x100; \
        unsigned short flags; \
        if (member == 0) goto end_##field; \
        flags = *(unsigned short *)(member + 0x34) & 0xffbc; \
        *(unsigned short *)(member + 0x34) = flags | 0x1000; \
        *(int *)(member + 0x98) = *(int *)(*(char **)(member + 0x24) + 0x10); \
        if (*(short *)(owner + 0xaa) != 0xb69) { \
            member[0x31] = 0; \
            *(unsigned short *)(member + 0x34) = flags | 0x1001; \
        } \
        entry++; \
    test_##field: \
        if (!((GROUP_ID(field) & 0xff) != 0xff && GROUP_ID(field) <= D_L18_001B2838 && entry != 0 && \
              ((short)entry[-1] >= 0 || entry == D_L18_001FB320[GROUP_ID(field)]))) goto end_##field; \
    } \
end_##field:; \
}

void func_L18_003FA180(char *owner, unsigned int mode)
{
    char *state = *(char **)(owner + 0x68);
    if (mode >= 8) return;
    switch (mode) {
    case 0:
        DISABLE_GROUP(0x60)
        DISABLE_GROUP(0x64)
        break;
    case 1:
        DISABLE_GROUP(0x68)
        break;
    case 2:
        DISABLE_GROUP(0x6c)
        DISABLE_GROUP(0x74)
        DISABLE_GROUP(0x78)
        break;
    case 3:
        DISABLE_GROUP(0x70)
        break;
    case 4:
        DISABLE_GROUP(0x7c)
        break;
    case 5: case 6: case 7:
        return;
    }
}
