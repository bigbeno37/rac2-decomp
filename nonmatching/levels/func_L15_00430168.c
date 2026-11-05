/* Tier: opus-2 (Claude Opus 5.5 xhigh), task v2622, 5 November 2026. USA v1.01 only: this function is in
 * neither the PAL nor the v2.00 catalogue. FAILED, not landable as written.
 * Unit: a new -G8 unit of its own (for example src/levels/15_gorn/L15_00430168-g8.c, default -O2 -G8
 * -ffunction-sections), with the marker taken out of L15_00315428.c while testing.
 * Best check.py: 1064/1064 bytes, 0 of 1 places, 2 real rows differ once relocations are masked (the opus-1
 * draft had about 25). They are one swap in row 1: retail `ori $a1` then `mov.s $f14,$f22`, here the reverse.
 * NOT LANDABLE: four `if (D_L15_001B2E80 == 1) do { <dead store> } while (0);` wrappers, one of them an extra
 * test after row 1's draw that has no retail evidence. func_L05_00303000 is declared int here while its
 * definition in src/levels/shared/L05_003008C0.c is void (it ends in a call to the int func_L05_00302CE8, so
 * an int result may be right; the definition was not changed or recompiled that way).
 *
 * Each point below moved real rows in a compile. The reasons given for them are read from the dumps or inferred.
 * - This final.c gives .p2align 3 to the first label after ANY NOTE_INSN_LOOP_BEG; it does not stop at LOOP_END.
 *   Retail's label at 0x004303D4 is not 8-aligned, so no group may open between the 46-row join and that label.
 *   A group around a 46-row or 131-row draw alone adds padding nops (1072 bytes).
 * - The group from before the first func_00115DA8 to the end of the 131 rounding gives retail's
 *   a3 a1 | a0 a2 | t0 at the 46-row. Hand count of sched2 dependents with the barrier after the 131 rounding:
 *   a1 and a3 4, a0, a2 and t0 3. y46 must be its own local; with y reused, a1 comes before a3.
 * - Groups double a pseudo's refs in global-alloc, so never group a sprite draw (1.0/0.0 pass 0.5), 0x8710
 *   (extra save of $s3) or, by the priority formula, 200.0. Setting k131 in a local gives K f2 and 0.5 f1 inside
 *   the group; it must not share k with the 210 row.
 * - 210 and first 294 rows: `k = K; scaled = scale; scaled *= k; scaled += 0.5f` makes scaled global in f1 and
 *   leaves K alone in f0. Setting K first also keeps the l.s out of the `b` delay slot after the count test.
 * - func_L05_00303000 returning int: the call then sets v0, which drops insn t2<-v0's anti-dependent and gives
 *   retail's t2 in the delay slot.
 * - func_00300968(x, y, sx, sprite, color, sy) keeps both register banks in order and gives 1.0/0.0 in f22/f23.
 *   Retail's colours need their live lengths tied (0.0 is one insn shorter otherwise); the a0 copy between f14
 *   and f15 in row 2 is the likely reason, not read from the dump.
 * - The test after row 1's draw is a barrier before `addiu $a1,0x44`, which ties the row-1 argument moves.
 *   Retail most likely has a code-free construct here, such as an empty do/while(0) macro.
 * Still open: row 1's order is a sched2 tie settled by sched1's order. With this prototype f14 comes before the
 * li of a1. func_00300968(u8 *, float, int, float, float, float) has a1 right but 1.0/0.0 in f23/f22 (6 rows);
 * adding `zero = 0.0f;` before row 1's draw fixes both but loads 0.0 before 1.0 (3 rows).
 * Tried without gain: u64 colour (row 2's lui moves), colour in a local (tie), length locals (tie), identical
 * arms at the 200 rows (a real join loses the shared 200.0, 1.0, 0.0, 0x8710 and 1 in cse), a real block after
 * the 46-row draw (a0 a1 a3 order). Declarations: D_L15_001A904C, D_L15_001B2E80, D_L15_001B2E84 and
 * D_L15_001B36A4 plain (lui), the two tables through SDATA views ($gp addiu), and the colour of
 * func_L05_00303000 is 64-bit in retail (ori/dsll/ori) although L05_003008C0.c declares it int.
 * The L10 twin func_L10_004239A0 has the same body, so this form should port to it.
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
extern void func_00300968(float, float, float, u8 *, int, float);
extern u8 *func_00336D50(char *, int);
extern int func_L05_00303000(int, int, int, int, u64, char *, int);

s32 func_L15_00430168(void) {
    char buf[128];
    f32 center = func_00283CE0(D_001A7340) * 0.5f;
    u8 *sprite;
    char *text;
    s32 x, y, y46, y131, count, index, sy;
    f32 scaled, fy, k, k131;
    if (D_L15_001A904C != 0) {
        sprite = func_00336D50(D_L15_001A904C + 0x8710, 0x45);
        sy = (s32)(D_L15_001B2E84 * 200.0f + 0.5f);
        fy = sy;
        if (D_L15_001B2E80 == 1) do { scaled = sy - 2; } while (0);
        func_00300968(center, fy, 1.0f, sprite, 0x60442D00, 0.0f);
        if (D_L15_001B2E80 == 1) do { scaled = sy - 2; } while (0);
        sprite = func_00336D50(D_L15_001A904C + 0x8710, 0x44);
        sy = (s32)(D_L15_001B2E84 * 200.0f + 0.5f);
        fy = sy;
        if (D_L15_001B2E80 == 1) do { scaled = sy - 2; } while (0);
        func_00300968(center, fy, 1.0f, sprite, 0x55F0C070, 0.0f);
        do {
            func_00115DA8(buf, func_00288FB8(0x2F72), D_L15_001B15C8 + 1);
            text = func_00288FB8(0x2F93);
            x = func_00283CF0(center);
            scaled = D_L15_001B2E84 * 46.0f + 0.5f;
            if (D_L15_001B2E80 == 1)
                y46 = (s32)(scaled + -2.0f);
            else
                y46 = (s32)scaled;
            func_0027FB40(x, y46, 0x80F0F0F0, text, func_001157AC(text));
            text = func_00288FB8(0x31CB);
            k131 = 131.0f;
            y131 = (s32)(D_L15_001B2E84 * k131 + 0.5f);
        } while (0);
        func_L05_00303000(117, y131, 280, 400, 0x80F0F0F0, text, func_001157AC(text));
        index = D_L15_001B1520[D_L15_001B36A4] - 1;
        if (index >= 3) index = 2;
        count = D_L15_001B15D8[D_L15_001B36A4][index];
        if (D_001A7A0A != 0) count *= 10;
        text = buf;
        if (count == 0) func_00115DA8(buf, func_00288FB8(0x11FF));
        else func_00115DA8(buf, func_00288FB8(0x2FC6), count);
        k = 210.0f;
        scaled = D_L15_001B2E84;
        scaled *= k;
        scaled += 0.5f;
        if (D_L15_001B2E80 == 1) do { fy = scaled + -2.0f; } while (0);
        y = (s32)scaled;
        do {
            func_L05_00303000(117, y, 280, 400, 0x80F0F0F0, text, func_001157AC(text));
        } while (0);
        k = 294.0f;
        scaled = D_L15_001B2E84;
        scaled *= k;
        scaled += 0.5f;
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
