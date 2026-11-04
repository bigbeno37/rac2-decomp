/*
 * func_L09_003EA798: strict build rejected it: PAL 09/20 strict: func_L09_003EAA18 (C in PAL, same unit L09_002B7238-g8.c) needs .text.func_L09_003EA798 at its place, which PAL's catalogue does not place
 */
void func_L09_003EA798(char *object) {
    DobboMoverState *state = *(DobboMoverState **)(object + 0x68);
    char *pos = object + 0x10;
    int entry;

    qcopy((char *)state + 0x330, pos);
    qcopy((char *)state + 0x340, object + 0xF0);
    state->f48 = func_00283CF0(25.0f);
    state->f4A = func_00283CF0(19.2f);
    state->f1EC = 4;
    state->f1EE = 4;
    state->f191 = 2;
    state->f192 = 2;
    state->f196 = 2;
    state->f197 = 2;
    state->f1ED = 8;
    state->f1EF = 8;
    state->f190 = 5;
    state->f1F2 = 4;
    state->f1F3 = 8;
    state->f1F6 = 1;
    state->f28 = 3;
    state->f2F = 2;
    state->f1C8 = 5;
    state->f1AC = 0.0013888889f;
    state->f1A4 = 0.0025f;
    state->f1A8 = 0.0025f;
    state->f1B0 = 1.5f;
    state->f1BC = 0.4f;
    state->f30 = 1.0f;
    state->f19C = 0;
    state->f1A0 = 0;
    state->f1B4 = 0;
    state->f1B8 = 0;
    state->f54 = 0;
    state->f58 = 0;
    state->f5C = 0;
    state->f7C = 0;
    state->f50 = 3.5f;
    state->f60 = 3.141593f;
    state->f80 = 0.52359885f;
    func_002B60B0(object, ref_L09_003EA798_00 * 0.00027777778f, ref_L09_003EA798_01 * 0.016666668f,
                  ref_L09_003EA798_02 * 4.8481374e-06f, ref_L09_003EA798_03 * 0.00029088825f);
    state->f354 = state->f374;
    func_L09_0033E7A0(state->f36C);
    func_L09_0033E7A0(state->f350);
    entry = state->f36C;
    if (entry != -1) {
        qcopy(pos, (char *)D_L09_001FE6C0[entry] + 0x10);
        func_L09_003EA2D0(object, entry, -1);
        state->f380 = 0;
    } else {
        state->f380 = 1;
    }
    func_002A7750(object, 1, 1);
}
