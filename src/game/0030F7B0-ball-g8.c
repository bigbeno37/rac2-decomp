#include "common.h"

typedef struct { float x, y, z; } BallVec3;
typedef struct {
    u32 points[4][4];
    u32 colors[4];
    float texcoords[8];
    u64 texture[4];
} __attribute__((aligned(16))) BallQuad;
extern float D_001AF12C __attribute__((sda));
extern float D_001AF250 __attribute__((sda));
extern float D_001AF260 __attribute__((sda));
extern float D_001AF264 __attribute__((sda));
extern int D_001AF230 __attribute__((sda));
extern int D_001AF234 __attribute__((sda));
extern int D_001AF238 __attribute__((sda));
extern int D_001AF23C __attribute__((sda));
extern int D_001AF240 __attribute__((sda));
extern u32 D_001AF244 __attribute__((sda));
extern u32 D_001AF248 __attribute__((sda));
extern u32 D_001AF24C __attribute__((sda));
extern u32 D_001AF254 __attribute__((sda));
extern u32 D_001AF258 __attribute__((sda));
extern u32 D_001AF25C __attribute__((sda));
extern char D_001B5340[];
extern char D_001B24C0[] MACRO_ADDR;
extern char D_002668D0[];
extern char D_00267AF0[];
extern char D_00268410[];
extern char D_00268A10[];
extern char D_00268B10[];
extern char D_00268C10[];
extern char D_00229C70[];
extern short D_001AF210[] SDATA(D_001AF210);
extern int D_001B24D0;
extern s64 func_0027C878(int);
extern void func_002FC560(int,u64);
extern void func_00282CC0(void *,void *,void *), func_00282CF0(void *,void *,void *), func_00282DD8(void *,void *,void *), func_002830C0(void *,void *,void *);
extern void func_00282F20(void *,void *,float), func_00282D30(void *,void *,float), func_00282F70(void *,void *,float);
extern void func_00280328(void), func_00281810(int,void *,void *,void *,int), func_00280B90(void *,void *,int);
extern int func_00283CF0(float), func_00312E10(unsigned char *);
extern float func_00283D00(float,float);
extern void func_002B00D8(void *,void *,void *,float);
/* Adapted from RAC1 draw_nanotech_ball (GPL-3.0); RAC2 selects alternate colors and owner geometry. */
