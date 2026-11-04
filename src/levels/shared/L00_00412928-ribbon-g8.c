#include "common.h"

/* func_L00_00413918 reads its ribbon colour and widths through $gp, hence -G8; it is built
 * with -g like the original, whose line notes give the two nops before the division. */

typedef unsigned int Quad __attribute__((mode(TI)));
typedef union { Quad q; float f[4]; } Vec;

/* A textured quad as func_00280B90 draws it. */
typedef struct {
    Vec v[4];
    unsigned color[4];
    float uv[4][2];
    long tag0, texture, tag2, tag3;
} QuadPacket;

typedef struct { Vec row[3], pos; } Frame;

extern unsigned D_L00_001B1380 __attribute__((sda));
extern float D_L00_001B1384 __attribute__((sda));
extern float D_L00_001B1388 __attribute__((sda));
extern float D_L00_001B138C __attribute__((sda));
extern int D_L00_001B13D0 __attribute__((sda));
extern char D_L00_001B8C80[];

extern long func_0027C878(int id);
extern void func_00282CF0(void *dst, void *a, void *b);
extern void func_00282F20(void *dst, void *src, float length);
extern void func_00282DD8(void *dst, void *a, void *b);
extern void func_00282D30(void *dst, void *src, float scale);
extern void func_002830C0(void *dst, void *src, void *frame);
extern void func_00280B90(void *packet, int a, int b);
extern float func_00283CE0(int value);
extern float func_002A7AA8(float a, float b, float c, float d, float t);

/* Draws a two-layer ribbon through count points, one quad per segment. */
