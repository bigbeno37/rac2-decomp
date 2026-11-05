/* func_US2_002D5A44, the v2.00 copy of func_002D4E48 (tier: opus-1, task v2856).
 *
 * Status: CATALOGUE, not a C problem. This C is exact for the real function.
 * Best check.py result: 448 bytes, expected 452, 0 of 28 places (src/us2/game/002D5A44-g8.c).
 * The same object passes build.Placement.derive at the catalogued address + 4 in 28 of 28
 * places (boot and every level). Not linked: no build.py run covers it.
 *
 * What differs: only the row. The catalogue gives 0x2D5A44, 452 bytes, but the word there is 0,
 * the alignment nop after func_002D4E10 (0x2D5A10, 52 bytes). The real entry is 0x2D5A48
 * (addiu $sp, $sp, -0x30), which is 8-aligned as its neighbours are; v1.01 has the
 * same layout with the nop outside both rows (func_002D4E48 at 0x2D4E48).
 *
 * What would unblock it, not tried here (tools/ is off limits to workers):
 * "func_US2_002D5A44": (452, 4) in FUNCTION_ENTRY_OFFSETS in tools/levels.py, then the catalogue
 * rerun. To land it: save below the marker as src/us2/game/002D5A44-g8.c, delete the INCLUDE_ASM
 * line from src/us2/game/002D5A44.c, and append to config/cflags.txt:
 *   src/us2/game/002D5A44-g8.c	-O2 -G8 -fno-expensive-optimizations -ffunction-sections -fno-builtin
 * Those flags matter: without -fno-expensive-optimizations cse also shares 1.0f in $f21, which
 * retail reloads per call; with them, the shared 0.0f in $f20 needs the zero local.
 */
#include "common.h"

extern void func_0027C540(s32);
extern void func_0027C660(void);
extern void func_0027F980(s32, s32, u64, s32, s32);
extern void func_0027FAA8(s32, s32, u64, s32, s32);
extern s32 func_00288FB8(s32);
extern void func_00300A00(s32, s32, f32, f32, f32, f32, f32);
extern s32 func_00336D50(s32, s32);
extern s32 D_001A8CFC;
extern s32 D_001ABA8C;
extern s32 D_001ABA94;
extern s32 D_001ABA9C;
extern f32 D_001B23A8;
extern s32 D_001ABA90 __attribute__((sda));
extern s32 D_001ABA88 __attribute__((sda));
extern s32 D_001ABA98 __attribute__((sda));

s32 func_US2_002D5A44(void) {
    s32 s0;
    s32 s1;
    f32 zero;

    func_0027C540(0);
    if (D_001A8CFC != 0) {
        zero = 0.0f;
        func_00300A00(func_00336D50(D_001A8CFC + 0x8710, 0x96), 0x60442D00, (f32)D_001ABA90, (f32)D_001ABA94, 1.0f, D_001B23A8, zero);
        func_00300A00(func_00336D50(D_001A8CFC + 0x8710, 0x97), 0x60241700, (f32)D_001ABA90, (f32)D_001ABA94, 1.0f, D_001B23A8, zero);
        func_00300A00(func_00336D50(D_001A8CFC + 0x8710, 0x98), 0x55F0C070, (f32)D_001ABA90, (f32)D_001ABA94, 1.0f, D_001B23A8, zero);
        s1 = D_001ABA90;
        s1 = s1 + D_001ABA88;
        s0 = D_001ABA94;
        s0 = s0 + D_001ABA8C;
        func_0027FAA8(s1, s0, (u64)0x80F0F0F0, func_00288FB8(0x2BF8), -1);
        s1 = D_001ABA90;
        s1 = s1 + D_001ABA98;
        s0 = D_001ABA94;
        s0 = s0 + D_001ABA9C;
        func_0027F980(s1, s0, (u64)0x80F0F0F0, func_00288FB8(0x2BE5), -1);
    }
    func_0027C660();
    return 0;
}
