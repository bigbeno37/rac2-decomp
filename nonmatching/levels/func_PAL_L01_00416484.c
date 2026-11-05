/* func_PAL_L01_00416484, the PAL copy of func_L01_004110E0: the same C, where both float
 * arguments are scaled by 6/5 and case 1 first leaves when D_001A79F8 (the current area) is 0x33.
 *
 * Tier: opus-1 (v2669). Result: CATALOGUE, not a C problem. This C is exact once the row is fixed.
 * check.py: 184 bytes, expected 188, 0 of 6 places. placement.derive() at each place + 4: OK at 6 of 6.
 * What differs: only the row. Its 188 bytes start with the alignment nop after the previous
 * function's completed return (jr $ra; addiu $sp,$sp,N; nop), and every place is 4 mod 8, while
 * this section has 8-byte alignment. The real entry is place + 4, size 184:
 * 01:00416488,02:0043FC48,04:00451868,06:0045B2D8,13:00422278,16:00419A60.
 * To land after the catalogue is regenerated: src/pal/levels/shared/L01_00416484-rc2-g8.c with this
 * C, config/cflags.txt flags -O2 -G8 -ffunction-sections -fno-builtin, and the marker removed from
 * src/pal/levels/shared/L01_00416484.c. It needs no other declarations.
 */
#include "common.h"

extern s32 D_001A79F8 MACRO_ADDR;
extern s32 D_L01_001A8F40 __attribute__((sda));

extern void func_002A7750(s8 *, s32, s32);
extern void func_L00_00326048(s8 *, s8 *, s32, s32, f32);

void func_PAL_L01_00416484(s8 *arg0) {
    s8 *s;
    u8 t;

    t = *(u8 *)(arg0 + 0x20);
    s = *(s8 **)(arg0 + 0x68);
    switch (t) {
    case 0:
        func_002A7750(arg0, 1, -1);
        return;
    case 1:
        if (D_001A79F8 == 0x33) {
            break;
        }
        if (s != 0) {
            if (D_L01_001A8F40 == 2) {
                func_L00_00326048(arg0, s + 0xC, 0x80AABED2, 0x80466464, 0.06632252f);
            } else {
                func_L00_00326048(arg0, s + 0xC, 0x8096AAAA, 0x80466464, 0.04537857f);
            }
        }
        break;
    }
}
