/*
 * func_PAL_L20_0039FEA0, the PAL copy of func_L20_0039CF58: EXACT C, held out of src/ only by a
 * catalogue defect. Tier: opus-1 (Claude Opus 5.5), task v2672.
 *
 * Best check.py --version pal: "1000 bytes, expected 996", 0 of 1 places. No instruction differs.
 * The same Placement.derive call check.py makes, run on the whole 1000-byte section with the size
 * test skipped, accepts it at 0x39FEA0 in PAL level 20, relocations included. Changing the last
 * word makes derive reject it ("instruction at +0x3e4 differs"), so that word is compared. This
 * is not a linked build: no program has been linked with this C.
 *
 * What differs: config/pal/levels/functions.tsv ends the row 4 bytes early. Retail has `jr $ra` at
 * 0x3A0280, its delay slot `addiu $sp,$sp,0x70` at 0x3A0284, and the next row at 0x3A0288, so the
 * function is 1000 bytes. C cannot give the 996-byte extent: a jr needs its delay slot.
 *
 * Why: v1.01's stretch is also 1000 bytes, 996 of function plus a padding nop at 0x39D33C. 0.25f
 * (lui, mtc1) became 0.3f (lui, ori, mtc1), and the extra instruction took the padding word.
 * tools/carry.py step 2 gave equal-length stretches v1.01's cuts at the same offsets.
 *
 * Unblock: make this row 1000 bytes in tools/carry.py or the regenerated catalogue. Then put this
 * C in a new unit, src/pal/levels/20_yeedil/L20_0039FEA0-qcopy.c, and delete the marker from
 * L20_0039FEA0.c. Append to config/cflags.txt (template flags, tested with this C):
 *   src/pal/levels/20_yeedil/L20_0039FEA0-qcopy.c  -O2 -G0 -ffunction-sections -frac2-qcopy -frac2-qcopy-barrier
 *
 * Edits against the template: 20 becomes 17 (three func_L20_0039DB50 calls), 0.25f becomes 0.3f,
 * `> 120` becomes `> 100`, and six callees take their PAL names.
 *
 * Needs only the declarations below.
 */
#include "common.h"
typedef int Quadword __attribute__((mode(TI)));
typedef union { Quadword q; float f[4]; } Vector;
typedef struct {
    unsigned char reserved_00[0x18];
    unsigned char enabled, count, kind;
    unsigned char reserved_1b[0x65];
    unsigned char payload[1];
} DispatchContext;
extern unsigned long func_002B1C20(void *, DispatchContext *);
extern void func_PAL_L20_003A0288(unsigned char *);
extern void func_0029F338(void *);
extern void func_002A7750(unsigned char *, unsigned char, int);
extern unsigned char *func_PAL_L00_003FB330(void *, int, int, int, int, int, int, int, int, int, int, int, float, float);
extern void func_PAL_002A7650(void *, int, int);
extern void func_L20_0039DE88(char *);
extern void func_L20_0039DB50(char *, int, int, float);
extern int func_002B3868(unsigned char *);
extern void func_L20_0039E158(char *);
extern void func_L20_0039E1E8(void *);
extern float func_00283248(float, float);
extern int func_L01_00322288(int, float);
extern float func_00283C80(float, float);
extern void func_PAL_L01_00323068(char *, char *);
extern void func_0029FCC0(char *, int, char *);
extern void func_00282CF0(void *, const void *, const void *);
extern void func_00282F20(void *, const void *, float);
extern int func_002E5CF8(int, int, char *);
extern unsigned char *func_PAL_L09_003A3CA0(void *, void *, void *, float);
extern void func_PAL_L20_003A06F8(char *);
extern int D_L20_001A8F00[] __attribute__((sda));
extern int D_L20_001BDCF0;

void func_PAL_L20_0039FEA0(unsigned char *object) {
    Vector position, velocity;
    unsigned char *state = *(unsigned char **)(object + 0x68);
    unsigned char *child, *target;
    unsigned long flags;
    int mode, code, hand;
    unsigned int tick;
    float angle;
    {
        unsigned char *initial = *(unsigned char **)(state + 0x254);
        if (initial) *(int *)(initial + 0x98) = 0;
    }
    func_PAL_L20_003A0288(object);
    if (D_L20_001A8F00[0] == 2) {
        if (D_L20_001BDCF0 < 5) return;
        child = *(unsigned char **)(state + 0x258);
        if (child) func_0029F338(child);
        child = *(unsigned char **)(state + 0x25C);
        if (child) func_0029F338(child);
        child = *(unsigned char **)(state + 0x254);
        if (child) func_0029F338(child);
        func_0029F338(object);
        return;
    }
    mode = object[0x95];
    if (mode == 1) {
        flags = func_002B1C20(object, (DispatchContext *)(state + 0x140));
        if ((flags & 1UL) == 1UL) {
            position.q = *(Quadword *)(object + 0x10);
            position.f[2] += 0.5f;
            func_PAL_L00_003FB330(&position, 0, 25, 20, 20, 20, 45, 0, mode, 0, 0, 0, 0.5f, 20.0f);
            func_PAL_002A7650(object, 0, 3);
            func_L20_0039DE88((char *)object);
        } else if ((flags & 2UL) == 2UL) {
            func_002A7750(object, 3, 0);
            func_L20_0039DB50((char *)object, 0, 17, 0.0f);
        }
    }
    code = object[0x20];
    switch (code) {
    case 6:
        goto finished;
    case 2:
        if (object[0x60] & 2) func_002A7750(object, 3, -1);
        break;
    case 3:
        if (func_002B3868(object)) {
            func_L20_0039E158((char *)object);
            func_L20_0039E1E8(object);
            func_L20_0039DB50((char *)object, 11, 17, 0.0f);
        }
        if (*(float *)(state + 0x11C) <= *(float *)(state + 0x84)) {
            unsigned char *aimTarget = *(unsigned char **)(state + 0x200);
            angle = func_00283248(*(float *)(aimTarget + 0x10) - *(float *)(object + 0x10),
                                  *(float *)(aimTarget + 0x14) - *(float *)(object + 0x14));
            func_L01_00322288((int)object, angle);
            if (func_00283C80(*(float *)(object + 0xF8), angle) < 0.05235988274216652f)
                func_002A7750(object, 4, -1);
        } else {
            func_PAL_L01_00323068((char *)object, (char *)state + 0x260);
        }
        break;
    case 4:
        if (func_002B3868(object)) func_L20_0039DB50((char *)object, 10, 17, 0.0f);
        *(float *)(state + 0x284) = func_00283248(*(float *)(state + 0x1F0) - *(float *)(object + 0x10),
                                                *(float *)(state + 0x1F4) - *(float *)(object + 0x14));
        tick = *(unsigned short *)(object + 0x96) % 20;
        if (tick == 0 || tick == 10) {
            hand = tick == 0 ? 4 : 0;
            func_0029FCC0(*(char **)(hand + state + 0x258), 0, (char *)&position);
            func_00282CF0(&velocity, state + 0x1F0, &position);
            func_00282F20(&velocity, &velocity, 0.3f);
            func_002E5CF8(1, 0, *(char **)(state + 0x254));
            func_PAL_L09_003A3CA0(object, &velocity, &position, (float)state[0x34]);
        }
        if (*(unsigned short *)(object + 0x96) > 100 || ((target = *(unsigned char **)(state + 0x280)) && (*(unsigned short *)(target + 0x34) & 1)))
            func_002A7750(object, 3, -1);
        break;
    }
    func_PAL_L20_003A06F8((char *)object);
    child = *(unsigned char **)(state + 0x254);
    if (child) *(int *)(child + 0x98) = *(int *)(*(unsigned char **)(child + 0x24) + 0x10);
finished:
    return;
}
