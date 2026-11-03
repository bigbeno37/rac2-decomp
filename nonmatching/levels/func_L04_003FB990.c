/* func_L04_003FB990: opus-1 tier (Claude Opus 5.5), USA v1.01, 4 November 2026.
 * Best: 1008/1008 bytes, 0 of 2 places; six non-relocation instructions differ.
 * Place it where its INCLUDE_ASM is in src/levels/shared/L04_003C8380.c (default
 * -O2 -G0 -ffunction-sections) and change that file's later forward declaration to
 * `extern void func_L04_003FB990(unsigned char *, void *);`. It also uses the file's
 * declarations of func_00283CE0, func_00277F48, func_00283248, func_00282E48,
 * func_002E5CF8, func_00282CD8, func_002761F8, func_L04_0040C0C0 and D_L04_001BFAE0.
 *
 * Only two spill slots are exchanged: retail keeps &m->pos (m + 0x10) at sp+0x34 and
 * ray[1] (sp+0x10) at sp+0x30; this keeps them at sp+0x30/sp+0x34 (two stores, four loads).
 * Cause (measured in the -dG dump): both are gcse PRE reaching registers, numbered in
 * expression-hash bucket order, and the unallocated pseudos take slots in pseudo order.
 * Raw hashes: (plus at 16) = 14046, (plus (reg m) 16) = 14045 + regno(m) = 14129 (m is
 * pseudo 84). With this body max_cuid is 216, table 109 buckets: at+16 -> 94, m+16 -> 68,
 * so m+16 is numbered first. ray[1] comes first only for table sizes 93, 101, 115-123, 131
 * (max_cuid 184-187, 200-203, 228-247 or 260-263), i.e. retail's RTL has about 13-16 fewer
 * or 12-31 more insns at gcse time with the same final code. The index loop, the global
 * indexed directly, the count-down pointer loop (107 buckets), and the hit record and ray
 * vectors as separate scoped locals (gcc 2.95 shares sibling-block stack slots, so no union
 * is needed) all give the same bytes. A user pointer for ray[1] gets a register (worse);
 * an if-form for `sign` is much worse.
 */
#include "common.h"

extern float func_00283CE0(int);
extern int func_00277F48(float, void *, int, void *, void *);
extern float func_00283248(float, float);
extern int func_002E5CF8(int, int, char *);
extern void func_00282CD8(void *, void *, void *);
extern int func_002761F8(void *, void *, int, void *, void *);
extern void func_L04_0040C0C0(void *, void *, int, int);
extern unsigned char D_L04_001BFAE0[];

extern unsigned char *D_L04_001C3B40[];
extern float func_00283C80(float, float);
extern float func_00282E80(void *, void *);
extern float func_00282E48(char *, char *);
extern float func_00283180(float);
extern float func_00283198(float);
extern float func_00283C28(float);
extern float func_00283B98(float, float);
extern void func_002A8E20(void *, void *);
extern void func_002AF000(void *, float, float, float);

typedef union {
    struct {
        float impulse[4];
        void *owner;
        int flags;
        unsigned char kind, mode;
        unsigned short id;
        float damage;
        int extra;
        float pad[3];
    } hit;
    float vec[3][4];
} B990Scratch;

void func_L04_003FB990(unsigned char *m, void *vars)
{
    unsigned char *v = vars;
    float radius = *(float *)(v + 0x70) * *(float *)(v + 0x7C);
    float angle, angle0, angle1, sign;
    int count, n, side, i;
    if (radius < 0.75f) radius = 0.75f;
    n = func_00277F48(radius, m + 0x10, 16, *(void **)(v + 0x74), 0);
    for (count = 0; count < n; count++) {
        if (D_L04_001C3B40[count] == *(unsigned char **)(v + 0x74)) continue;
        if (*(float *)(v + 0x7C) > 0) {
            angle = func_00283248(*(float *)(D_L04_001C3B40[count] + 0x10) - *(float *)(m + 0x10),
                                  *(float *)(D_L04_001C3B40[count] + 0x14) - *(float *)(m + 0x14));
            if (func_00283C80(angle, *(float *)(m + 0xF8)) > 0.741765022277832f) continue;
            angle = func_00283248(func_00282E80(m + 0x10, D_L04_001C3B40[count] + 0x10),
                                  *(float *)(D_L04_001C3B40[count] + 0x18) - *(float *)(m + 0x18));
            if (func_00283C80(angle, -*(float *)(m + 0xF4)) > 0.523598849773407f) continue;
        }
        if (func_00282E48((char *)m + 0x10, (char *)D_L04_001C3B40[count] + 0x10) > radius * 0.75f) {
            B990Scratch scratch;
            scratch.hit.owner = *(void **)(v + 0x74);
            scratch.hit.flags = 0x230000;
            scratch.hit.kind = 2;
            scratch.hit.mode = 3;
            scratch.hit.id = *(unsigned short *)(m + 0xAA);
            scratch.hit.damage = 1.0f;
            scratch.hit.extra = 0x230000;
            angle = func_00283248(*(float *)(D_L04_001C3B40[count] + 0x10) - *(float *)(m + 0x10),
                                  *(float *)(D_L04_001C3B40[count] + 0x14) - *(float *)(m + 0x14));
            scratch.hit.impulse[0] = func_00283180(angle);
            scratch.hit.impulse[1] = func_00283198(angle);
            scratch.hit.impulse[2] = 1.0f;
            scratch.hit.impulse[3] = 5627.9248046875f;
            func_002A8E20(D_L04_001C3B40[count], &scratch);
            func_002E5CF8(10, 0, *(void **)(v + 0x74));
        }
    }
    {
    float ray[3][4];
    for (side = 0; side < 2; side++) {
        sign = (side & 1) ? 1.0f : -1.0f;
        for (i = 0; i < 5; i++) {
            angle0 = func_00283C28(func_00283CE0(i) * sign * 0.1396263688802719f);
            angle1 = func_00283C28(func_00283CE0(i + 1) * sign * 0.1396263688802719f);
            func_002AF000(ray[0], radius, func_00283B98(angle0, *(float *)(m + 0xF8)), -*(float *)(m + 0xF4));
            func_002AF000(ray[1], radius, func_00283B98(angle1, *(float *)(m + 0xF8)), -*(float *)(m + 0xF4));
            func_00282CD8(ray[2], ray[0], m + 0x10);
            func_00282CD8(ray[2], ray[1], m + 0x10);
            if (func_002761F8(ray[0], ray[1], 18, *(void **)(v + 0x74), 0)) {
                func_002AF000(ray[2], 0.33333334326744f, *(float *)(m + 0xF8), -*(float *)(m + 0xF4));
                func_L04_0040C0C0(&D_L04_001BFAE0, ray[2], 8, 0);
            }
        }
    }
    }
}
