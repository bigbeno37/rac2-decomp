/* func_PAL_L00_0039BE70: opus-1 (Claude Opus 5.5), task v2662. Not a near miss: a catalogue defect.
 *
 * Best check.py: 428 bytes, expected 424, 0 of 1 places. No instruction differs. With the size test
 * bypassed, build.Placement.derive(obj, section, 0x39BE70) accepts all 428 bytes against PAL level 00
 * and gives every call target one value.
 *
 * Retail's function is 428 bytes: `jr $ra` at 0x39C014, its delay slot at 0x39C018, a padding nop at
 * 0x39C01C, and the next function's frame at 0x39C020. The row keeps v1.01's 424, because 0.5f (two
 * instructions) became 0.6f (three).
 *
 * This is also not PAL level 00's own function. It is the level 00 place of boot func_PAL_0030FB38:
 * the same 428 bytes after masking jal and lui, and that catalogue row lists 26 places, all but level
 * 00. The same C has already landed as func_PAL_0030FB38 in src/pal/game/0030F2B0.c, so this copy
 * should not land: one definition per function. Fix the catalogue (tools/carry.py) instead.
 *
 * Needs: the extern lines below, and common.h.
 */

#include "common.h"

extern char *func_002A9640(char *, int, int);
extern void func_00282948(int *);
extern void func_002E5CF8(int, int, char *);
extern void func_PAL_002AC290(char *, char *);
extern void func_002ACB48(float *, float);
extern void func_PAL_002AC718(void *, void *, void *, float, float, int, int, int,
                              float, float, float, float, int, float, int, int, int, int, int);
extern void func_PAL_002AC3C0(char *, char *);

void func_PAL_L00_0039BE70(char *object)
{
    char *state = *(char **)(object + 0x68);
    char *hit = func_002A9640(object, 0x30001, 0);
    float direction[4];

    func_00282948((int *)(state + 0x28));
    func_00282948((int *)(state + 0x2C));
    if (hit != 0 && (*(int *)(hit + 0x20) != *(int *)(state + 0x34) ||
                     *(int *)(state + 0x28) == 0)) {
        *(int *)(state + 0x34) = *(int *)(hit + 0x20);
        *(int *)(state + 0x28) = 25;
        if (*(int *)(state + 0x2C) == 0) {
            if (*(unsigned short *)(hit + 0x28) == 0x101 && *(int *)(state + 0x30) == 0) {
                *(int *)(state + 0x2C) = 83;
                *(int *)(state + 0x30) = 1;
            } else {
                *(int *)(state + 0x30) = 0;
            }
        }
        func_002E5CF8(0, 0, object);
        *(float *)(state + 0x24) -= *(float *)(hit + 0x2C);
        func_PAL_002AC290(object, state);
        if (*(float *)(state + 0x24) < 0.0f) {
            func_002ACB48(direction, 0.6f);
            func_PAL_002AC718(object, direction, 0, 0.0f, 0.0f, 0, 0, 0,
                              4.0f, 3.0f, 9.0f, 1.0f, 2, 12.0f, 1, 1, -1, 0, 0x810001);
            object[0x20] = 6;
        }
    }
    ((unsigned char *)object)[0xA8] = 0xFF;
    func_PAL_002AC3C0(object, state);
}
