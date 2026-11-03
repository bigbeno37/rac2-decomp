/* Tier: opus-1 (Claude Opus 5.5 xhigh), task v2266, 3 November 2026. USA v1.01, FAILED, not landable.
 * Unit: a new -G8 unit of its own (e.g. src/levels/15_gorn/L15_00430168-g8.c, default -O2 -G8
 * -ffunction-sections); marker removed from L15_00315428.c while testing.
 * Best check.py: 1064/1064 bytes, 0 of 1 places, first difference +0x94, about 25 real rows once
 * relocations are masked (the previous draft here was 1064 bytes with ~110 rows). Ported from the L10
 * twin's draft (nonmatching/levels/func_L10_004239A0.c, read its header: the same function).
 * NOT LANDABLE: it uses `if (D_L15_001B2E80 == 1) do { <dead store> } while (0);` after the two 200-row
 * roundings and the 210-row rounding. Retail needs, at those three points, a basic-block boundary when
 * flow1 runs (200.0/1.0/0.0/0x8710 and the compare's 1 become global, so alpha f20 and 0.5 f21 are
 * allocated first; cse2 links the later real `== 1` compares to this 1: retail's early addiu $s0,1 and
 * addiu $s2,1) and a sched1+sched2 barrier with no code (haifa: only LOOP/EH/SETJMP notes or volatile asm).
 * Without the wrapper (plain if with a dead store) the split dies at flow2 and sched2 interleaves (L10: 55 rows).
 * Still different with the wrapper: 1.0/0.0 in f23/f22 (retail f22/f23); row 1 `ori $a1` vs `mov.s $f14`
 * order; after each func_001157AC (46-, 131-, 210-row) retail moves the length last (into the jal slot);
 * 210-row and first 294-row f0/f1 roles swapped. Declarations: D_L15_001A904C / D_L15_001B2E80 /
 * D_L15_001B2E84 / D_L15_001B36A4 plain (lui), the two tables through SDATA views ($gp addiu), the colour
 * of func_L05_00303000 is 64-bit in retail (ori/dsll/ori in $t0), so its 5th parameter is declared u64
 * here although L05_003008C0.c defines it as int (the callee only forwards it).
 */
#include "common.h"

extern int D_001A7340;
extern unsigned char D_001A7A0A;
extern char *D_L15_001A904C;
extern int D_L15_001B15C8 __attribute__((sda));
extern int D_L15_001B1520[] SDATA(D_L15_001B1520);
extern int D_L15_001B15D8[][3] SDATA(D_L15_001B15D8);
extern int D_L15_001B2E80;
extern float D_L15_001B2E84;
extern int D_L15_001B36A4;
extern int func_001157AC(char *);
extern int func_00115DA8(char *, char *, ...);
extern void func_0027F5B8(int, int, u64, char *, int);
extern int func_0027FB40(int, int, u64, char *, int);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern char *func_00288FB8(int);
extern void func_00300968(u8 *, int, float, float, float, float);
extern u8 *func_00336D50(char *, int);
extern void func_L05_00303000(int, int, int, int, u64, char *, int);

s32 func_L15_00430168(void) {
    char buf[128];
    f32 center = func_00283CE0(D_001A7340) * 0.5f;
    u8 *sprite;
    char *text;
    s32 x, y, count, index, sy;
    f32 scaled, fy;
    if (D_L15_001A904C != 0) {
        sprite = func_00336D50(D_L15_001A904C + 0x8710, 0x45);
        sy = (s32)(D_L15_001B2E84 * 200.0f + 0.5f);
        fy = sy;
        if (D_L15_001B2E80 == 1) do { scaled = sy - 2; } while (0);
        func_00300968(sprite, 0x60442D00, center, fy, 1.0f, 0.0f);
        sprite = func_00336D50(D_L15_001A904C + 0x8710, 0x44);
        sy = (s32)(D_L15_001B2E84 * 200.0f + 0.5f);
        fy = sy;
        if (D_L15_001B2E80 == 1) do { scaled = sy - 2; } while (0);
        func_00300968(sprite, 0x55F0C070, center, fy, 1.0f, 0.0f);
        func_00115DA8(buf, func_00288FB8(0x2F72), D_L15_001B15C8 + 1);
        text = func_00288FB8(0x2F93);
        x = func_00283CF0(center);
        scaled = D_L15_001B2E84 * 46.0f + 0.5f;
        if (D_L15_001B2E80 == 1)
            y = (s32)(scaled + -2.0f);
        else
            y = (s32)scaled;
        func_0027FB40(x, y, 0x80F0F0F0, text, func_001157AC(text));
        text = func_00288FB8(0x31CB);
        y = (s32)(D_L15_001B2E84 * 131.0f + 0.5f);
        func_L05_00303000(117, y, 280, 400, 0x80F0F0F0, text, func_001157AC(text));
        index = D_L15_001B1520[D_L15_001B36A4] - 1;
        if (index >= 3) index = 2;
        count = D_L15_001B15D8[D_L15_001B36A4][index];
        if (D_001A7A0A != 0) count *= 10;
        text = buf;
        if (count == 0) func_00115DA8(buf, func_00288FB8(0x11FF));
        else func_00115DA8(buf, func_00288FB8(0x2FC6), count);
        y = (s32)(D_L15_001B2E84 * 210.0f + 0.5f);
        if (D_L15_001B2E80 == 1) do { scaled = D_L15_001B2E84 * 210.0f + 0.5f + -2.0f; } while (0);
        func_L05_00303000(117, y, 280, 400, 0x80F0F0F0, text, func_001157AC(text));
        scaled = D_L15_001B2E84 * 294.0f + 0.5f;
        if (D_L15_001B2E80 == 1) y = (s32)(scaled + 3.0f);
        else y = (s32)scaled;
        func_0027F5B8(117, y, 0x80F0F0F0, func_00288FB8(0x31CC), -1);
        scaled = D_L15_001B2E84 * 294.0f + 0.5f;
        if (D_L15_001B2E80 == 1) y = (s32)(scaled + 3.0f);
        else y = (s32)scaled;
        func_0027F5B8(274, y, 0x80F0F0F0, func_00288FB8(0x31CD), -1);
    }
    return 1;
}
