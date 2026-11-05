/* func_US2_002D6024, the v2.00 copy of func_002D53F0 (tier: opus-1, task v2882).
 *
 * Status: CATALOGUE, not a C problem. This C is exact for the real function.
 * Best check.py result: 188 bytes, expected 192, 0 of 28 places (in place in
 * src/us2/game/002D5F10.c). The same object passes build.Placement.derive at the catalogued
 * address + 4 in 28 of 28 places (boot and every level), with every symbol at the catalogue's
 * address. Not linked: no build.py run covers it.
 *
 * What differs: only the row. The catalogue gives 0x2D6024, 192 bytes, but the word there is 0,
 * the alignment nop after func_US2_002D5F10 (0x2D5F10, 276 bytes), whose jr $ra and delay slot end
 * at 0x2D6020. The real entry is 0x2D6028 (addiu $sp, $sp, -0x10), 8-aligned as its neighbours
 * are. In the boot image one data word (0x1AAB3C) points at 0x2D6028; nothing calls, points at or
 * branches to 0x2D6024. build.py accepts zero padding after a function only, so no C can land here.
 *
 * What would unblock it, not tried here (tools/ is off limits to workers):
 * "func_US2_002D6024": (192, 4) in FUNCTION_ENTRY_OFFSETS in tools/levels.py, then the catalogue
 * regenerated, leaving a 188-byte body at 0x2D6028. Put this C in place of the marker in
 * src/us2/game/002D5F10.c, with the file's #include "common.h" above it.
 *
 * What the wording settles:
 * - func_0027F980 is declared void, its real type (src/game/0027F980.c). The template's callee
 *   returns int; copying that here swaps the $a1 and $a3 moves before the draw call.
 * - The inline helper, as in src/game/002D3390.c, gives retail's $f1/$f2. Written out in the
 *   function, the 329.0f and 0.5f registers swap.
 * - The template's one-pass group is not needed: the join after the two arms is the block boundary.
 *
 * Needs, besides common.h: func_0027C540 takes long here because the unit declares it so below.
 */
#include "common.h"

extern float D_001B23A4[] __attribute__((sda));
extern int D_001B23A0[] __attribute__((sda));
extern void func_0027C540(long);
extern void func_US2_0029D368(void);
extern s32 func_00288FB8(s32);
extern void func_0027F980(int, int, u64, u8 *, int);
extern void func_0027C660(void);

/* func_US2_002D6024, from func_002D53F0: a text draw whose y also moves in display mode 1. */
static __inline__ int scaled_screen_coord(float value, float offset)
{
    float scaled = value * D_001B23A4[0] + 0.5f;
    if (D_001B23A0[0] == 1)
        return (int)(scaled + offset);
    return (int)scaled;
}

int func_US2_002D6024(void)
{
    int y;

    func_0027C540(0);
    func_US2_0029D368();
    y = scaled_screen_coord(329.0f, -36.0f);
    func_0027F980(0x1C7, y, 0x80F0F0F0ULL, (u8 *)func_00288FB8(0x2BE5), -1);
    func_0027C660();
    return 0;
}
