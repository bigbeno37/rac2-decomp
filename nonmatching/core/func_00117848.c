/* Opus 5.5 (opus-1), 3 November 2026: FAILED, 1048/1108 bytes, 0/1 places
 * (154 of 277 rows differ).
 * Profile: its own unit, src/core/00117848.c with
 *   -O2 -G2 -ffunction-sections -mastra-quad-saves
 * (retail saves s0-s2/ra with sq; the sdk profile of 001163B0.c saves only ra
 * and CSEs the two dpmul calls, retail does not).
 * newlib libm __ieee754_fmod (math/e_fmod.c) with newlib fdlibm.h's
 * little-endian ieee_double_shape_type and EXTRACT_WORDS/INSERT_WORDS
 * (plain blocks; do-while(0) gives identical bytes). Now right: s1=x, s2=y,
 * s0=x*y, two dpmul + dpdiv calls, Zero[] = D_0013A3F8, `x *= one` folds away
 * (retail has no call there).
 * Still differs: retail keeps INSERT_WORDS' mask ops on the uninitialised
 * union pseudo (and t3,t3,0x00000000FFFFFFFF; or; and ...,0xFFFFFFFF00000000;
 * or s1) at both output sites; our combine folds them to (hi<<32)|zext(lo)
 * and, unable to place the dead union's REG_DEAD note, emits a (use (reg))
 * at a label (combine.c distribute_notes), which reload gives a stack slot:
 * that is the extra 16 bytes (frame 0x50 vs 0x40, "vars= 16"). Fix the fold
 * and the frame follows. Tried: signed fields, u32 w[2] array union,
 * fdlibm 5.3 __HI/__LO pointer casts, u64 union (no slot but no masks):
 * all fold. Next: find the source form whose and/or chain combine cannot
 * merge (3-insn limit), e.g. compare with another newlib-built libm object.
 * Declarations needed: everything below (D_0013A3F8 is extern).
 */
#include "common.h"

/* @(#)e_fmod.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/* newlib libm's __ieee754_fmod (math/e_fmod.c) with its fdlibm.h word
 * access macros. The signed-zero table Zero[] is D_0013A3F8; `x *= one`
 * folds away, so `one` emits nothing. */

typedef union
{
    double value;
    struct
    {
        u32 lsw;
        u32 msw;
    } parts;
} ieee_double_shape_type;

#define EXTRACT_WORDS(ix0, ix1, d)  \
    {                               \
        ieee_double_shape_type ew_u; \
        ew_u.value = (d);           \
        (ix0) = ew_u.parts.msw;     \
        (ix1) = ew_u.parts.lsw;     \
    }

#define INSERT_WORDS(d, ix0, ix1)   \
    {                               \
        ieee_double_shape_type iw_u; \
        iw_u.parts.msw = (ix0);     \
        iw_u.parts.lsw = (ix1);     \
        (d) = iw_u.value;           \
    }

static const double one = 1.0;
extern const double D_0013A3F8[2];
#define Zero D_0013A3F8

double func_00117848(double x, double y)
{
    s32 n, hx, hy, hz, ix, iy, sx, i;
    u32 lx, ly, lz;

    EXTRACT_WORDS(hx, lx, x);
    EXTRACT_WORDS(hy, ly, y);
    sx = hx & 0x80000000;       /* sign of x */
    hx ^= sx;                   /* |x| */
    hy &= 0x7fffffff;           /* |y| */

    /* purge off exception values */
    if ((hy | ly) == 0 || (hx >= 0x7ff00000) ||  /* y=0,or x not finite */
        ((hy | ((ly | -ly) >> 31)) > 0x7ff00000)) /* or y is NaN */
        return (x * y) / (x * y);
    if (hx <= hy) {
        if ((hx < hy) || (lx < ly)) return x;    /* |x|<|y| return x */
        if (lx == ly)
            return Zero[(u32)sx >> 31];          /* |x|=|y| return x*0*/
    }

    /* determine ix = ilogb(x) */
    if (hx < 0x00100000) {  /* subnormal x */
        if (hx == 0) {
            for (ix = -1043, i = lx; i > 0; i <<= 1) ix -= 1;
        } else {
            for (ix = -1022, i = (hx << 11); i > 0; i <<= 1) ix -= 1;
        }
    } else ix = (hx >> 20) - 1023;

    /* determine iy = ilogb(y) */
    if (hy < 0x00100000) {  /* subnormal y */
        if (hy == 0) {
            for (iy = -1043, i = ly; i > 0; i <<= 1) iy -= 1;
        } else {
            for (iy = -1022, i = (hy << 11); i > 0; i <<= 1) iy -= 1;
        }
    } else iy = (hy >> 20) - 1023;

    /* set up {hx,lx}, {hy,ly} and align y to x */
    if (ix >= -1022)
        hx = 0x00100000 | (0x000fffff & hx);
    else {      /* subnormal x, shift x to normal */
        n = -1022 - ix;
        if (n <= 31) {
            hx = (hx << n) | (lx >> (32 - n));
            lx <<= n;
        } else {
            hx = lx << (n - 32);
            lx = 0;
        }
    }
    if (iy >= -1022)
        hy = 0x00100000 | (0x000fffff & hy);
    else {      /* subnormal y, shift y to normal */
        n = -1022 - iy;
        if (n <= 31) {
            hy = (hy << n) | (ly >> (32 - n));
            ly <<= n;
        } else {
            hy = ly << (n - 32);
            ly = 0;
        }
    }

    /* fix point fmod */
    n = ix - iy;
    while (n--) {
        hz = hx - hy; lz = lx - ly; if (lx < ly) hz -= 1;
        if (hz < 0) { hx = hx + hx + (lx >> 31); lx = lx + lx; }
        else {
            if ((hz | lz) == 0)         /* return sign(x)*0 */
                return Zero[(u32)sx >> 31];
            hx = hz + hz + (lz >> 31); lx = lz + lz;
        }
    }
    hz = hx - hy; lz = lx - ly; if (lx < ly) hz -= 1;
    if (hz >= 0) { hx = hz; lx = lz; }

    /* convert back to floating value and restore the sign */
    if ((hx | lx) == 0)             /* return sign(x)*0 */
        return Zero[(u32)sx >> 31];
    while (hx < 0x00100000) {       /* normalize x */
        hx = hx + hx + (lx >> 31); lx = lx + lx;
        iy -= 1;
    }
    if (iy >= -1022) {  /* normalize output */
        hx = ((hx - 0x00100000) | ((iy + 1023) << 20));
        INSERT_WORDS(x, hx | sx, lx);
    } else {        /* subnormal output */
        n = -1022 - iy;
        if (n <= 20) {
            lx = (lx >> n) | ((u32)hx << (32 - n));
            hx >>= n;
        } else if (n <= 31) {
            lx = (hx << (32 - n)) | (lx >> n); hx = sx;
        } else {
            lx = hx >> (n - 32); hx = sx;
        }
        INSERT_WORDS(x, hx | sx, lx);
        x *= one;       /* create necessary signal */
    }
    return x;       /* exact output */
}
