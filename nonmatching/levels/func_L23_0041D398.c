/*
 * func_L23_0041D398: matched only with extern volatile on D_001A7AFD, which upstream declares non-volatile; user ruling 2026-11-05: find a form without volatile. Retail rereads the byte at 0041D43C on the entry-cleanup path.
 */
void func_L23_0041D398(char *object) {
    struct { Quad velocity, matrix[4], random; } local;
    /* The encounter state becomes a cursor into its part table in each phase. */
    union { PartState *state; char **parts; } cursor;
    char *particle;
    unsigned short timer;
    float scale;
    cursor.state = *(PartState **)(D_L23_001B1484 + 0x68);
    if (D_001A7AFD) {
        if (*(unsigned short *)(object + 0x96) > D_L23_001B1468 + 60) {
            func_002A6C58(object, 0, 3);
            func_L23_0041D9B0();
            return;
        }
    } else {
        if (*(unsigned short *)(object + 0x96) > D_L23_001B1468 + 200 && !func_002F5D10()) {
            func_002A6C58(object, 0, 3);
            func_L23_0041D9B0();
            return;
        }
        if (!D_001A7AFD && *(unsigned short *)(object + 0x96) > D_L23_001B1468 + 60) {
            char *entryState;
            if (!D_L23_001B13D0) return;
            entryState = *(char **)(D_L23_001B13D0 + 0x68);
            entryState[13] = 1;
            entryState[8] = 0;
            func_L00_003294F0((unsigned char *)D_L23_001B13D0, 1);
            D_L23_001B13D0 = 0;
            return;
        }
    }
    if (D_L23_001B1434) {
        int i;
        cursor.parts = cursor.state->parts + 5;
        for (i = 5; i < 10; i++, cursor.parts++) {
            if (!D_L23_001B3720) {
                func_0029F338(*cursor.parts);
                *cursor.parts = 0;
                *(unsigned short *)(object + 0x34) |= 0x41;
                func_0032C700(object + 16, 1, 135, 120, 120, 115, 125, 1, 6.5f, 20.0f, 1, 1, 1, 0);
                func_002E5CF8(12, 0, object);
            } else if (*cursor.parts) {
                int debrisIndex = 10 - D_L23_001B3720;
                func_0029FC20(object, i - 4, (char *)local.matrix);
                func_00282D30((char *)&local.velocity, (char *)&local.matrix[1], D_L23_001B16D0);
                scale = func_002A7878(0.0f, D_L23_001B16D0 * 0.5f);
                func_00282D30((char *)&local.random, object + 0xE0, scale);
                func_00282CC0((char *)&local.velocity, (char *)&local.velocity, (char *)&local.random);
                {
                    char *oldPart = *cursor.parts;
                    particle = func_00273730(oldPart, 1, *(short *)(oldPart + 0xAA), debrisIndex, oldPart + 16, oldPart + 0xF0, (float *)&local.velocity, 255, 140.0f, 25.0f);
                    if (particle) *(void (**)(u8 *))(particle + 0x64) = func_00310C88;
                }
            }
        }
        D_L23_001B3720--;
        return;
    }
    timer = *(unsigned short *)(object + 0x96);
    if ((D_L23_001B1468 << 1) / 3 < timer) {
        int i;
        cursor.parts = cursor.state->parts;
        func_0032C700(object + 16, 1, 45, 30, 40, 25, 85, 1, 4.5f, 20.0f, 1, 1, 1, 0);
        func_002E5CF8(11, 0, object);
        for (i = 4; i >= 0; i--, cursor.parts++) {
            char *replacement = cursor.parts[5];
            if (replacement) {
                char *old = *cursor.parts;
                if (!old) continue;
                qcopy(replacement + 0xC0, old + 0xC0);
                qcopy(replacement + 0xD0, old + 0xD0);
                qcopy(replacement + 0xE0, old + 0xE0);
                qcopy(replacement + 0x10, old + 0x10);
                *(unsigned short *)(replacement + 0x34) &= 0xFFF9;
                *(unsigned short *)(cursor.parts[5] + 0x34) |= 0x4000;
                *(unsigned short *)(cursor.parts[5] + 0x34) &= 0xFFBE;
                func_0029F338(*cursor.parts);
                *cursor.parts = 0;
            }
        }
        {
            int countdown = 10;
            D_L23_001B1434 = 1;
            D_L23_001B3720 = countdown;
        }
        return;
    }
    if (D_L23_001B1468 / 3 < timer) {
        if (timer & 15) return;
        {
            int index = func_002A77E0(5);
            char **base = cursor.state->parts;
            char **slot = base + index;
            char *direction = *slot + 0xD0;
            int parity;
            scale = func_002A7878(4.0f, 12.0f);
            func_00282D30((char *)&local.velocity, direction, scale);
            func_00282CC0((char *)&local.velocity, (char *)&local.velocity, *slot + 16);
            scale = func_002A7878(2.3f, 3.7f);
            parity = index & 1;
            func_0032C700(&local.velocity, parity, 20, 10, 14, 15, 25, 0, scale, 20.0f, parity, 0, 0, 0);
        }
        func_002E5CF8(func_002A77E0(3) + 13, 0, object);
        return;
    }
    if (timer & 31) return;
    {
        int index = func_002A77E0(5);
        char **base = cursor.state->parts;
        char **slot = base + index;
        char *direction = *slot + 0xD0;
        int parity;
        scale = func_002A7878(4.0f, 12.0f);
        func_00282D30((char *)&local.velocity, direction, scale);
        func_00282CC0((char *)&local.velocity, (char *)&local.velocity, *slot + 16);
        scale = func_002A7878(2.3f, 3.7f);
        parity = index & 1;
        func_0032C700(&local.velocity, parity, 20, 10, 14, 15, 25, 0, scale, 20.0f, parity, 0, 0, 0);
    }
    func_002E5CF8(func_002A77E0(3) + 13, 0, object);
}
