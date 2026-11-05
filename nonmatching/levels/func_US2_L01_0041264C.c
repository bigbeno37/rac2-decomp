/* func_US2_L01_0041264C (opus-1, task v2818): CATALOGUE, not a C near miss.
 *
 * check.py: 184 bytes, expected 188; 0 of 6 places. The 188-byte row starts with one padding
 * word. Placed at row address + 4, this C gives Placement.derive() no difference at all six
 * places: 184/184 bytes.
 *
 * The row's first word is 0 in every program, right after the previous function's
 * `jr $ra` delay slot. The body starts 8-aligned at +4, and each program holds one raw
 * pointer to +4 and no raw pointer, jal or lui+addiu to the row address. Bytes at +4
 * fingerprint as 35f18188e1353eaf, the same as func_US2_L07_0040A6A0's 184-byte row.
 *
 * Unblock: the catalogue must start this row at +4 (size 184), which would join it to the
 * L07 row. C cannot produce the row as it stands: a function section is 8-aligned and these
 * places are 4 mod 8.
 *
 * Flags: this draft was compiled at the default flags, in place of the marker.
 * Declarations: the externs below. Symbols are v1.01 names from func_L01_004110E0; a merged
 * row would use the ref_L07_00409300_NN names in src/us2/levels/shared/L07_0040A6A0-g8.c.
 */
#include "common.h"

extern s32 D_L01_001A8F40 __attribute__((sda));
extern s32 D_001A79F0 __attribute__((sda));

extern void func_002A7750(s8 *, s32, s32);
extern void func_L00_00326048(s8 *, s8 *, f32, s32, s32);

void func_US2_L01_0041264C(s8 *arg0) {
    s8 *s;
    u8 t;

    t = *(u8 *)(arg0 + 0x20);
    s = *(s8 **)(arg0 + 0x68);
    switch (t) {
    case 0:
        func_002A7750(arg0, 1, -1);
        return;
    case 1:
        if (D_001A79F0 == 0x33) {
            return;
        }
        if (s != 0) {
            if (D_L01_001A8F40 == 2) {
                func_L00_00326048(arg0, s + 0xC, 0.055268768f, (s32)0x80AABED2u, (s32)0x80466464u);
            } else {
                func_L00_00326048(arg0, s + 0xC, 0.037815473f, (s32)0x8096AAAAu, (s32)0x80466464u);
            }
        }
        break;
    }
}
