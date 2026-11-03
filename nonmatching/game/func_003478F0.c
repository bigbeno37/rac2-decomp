/* opus-1 (Claude Opus 5.5, xhigh), USA v1.01, game -O2 -G8 -ffunction-sections
 * (compile in a -G8 unit such as src/game/00345358-<topic>-g8.c; no $gp access in retail,
 * but -G8 gives retail's `lui $at` macro loads of the scalar layout globals; -G0 is far worse).
 * Best: 1452/1452 bytes, 0/28 places, 129 of 363 rows differ in check.py --diff (sol-high draft
 * recompiled here: 142); first mismatch +0x4C (spill slot).
 * Fixed versus the sol-high draft: the grid table is indexed as D_00259F58[row * 4 + col].id,
 * which gives retail's `la` + `addiu 6` and removes a nop; the bar position is written
 * `stride.x * col + base.x`, which fixes the v1/v0 versus a2/a3 operand registers at +0x30C.
 * Still differs:
 *  - spill slots of the hoisted child pointers (o+0x3B4/0x98/0xE4/0x17C/0x210/0x258/0x2A0)
 *    and row+1. Retail order: 0x48 o+0x98, 0x4C o+0x2A0, 0x50 row+1, 0x54 o+0x210, 0x58 o+0x17C,
 *    0x5C o+0xE4, 0x60 o+0x258, 0x64 o+0x3B4. This is gcse/PRE bucket order (see x.i.gcse).
 *  - first coordinate block (+0x11C..): retail order is flt(D_754), ld D_758, flt(D_750),
 *    ld D_75C. sched2 then breaks ties by luid. Writing `float by = (float)D_754;` before x and
 *    `y = D_75C * row + by` reproduces that load order, but the registers stay f3/f1/f2/f0
 *    (retail f2/f0/f1/f13). Retail ties y+p[1] to y (op1) and x+p[0] to p[0] (op2).
 *  - +0x1DC..0x1F4: retail puts `lui %hi(D_00239BA0)` early, then `addu a0,a0,s6`.
 *  - D_00139688 block (+0x400..): retail rebuilds HIGH/LO_SUM of D_00139568 and D_00239BA0 and
 *    0xE0 in the block, using mult+addu. Mine gets an adjacent la plus mtlo/madd: the base reaches
 *    combine from outside the block. Retail also uses bnel there, where mine uses bnez.
 *  - +0x468 bne versus bnel, and register choice in the final float-pair copies (+0x484..).
 * `__attribute__((nosda))` on the arrays changed nothing. All declarations follow.
 */
#include "include_asm.h"
extern void func_00335E68(void *, int);
extern int *func_00335E20(void *);
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
extern MenuGridEntry D_00259F58[];
extern unsigned char D_001A7AF8[];
extern unsigned char D_00139568[];
extern int D_00139868[];
extern int D_00139688[];
extern MenuWeaponRecord D_00239BA0[];
extern void *D_001F2880[];
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
                    int *color = func_00335E20(o + 0x3B4);
                    float y = D_L00_001AE75C * (float)row + (float)D_L00_001AE754;
                    float x = D_L00_001AE758 * (float)col + (float)D_L00_001AE750;
                    *color = 0x60241700;
                    func_00335E38(o + 0x3B4, x + (*(float **)(o + 0x498))[0], y + (*(float **)(o + 0x498))[1], 0.0f, 0.0f);
                }
                *func_00335E20(o + 0x98) = 0x30F0C070;
                *func_00335E20(o + 0x3B4) = 0x60241700;
                func_00335E68(o + 0x1C8, 0);
                func_00335E68(o + 0x378, 0);
                if (D_001A7AF8[D_00259F58[row * 4 + col].id]) {
                    MenuWeaponRecord *weapons = D_00239BA0;
                    unsigned char *slots = D_00139568;
                    func_00336920(o + 0x378, weapons[slots[D_00259F58[row * 4 + col].id]].icon, 0);
                    func_00335E38(o + 0x378, (53.5f * (float)col + (float)D_L00_001AE748) + (*(float **)(o + 0x498))[0],
                                  (47.0f * (float)row + (float)D_L00_001AE74C) + (*(float **)(o + 0x498))[1], 0.0f, 0.0f);
                    if (weapons[slots[D_00259F58[row * 4 + col].id]].icon == 0xEA7E) {
                        *func_00335E20(o + 0x378) = 0x80808080;
                    } else {
                        MenuIntegerPair base = D_L00_001AE588[0], stride = D_L00_001AE590[0];
                        int a, b, color;
                        func_00336B90(o + 0x1C8, weapons[slots[D_00259F58[row * 4 + col].id]].max);
                        func_00336B98(o + 0x1C8, D_00139868[D_00259F58[row * 4 + col].id] >> 5);
                        func_00335E38(o + 0x1C8, (float)(stride.x * col + base.x) + (*(float **)(o + 0x498))[0],
                                      (float)(stride.y * row + base.y) + (*(float **)(o + 0x498))[1], 0.0f, 0.0f);
                        func_00335E68(o + 0x1C8, 1);
                        a = 0;
                        b = 0;
                        color = 0;
                        func_00336F20(D_00259F58[row * 4 + col].id, &a, &b, &color);
                        func_00336498(o + 0x1C8, a, b);
                        *func_00335E20(o + 0x378) = color;
                        if (weapons[slots[D_00259F58[row * 4 + col].id]].max <= 0) {
                            func_00336B90(o + 0x1C8, 100);
                            func_00336B98(o + 0x1C8, 100);
                        }
                    }
                    if (D_00139688[D_00259F58[row * 4 + col].id] == 0) {
                        if (D_00239BA0[D_00139568[D_00259F58[row * 4 + col].id]].ammo != 0) *func_00335E20(o + 0x3B4) = 0x60202080;
                    }
                    func_00335E68(o + 0x378, 1);
                    if (func_0026F7A0(D_001F2880[0]) == D_00259F58[row * 4 + col].id) *func_00335E20(o + 0x98) = 0x80FFDE8D;
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
