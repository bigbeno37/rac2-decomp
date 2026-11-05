/* func_PAL_L00_0030BA80, the PAL copy of func_00298BB8: the same C, unchanged.
 *
 * Tier: opus-1 (Claude Opus 5.5).
 * check.py --version pal: 0 of 27 places, "340 bytes, expected 336". Not a wording problem:
 * every instruction agrees, and the only difference is the catalogue's extent.
 *
 * What differs: the catalogue ends this function 4 bytes early. Retail has jr $ra at +0x14C and
 * its delay slot, addiu $sp,$sp,0x20, at +0x150 (0x30BBD0 in level 00). That word opens the next
 * row, func_L00_00309DB0, at func+336 in all 27 levels. C cannot produce the 336-byte extent: a
 * jr needs its delay slot. docs/MATCHING-BLOCKERS.md records the same PAL exception ("all 27 PAL
 * copies begin in the preceding return's delay slot").
 *
 * Measured with this C and the row stretched to 340 bytes in memory only (build.Placement.derive
 * over all 340 bytes, relocations included; corrupting +0x150 is rejected, so the last word is
 * compared): 27 of 27 places, both in a -G8 unit (template flags, -O2 -G8 -ffunction-sections)
 * and in place in src/pal/levels/shared/L00_0030BA48.c at the default flags. This is not a linked
 * build: no program has been linked with it.
 *
 * Unblock: settle the PAL extents of this row and func_L00_00309DB0 (340 bytes here, the residue
 * row 24 bytes from func+340), including the aligned PAL level 20 word the blockers entry names.
 * Then this C lands in place of the INCLUDE_ASM marker, with the default flags.
 *
 * Needs only the declarations below.
 */

extern unsigned char D_001A7930[];
extern unsigned char D_001A7948[];
extern unsigned char D_001A7960[];
extern unsigned char D_001A7980[];
extern unsigned char D_001A79A0[];
extern unsigned char D_001A79D0[];
extern void func_00115AC0(void *, int, int);

void func_PAL_L00_0030BA80(unsigned char *source)
{
    int i;
    int value = source[0x12];
    if (value == 0x45)
        D_001A7930[2] = value;
    else if (value == 0x50)
        D_001A7930[2] = 0x49;
    for (i = 3; i < 7; i++)
        D_001A7930[i] = source[i + 0xd];
    for (i = 8; i < 11; i++)
        D_001A7930[i] = source[i + 0xd];
    for (i = 11; i < 13; i++)
        D_001A7930[i] = source[i + 0xe];
    func_00115AC0(D_001A7948, (int)D_001A7930, 13);
    func_00115AC0(D_001A7960, (int)D_001A7930, 13);
    func_00115AC0(D_001A7980, (int)D_001A7930, 13);
    func_00115AC0(D_001A79A0, (int)D_001A7930, 13);
    func_00115AC0(D_001A79A0 + 0x14, (int)D_001A7930, 13);
    func_00115AC0(D_001A79D0, (int)D_001A7930, 13);
}
