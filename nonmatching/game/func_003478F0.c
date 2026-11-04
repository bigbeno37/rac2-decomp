/* opus-2 (Claude Opus 5.5, xhigh), USA v1.01, game -O2 -G8 -ffunction-sections; compile in a -G8
 * unit (e.g. src/game/00345358-<topic>-g8.c; -fno-gcse is far worse: 1392 bytes).
 * Best: 1452/1452 bytes, 0/28 places, 129 of 363 rows differ (cdiff.py); first mismatch +0x4C.
 * This is opus-1's body with level-safe names (ref_00347468_10, D_L00_00239BA0, D_L00_001F2880)
 * and func_00335E20's int return contract (pointer casts at the uses); bytes are unchanged.
 * Still differs:
 *  - Spill slots of the hoisted child pointers. gcse raw hash of (plus (reg r) N) = 14029+r+N,
 *    bucket = raw % 173 here; reaching regs (and slots) follow bucket order. Mine with base reg 84:
 *    o+0x258, o+0x3B4, o+0x98, o+0x2A0, row+1, o+0x210, o+0x17C, o+0xE4. Retail has the same order
 *    rotated by two (o+0x258 and o+0x3B4 last). No odd table size fits retail with base reg 84;
 *    it fits only if the base of these sums is a pseudo numbered about 180 or more (any size 161..189), or if
 *    o+0x258/o+0x3B4 get slots later (reload spills). A static inline helper for the grid loop
 *    left the order unchanged (and lost the f23 save, 1440 bytes).
 *  - First coordinate block (+0x118..): `float by = (float)D_L00_001AE754;` before x, then
 *    `y = D_L00_001AE75C * row + by`, gives retail's load and mul order (130 rows), but p[1]
 *    still wins $f13 over y in local-alloc (both get a phys suggestion; p[1]'s shorter life wins).
 *    Reading p[1] into a local before y gives retail's y=f13/by=f2/p[1]=f1, but moves x to f12
 *    and the D_75C load after the colour store (135 rows); read before the store it costs a
 *    second load of o+0x498 (1456 bytes). Locals for the argument sums changed nothing.
 *  - D_00139688 block (+0x3F8..): %hi(D_L00_00239BA0)/%hi(D_00139568) are PRE-inserted at the
 *    end of BB4 (reaching regs), rematerialised by reload, so combine sees an outside addend and
 *    forms mtlo/madd; retail has both lui at block start and mult/addu, plus bnel at +0x400.
 *  - +0x1DC early lui of D_L00_00239BA0; bnel/bne at +0x468; float-pair copy registers (+0x484..).
 */
#include "include_asm.h"
extern void func_00335E68(void *, int);
extern int func_00335E20(void *);
extern void func_00335E38(void *, float, float, float, float);
extern void func_00336320(void *);
extern void func_00336A78(void *);
extern void func_00336828(void *);
extern void func_00336920(void *, int, int);
extern void func_00336B90(void *, int);
extern void func_00336B98(void *, int);
extern void func_00336F20(int, int *, int *, int *);
extern void func_00336498(void *, int, int);
extern void func_00336548(void *);
extern int func_0026F7A0(void *);
extern void func_003472B8(void *);
extern void func_00347468(void *);
typedef struct { int x, y; } MenuIntegerPair;
typedef struct { float x, y; } MenuFloatPair;
typedef struct { short a, b, c, id, e; } MenuGridEntry;
typedef struct {
    char pad0[0x3C];
    unsigned short icon;
    char pad3E[0x2E];
    int max;
    char pad70[0x18];
    unsigned short ammo;
    char pad8A[0x56];
} MenuWeaponRecord;
extern MenuGridEntry ref_00347468_10[];
extern unsigned char D_001A7AF8[];
extern unsigned char D_00139568[];
extern int D_00139868[];
extern int D_00139688[];
extern MenuWeaponRecord D_L00_00239BA0[];
extern void *D_L00_001F2880[];
extern int D_L00_001AE748, D_L00_001AE74C, D_L00_001AE750, D_L00_001AE754;
extern float D_L00_001AE758, D_L00_001AE75C;
extern MenuIntegerPair D_L00_001AE588[], D_L00_001AE590[];
extern MenuFloatPair D_L00_001AE598[], D_L00_001AE5A0[];
void func_003478F0(char *o)
{
    if (*(int *)(o + 0x4F8)) {
        int row, col;

        func_00336320(o);
        func_00336320(o + 0x4C);
        func_00336320(o + 0x130);
        for (row = 0; row < 6; row++) {
            for (col = 0; col < 4; col++) {
                short id;
                {
                    int *color = (int *)func_00335E20(o + 0x3B4);
                    float y = D_L00_001AE75C * (float)row + (float)D_L00_001AE754;
                    float x = D_L00_001AE758 * (float)col + (float)D_L00_001AE750;
                    *color = 0x60241700;
                    func_00335E38(o + 0x3B4, x + (*(float **)(o + 0x498))[0], y + (*(float **)(o + 0x498))[1], 0.0f, 0.0f);
                }
                *(int *)func_00335E20(o + 0x98) = 0x30F0C070;
                *(int *)func_00335E20(o + 0x3B4) = 0x60241700;
                func_00335E68(o + 0x1C8, 0);
                func_00335E68(o + 0x378, 0);
                if (D_001A7AF8[ref_00347468_10[row * 4 + col].id]) {
                    MenuWeaponRecord *weapons = D_L00_00239BA0;
                    unsigned char *slots = D_00139568;
                    func_00336920(o + 0x378, weapons[slots[ref_00347468_10[row * 4 + col].id]].icon, 0);
                    func_00335E38(o + 0x378, (53.5f * (float)col + (float)D_L00_001AE748) + (*(float **)(o + 0x498))[0],
                                  (47.0f * (float)row + (float)D_L00_001AE74C) + (*(float **)(o + 0x498))[1], 0.0f, 0.0f);
                    if (weapons[slots[ref_00347468_10[row * 4 + col].id]].icon == 0xEA7E) {
                        *(int *)func_00335E20(o + 0x378) = 0x80808080;
                    } else {
                        MenuIntegerPair base = D_L00_001AE588[0], stride = D_L00_001AE590[0];
                        int a, b, color;
                        func_00336B90(o + 0x1C8, weapons[slots[ref_00347468_10[row * 4 + col].id]].max);
                        func_00336B98(o + 0x1C8, D_00139868[ref_00347468_10[row * 4 + col].id] >> 5);
                        func_00335E38(o + 0x1C8, (float)(stride.x * col + base.x) + (*(float **)(o + 0x498))[0],
                                      (float)(stride.y * row + base.y) + (*(float **)(o + 0x498))[1], 0.0f, 0.0f);
                        func_00335E68(o + 0x1C8, 1);
                        a = 0;
                        b = 0;
                        color = 0;
                        func_00336F20(ref_00347468_10[row * 4 + col].id, &a, &b, &color);
                        func_00336498(o + 0x1C8, a, b);
                        *(int *)func_00335E20(o + 0x378) = color;
                        if (weapons[slots[ref_00347468_10[row * 4 + col].id]].max <= 0) {
                            func_00336B90(o + 0x1C8, 100);
                            func_00336B98(o + 0x1C8, 100);
                        }
                    }
                    if (D_00139688[ref_00347468_10[row * 4 + col].id] == 0) {
                        if (D_L00_00239BA0[D_00139568[ref_00347468_10[row * 4 + col].id]].ammo != 0) *(int *)func_00335E20(o + 0x3B4) = 0x60202080;
                    }
                    func_00335E68(o + 0x378, 1);
                    if (func_0026F7A0(D_L00_001F2880[0]) == ref_00347468_10[row * 4 + col].id) *(int *)func_00335E20(o + 0x98) = 0x80FFDE8D;
                }
                {
                    MenuFloatPair stride = D_L00_001AE598[0], base = D_L00_001AE5A0[0];
                    func_00335E38(o + 0x98, (stride.x * (float)col + base.x) + (*(float **)(o + 0x498))[0],
                                  (stride.y * (float)row + base.y) + (*(float **)(o + 0x498))[0], 0.0f, 0.0f);
                }
                func_00336320(o + 0x3B4);
                func_00336828(o + 0x378);
                func_00336548(o + 0x1C8);
                func_00336320(o + 0x98);
            }
        }
        func_00336320(o + 0xE4);
        func_00336320(o + 0x17C);
        func_00336A78(o + 0x210);
        func_00336A78(o + 0x258);
        func_00336A78(o + 0x2A0);
        func_003472B8(o);
        func_00347468(o);
    }
}
