/* Claude Opus 5.5 (xhigh), 2026-11-05, task v2465: 1004/1004 bytes, 0/1 USA v1.01 places,
 * 8 differing instructions (was 16), all one $s7/$fp swap. All stack slots now match.
 * Profile (own unit, e.g. L02_003EB258-g8.c):
 * -O2 -G8 -ffunction-sections -frac2-qcopy -frac2-qcopy-barrier -mno-split-addresses -g
 * (-mno-split-addresses IS needed here: without it the D_L02_001B0DAC/DB0/DB4 lui+lw pairs split;
 *  -g only places two nops around the div.s).
 * New in v2465:
 * - `float (*uv)[2] = packet[0].uv;` (top-level local, used for the U store): sp+0x50 is held by a
 *   user pseudo instead of the last-bucket gcse PRE reaching reg, so it no longer gets the last of
 *   the four entry slots. The alternative, PRE table size 111 (max_cuid 220..223, now 206/207),
 *   would need 14..17 more insns at gcse that disappear later; none found.
 * - `mask = 0x00FFFFFF;` in a top-level local assigned just before the vertex loop: keeps the
 *   spilled constant's slot (0x194) ahead of uv's (0x198), as retail has. Assigning it inside the
 *   fade block or before the blend code gives the same bytes; initializing it at the declaration
 *   is far worse (set moves above the layer loop).
 * - FastTweenColor uses the canonical (int, int, float) prototype (same bytes as float-first).
 * Remaining: V-source cursor (la D_L02_001B0D6C) gets $fp and destination-U cursor (copy of uv)
 * gets $s7; retail has the reverse. Priorities: U-src 27/196 (s6) > U-dest 14/102 = 1372 >
 * V-src 27/198 = 1363 (REG_EQUAL doubles it) > spilled V0-dest giv 14/104 = 1346. Only U-dest
 * live length exactly 103 gives retail. In sched1 the U-dest copy (insn 891) is in the big block
 * after the layer loop (it is hoisted above the calls), 3 slots ahead of the V-src set; it needs
 * one more insn between itself and the V-src set, or to issue one slot earlier. Adding an insn
 * after both does not help (V-src goes up by 2).
 * Tried, worse: indexed packet[k].vertex[i][j] stores (1108 bytes); mask initialized at the
 * declaration. No linked build.
 */
#include "common.h"
typedef int Quad __attribute__((mode(TI)));
typedef struct {
    float vertex[4][4];
    unsigned int color[4];
    float uv[4][2];
    unsigned long long state[4];
} SurfacePacket __attribute__((aligned(16)));
extern int D_L02_001B0D60[2] __attribute__((sda));
extern float D_L02_001B0D68[][2] SDATA(D_L02_001B0D68);
extern float D_L02_001B0D6C[][2] __attribute__((sda));
extern unsigned int D_L02_001B0DA8 __attribute__((sda));
extern unsigned int D_L02_001B0DAC[] NOT_SDA;
extern unsigned int D_L02_001B0DB0[] NOT_SDA;
extern unsigned int D_L02_001B0DB4[] NOT_SDA;
extern char *D_L02_001B368C;
extern void func_00281DE8(void *, int, int, int);
extern float func_00283198(float);
extern float func_00283B98(float, float);
extern float func_00282DF0(void *);
extern int func_00283CF0(float);
extern unsigned int func_00283D38(unsigned int, unsigned int, float);
extern void func_00283658(void *, void *);
extern void func_00280B90(void *, void *, int);

void func_L02_003EB258(char *actor) {
    SurfacePacket packet[2];
    typedef struct { float weights[2]; } BlendWeights;
    BlendWeights blend;
    BlendWeights calculated __attribute__((aligned(16)));
    float normal[4] __attribute__((aligned(16)));
    float matrix[16] __attribute__((aligned(16)));
    char *state = *(char **)(actor + 0x68);
    int i, slice, count;
    float step;
    unsigned int mask;
    char *vertex;
    float (*uv)[2] = packet[0].uv;
    {
        int layer;
        for (layer = 0; layer < 2; layer++) func_00281DE8(packet + layer, D_L02_001B0D60[layer], 0, 0x80);
    }
    calculated.weights[0] = func_00283198(*(float *)(state + 0x14)) * 0.5f + 0.5f;
    calculated.weights[1] = func_00283198(func_00283B98(*(float *)(state + 0x14), 3.1415927f)) * 0.5f + 0.5f;
    blend = calculated;
    count = func_00283CF0(func_00282DF0(D_L02_001B368C + *(int *)(state + 8) * 0x80 + 0x10));
    *(Quad *)normal = 0;
    normal[3] = 1.0f;
    step = 2.0f / (float)count;
    mask = 0x00FFFFFF;
    for (i = 0, vertex = (char *)&packet; i < 4; vertex += 16, i++) {
        uv[i][0] = D_L02_001B0D68[i][0];
        packet[0].uv[i][1] = D_L02_001B0D6C[i][0];
        packet[1].uv[i][0] = D_L02_001B0D68[i+4][0] + *(float *)(state + 0x18);
        packet[1].uv[i][1] = D_L02_001B0D6C[i+4][0];
        packet[0].color[i] = func_00283D38(D_L02_001B0DA8, D_L02_001B0DAC[0], blend.weights[0]);
        packet[1].color[i] = func_00283D38(D_L02_001B0DB0[0], D_L02_001B0DB4[0], blend.weights[1]);
        if (*(float *)(state + 0x1C) < 1.0f) {
            packet[0].color[i] = func_00283D38(packet[0].color[i] & mask, packet[0].color[i], *(float *)(state + 0x1C));
            packet[1].color[i] = func_00283D38(packet[1].color[i] & mask, packet[1].color[i], *(float *)(state + 0x1C));
        }
        qcopy(vertex, normal);
        if (i >= 2) *(float *)vertex = -1.0f; else *(float *)vertex = 1.0f;
        *(float *)(vertex + 4) = (i & 1) ? -1.0f : step + -1.0f;
        qcopy(packet[1].vertex[i], normal);
        if (i >= 2) *(float *)(vertex + 0x90) = -1.0f; else *(float *)(vertex + 0x90) = 1.0f;
        *(float *)(vertex + 0x94) = (i & 1) ? -1.0f : step + -1.0f;
    }
    func_00283658(matrix, D_L02_001B368C + *(int *)(state + 8) * 0x80);
    qcopy(matrix + 12, D_L02_001B368C + *(int *)(state + 8) * 0x80 + 0x30);
    for (slice = 0; slice < count; slice++) {
        func_00280B90(packet, matrix, 0);
        func_00280B90(packet + 1, matrix, 0);
        {
        int j;
        char *sliceVertex = (char *)&packet;
        for (j = 0; j < 4; j++, sliceVertex += 16) {
            *(float *)(sliceVertex + 4) += step;
            *(float *)(sliceVertex + 0x94) += step;
        }
        }
    }
}
