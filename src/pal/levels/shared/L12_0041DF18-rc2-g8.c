#include "common.h"

/* func_PAL_L12_0041DF18, the PAL copy of func_L01_004110E0 in levels 12, 14, 17 and 19: the same C,
 * where both float arguments are scaled by 6/5 and case 1 first leaves when D_001A79F8 (the current
 * area) is 0x33. func_PAL_L07_0040E120 is the same function for levels 7, 8, 9 and 26. */

extern s32 D_001A79F8 MACRO_ADDR;
extern s32 D_L01_001A8F40 __attribute__((sda));

extern void func_002A7750(s8 *, s32, s32);
extern void func_L00_00326048(s8 *, s8 *, f32, s32, s32);
