/* func_US2_L14_004284F4, the USA v2.00 copy of func_L14_00426F28. Tier: opus-1 (task v2864).
 *
 * The C is exact; the catalogue row is not. tools/check.py: `756 bytes, expected 760`,
 * 0 of 1 places. The row starts at 0x4284F4, one word early: func_L14_00426E60 ends there
 * (0x428430 + 196, last word `addiu $sp,$sp,0x30`), 0x4284F4 is the padding nop, and the real
 * entry is the 8-aligned 0x4284F8 (`addiu $sp,$sp,-0x40`). In v1.01 the same nop at 0x426F24
 * belongs to no row. build.Placement.derive on this object, run from a scratch script: OK at
 * 0x4284F8, `instruction at +0x0 differs` at 0x4284F4. Not yet linked.
 *
 * To land it: add `"func_US2_L14_004284F4": (760, 4),` to FUNCTION_ENTRY_OFFSETS in
 * tools/levels.py, run `tools/carry.py --version us2 --entries` (as in a27ea06ea), then move this
 * file to src/us2/levels/14_aranos_prison/L14_004284F4-g8.c, delete the marker from
 * L14_004284F4.c and add to config/cflags.txt the template unit's flags:
 *   src/us2/levels/14_aranos_prison/L14_004284F4-g8.c	-O2 -G8 -ffunction-sections
 *
 * What changed from the template: both messages are now looked up by id,
 * func_00288FB8(0x2F53) and func_00288FB8(0x2F54), and func_L00_0030DD70 is
 * func_US2_L00_0030EFB8 here. Everything else in the C is the template's.
 */
#include "common.h"

/* func_US2_L14_004284F4, the USA v2.00 copy of func_L14_00426F28. */

extern void func_L14_00427250(char *);

typedef struct PrisonLight {
    u8 pad00[0x20];
    u8 state;            /* 0x20 */
    u8 pad21[0x10];
    u8 update;           /* 0x31 */
    u8 pad32[2];
    u16 flags;           /* 0x34 */
    u8 pad36[0x32];
    struct PrisonLightState *vars; /* 0x68 */
    u8 pad6C[0x2A];
    u16 frames;          /* 0x96 */
    u32 pending;         /* 0x98 */
} PrisonLight;

/* Its state block: the beam level, the done flag and the sound slot. */
typedef struct PrisonLightState {
    float level;         /* 0x0 */
    int done;            /* 0x4 */
    int sound;           /* 0x8 */
} PrisonLightState;

/* A sound slot as seen from D_00188660 plus 0x70 per slot number. */
typedef struct { char pad[0x74]; unsigned char active; char gap[0x13]; void *owner; } PrisonSoundAnchor;
extern char D_00188660[];
extern unsigned char D_0018C0D4 NOT_SDA;
extern int D_L14_001B20C8;
extern int D_L14_001B17C0 SDATA(D_L14_001B17C0);
extern float D_L14_001B17C4 SDATA(D_L14_001B17C4);
extern float D_L14_001B17C8 SDATA(D_L14_001B17C8);
extern float D_L14_001B17D0 SDATA(D_L14_001B17D0);
extern float D_L14_001B17D4 MACRO_ADDR;

extern void func_002A7750(void *self, int state, int mode);
extern void *func_002A9640(void *a0, int a1, int a2);
extern int func_002B38B8(void *);
extern int func_002E5CF8(int a, int b, void *self);
extern void func_002E59F8(int);
extern float func_00283CE0(int);
extern float func_00283198(float);
extern float func_002AA2E8(float *, float, float);
extern void func_0029F338(void *a0);
extern void *func_00288FB8(int);
extern void func_US2_L00_0030EFB8(void *, int);
extern void func_L00_002E6CF0(void *, void *);
extern void func_L14_004272D8(void *);

void func_US2_L14_004284F4(PrisonLight *object)
{
    PrisonLightState *state = object->vars;
    float angle, denominator;
    int period, ticks;
    unsigned int counter, previous;
    unsigned short flags;

    switch (object->state) {
    case 0:
        func_002A7750(object, 1, -1);
        state->sound = -1;
        break;
    case 1:
        if (D_0018C0D4 != 1)
            break;
        if (object->update)
            func_L14_004272D8(object);
        if (func_002A9640(object, 0x10000, 0) || func_002B38B8(object)) {
            func_US2_L00_0030EFB8(func_00288FB8(0x2F53), 240);
            func_002A7750(object, 2, -1);
        }
        if (((PrisonSoundAnchor *)(D_00188660 + state->sound * 112))->owner != object || !((PrisonSoundAnchor *)(D_00188660 + state->sound * 112))->active)
            state->sound = func_002E5CF8(0, 4, object);
        break;
    case 2:
        ticks = D_L14_001B20C8;
        period = D_L14_001B17C0;
        angle = func_00283CE0(ticks % period);
        denominator = func_00283CE0(period);
        angle *= 6.2831855f;
        angle /= denominator;
        angle = func_00283198(angle - 3.1415927f);
        func_002AA2E8(&state->level, D_L14_001B17C4 * angle + D_L14_001B17C8, D_L14_001B17D0 * 0.016666668f);
        func_L00_002E6CF0((void *)func_L14_00427250, object);
        counter = object->frames;
        previous = counter++;
        object->frames = counter;
        if (previous >= 181)
            state->done = 1;
        if (state->sound != -1 && ((PrisonSoundAnchor *)(D_00188660 + state->sound * 112))->owner == object && ((PrisonSoundAnchor *)(D_00188660 + state->sound * 112))->active)
            func_002E59F8(state->sound);
        state->sound = -1;
        if (!D_0018C0D4) {
            func_US2_L00_0030EFB8(func_00288FB8(0x2F54), 240);
            flags = object->flags;
            object->pending = 0;
            flags |= 0x41;
            flags &= 0xefff;
            object->flags = flags;
            func_002A7750(object, 3, -1);
        }
        break;
    case 3:
        func_L00_002E6CF0((void *)func_L14_00427250, object);
        func_002AA2E8(&state->level, 0.0f, D_L14_001B17D4 * 0.016666668f);
        if (state->level == 0.0f)
            func_0029F338(object);
        break;
    }
}
