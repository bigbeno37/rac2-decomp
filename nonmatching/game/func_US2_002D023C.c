/* func_US2_002D023C, the v2.00 relative of func_002CF2B8 (tier: opus-1, task v2962).
 *
 * Status: CATALOGUE, not a C problem. This C is exact for the real function.
 * Best check.py result: 92 bytes, expected 96, 0 of 1 places (as src/us2/game/002D023C-g8.c).
 * The same object passes build.Placement.derive (scratch script, tools unchanged) at the row
 * address + 4 in boot, and at row + 4 of func_US2_L00_00349AAC in 27 of 27 levels, the level
 * copy of this body with the same leading nop. Not linked: no build.py run covers it.
 *
 * What differs: only the row. The catalogue gives 0x2D023C, 96 bytes, but the word there is 0,
 * the alignment nop after func_US2_002D0158 (jr $ra at 0x2D0234, delay slot 0x2D0238). The real
 * entry is 0x2D0240 (addiu $sp, $sp, -0x10), 8-aligned: data word 0x1AABBC points at it, and
 * nothing calls, points at or builds 0x2D023C.
 *
 * What would unblock it, not tried here (tools/ is off limits to workers):
 * "func_US2_002D023C": (96, 4) in FUNCTION_ENTRY_OFFSETS in tools/levels.py, the same for
 * func_US2_L00_00349AAC, then the catalogue rerun. To land it: save below the marker as
 * src/us2/game/002D023C-g8.c (its name gives it -G8; no config/cflags.txt line is needed) and
 * delete the INCLUDE_ASM line from src/us2/game/002D0158.c. The level row needs the same C.
 *
 * Against the template: the unit is -G8, not -G0, because x is $gp-relative. x and y are
 * v2.00-only data (0x1C7 and 0x100) instead of 0x1A5 and the scaled 256.0f; the pair is not
 * in v1.01's data and these are their only references in the boot text. The callee is
 * func_0027F980 instead of func_0027FAA8, the text id 0x2BE5 instead of 0x2D4F, and
 * func_0029C530 is v2.00's own func_US2_0029D368.
 */
#include "common.h"

extern void func_0027C540(int);
extern void func_US2_0029D368(void);
extern s32 func_00288FB8(s32);
extern int func_0027F980(int, int, u64, u8 *, int);
extern void func_0027C660(void);
extern int D_US2_001ABA28 __attribute__((sda));
extern int D_US2_001ABA2C;

int func_US2_002D023C(void)
{
    func_0027C540(0);
    func_US2_0029D368();
    func_0027F980(D_US2_001ABA28, D_US2_001ABA2C, 0x80F0F0F0ULL, (u8 *)func_00288FB8(0x2BE5), -1);
    func_0027C660();
    return 0;
}
