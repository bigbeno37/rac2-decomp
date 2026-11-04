/* func_00329488: draw every particle in the pool that belongs to an object, then release them.
   Its own unit for -fno-gcse: each loop loads the pool's address in its own preheader. The pool
   address is an int in the first loop so that the counter survives and the loop test stays on i. */
#include "common.h"

typedef struct {
    float v[4];
    float f10;
    float f14;
    float f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int pad2C;
    int f30;
    int f34;
    void *owner;
    int pad3C;
} F328Tail;

typedef struct {
    float m[4];
    F328Tail t;
} F328Particle;

extern F328Particle D_0022C9A0[300];
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern float func_00283180(float);
extern void func_002820A0(void);
extern void func_002820C8(void);
extern void func_002820D0(float, float, float, void *, int, float **);
extern void func_00282CC0(void *, void *, void *);
extern void func_00329630(char *);
