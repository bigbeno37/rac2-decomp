/* Claude Opus 5.5 xhigh (opus-2), task v2435, 4 November 2026.
 * USA v1.01: 1152/1164 bytes, 0/8 places, 166/291 cdiff rows differ (opus-1's
 * draft: 1160 bytes, 187/291); default -O2 -G0 -ffunction-sections, in the
 * original unit L02_002B5F50.c.
 * Changes from opus-1:
 * - 0xFF neighbour init written count-up (slot = 0..3): loop.c reverses it and
 *   gives retail's descending pointer giv; only the giv start order differs
 *   (ours (n*32+3)+base, retail (n*32+base)+3).
 * - Slot search A: `unsigned char *neighbor = &regions[nodeA].neighbors[slotA];`
 *   inside the while/continue form. The break is then within expand_end_loop's
 *   30-insn scan and is the loop's last insn, so A is not rotated (top-tested,
 *   row base and 255 inside, as in retail). Still different: our compare
 *   branches `beq` to the found block with `b top; slot++` on the fallthrough;
 *   retail branches `bnel top; slot++` and the found block falls through. That is
 *   the layout of a loop rotated up to the break (`goto start; newstart: slot++;
 *   start: test; compare -> newstart; stores; break`) whose exit-test copy jump.c
 *   refused (duplicate_loop_exit_test refuses a CODE_LABEL, call, LOOP_CONT note
 *   or more than 30 insns); the label before TOP is aligned, so a LOOP_BEG precedes it.
 * - Measured but not kept: `unsigned char *mark = visited;` (declared after
 *   depth, used in both search loops, `mark[start] = depth`) gives retail's
 *   visited-base spill at 0x12C and the first section's loads exactly, but the
 *   nodeB-start copy (retail $fp) then spills too, to 0x130, making the frame
 *   0x1A0 (241 rows). Needs: the pair loop's register pressure reduced (retail
 *   does not hoist either 255 out of the nodeB loop; ours puts B's 255 in $s5).
 *   Tried: if/else A (same bytes as kept form), for+continue A (worse, 264).
 * Declarations needed: the externs and L02RegionLink below; the unit already
 * declares func_00282E80(void *, void *), func_00282C88, func_00282CC0,
 * func_00282E48 and TI128, so drop those copies when inserting.
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
        for (slot = 0; slot < 4; ++slot) regions[node].neighbors[slot] = 0xFF;
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
                    unsigned char *neighbor = &regions[nodeA].neighbors[slotA];
                    if (*neighbor != 0xFF) {
                        ++slotA;
                        continue;
                    }
                    *neighbor = nodeB;
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
