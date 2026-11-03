/* func_L00_003A91A0: draws three 20-point rings per active segment, a textured
 * ring strip between them, and two ribbon phases, all through quad packets.
 * Tier: opus-1 (Claude Opus 5.5, xhigh), 4 November 2026. Not a match.
 * Profile: -O2 -G8 (place it in src/levels/shared/L00_003A8E28-g8.c; the sda
 * globals are declared __attribute__((sda)) below).
 * Best result: 1524/1524 bytes, 0/27 places, 131 of 381 rows differ.
 * Matched: 0x560 frame (packet 0x90 at sp+0, matrix[4] at 0x90, rings[3][20]
 * at 0xD0, temp 0x490, width 0x4A0), segments pointer in $fp, i spilled at
 * 0x4B0, FPRs f22 alpha / f23 1.0 / f24 0.5, strip loop f20 v / f21 u,
 * phase 3 f20 scroll / f21 3.0, count-down ring loop (bgez), one div for the
 * two (k+1)%20 sites, strip giv bias (sp+0x140 / sp+0x3C0 via j=1..2 with
 * rings[j] and rings[j-1]).
 * Still differs:
 *  - spill-slot order of the PRE sp+C pseudos and i+1 (retail i+1 at 0x4C4,
 *    here 0x4D0; the 0x4BC..0x4D8 slots are permuted);
 *  - retail loads each colour global into $s1 (lw s1; sll v1,v0,24;
 *    or s3,v1,s1); here it goes to a scratch register.  Sharing the `inner`
 *    variable for that load costs $fp for the segments pointer;
 *  - phase-1 strip store order (retail uv5,uv1,c2,c3,uv6,uv0,uv2,uv3,uv4,uv7);
 *  - phase 2/3 address operand order (retail addu v1,fp,v0; addu s2,v0,fp)
 *    and s5/s6 counter naming.
 * Both phases set the standard UVs {0,0,0,1,1,0,1,1} before overriding
 * uv[4]/uv[6]: flow deletes phase 2's dead 1.0 stores but keeps phase 3's
 * because the segments[i].v load sits in between, as in retail.
 */
#include "common.h"

typedef float SegVec[4] __attribute__((aligned(16)));
typedef struct {
    SegVec axis[3];
    SegVec start, end;
    short active, alpha;
    float u, v, alphaScale;
} RingSegment;
typedef struct {
    SegVec vertex[4];
    unsigned int color[4];
    float uv[8];
    long tail[4];
} RingPacket;
extern char *func_L00_0032A088(int, int);
extern int *func_L00_0032A0B8(int, int);
extern void func_00281DE8(void *, int, int, int);
extern void func_00280B90(void *, int, int);
extern void func_00282DD8(void *, void *, void *);
extern void func_00282D30(void *, void *, float);
extern void func_002830C0(void *, void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
extern int func_00283CF0(float);
extern SegVec D_L00_00294B60[];
extern SegVec D_L00_001B8DC0;
extern int D_L00_001AF76C __attribute__((sda));
extern int D_L00_001AF784 __attribute__((sda));
extern int D_L00_001AF794 __attribute__((sda));
extern unsigned int D_L00_001AF764 __attribute__((sda));
extern unsigned int D_L00_001AF768 __attribute__((sda));
extern unsigned int D_L00_001AF780 __attribute__((sda));
extern unsigned int D_L00_001AF77C __attribute__((sda));
extern unsigned int D_L00_001AF790 __attribute__((sda));
extern unsigned int D_L00_001AF78C __attribute__((sda));
extern float D_L00_001AF754 __attribute__((sda));
extern float D_L00_001AF758 __attribute__((sda));
extern float D_L00_001AF75C __attribute__((sda));
extern float D_L00_001AF778 __attribute__((sda));
extern float D_L00_001AF788 __attribute__((sda));

void func_L00_003A91A0(char *object)
{
    RingPacket packet;
    SegVec matrix[4];
    SegVec rings[3][20];
    SegVec temp, width;
    char *state = *(char **)(object + 0x68);
    char *base = func_L00_0032A088(0x5CB, *(short *)(state + 0x1A));
    RingSegment *segments = (RingSegment *)(base + *func_L00_0032A0B8(0x5CB, *(short *)(state + 0x1A)));
    int i, j, k;
    unsigned int color, inner;
    float alpha, v, u, next, scroll;

    func_00281DE8(&packet, 0x15, 0, 0x80);
    for (i = 0; i < 3; i++) {
        if (segments[i].active == 0)
            continue;
        qcopy(matrix[0], segments[i].start);
        qcopy(matrix[2], segments[i].end);
        func_00282DD8(matrix[1], matrix[0], matrix[2]);
        alpha = segments[i].alpha * segments[i].alphaScale;
        color = (func_00283CF0(alpha * D_L00_001AF76C) << 24) | D_L00_001AF764;
        packet.color[0] = packet.color[1] = packet.color[2] = packet.color[3] = color;
        for (j = 0; j < 3; j++) {
            qcopy(matrix[3], segments[i].axis[j]);
            for (k = 0; k < 20; k++) {
                func_00282D30(temp, D_L00_00294B60[k], D_L00_001AF75C);
                func_002830C0(rings[j][k], temp, matrix);
            }
        }
        inner = D_L00_001AF768;
        u = segments[i].u;
        for (j = 1; j < 3; j++) {
            v = 0.0f;
            for (k = 0; k < 20; k++) {
                qcopy(packet.vertex[0], rings[j][k]);
                qcopy(packet.vertex[1], rings[j][(k + 1) % 20]);
                qcopy(packet.vertex[2], rings[j - 1][k]);
                qcopy(packet.vertex[3], rings[j - 1][(k + 1) % 20]);
                next = v + D_L00_001AF758;
                packet.uv[1] = packet.uv[5] = v;
                packet.uv[3] = packet.uv[7] = next;
                packet.color[3] = packet.color[2] = inner;
                packet.uv[4] = packet.uv[6] = u + D_L00_001AF754;
                packet.uv[0] = packet.uv[2] = u;
                v = next;
                func_00280B90(&packet, 0, 0);
            }
            inner = color;
        }

        func_00281DE8(&packet, 0xE, 1, 0x80);
        color = (func_00283CF0(alpha * D_L00_001AF784) << 24) | D_L00_001AF780;
        packet.uv[0] = 0.0f;
        packet.uv[1] = 0.0f;
        packet.uv[2] = 0.0f;
        packet.uv[3] = 1.0f;
        packet.uv[4] = 1.0f;
        packet.uv[5] = 0.0f;
        packet.uv[6] = 1.0f;
        packet.uv[7] = 1.0f;
        packet.color[0] = packet.color[1] = packet.color[2] = packet.color[3] = color;
        packet.uv[4] = packet.uv[6] = 0.5f;
        func_00282CF0(temp, D_L00_001B8DC0, segments[i].axis[0]);
        inner = D_L00_001AF77C;
        func_00282DD8(matrix[1], matrix[0], temp);
        func_00282F20(width, matrix[1], D_L00_001AF778);
        for (j = 0; j < 2; j++) {
            func_00282CC0(packet.vertex[0], segments[i].axis[j + 1], width);
            func_00282CF0(packet.vertex[1], segments[i].axis[j + 1], width);
            func_00282CC0(packet.vertex[2], segments[i].axis[j], width);
            func_00282CF0(packet.vertex[3], segments[i].axis[j], width);
            packet.color[2] = packet.color[3] = inner;
            func_00280B90(&packet, 0, 0);
            inner = color;
            packet.uv[0] = packet.uv[2] = packet.uv[4];
            packet.uv[4] = packet.uv[6] = 3.0f;
        }

        func_00281DE8(&packet, 0x17, 1, 0x80);
        color = (func_00283CF0(alpha * D_L00_001AF794) << 24) | D_L00_001AF790;
        packet.color[0] = packet.color[1] = packet.color[2] = packet.color[3] = color;
        packet.uv[0] = 0.0f;
        packet.uv[1] = 0.0f;
        packet.uv[2] = 0.0f;
        packet.uv[3] = 1.0f;
        packet.uv[4] = 1.0f;
        packet.uv[5] = 0.0f;
        packet.uv[6] = 1.0f;
        packet.uv[7] = 1.0f;
        scroll = segments[i].v;
        packet.uv[4] = packet.uv[6] = scroll + 0.5f;
        inner = D_L00_001AF78C;
        func_00282F20(width, matrix[1], D_L00_001AF788);
        for (j = 0; j < 2; j++) {
            func_00282CC0(packet.vertex[0], segments[i].axis[j + 1], width);
            func_00282CF0(packet.vertex[1], segments[i].axis[j + 1], width);
            func_00282CC0(packet.vertex[2], segments[i].axis[j], width);
            func_00282CF0(packet.vertex[3], segments[i].axis[j], width);
            packet.color[2] = packet.color[3] = inner;
            func_00280B90(&packet, 0, 0);
            inner = color;
            packet.uv[0] = packet.uv[2] = packet.uv[4];
            packet.uv[4] = packet.uv[6] = scroll + 3.0f;
        }
    }
}
