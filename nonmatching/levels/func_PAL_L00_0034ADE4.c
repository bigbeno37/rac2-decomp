/* func_PAL_L00_0034ADE4, tier opus-1 (Claude Opus 5.5). Status: CATALOGUE, not a C difference.
 *
 * Best check.py result: "92 bytes, expected 96", 0 of 27 places. That is the whole difference:
 * the row starts one word early. Its first word, at 0x34ADE4 in level 00, is a zero word at
 * 4 mod 8 in all 27 places, the fill before an 8-aligned function, and the row's end leaves a
 * one-word gap before the next function. The function really starts at row address + 4
 * (0x34ADE8), 92 bytes long.
 *
 * Measured with a one-off script calling build.Placement.derive(obj, section, address + 4),
 * not through check.py: this exact C equals retail at all 27 places, relocations included.
 * Not linked: no build can place it until the row moves.
 *
 * What unblocks it: move the row (start + 4, size 92). No C can supply the word, since a
 * compiled function starts with its prologue at an 8-aligned section start.
 *
 * To land: put it in src/pal/levels/shared/L00_0034ADE4-g8.c and delete the INCLUDE_ASM line
 * from L00_0034ADE4.c. The -g8 name gives -O2 -G8 -ffunction-sections, so no cflags line is
 * needed. Both data words move per level (5 distinct addresses), hence the ref_ names. */
#include "common.h"

extern void func_0027C540(int);
extern void func_PAL_L00_0030F458(void);
extern s32 func_00288FB8(s32);
extern void func_0027F980(int, int, u64, u8 *, int);
extern void func_0027C660(void);
extern int ref_PAL_L00_0034ADE4_00 __attribute__((sda));
extern int ref_PAL_L00_0034ADE4_01[] __attribute__((sda));

/* func_PAL_L00_0034ADE4, the PAL copy of func_002CF2B8: the same C, where the
 * text position comes from two PAL words in place of 0x1A5 and the scaled 256,
 * the string id becomes 0xB60, and the draw call is func_0027F980. Retail reads
 * the first word through $gp and the second through lui, hence the two
 * declaration forms, as in func_PAL_002CE388. */
int func_PAL_L00_0034ADE4(void)
{
    func_0027C540(0);
    func_PAL_L00_0030F458();
    func_0027F980(ref_PAL_L00_0034ADE4_00, ref_PAL_L00_0034ADE4_01[0], 0x80F0F0F0ULL,
                  (u8 *)func_00288FB8(0xB60), -1);
    func_0027C660();
    return 0;
}
