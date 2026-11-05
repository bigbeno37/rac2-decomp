/* func_PAL_002D022C (catalogue name): opus-1, task v2963. NOT a matchable row: CATALOGUE defect.

   The C below is right; the row's extent is not. The row says 0x002D022C, 96 bytes, but its first word is
   padding: 0x002D0224/28 are the previous function's `jr $ra` and delay slot, 0x002D022C is 0x00000000, and
   the function opens at 0x002D0230 with `addiu $sp,$sp,-0x10`. The only reference in the boot program is the
   data pointer at 0x001AAC2C, which holds 0x002D0230; nothing refers to 0x002D022C.

   Best check.py result: 92 bytes, expected 96, 0 of 1 places. No C can emit the leading word.
   With the extent 0x002D0230, 92 bytes, supplied in memory to check.py's own comparison (no file changed):
   `boot: OK`, `1 of 1 places match`. That is a dry run under -O2 -G8 -ffunction-sections, not a linked match.

   To unblock: add "func_PAL_002D022C": (96, 4) to FUNCTION_ENTRY_OFFSETS in tools/levels.py and rerun the
   catalogue, then put this in a unit of its own, src/pal/game/002D0230-g8.c. Do not use 002D022C-g8.c
   untested: that name is taken by func_PAL_002D0290. */
#include "common.h"

extern void func_0027C540(int);
extern void func_0027C660(void);
extern s32 func_00288FB8(s32);
extern void func_PAL_0029CEC8(void);
extern int func_0027F980(int v, int p7, unsigned long long k, char *r, int m1);
extern int D_PAL_001ABA98 __attribute__((sda));
extern int D_PAL_001ABA9C[] __attribute__((sda));

/* func_PAL_002D022C, the PAL copy of func_002CF2B8: the same C, where the text
 * position comes from two PAL words in place of 0x1A5 and the scaled 256, and
 * the string id becomes 0xB60. */
int func_PAL_002D022C(void)
{
    func_0027C540(0);
    func_PAL_0029CEC8();
    func_0027F980(D_PAL_001ABA98, D_PAL_001ABA9C[0], 0x80F0F0F0ULL, (char *)func_00288FB8(0xb60), -1);
    func_0027C660();
    return 0;
}
