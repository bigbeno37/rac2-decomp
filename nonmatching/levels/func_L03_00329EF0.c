/* Claude Opus 5.5 (opus-1, task v2310), 3 November 2026. Pinned GNU game compiler.
 * Best US check: 1200/1200 bytes, 0/2 placements, 71 of 300 cdiff rows differ,
 * -O2 -G0 -ffunction-sections -g (own unit L03_002AB5C8-region.c). Frame 0x1d0
 * now matches retail with no spill: `int none = 255;` in the second link scan
 * keeps 255 out of loop.c's movables (a multi-block, maybe_never register), so
 * s5 and the sp+0x120 spill from the sol-high draft are gone.
 * First link scan: retail's has a .p2align nop at 0x32A194, so it is a real C loop
 * whose exit test jump.c did not duplicate (not peeled). The goto spelling below
 * gives retail's bnel back edge but no loop note. for+break, for(;;) with two
 * breaks, and an empty-body for followed by `if (k < 4)` are all peeled (213-218
 * rows). Next idea: an exit test containing a CODE_LABEL (e.g. an || condition)
 * or over 30 RTL insns, so duplicate_loop_exit_test refuses.
 * Other differences: flood-loop head order (retail i=0, nop, changed=0, blez,
 * nextPass; back edge bnel with i=0 in the slot; ours bnez); swapped -1 compare
 * registers; link init hoists fp+4 then +3 (ours folds to +7, natural-index forms
 * give 76 rows); second scan k/edge registers and links/edges store order swapped;
 * center section i/geometry/point permuted across s1-s3.
 * Initialised declarations: pass, count, none, edge (review if matched).
 * Declarations below are complete. Preserve source-line layout with -g.
 */
#include "common.h"
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) RegionPoint;
typedef struct { int id; unsigned char links[4]; unsigned char edges[4]; int reserved; RegionPoint sphere; } RegionRecord;
typedef struct { int count; int reserved[3]; RegionPoint points[1]; } RegionGeometry;
extern float (*D_L03_001FE700[])[4];
extern void func_00282C88(void *);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282A88(void *, int);
extern int func_L02_00349BD8(int, int, int *, int *);
extern void func_00282D30(void *, void *, float);
extern float func_00282E80(void *, void *);

int func_L03_00329EF0(void *unused, int *ids, RegionRecord *records, int capacity) {
    unsigned char depth[256];
    RegionPoint center;
    unsigned int visited;
    int pass = 1;
    int count = 0;
    int changed, edgeA, edgeB;
    int sourceEdge, targetEdge;
    RegionGeometry *geometry;
    RegionPoint *point;
    float distance;
    func_00282A88(depth, 0x100);
    depth[0] = pass;
    visited = (unsigned int)depth;
    { int i, j;
    do {
        int nextPass;
        changed = 0;
        nextPass = pass + 1;
        for (i = 0; i < capacity && count < capacity && -1 != ids[i]; i++) {
            if (*(unsigned char *)(visited + i) == pass) {
                records[count++].id = ids[i];
                for (j = 0; j < capacity; j++) {
                    if (i == j || *(unsigned char *)(visited + j))
                        continue;
                    if (ids[j] == -1)
                        break;
                    if (func_L02_00349BD8(ids[i], ids[j], &sourceEdge, &targetEdge)) {
                        changed = 1;
                        *(unsigned char *)(visited + j) = nextPass;
                    }
                }
            }
        }
        pass = nextPass;
        if (!changed) break;
    } while (pass < 0x20);
    }
    { int i, j;
    for (i = 0; i < count; i++) {
        unsigned char *link = &records[i].links[3];
        for (j = 3; j >= 0; j--)
            *link-- = 255;
    }
    }
    { int i, j, k;
    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (func_L02_00349BD8(records[i].id, records[j].id, &edgeA, &edgeB)) {
                ((RegionGeometry *)D_L03_001FE700[records[i].id])->points[edgeA].w = 0;
                ((RegionGeometry *)D_L03_001FE700[records[j].id])->points[edgeB].w = 0;
                k = 0;
                goto find_source_link;
            next_source_link:
                k++;
            find_source_link:
                if (k >= 4) goto source_link_done;
                if (records[i].links[k] != 255) goto next_source_link;
                records[i].links[k] = j;
                records[i].edges[k] = edgeA;
            source_link_done:
                { unsigned char edge = edgeB;
                int none = 255;
                for (k = 0; k < 4; k++) {
                    if (records[j].links[k] == none) {
                        records[j].edges[k] = edge;
                        records[j].links[k] = i;
                        break;
                    }
                }}
            }
        }
    }
    }
    { int i, j;
    for (i = 0; i < count; i++) {
        geometry = (RegionGeometry *)D_L03_001FE700[records[i].id];
        func_00282C88(&center);
        for (j = 0; j < geometry->count; j++)
            func_00282CC0(&center, &center, &geometry->points[j]);
        func_00282D30(&center, &center, 1.0f / (float)geometry->count);
        center.w = 0;
        for (j = 0; j < geometry->count; j++) {
            distance = func_00282E80(&center, &geometry->points[j]);
            if (distance > center.w)
                center.w = distance;
        }
        qcopy(&records[i].sphere, &center);
    }
    }
    return count;
}
