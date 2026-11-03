/* Claude Opus 5.5 xhigh (opus-1), task v2317, 3 November 2026.
 * USA v1.01: 1160/1164 bytes, 0/8 placements, 187/291 cdiff rows differ;
 * default -O2 -G0 -ffunction-sections, reproduced in the original unit (the
 * earlier v0862 draft recompiles here to 1176 bytes, 218/294 rows differ).
 * Change from v0862: neighbor-slot search A written as while/continue. The
 * cause is the LOOP_TEST_THRESHOLD (30) logic. stmt.c expand_end_loop rolls
 * the exit test only up to the break when the break is insn <= 30;
 * jump.c duplicate_loop_exit_test peels when the exit code is <= 30 insns
 * and has no CODE_LABEL. Both for-form slot loops are 29 insns, so both
 * were peeled. Retail peels only B; A is a real loop entered in the middle:
 * aligned top-test label, 255 and the row base not hoisted, bnel back with
 * slot++ in the delay slot. The while/continue form keeps a found-block
 * label, but its break is insn 31, so only the test rolls and A is
 * bottom-tested. A natural A form with the break at <= 30 insns that keeps
 * a label (or otherwise refuses duplication) should give retail A exactly.
 * Also still different (carried over from v0862): visited-base spill at
 * +0x12C, count-increment scheduling, descending neighbor initialization,
 * final vector address materialization, and vertex-loop allocation/order.
 * Declarations needed: the externs and L02RegionLink below; the unit already
 * declares func_00282E80(void *, void *) and TI128, so drop those copies
 * when inserting.
 */

#include "common.h"

typedef int TI128 __attribute__((mode(TI)));
extern void func_00282A88(void *, int);
extern int D_L02_001B36A4 __attribute__((sda));
extern char *D_L02_001FF1C0[];
extern float func_00282E48(void *, void *);
extern float func_00282E80(const void *, const void *);
extern int func_L02_00349BD8(int, int, int *, int *);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282D30(void *, void *, float);

typedef struct L02RegionLink {
    int index;
    unsigned char neighbors[4];
    unsigned char edges[4];
    int unused;
    TI128 center;
} L02RegionLink;

extern void func_00282C88(void *);

int func_L02_0032E508(void *unused, int start, L02RegionLink *regions, int capacity) {
    unsigned char visited[256];
    struct {
        TI128 center;
        int edgeA, edgeB, connectionA, connectionB;
    } scratch;
    int count = 0, depth = 1, changed, i, j;
    char *polygon;
    float distance;
    func_00282A88(visited, 256);
    visited[start] = depth;
    for (;;) {
        changed = 0;
        for (i = 0; i < D_L02_001B36A4; ++i) {
            if (visited[i] == depth && count < capacity) {
                regions[count++].index = i;
                for (j = 0; j < D_L02_001B36A4; ++j) {
                    if (i != j && visited[j] == 0) {
                        char *candidate = D_L02_001FF1C0[j];
                        if (*(int *)candidate <= 64 && !(0.2f < func_00282E48(candidate + 0x10, candidate + (*(int *)candidate << 4)))) {
                            if (func_L02_00349BD8(i, j, &scratch.edgeA, &scratch.edgeB)) {
                                visited[j] = depth + 1;
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
        ++depth;
        if (!changed || depth >= 32)
            break;
    }
    {
        int node, slot;
    for (node = 0; node < count; ++node)
        for (slot = 3; slot >= 0; --slot) regions[node].neighbors[slot] = 0xFF;
    }
    {
        int nodeA, nodeB;
    for (nodeA = 0; nodeA < count; ++nodeA) {
        for (nodeB = nodeA + 1; nodeB < count; ++nodeB) {
            if (func_L02_00349BD8(regions[nodeA].index, regions[nodeB].index, &scratch.connectionA, &scratch.connectionB)) {
                *(int *)(D_L02_001FF1C0[regions[nodeA].index] + (scratch.connectionA << 4) + 0x1C) = 0;
                *(int *)(D_L02_001FF1C0[regions[nodeB].index] + (scratch.connectionB << 4) + 0x1C) = 0;
                {
                    int slotA = 0;
                while (slotA < 4) {
                    if (regions[nodeA].neighbors[slotA] != 0xFF) {
                        ++slotA;
                        continue;
                    }
                    regions[nodeA].neighbors[slotA] = nodeB;
                    regions[nodeA].edges[slotA] = scratch.connectionA;
                    break;
                }
                }
                {
                    int slotB;
                for (slotB = 0; slotB < 4; ++slotB) {
                    if (regions[nodeB].neighbors[slotB] == 0xFF) {
                        regions[nodeB].neighbors[slotB] = nodeA;
                        regions[nodeB].edges[slotB] = scratch.connectionB;
                        break;
                    }
                }                }
            }
        }
    }
    }
    {
        int node;
    for (node = 0; node < count; ++node) {
        polygon = D_L02_001FF1C0[regions[node].index];
        func_00282C88(&scratch.center);
        {
            int sumVertex;
        for (sumVertex = 0; sumVertex < *(int *)polygon; ++sumVertex)
            func_00282CC0(&scratch.center, &scratch.center, polygon + (sumVertex << 4) + 0x10);
        }
        func_00282D30(&scratch.center, &scratch.center, 1.0f / (float)*(int *)polygon);
        ((float *)&scratch.center)[3] = 0.0f;
        {
            int radiusVertex;
        for (radiusVertex = 0; radiusVertex < *(int *)polygon; ++radiusVertex) {
            distance = func_00282E80(&scratch.center, polygon + (radiusVertex << 4) + 0x10);
            if (((float *)&scratch.center)[3] < distance) ((float *)&scratch.center)[3] = distance;
        }
        }
        regions[node].center = scratch.center;
    }
    }
    return count;
}
