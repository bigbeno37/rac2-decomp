/* func_L04_0040DCA8 (740 bytes, 6 places). Tier opus-2, 4 November 2026.
 * Unit: default flags (-O2 -G0 -ffunction-sections), in L04_00402D90.c (the
 * B781AimQuad typedef and the vector helper externs come from that file).
 * Best check.py: 740/740 bytes, 0/6 places, first mismatch +0x224: 5 words of
 * corner-store order (retail c2.z c0.z c1.w c3.z c1.z c2.y; here c1.w c2.z
 * c0.z c2.y c1.z c3.z). Frame, saved registers, allocation, the UV loop and
 * everything else match.
 * New against the 3 November draft (728 bytes):
 *  - UV loop indexed (packet.uv[j].u/.v): loop.c reduces the two DEST_ADDR
 *    givs separately (v0 = uv+0, v1 = uv+4) like retail, with no pointer past
 *    the array; the counter is reversed into a0.
 *  - The UV loop needs its own counter (j). Sharing i with the vertex loop
 *    makes the counter cross calls, so sched1 floats its init to the top
 *    (s3 in the first jal slot) and position moves to s7.
 *  - Corner rotation[3] (48 bytes, unused) gives retail's 0x1D0 frame and
 *    corners at sp+0x120. Evidence: the boot sibling func_00281EB8
 *    (src/game/00281EB8.c) has the same arguments minus skew, and its
 *    DrawFrame keeps Vector rotation[3] right after the offset vector. Here
 *    the angle rotates the UVs instead, so the matrix is not used. Check this
 *    unused local is acceptable before landing.
 * What still differs (sched1 -fsched-verbose-5): h = 1 - skew*0.3f is ready
 * at t31 (0.3 load t23, mul t25, sub t28). The 0.3 and -1 loads are typed
 * memory before reload and the four TImode const-0 clears alu, so only one
 * memory slot is lost (t25). Stores then fill t26..t30, and the fifth (c1.w)
 * takes t30. sched2 keeps sched1's order for ties. Retail's order equals the
 * C order below (c3.w first as the dying use of 1.0, c1.y before c0.y as the
 * dying use of -1), so retail's sched1 lost a second memory slot: one more
 * prio>=3 alu insn there, or clears that wait for all four sq. Tried without
 * gain: a w = 1.0f local (cse keeps the older 1.0 pseudo canonical because w
 * lives only in the block, so the copy is deleted), chained clears
 * (a = b = c = d = 0: same timing, sq order reversed), a block-scoped
 * `Corner corners[4] = {{0}}` (memset call), per-corner statement order.
 */
#include "common.h"
typedef unsigned int B781AimQuad __attribute__((mode(TI)));
extern void func_00282CF0(void *,void *,void *), func_00282F20(void *,void *,float);
extern void func_00282DD8(void *,void *,void *), func_00282D30(void *,void *,float), func_00282CC0(void *,void *,void *);
extern float func_00283180(float), func_00283198(float);
extern void func_002830C0(void *, void *, void *);
typedef union { B781AimQuad bits; float f[4]; } Corner;
typedef struct { float u, v; } UVPair;
typedef struct { float vertices[4][4]; int colors[4]; UVPair uv[4]; long state[4]; } B971Packet;
extern void func_00281DE8(void *, int, int, int);
extern void func_002FC560(int, unsigned long long);
extern void func_00280B90(void *, int, int);
extern char D_L04_001B9680[], D_0018A0E0[];
void func_L04_0040DCA8(B781AimQuad *position, int texture, int color, int flags, float scale, float size, float angle, float skew)
{
    B971Packet packet;
    float tmp[2];
    float transform[4][4];
    float offset[4];
    Corner rotation[3];
    Corner corners[4];
    float c, sn;
    float *out;
    int i, j;
    func_00281DE8(&packet, texture, flags, 128);
    packet.state[0] = 5;
    packet.colors[0] = packet.colors[1] = packet.colors[2] = packet.colors[3] = color;
    packet.uv[0].u = 0; packet.uv[0].v = 0; packet.uv[1].u = 0; packet.uv[1].v = 1;
    packet.uv[2].u = 1; packet.uv[2].v = 0; packet.uv[3].u = 1; packet.uv[3].v = 1;
    c = func_00283180(angle);
    sn = func_00283198(angle);
    for (j = 0; j < 4; j++) {
        tmp[0] = packet.uv[j].u - 0.5f;
        tmp[1] = packet.uv[j].v - 0.5f;
        packet.uv[j].u = tmp[0] * c - tmp[1] * sn;
        packet.uv[j].v = tmp[0] * sn + tmp[1] * c;
        packet.uv[j].u += 0.5f;
        packet.uv[j].v += 0.5f;
    }
    qcopy(transform[3], position);
    func_00282CF0(transform[0], D_L04_001B9680, transform[3]);
    func_00282F20(transform[0], transform[0], 1.0f);
    func_00282DD8(transform[1], transform[0], D_0018A0E0);
    func_00282F20(transform[1], transform[1], 1.0f);
    func_00282DD8(transform[2], transform[1], transform[0]);
    func_00282D30(offset, transform[0], size);
    func_00282CC0(transform[3], transform[3], offset);
    corners[0].bits = 0; corners[1].bits = 0; corners[2].bits = 0; corners[3].bits = 0;
    corners[0].f[1] = -1; corners[1].f[1] = -1;
    corners[0].f[3] = 1;
    corners[2].f[2] = 1 - skew * 0.3f; corners[0].f[2] = 1 - skew * 0.3f;
    corners[1].f[3] = 1;
    corners[3].f[2] = -(1 - skew * 0.3f); corners[1].f[2] = -(1 - skew * 0.3f);
    corners[2].f[1] = 1; corners[2].f[3] = 1; corners[3].f[1] = 1; corners[3].f[3] = 1;
    for (i = 0; i < 4; i++) {
        out = packet.vertices[i];
        func_00282D30(out, corners[i].f, scale);
        func_002830C0(out, out, transform);
    }
    func_002FC560(0x47, 0x33001);
    func_00280B90(&packet, 0, 0);
    func_002FC560(0x47, 0x5360b);
}
