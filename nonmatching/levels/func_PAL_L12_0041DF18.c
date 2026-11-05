/*
 * func_PAL_L12_0041DF18: strict build rejected it: D_001A79F8 needs two values
 */
void func_PAL_L12_0041DF18(s8 *arg0) {
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
                func_L00_00326048(arg0, s + 0xC, 0.06632252f, 0x80AABED2, 0x80466464);
            } else {
                func_L00_00326048(arg0, s + 0xC, 0.04537857f, 0x8096AAAA, 0x80466464);
            }
        }
        break;
    }
}
