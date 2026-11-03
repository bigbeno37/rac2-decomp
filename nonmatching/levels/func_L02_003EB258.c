/* Claude Opus 5.5 (xhigh), 2026-11-03, task v2261: 1004/1004 bytes, 0/1 USA v1.01 places,
 * 16 differing instructions (was 23). Profile (own unit, e.g. L02_003EB258-g8.c):
 * -O2 -G8 -ffunction-sections -frac2-qcopy -frac2-qcopy-barrier -mno-split-addresses -g
 * (-g is needed: without it 996 bytes; -mno-split-addresses makes no difference here).
 * New: `vertex += 16, i++` (pointer before index) fixes the vertex-loop increment schedule.
 * Remaining, both allocation only:
 * 1. Entry spill slots: retail 0x198..0x1A4 = sp+0x50, +0x140, +0x150, +0x180; ours put sp+0x50
 *    last. These are gcse PRE reaching regs (numbered in bucket order); raw hash of (plus fp C) =
 *    14030+C, table 103 buckets here. Only 111 buckets (max_cuid 220..223, about 16 more insns at
 *    gcse than this C) gives retail's order; 105/107 do not, so five declaration inits cannot fix it.
 * 2. $s7/$fp: V-source cursor (la D_L02_001B0D6C) gets $fp, destination-U cursor (sp+0x50) $s7.
 *    The V cursor's init has a REG_EQUAL constant, so update_equiv_regs doubles its live length
 *    (99 -> 198): priority 27/198 = 1363 < U-dest 14/102 = 1372. Retail order needs U-dest live
 *    exactly 103 (1359, still above the spilled V-dest giv 1346), i.e. its sched1 copy one insn
 *    earlier. Moving step/normal/vertex inits, or U/V statement order, does not help (V first is worse).
 * Both point to a source with more gcse-time insns around the vertex loop (inline helper?).
 * Indexed packet[k].vertex[i] form: 1116 bytes, frame 0x240 (worse). No linked build.
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
extern unsigned int func_00283D38(float, unsigned int, unsigned int);
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
    char *vertex;
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
    for (i = 0, vertex = (char *)&packet; i < 4; vertex += 16, i++) {
        packet[0].uv[i][0] = D_L02_001B0D68[i][0];
        packet[0].uv[i][1] = D_L02_001B0D6C[i][0];
        packet[1].uv[i][0] = D_L02_001B0D68[i+4][0] + *(float *)(state + 0x18);
        packet[1].uv[i][1] = D_L02_001B0D6C[i+4][0];
        packet[0].color[i] = func_00283D38(blend.weights[0], D_L02_001B0DA8, D_L02_001B0DAC[0]);
        packet[1].color[i] = func_00283D38(blend.weights[1], D_L02_001B0DB0[0], D_L02_001B0DB4[0]);
        if (*(float *)(state + 0x1C) < 1.0f) {
            packet[0].color[i] = func_00283D38(*(float *)(state + 0x1C), packet[0].color[i] & 0x00FFFFFF, packet[0].color[i]);
            packet[1].color[i] = func_00283D38(*(float *)(state + 0x1C), packet[1].color[i] & 0x00FFFFFF, packet[1].color[i]);
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
