/* opus-1, Claude Opus 5.5 (xhigh effort), 3 November 2026: check.py 0 of 2 places, but 1492/1492 bytes
 * and cdiff 76/373 rows, nearly all of them relocations. This is the func_L15_00432A60 port with
 * float locals zero = 0 and one = 1.0f set just before the core loop, used only in that loop's
 * normalisation, and i == count - 1 there. The halo loop keeps the literals.
 * What differs:
 * (1) points (pseudo 84, 27/176 -> 1534) and a 17-ref pseudo (269, 68/444 -> 1531) swap $s7/$fp.
 *     Retail gives points $fp.
 * (2) width and extend (88: 3/289 -> 103, 89: 3/288 -> 104) swap $f23/$f24. Retail gives width $f23.
 * (3) The core loop's constant 2 takes $v1. Retail uses $v0.
 * (1) and (2) are integer-truncated priority ties. One more RTL insn live across the core loop
 * would flip both to retail (1525 < 1528; 103 = 103, lower regno wins).
 * With literals in the core loop, the prologue matches exactly, but at 1508 bytes: loop.c finds 1.0
 * "not desirable" (pass 2: 60 insns, threshold 60-62) and hoists zero as a copy.
 * -fmove-all-movables fixes the core loop, but it also hoists count - 2 and 1.0 out of the halo loop.
 * Setting the locals earlier, or as initializers, breaks the prologue.
 * Needs its own unit, because L10_0037D200.c line 125 declares a different prototype:
 * src/levels/shared/L10_0037D200-ribbon.c with
 * -O2 -G0 -ffunction-sections -frac2-qcopy -frac2-qcopy-barrier -frac2-hazard-labels,
 * and the declarations below. */
/* Gorn variant of the colored polyline ribbon renderer. */
#include "common.h"
typedef int Quad __attribute__((mode(TI)));
typedef union { Quad quad; float f[4]; } Vector;
typedef struct { Vector vertex[4]; u32 color[4]; float uv[8]; u64 primitive, texture, mode, flags; } RibbonPacket;
extern char D_L10_001B9680[];
extern u64 func_0027C878(int);
extern void func_00280B90(void *, void *, int), func_002FC560(int, u64);
extern void func_00282CC0(void *, void *, void *), func_00282CF0(void *, void *, void *);
extern void func_00282DD8(void *, void *, void *), func_00282D30(void *, void *, float);
extern float func_00282DF0(void *);
/* Draws the ribbon (texture 14) and its camera-facing halo (texture 16) along
 * the point chain. Each colour carries its own alpha and fades to the bare
 * colour at both ends. The EE ABI independently orders integer and FP
 * arguments: count is a3, innerColor t0, outerColor t1, coreWidth f12,
 * width f13 and extend f14. */
void func_L10_0039DA48(Vector *points, u32 alpha, u32 outerAlpha, float coreWidth, float width, float extend, int count, u32 innerColor, u32 outerColor) {
    RibbonPacket core, halo;
    Vector normal, widthVector, eye, tangent, first, last, firstRay, lastRay, toEye, extension;
    float zero, one;
    float length, initialLength, haloLength, firstLength, lastLength;
    int i;
    u32 coreColor, haloColor;
    core.texture = func_0027C878(14);
    coreColor = innerColor | (alpha << 24);
    core.flags = 0x8000000048ULL;
    core.mode = 0xFF9000000260ULL;
    core.primitive = 0;
    core.color[3] = coreColor; core.color[2] = coreColor;
    core.color[1] = coreColor; core.color[0] = coreColor;
    halo.texture = func_0027C878(16);
    haloColor = outerColor | (outerAlpha << 24);
    halo.flags = core.flags; halo.mode = core.mode; halo.primitive = core.primitive;
    halo.color[3] = haloColor; halo.color[2] = haloColor;
    halo.color[1] = haloColor; halo.color[0] = haloColor;
    halo.uv[0] = 0; halo.uv[1] = 0; halo.uv[2] = 0; halo.uv[3] = 1;
    halo.uv[4] = 1; halo.uv[5] = 0; halo.uv[6] = 1; halo.uv[7] = 1;
    core.uv[0] = 0; core.uv[1] = 0; core.uv[2] = 0; core.uv[3] = 1;
    core.uv[4] = 1; core.uv[5] = 0; core.uv[6] = 1; core.uv[7] = 1;
    func_00282CF0(&eye, points, D_L10_001B9680);
    func_00282CF0(&tangent, points + 1, points);
    func_00282DD8(&normal, &tangent, &eye);
    initialLength = func_00282DF0(&normal);
    if (initialLength != 0) initialLength = 1.0f / initialLength;
    func_00282D30(&normal, &normal, coreWidth * initialLength);
    func_00282CC0(&core.vertex[0], points, &normal);
    func_00282CF0(&core.vertex[1], points, &normal);
    zero = 0; one = 1.0f;
    for (i = 1; i < count - 1; i++) {
        func_00282CF0(&eye, points + i, D_L10_001B9680);
        func_00282CF0(&tangent, points + i + 1, points + i);
        func_00282DD8(&normal, &tangent, &eye);
        length = func_00282DF0(&normal);
        if (length != zero) length = one / length;
        func_00282D30(&normal, &normal, coreWidth * length);
        func_00282CC0(&core.vertex[2], points + i, &normal);
        func_00282CF0(&core.vertex[3], points + i, &normal);
        if (i == 1) { core.color[1] = innerColor; core.color[0] = innerColor; }
        else if (i == count - 1) { core.color[3] = innerColor; core.color[2] = innerColor; }
        else if (i == 2) { core.color[1] = coreColor; core.color[0] = coreColor; }
        func_00280B90(&core, 0, 0);
        qcopy(&core.vertex[0], &core.vertex[2]); qcopy(&core.vertex[1], &core.vertex[3]);
    }
    for (i = 0; i < count - 1; i++) {
        func_00282CF0(&firstRay, points + i, D_L10_001B9680);
        func_00282CF0(&lastRay, points + i + 1, D_L10_001B9680);
        firstLength = func_00282DF0(&firstRay);
        lastLength = func_00282DF0(&lastRay);
        if (lastLength < firstLength) {
            if (lastLength == 0) { qcopy(&lastRay, &firstRay); }
            else func_00282D30(&lastRay, &lastRay, firstLength / lastLength);
        } else if (firstLength == 0) { qcopy(&firstRay, &lastRay); }
        else func_00282D30(&firstRay, &firstRay, lastLength / firstLength);
        func_00282CC0(&first, D_L10_001B9680, &firstRay);
        func_00282CC0(&last, D_L10_001B9680, &lastRay);
        func_00282CF0(&tangent, &last, &first);
        func_00282D30(&extension, &tangent, extend);
        func_00282CF0(&first, &first, &extension);
        func_00282CC0(&last, &last, &extension);
        func_00282CF0(&toEye, D_L10_001B9680, &first);
        func_00282DD8(&normal, &toEye, &tangent);
        haloLength = func_00282DF0(&normal);
        if (haloLength != 0) haloLength = 1.0f / haloLength;
        func_00282D30(&widthVector, &normal, width * haloLength);
        func_00282CC0(&halo.vertex[0], &first, &widthVector);
        func_00282CF0(&halo.vertex[1], &first, &widthVector);
        func_00282CC0(&halo.vertex[2], &last, &widthVector);
        func_00282CF0(&halo.vertex[3], &last, &widthVector);
        if (i == 0) { halo.color[1] = outerColor; halo.color[0] = outerColor; }
        else if (i == count - 2) { halo.color[3] = outerColor; halo.color[2] = outerColor; }
        else if (i == 1) { halo.color[1] = haloColor; halo.color[0] = haloColor; }
        func_00280B90(&halo, 0, 0);
    }
    func_002FC560(0x47, 0x5360B);
}
