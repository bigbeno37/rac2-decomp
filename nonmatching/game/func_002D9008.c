/* Claude Opus 5.5 (xhigh), task v2506, 5 November 2026: retained near miss.
 * Profile: -O2 -G8 -ffunction-sections, in its own -g8.c unit (func_002D8EC0
 * extern; both have the same 28 places).
 * check.py: 1472/1484 bytes, 0 of 28 places (USA v1.01); 298/371 rows differ.
 * The earlier draft (1480 bytes, 300 rows) is superseded: retail draws the
 * white state-4 frame OUTSIDE the cursor test (the not-selected bne lands on
 * `addiu v0,4; bne s6,v0`), which this draft now reproduces, and state is
 * written directly (no itemState copy).
 *
 * Remaining 1: frame 0xA0 vs retail 0xB0. Retail allocates 16 bytes BEFORE
 * reload that nothing accesses. Diagnostic only: adding an unused
 * `int probe[4];` first local gives the 0xB0 frame and every spill offset
 * (264 rows differ). An unused local is not allowed, so the real construct
 * is still unknown. It must be an expand-time allocation with no surviving
 * access (an aggregate local whose uses were deleted as unreachable, an
 * inlined helper's frame, or OUTGOING_ARGS_SIZE carried over by
 * integrate.c:1634). A plain inline cursor helper changed nothing.
 *
 * Remaining 2: purchased state. Retail `li v0,3; move s6,0; movn s6,v0,v1`
 * comes from regmove tying a pseudo holding 0 (operand 2 of
 * (if_then_else (eq p 0) r0 r3)) to state, leaving r3 local. Here
 * cse folds the jump-equivalent 0 into the then-arm, so regmove ties r3
 * (`li s5,3; movz s5,zero,p`). Tried: ternary, `state == 0 && purchased`,
 * `{ state = 0; if (purchased) state = 3; }`, `? 3 : state`, and an
 * out-pointer inline helper (spills state, 1500 bytes). All fold the 0. If
 * the 3 were hoisted (loop.c: life 1, savings 1, not desirable), reload
 * would still pick movz. Next: find a form where the 0 reaching the cmove is
 * a pseudo that cse cannot fold, e.g. a conversion done only by the jump
 * after cse2.
 *
 * Also: state sits in s5 vs retail s6, and xStep in s6 vs s7. With the probe,
 * nextY/row+1/(y>>4) spill slots are 0x2C/0x34/0x30 vs retail 0x34/0x30/0x2C.
 */
#include "common.h"
extern void func_002FC560(unsigned int, unsigned long long);
extern void func_0027C540(int);
extern void func_0027C660(void);

extern int D_001ABAD8 __attribute__((sda)), D_001ABADC __attribute__((sda)), D_001ABAE8 __attribute__((sda));
extern int D_001AA43C __attribute__((sda));
extern int D_001B1598;
extern unsigned char D_001A7AF8[], D_001395B8[];
extern unsigned int func_002884B0(unsigned int);
extern int func_00282C30(int);
extern void func_0028FC48(int, int, int, int, unsigned long, int);
extern void func_0028EE98(int, int, int, int, int, int);

typedef struct { char pad[0x14]; char *owner; char pad18[0x40-0x18]; int slots[65]; int override144; int override148; } GridContext;
extern GridContext D_001F2840;
extern unsigned char D_00139568[];
typedef struct { unsigned char pad[4]; unsigned char purchased; unsigned char pad05[7]; int slot; unsigned char pad10[0x3C-0x10]; unsigned short icon; unsigned char pad3E[224-0x3E]; } GridItem;
extern GridItem D_00239BA0[];
extern void func_002D8EC0(char *, short *, int, int, int, int);
extern int func_0028E588(int, int);

int func_002D9008(char *object)
{
    short *record = *(short **)(object + 0x48);
    int active = *(char **)(D_001F2840.owner + 0xE8) == object;
    int xStart, xStep, yStart, yStep;
    int width, height, areaWidth, areaHeight, gap;
    int index, row, column, x, y, nextY, state, bottom;
    int texture;
    unsigned long colour, pulse;
    GridItem *entry;
    func_002FC560(66, 0x8000000044ULL);
    func_002FC560(71, 11);
    func_0027C540(0);
    width = D_001ABAD8;
    height = D_001ABADC;
    areaWidth = *(int *)(object + 0x20) << 4;
    areaHeight = *(int *)(object + 0x24) << 4;
    gap = D_001ABAE8;
    if (*(int *)(object + 0x44) >= 2) {
        xStart = *(int *)(object + 0x34) << 4;
        xStep = width + (areaWidth -
                (*(int *)(object + 0x34) << 5) - *(int *)(object + 0x44) * width) /
                (*(int *)(object + 0x44) - 1);
    } else {
        xStart = (areaWidth - width) >> 1;
        xStep = 0;
    }
    if (*(int *)(object + 0x30) & 2) {
        yStep = height + gap;
        yStart = *(int *)(object + 0x38) << 4;
    } else if (*(int *)(object + 0x40) >= 2) {
        yStart = *(int *)(object + 0x38) << 4;
        yStep = height + (areaHeight -
                (*(int *)(object + 0x38) << 5) - *(int *)(object + 0x40) * height) /
                (*(int *)(object + 0x40) - 1);
    } else {
        yStep = 0;
        yStart = (areaHeight - height) >> 1;
    }
    index = 0;
    y = yStart;
    for (row = 0; row < *(int *)(object + 0x40); row++) {
        nextY = y + yStep;
        x = xStart;
        for (column = 0; column < *(int *)(object + 0x44); column++) {
            bottom = y + height;
            state = 0;
            if (record[2] == 0 ? D_001A7AF8[record[3]] : D_001395B8[record[3]]) {
                if ((unsigned short)record[2] == 0) {
                    int key = record[3];
                    entry = &D_00239BA0[D_00139568[key]];
                    state = 0;
                    if (D_001F2840.slots[entry->slot] == key)
                        state = ((*(int *)(object + 0x30) ^ 1) & 1) << 2;
                    if (state == 0) state = entry->purchased ? 3 : 0;
                    if (D_001F2840.override144 && (*(int *)(object + 0x30) & 8)) state = 2;
                    if (D_001F2840.override148 && (*(int *)(object + 0x30) & 4)) state = 2;
                    if (func_002884B0((unsigned short)record[0]))
                        func_002D8EC0(object, record, column, row, x >> 4, (y >> 4) + 36);
                }
                if (active && *(int *)(object + 0x3C) == index) {
                    pulse = (unsigned int)(((func_00282C30((D_001B1598 & 63) - 32) + 64) * 0x10202) | 0x80000000u);
                    colour = state == 4 ? (unsigned long)(int)0x80101010u : (unsigned long)D_001AA43C;
                    func_0028FC48(x - 48, y - 48, x + width + 48, bottom + 48, pulse, 1);
                    func_0028FC48(x - 16, y - 16, x + width + 16, bottom + 16, colour, 1);
                }
                if (state == 4)
                    func_0028FC48(x - 64, y - 64, x + width + 64, bottom + 64, 0x80FFFFFFUL, 1);
                entry = &D_00239BA0[D_00139568[record[3]]];
                texture = func_0028E588(entry->icon, record[1]);
                func_0028EE98(texture, x, y, width, height, 128);
                if (state == 2)
                    func_0028FC48(x - 64, y - 64, x + width + 64, bottom + 64, 0x60202020UL, 1);
            } else if (active && *(int *)(object + 0x3C) == index) {
                pulse = (unsigned int)(((func_00282C30((D_001B1598 & 63) - 32) + 64) * 0x10202) | 0x80000000u);
                colour = (unsigned long)D_001AA43C;
                func_0028FC48(x - 48, y - 48, x + width + 48, bottom + 48, pulse, 1);
                func_0028FC48(x - 16, y - 16, x + width + 16, bottom + 16, colour, 1);
            }
            index++;
            record += 5;
            x += xStep;
        }
        y = nextY;
    }
    func_0027C660();
    return 2;
}
