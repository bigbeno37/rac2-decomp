/*
 * func_US2_0029CD18: strict build rejected it: D_001A79F8 needs two values
 */
void func_US2_0029CD18(void)
{
    int result;
    int i;
    CbCd fn;
    SubCd *sub;
    char *g;

    if (ref_L00_0030CDB0_00[0] != 0)
        return;

    if (D_001A8CFC[0] == 0)
        goto states;

    if (D_001A8BB0[0] == 4) {
        if (func_002857B0() == 8)
            return;
    }

    func_00290680();
    result = 0;
    func_US2_0029CCB8();

    fn = (CbCd)((SubCd *)(D_001A8CFC[0] + 0x38000))->cb2;
    if (fn != 0)
        result = fn() != 0;

    for (i = 0; i < 4; i++) {
        sub = (SubCd *)(D_001A8CFC[0] + 0x38000);
        if (sub->slot[i].cb != 0)
            result |= ((CbCd)((FnCd *)(D_001A8CFC[0] + 0x3F9F8))[i].fn)();
    }

    for (i = 0; i < 2; i++) {
        sub = (SubCd *)(D_001A8CFC[0] + 0x38000);
        if (sub->slot2[i].cb != 0)
            result |= ((CbCd)((FnCd *)(D_001A8CFC[0] + 0x3FA18))[i].fn)();
    }

    if (result != 0) {
        if (D_001A79F8[0] == 8 || D_001A79F8[0] == 0x13) {
            if (func_0026F7E0(2))
                func_US2_0029CC48();
        }
    }

    if (result != 0)
        return;

states:
    if (D_001A8BB0[0] != 0 && D_001A8BB0[0] != 5) {
        if (D_001A8BB0[0] == 4) {
            if (func_00285840() != 7)
                return;
        }
    }

    g = D_001A8CFC[0];
    if (g != 0) {
        char *b = g + 0x38000;
        g = (char *)((SubCd *)b)->cb1;
        ((CbCd)g)();
        func_US2_0029CC48();
    }
}
