#include "include_asm.h"
#include "common.h"

/* func_L05_004432E0 needs -g -frac2-qcopy -frac2-qcopy-barrier besides -G8, so it has a unit
   of its own (config/cflags.txt). */

typedef int Quad __attribute__((mode(TI)));
typedef union { Quad q; float f[4]; } Vector_L05_004432E0;

extern char *D_001B1EC4, *D_001B251C;
extern int D_001A79F0;
/* Retail splits this address: its lui fills a branch delay slot on its own. */
extern char *D_L05_00221D88 __attribute__((nosda));
extern char *D_L05_001B2614 __attribute__((sda));
/* The output vector. As an incomplete sda array its address stays one `la` (an adjacent
   lui/addiu pair), which reorg does not copy into the early exits' delay slots. */
extern char D_L05_001B3600[] __attribute__((sda));
extern float D_L05_001B1AC0 __attribute__((sda));
extern float D_L05_001B1AC4 __attribute__((sda)), D_L05_001B1AC8 __attribute__((sda));
extern float D_L05_001B1BF0 __attribute__((sda)), D_L05_001B1BF4 __attribute__((sda));
extern float D_L05_001B1BD0 __attribute__((sda)), D_L05_001B1BD4 __attribute__((sda));
extern float D_L05_001B1BD8 __attribute__((sda)), D_L05_001B1BDC __attribute__((sda));
extern float D_L05_001B1BE0 __attribute__((sda)), D_L05_001B1BE4 __attribute__((sda));
extern float D_L05_001B1BE8 __attribute__((sda)), D_L05_001B1BEC __attribute__((sda));
extern float D_L05_001B1A7C __attribute__((sda));
extern int func_L05_00379F38(void);
extern float func_002702C8(float *velocity, float current, float goal, float a, float b, float c);
extern float func_00283CE0(int);
extern float func_00283198(float);
extern float func_00282DF0(void *);
extern void func_00282D30(void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);

#define FRAME_TIME 0.0166666675f /* 1/60 s */
#define DEG_TO_RAD 0.017453294f

static inline float Lerp_L05_004432E0(float from, float to, float t)
{
    return from + (to - from) * t;
}

/* Moves the camera position at camera+0x30 along its moby's +0xC0 direction by the moby
   state's speed, which outside level 20 first approaches a goal (func_002702C8). Level 20 adds
   a height and a side step interpolated from the record's +0x50 block; level 15 adds an
   interpolated climb (written per arm, so jump2 cross-jumps the common tail) and clamps the
   height to 400..650. A sway along camera+0x10 driven by the record's +0xD4 block follows, then
   the [5, 1000] box clamp and a pull back towards entry `index` of D_L05_001B2614. The frame's
   motion is accumulated at record+0x30; the position is copied to D_L05_001B3600. */
