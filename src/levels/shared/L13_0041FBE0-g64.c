#include "common.h"

/* func_L13_0041FBE0 (-G64, split addresses, -g, -fno-cse-skip-blocks).
   The color table D_L13_001B1A90 and texture pair D_L13_001B1A40 are read
   through $gp at element 0 and as absolute sym+offset macro loads after it:
   the _1 views have no declared size, so the assembler keeps them absolute
   and cc1 neither splits their %hi nor puts them in delay slots. */

typedef int Quad __attribute__((mode(TI)));
typedef union { Quad quad; float f[4]; } Vector;
typedef struct { Vector vertex[4]; unsigned int color[4]; float uv[4][2]; unsigned long long state[4]; } DrawPacket;
extern float D_L13_001B98C0[];
extern unsigned int D_L13_001B1A90 __attribute__((sda));
extern unsigned int D_L13_001B1A90_1[] __attribute__((sda));
extern int D_L13_001B1A40 __attribute__((sda));
extern int D_L13_001B1A40_1[] __attribute__((sda));
extern float D_L13_001B1A50[4][2] __attribute__((sda));
extern float D_L13_001B1A70[4][2] __attribute__((sda));
extern Vector D_L13_001B1AB0[4] __attribute__((sda));
extern Vector D_L13_001B1AF0[4] __attribute__((sda));
extern Vector D_L13_001B1B30[4] __attribute__((sda));
extern float D_L13_001B1BB0 __attribute__((sda));
extern float D_L13_001B1BB4 __attribute__((sda));
extern float D_L13_001B1BB8 __attribute__((sda));
extern int D_L13_001B1BBC __attribute__((sda));
extern int D_L13_001B1BC0 __attribute__((sda));
extern void func_00283658(void *, void *);
extern void func_00282F20(void *, void *, float);
extern void func_00282CF0(void *, void *, void *);
extern void func_002AFDD8(char *, char *, char *, char *);
extern void func_00282D30(void *, void *, float);
extern float func_00283198(float);
extern unsigned int func_00283D38(unsigned int, unsigned int, float);
extern void func_L02_002EF968(void *, int, unsigned int, void *, void *, int);
extern void *func_00115484(void *, int, unsigned int);
extern void func_002833F0(void *, void *);
extern void func_L01_002E1510(void *, int, void *, int, void *);
extern void func_00282CC0(void *, void *, void *);
