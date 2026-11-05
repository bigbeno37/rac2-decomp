/*
 * func_PAL_L07_0040E120: strict build rejected it: D_001A79F8 needs two values
 */
void func_PAL_L07_0040E120(s8 *arg0) {
    s8 *s;
    u8 t;

    t = *(u8 *)(arg0 + 0x20);
    s = *(s8 **)(arg0 + 0x68);
    switch (t) {
    case 0:
        ref_L07_00409300_01(arg0, 1, -1);
        return;
    case 1:
        if (D_001A79F8 == 0x33) {
            break;
        }
        if (s != 0) {
            if (ref_L07_00409300_00 == 2) {
                ref_L07_00409300_02(arg0, s + 0xC, 0x80AABED2, 0x80466464, 0.06632252f);
            } else {
                ref_L07_00409300_02(arg0, s + 0xC, 0x8096AAAA, 0x80466464, 0.04537857f);
            }
        }
        break;
    }
}
