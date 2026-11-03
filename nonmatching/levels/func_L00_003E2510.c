/* opus-1, task v2353; USA v1.01, -O2 -G0 -ffunction-sections -g, tested in
 * a dedicated unit (src/levels/shared/L00_003D37E8-missile.c with that cflags
 * line). Best check.py result: 1724 bytes versus 1724 retail; 0 of 27 places
 * match (first difference at +0xdc). All required declarations are below.
 *
 * Fixes over the sol-high draft (1700 bytes): the early exit runs
 * func_002FEE50(state sound) and func_0029F338(obj) before returning.
 * func_002E5CF8 is declared as returning int (its real type, see
 * src/game/002E10B8.c). Its ignored call_value v0 shifts local-alloc, so the
 * collision arm keeps its own store tail and no longer cross-jumps into the
 * timeout arm. The structure follows func_L00_003ED198 (L00_003EA0E0-seeker.c).
 *
 * Remaining non-relocation difference: in the lead-correction call
 * func_00282D30(temp, scratch, info->0x10), retail sets a1 (sp) before a0 (s0).
 * Ours sets a0 first because the arguments tie in sched1 (same priority,
 * weight and dependents), so luid order wins. Retail probably had a dying
 * pseudo as the a1 source (weight 0), which update_equiv_regs later folded
 * to sp. A `float *` copy of scratch (used once or twice) and a scoped copy of
 * temp did not change it, because cse propagates the fp copy. The scale-table
 * byte is D_00139568[0x1E]: retail reads it as lui 0x14 / lbu -0x6A7A, which
 * should be the same relocation (it could also be declared as D_00139586).
 * That has not been confirmed by a linked match.
 */
#include "common.h"

extern int func_002B4038(int, int);
extern void func_00282D30(char *, char *, float);
extern void func_002AFFD8(void *, void *, int);
extern int func_L00_003E39B8(char *);
extern char *func_002AB1F0(char *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282CC0(void *, void *, void *);
extern float func_00282DF0(void *);
extern void func_002FEE70(int, void *, int);
extern void func_00300190(void *, void *);
extern float func_00283B98(float, float);
extern void func_L00_003E3890(char *);
extern void func_00282F20(void *, void *, float);
extern int func_002761F8(void *, void *, int, void *, void *);
extern void func_L00_003E3258(void *, int, int);
extern void func_00334160(void *, int, int);
extern int func_002E5CF8(int, int, char *);
extern int func_00282948(void *);
extern void func_L00_003E3668(void *);
extern int func_002A77E0(int);
extern float func_002A7878(float, float);
extern float func_002A78D8(float, float);
extern char *func_002C65A0(void *, void *, int, unsigned int, int, int, int, float, float, float, int);
extern void func_L00_002E6EF8(void *, void *);
extern void func_002FEE50(int);
extern void func_0029F338(void *);
extern char D_00139568[];
typedef struct { unsigned char flag[4]; unsigned char enabled; char pad[0x6B]; float value; char tail[0x6C]; } B2510Entry;
extern B2510Entry D_L00_00262570[];
extern char *D_0018C0B0 NOT_SDA;
typedef struct { char pad[0x18]; char *target; char pad1[4]; float position[4]; } B2510Hit;
extern B2510Hit D_L00_001BF200;
extern char D_L00_001A8F20[] __attribute__((sda));
extern unsigned char *D_L00_001FEA14;
extern unsigned char *D_L00_001FE9F4;
extern int D_L00_001B19C8 __attribute__((sda));
extern char func_L00_003E2BD0[];

void func_L00_003E2510(char *obj)
{
    float scratch[4], previous[4], delta[4], temp[4];
    unsigned char hit[48];
    float particle[4];
    char *state = *(char **)(obj + 0x68);
    char *target;
    char *info;
    char *p;
    float *tracking_delta;
    float length, speed, scale;
    int work, i;

    if (!func_002B4038(*(int *)(state + 0x18), *(int *)(state + 0x14))) {
        func_002FEE50(*(int *)(state + 0x34));
        func_0029F338(obj);
        return;
    }
    func_002AFFD8(obj + 0x10, scratch, 1);
    qcopy(previous, obj + 0x10);
    target = *(char **)(state + 0x1C);
    if (target) {
        if (*(unsigned char *)(target + 0x20) == 0xFE) goto clear_target;
        if (*(unsigned char *)(target + 0x20) == 0xFD) goto clear_target;
        if (func_L00_003E39B8(target)) goto clear_target;
        {
            { float *t = delta;
            func_00282CF0(t, *(char **)(state + 0x1C) + 0x10, obj + 0x10);
            tracking_delta = t; }
            info = func_002AB1F0(*(char **)(state + 0x1C));
            if (info) {
                func_00282D30((char *)temp, (char *)scratch, *(float *)(info + 0x10));
                func_00282CC0(tracking_delta, tracking_delta, temp);
            }
            length = func_00282DF0(tracking_delta);
            if (length == 0.0f) goto clear_target;
            {
                speed = *(float *)(state + 0x2C);
                func_00282D30(state + 0x20, (char *)tracking_delta, speed / length);
                *(float *)(state + 0x2C) = speed;
            }
        }
    }
    goto target_done;
clear_target:
    *(char **)(state + 0x1C) = 0;
target_done:
    switch (*(unsigned char *)(obj + 0x20)) {
    case 0:
        func_002FEE70(*(int *)(state + 0x34), state + 0x20, 0);
        func_00282CC0(obj + 0x10, obj + 0x10, state + 0x20);
        func_00300190(obj + 0x10, obj + 0x10);
        *(float *)(obj + 0xF0) = func_00283B98(*(float *)(obj + 0xF0), *(float *)(state + 0x44));
        *(float *)(obj + 0xF4) = func_00283B98(*(float *)(obj + 0xF4), *(float *)(state + 0x48));
        *(float *)(obj + 0xF8) = func_00283B98(*(float *)(obj + 0xF8), *(float *)(state + 0x4C));
        func_L00_003E3890(obj);
        *(char **)(hit + 0x10) = obj;
        *(int *)(hit + 0x14) = 0x10000;
        *(float *)(hit + 0x1C) = ((B2510Entry *)((unsigned char)D_00139568[0x1E] * 0xE0 + (char *)D_L00_00262570))->value;
        *(int *)(hit + 0x20) = 1;
        qcopy(hit, state + 0x20);
        func_00282F20(hit, hit, 1.0f);
        *(float *)(hit + 0xC) = 5627.925f;
        *(short *)(hit + 0x1A) = *(unsigned short *)(obj + 0xAA);
        hit[0x18] = 1;
        hit[0x19] = 1;
        if (func_002761F8(previous, obj + 0x10, 0, D_0018C0B0, hit)) {
            if (!D_L00_001BF200.target) func_L00_003E3258(D_L00_001BF200.position, 15, 0);
            else if (*(int *)(D_L00_001BF200.target + 0x98)) {
                func_L00_003E3258(D_L00_001BF200.position, 10, 1);
                func_00334160(D_L00_001BF200.target, 0x1E, *(int *)(obj + 0xB8));
            }
            func_002E5CF8(0, 0, obj);
            *(unsigned char *)(obj + 0x20) = 1;
            *(char **)(state + 0x1C) = 0;
            *(unsigned short *)(obj + 0x34) |= 0x41;
            *(int *)(state + 0x38) = 6;
        } else if (func_00282948(state + 0x38)) {
            func_L00_003E3668(obj + 0x10);
            *(unsigned char *)(obj + 0x20) = 1;
            *(char **)(state + 0x1C) = 0;
            *(unsigned short *)(obj + 0x34) |= 0x41;
            *(int *)(state + 0x38) = 6;
        }
        work = func_002A77E0(255);
        p = func_002C65A0(obj + 0x10, D_L00_001A8F20, 2, 0x202080, work, 64, 16, 420000.0f, 420000.0f, 0.0f, 1);
        if (p) p[2] = *D_L00_001FEA14;
        p = func_002C65A0(obj + 0x10, D_L00_001A8F20, 2, 0x202080, (work + 128) % 255, 64, 16, 420000.0f, 420000.0f, 0.0f, 1);
        if (p) p[2] = *D_L00_001FEA14;
        work = (int)delta;
        func_00282CF0((void *)work, obj + 0x10, previous);
        func_00282D30((char *)work, (char *)work, 0.2f);
        qcopy(temp, previous);
        tracking_delta = (float *)work;
        for (i = 4; i >= 0; i--) {
            work = func_002A77E0(255);
            scale = func_002A7878(0.1f, 0.3f);
            particle[0] = temp[0] + func_002A78D8(0.0f, 0.1f);
            particle[1] = temp[1] + func_002A78D8(0.0f, 0.1f);
            particle[2] = temp[2] + func_002A78D8(0.0f, 0.1f);
            func_002C65A0(particle, D_L00_001A8F20, 15, 0x808080, work, 32, 0, scale * 210000.0f, scale * 42000.0f, 0.0f, 1);
            func_00282CC0(temp, temp, tracking_delta);
        }
        if (D_L00_001B19C8 % 10 == 0) {
            p = func_002C65A0(obj + 0x10, D_L00_001A8F20, 30, 0x202080, 0, 32, 0, 2100.0f, 420000.0f, 0.0f, 1);
            if (p) p[2] = *D_L00_001FE9F4;
        }
        func_L00_002E6EF8(func_L00_003E2BD0, obj);
        break;
    case 1:
        if (func_00282948(state + 0x38)) {
            func_002FEE50(*(int *)(state + 0x34));
            func_0029F338(obj);
            return;
        }
        func_L00_002E6EF8(func_L00_003E2BD0, obj);
        break;
    }
    if (*(float *)(obj + 0x10) < 2.0f || *(float *)(obj + 0x10) > 1021.0f ||
        *(float *)(obj + 0x14) < 2.0f || *(float *)(obj + 0x14) > 1021.0f ||
        *(float *)(obj + 0x18) < 2.0f || *(float *)(obj + 0x18) > 1021.0f) {
        func_002FEE50(*(int *)(state + 0x34));
        func_0029F338(obj);
    }
}
