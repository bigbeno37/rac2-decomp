/* opus-1; USA v1.01; -O2 -G8 -ffunction-sections -g -frac2-zero-ti-por
 * (own -g8 unit). Best: 1156/1156 bytes, 0/2 places (130 of 289 aligned
 * rows differ). Closer than the 1144/1152-byte drafts: the u64 colour local
 * restores retail's extendsidi2 before the texture call, and testing then
 * re-reading the list entry gives retail's lw/beqz/daddu cursor copy and
 * loop padding; D_L08_001B1BA8 is s32.
 * Remaining: sched1 floats the 0.8/-0.8/0 constant sets (no dependences)
 * up between the three func_00336D50 calls; retail sets them, and all
 * argument moves of each icon draw, only after that draw's y chain, with
 * the trunc temp in v1 (mine v0), x loaded into f1 (mine f12), constants
 * 0.8=f22/-0.8=f23 (swapped here), second/state in s3/s4 (mine s2/s3).
 * Retail looks as if each icon draw had a scheduling barrier after its y
 * computation; no allowed construct for that was found. An inline icon
 * helper with literal constants was worse (1168 bytes).
 * All declarations needed by this standalone draft follow below.
 */
#include "common.h"
typedef union { s32 q __attribute__((mode(TI))); f32 f[4]; } Vector;
extern u8 *D_L08_001A904C MACRO_ADDR, *D_L08_001B29DC MACRO_ADDR;
extern f32 D_L08_001B3244 MACRO_ADDR;
extern f32 D_L08_001B1B84 __attribute__((sda)), D_L08_001B1B88 __attribute__((sda));
extern f32 D_L08_001B1B8C __attribute__((sda)), D_L08_001B1B90 __attribute__((sda));
extern s32 D_L08_001B1B94 __attribute__((sda)), D_L08_001B1B98 __attribute__((sda));
extern f32 D_L08_001B1B9C __attribute__((sda));
extern s32 D_L08_001B1BA0 __attribute__((sda));
extern f32 D_L08_001B1BA4 __attribute__((sda));
extern s32 D_L08_001B1BA8 __attribute__((sda));
extern u16 *D_L08_001FB660[];
extern u8 D_L08_001B9D30[];
extern s32 func_L08_00300090(void), func_L08_003000C0(void);
extern void func_L08_00300100(void);
extern char *func_00336D50(void *, s32);
extern void func_00300A00(char *, u32, f32, f32, f32, f32, f32);
extern u64 func_0027C878(s32);
extern void func_L05_003008C0(f32, f32, f32, f32, s32, s32, s32, s32, u64, u64);
extern f32 func_00282E48(void *, void *);
extern void func_002833F0(void *, void *), func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, f32), func_002836E8(void *, void *);
extern void func_00283098(void *, void *, void *);
extern s32 func_00283CF0(f32);

void func_L08_004207D0(u8 *object)
{
    Vector difference, euler;
    f32 rotation[12], inverse[12];
    u8 *state, *entry, *anchor;
    char *first, *second, *third;
    u16 *cursor;
    f32 y, distance, scale, size, x;
    s32 alpha;
    u64 color;
    u64 texture;
    func_L08_00300090();
    func_L08_00300100();
    state = *(u8 **)(object + 0x68);
    first = func_00336D50(D_L08_001A904C + 0x8710, 0xBA);
    second = func_00336D50(D_L08_001A904C + 0x8710, 0xBB);
    third = func_00336D50(D_L08_001A904C + 0x8710, 0xBC);
    func_00300A00(first, 0x60442D00, D_L08_001B1B84 + 4.0f,
                 (f32)(s32)((D_L08_001B1B88 + 4.0f) * D_L08_001B3244 + 0.5f), 0.8f, -0.8f, 0.0f);
    func_00300A00(second, 0x60241700, D_L08_001B1B84 + 4.0f,
                 (f32)(s32)((D_L08_001B1B88 + 4.0f) * D_L08_001B3244 + 0.5f), 0.8f, -0.8f, 0.0f);
    func_00300A00(third, 0x55F0C070, D_L08_001B1B84 + 4.0f,
                 (f32)(s32)((D_L08_001B1B88 + 4.0f) * D_L08_001B3244 + 0.5f), 0.8f, -0.8f, 0.0f);
    y = (f32)(s32)(D_L08_001B1B88 * D_L08_001B3244 + 0.5f);
    texture = func_0027C878(D_L08_001B1BA0);
    func_L05_003008C0(D_L08_001B1B84, y, D_L08_001B1B8C, D_L08_001B1B90,
                     0, 0, D_L08_001B1B94, D_L08_001B1B98, 0x80808080UL, texture);
    func_L08_003000C0();
    anchor = *(u8 **)(state + 0x1B0);
    if (!anchor) return;
    if (!D_L08_001FB660[anchor[0x21]]) return;
    cursor = D_L08_001FB660[anchor[0x21]];
    do {
        entry = D_L08_001B29DC + ((*cursor & 0x7FFF) << 8);
        if (!entry || entry[0x20] == 0xFE || entry[0x20] == 0xFD || *(s16 *)(entry + 0xAA) == 0x96A) return;
        distance = func_00282E48(object + 0x10, entry + 0x10);
        if (distance <= D_L08_001B1BA4) {
            scale = distance / D_L08_001B1BA4;
            euler.q = 0;
            euler.f[2] = 1.5707964f;
            func_002833F0(rotation, &euler);
            scale *= D_L08_001B1B9C;
            func_00282CF0(&difference, entry + 0x10, object + 0x10);
            func_00282F20(&difference, &difference, scale);
            func_002836E8(inverse, D_L08_001B9D30);
            func_00283098(&difference, &difference, inverse);
            func_00283098(&difference, &difference, rotation);
            alpha = func_00283CF0((1.0f - distance / D_L08_001B1BA4) * 128.0f);
            size = difference.f[2] * 0.125f + D_L08_001B1B8C * 0.5f;
            if (size > D_L08_001B1B8C + D_L08_001B1B8C) size = D_L08_001B1B8C + D_L08_001B1B8C;
            else if (size < 0.0f) size = 0.0f;
            x = difference.f[0] + D_L08_001B1B84;
            y = (f32)(s32)((D_L08_001B1B88 - difference.f[1]) * D_L08_001B3244 + 0.5f);
            color = D_L08_001B1BA8 | (alpha << 24);
            texture = func_0027C878(D_L08_001B1BA0);
            func_L05_003008C0(x, y, size, size, 0, 0, D_L08_001B1B94, D_L08_001B1B98, color, texture);
        }
    } while ((s16)*cursor++ >= 0);
}
