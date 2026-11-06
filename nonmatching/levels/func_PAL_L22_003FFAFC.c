/* func_PAL_L22_003FFAFC, the PAL copy of func_L22_003FB310: the same C, with the 50 Hz values below.
 *
 * Tier: opus-1 (Claude Opus 5.5), task v3021. Status: CATALOGUE, not a C near miss.
 *
 * Best check.py --version pal: "1764 bytes, expected 1768", 0 of 2 places.
 * Measured with a one-off script calling build.Placement.derive(obj, section, address + 4), not through
 * check.py: this exact C equals retail at 2 of 2 places, relocations included. Not linked: no build can
 * place it until the row moves.
 *
 * What differs: only the row's first word. It is 0x00000000 at 22:003FFAFC and 23:00406BF4, both 4 mod 8,
 * right after func_L22_003FB1F8's completed return (jr $ra; addiu $sp,$sp,0x40), and the prologue
 * (addiu $sp,$sp,-0xF0) is at row + 4. No C can supply that word, since a compiled function starts with
 * its prologue at an 8-aligned section start.
 *
 * To land: add "func_PAL_L22_003FFAFC": (1768, 4), to FUNCTION_ENTRY_OFFSETS in tools/levels.py and regenerate
 * the catalogue with tools/carry.py --version pal --entries; the name stays. Then move this file to a new
 * unit, src/pal/levels/shared/L22_003FFAFC-obj-g8.c (measured there: exact at row + 4, 2 of 2), and delete the
 * marker from L22_003FFAFC.c. Do not use L22_003FFAFC-g8.c: that unit already holds the matched callee
 * func_PAL_L22_004001E8, and this function must call it externally. The -g8 suffix alone gives the template's
 * flags (-O2 -G8 -ffunction-sections), so config/cflags.txt needs no line.
 *
 * Edits from the template: 45 -> 37, 46 -> 38, 0.022222223 -> 0.027027028, 0.018907737 -> 0.022689285,
 * 0.00029088825 -> 0.0003490659, and the switch's second distance scaled by 1.2f in all four cases.
 * Callees func_L22_003FB9B8 and func_L22_003FC048 take their PAL names. The declarations are all below.
 */
#include "common.h"
extern u8 D_00189E20[];
extern f32 func_00282E48(void *, void *);
extern void func_002AFFD8(void *, void *, int);
extern int func_002B3868(void *);
extern void func_L00_0035F5A0(int, int, int);
extern void func_002833F0(void *, void *);
extern void func_00283738(void *, void *, void *);
extern void func_L00_00325600(void *, void *, int);
extern void func_00282D30(void *, void *, f32);
extern void func_00282CC0(void *, void *, void *);
extern void func_00283360(void *);
extern void func_002B9180(void *);
extern void func_0029F338(void *);
extern void func_00282F20(void *, void *, f32);
extern int func_002761F8(void *, void *, int, int, int);
extern void func_L22_003A2988(void *, f32);
extern int func_002A77E0(int);
extern f32 ref_L22_003FB310_00 __attribute__((sda));
extern f32 ref_L22_003FB310_01 __attribute__((sda));
extern int ref_L22_003FB310_02 __attribute__((sda));
extern f32 ref_L22_003FB310_03 __attribute__((sda));
extern u8 * ref_L22_003FB310_04 __attribute__((sda));
extern f32 ref_L22_003FB310_05 __attribute__((sda));
extern f32 ref_L22_003FB310_06;
extern const f32 ref_L22_003FB310_07;
extern u8 ref_L22_003FB310_08[];
extern u8 * ref_L22_003FB310_09;
extern void func_PAL_L22_004001E8(void *, void *, int, int);
extern void func_PAL_L22_00400890(void *);

void func_PAL_L22_003FFAFC(u8 *obj) {
    f32 axis[4] __attribute__((aligned(16)));
    u8 *ctl = *(u8 **)(obj + 0x68);
    f32 f;
    int i;
    f32 rot[4] __attribute__((aligned(16)));
    /* Transform construction and collision recovery use this scratch space
     * in separate phases. */
    union {
        struct {
            f32 mat[12];
            f32 start[4];
            f32 end[4];
            f32 scale[12];
        } transform;
        struct {
            f32 mat[4];
            f32 move[4];
            f32 start[4];
            f32 end[4];
        } collision;
    } scratch __attribute__((aligned(16)));
    if (obj[0x20] == 0x62) return;
    f = func_00282E48(D_00189E20 + 0x80, obj + 0x10);
    *(f32 *)(ctl + 0x2C) = f;
    if (ref_L22_003FB310_07 <= f) obj[0x23] = 255;
    func_002AFFD8(obj + 0x10, axis, 0);
    if (func_002B3868(obj) && obj[0x20] == 0x10) func_L00_0035F5A0(0x12, 0, 0);
    if (obj[0x20] == 0) {
        qzero_nc(rot);
        rot[2] = 0.022689285f;
        func_002833F0(scratch.transform.mat, rot);
        func_00283738(obj + 0xC0, obj + 0xC0, scratch.transform.mat);
        func_L00_00325600(obj + 0xC0, axis, 0);
        func_PAL_L22_00400890(obj);
    } else {
        qzero_nc(rot);
        rot[2] = ((37.0f - (f32)*(u16 *)(obj + 0x96)) * 22.0f + 155.0f) * 0.0003490659f;
        func_002833F0(scratch.transform.mat, rot);

        if (obj[0x23] >= 3) obj[0x23]--;
        if (obj[0x23] >= 11 && obj[0x20] == 0x10) {
            switch (*(u16 *)(obj + 0x96) & 3) {
            case 0:
                func_00282D30(scratch.transform.start, obj + 0xC0, -ref_L22_003FB310_00);
                func_00282CC0(scratch.transform.start, scratch.transform.start, obj + 0x10);
                func_00282D30(scratch.transform.end, obj + 0xD0, -(ref_L22_003FB310_01 * 1.2f));
                break;
            case 1:
                func_00282D30(scratch.transform.start, obj + 0xC0, ref_L22_003FB310_00);
                func_00282CC0(scratch.transform.start, scratch.transform.start, obj + 0x10);
                func_00282D30(scratch.transform.end, obj + 0xD0, ref_L22_003FB310_01 * 1.2f);
                break;
            case 2:
                func_00282D30(scratch.transform.start, obj + 0xD0, ref_L22_003FB310_00);
                func_00282CC0(scratch.transform.start, scratch.transform.start, obj + 0x10);
                func_00282D30(scratch.transform.end, obj + 0xC0, ref_L22_003FB310_01 * 1.2f);
                break;
            case 3:
                func_00282D30(scratch.transform.start, obj + 0xD0, -ref_L22_003FB310_00);
                func_00282CC0(scratch.transform.start, scratch.transform.start, obj + 0x10);
                func_00282D30(scratch.transform.end, obj + 0xC0, -(ref_L22_003FB310_01 * 1.2f));
                break;
            }
            func_PAL_L22_004001E8(scratch.transform.start, scratch.transform.end, ref_L22_003FB310_02, 0);
        }
        func_00283360(scratch.transform.scale);
        f = (37.0f - (f32)*(u16 *)(obj + 0x96) * 0.027027028f) * (ref_L22_003FB310_03 * 0.027027028f);
        scratch.transform.scale[0] = scratch.transform.scale[5] = scratch.transform.scale[10] = f;
        func_00283738(scratch.transform.mat, scratch.transform.scale, scratch.transform.mat);
        func_00283738(obj + 0xC0, obj + 0xC0, scratch.transform.mat);
        for (i = 0; i < 8; i++) {
            void **slot = (void **)(ctl + 0x60 + i * 4);
            if (*slot) { func_002B9180(*slot); *slot = 0; }
        }
        if (*(u16 *)(obj + 0x96) >= 38) {
            if (ref_L22_003FB310_09 == obj) obj[0x20] = 0x62; else func_0029F338(obj);
            return;
        }
        if (obj[0x20] == 0x10) {
            qcopy(obj + 0x10, D_00189E20 + 0x80);
            func_002AFFD8(obj + 0x10, axis, 0);
            func_L00_00325600(obj + 0xC0, axis, 0);
            func_00282F20(scratch.transform.start, axis, -((f32)*(u16 *)(obj + 0x96) * 0.027027028f) * 7.5f);
            func_00282CC0(obj + 0x10, obj + 0x10, scratch.transform.start);
        } else {
            qcopy(obj + 0x10, ref_L22_003FB310_04 + 0x10);
            func_002AFFD8(obj + 0x10, axis, 0);
            func_L00_00325600(obj + 0xC0, axis, 0);
            func_00282F20(scratch.transform.start, axis, -((f32)*(u16 *)(obj + 0x96) * 0.027027028f) * 7.5f);
            func_00282CC0(obj + 0x10, obj + 0x10, scratch.transform.start);
        }
    }
    if (*(u16 *)(obj + 0x96) < 65535) (*(u16 *)(obj + 0x96))++;
    else *(u16 *)(obj + 0x96) = 0;
    if (*(s16 *)(ctl + 0x2A) != 0 && (*(u16 *)(obj + 0x96) & 31) == 0) {
        func_00282D30(rot, axis, 6.0f);
        func_00282D30(scratch.collision.mat, axis, -3.0f);
        func_00282CC0(rot, rot, obj + 0x10);
        func_00282CC0(scratch.collision.mat, scratch.collision.mat, obj + 0x10);
        if (func_002761F8(scratch.collision.mat, rot, 0x12, 0, 0) == 0) {
            func_L22_003A2988(obj + 0x10, 0.0f);
            func_00282D30(scratch.collision.move,
                obj + 0xC0 + func_002A77E0(2) * 16,
                func_002A77E0(2) ? ref_L22_003FB310_05 : -ref_L22_003FB310_06);
            func_00282CC0(obj + 0x10, obj + 0x10, scratch.collision.move);
            func_L22_003A2988(obj + 0x10, 0.0f);
            func_00282D30(scratch.collision.start, axis, 3.0f);
            func_00282D30(scratch.collision.end, axis, -3.0f);
            func_00282CC0(scratch.collision.start, scratch.collision.start, obj + 0x10);
            func_00282CC0(scratch.collision.end, scratch.collision.end, obj + 0x10);
            if (func_002761F8(scratch.collision.end, scratch.collision.start, 0x12, 0, 0)) qcopy(obj + 0x10, ref_L22_003FB310_08);
            *(s16 *)(ctl + 0x2A) = 0;
        }
    }
}
