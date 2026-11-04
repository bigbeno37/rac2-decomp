#include "common.h"

typedef int Quad __attribute__((mode(TI)));
typedef union { Quad quad; float f[4]; } RopeVector;
extern RopeVector D_L03_00238040[][16];

typedef struct {
    unsigned char pad_0[0x28];
    int groups[4];
    int count;
    int flags;
    unsigned char pad_40[0x44];
    float cycle;
    float cycleRate;
    unsigned char pad_8C[0x4];
    float jitterScale;
    float phaseA;
    float rate;
    unsigned char pad_9C[0x4];
    float amplitude;
    float swayScale;
    float phaseB;
    unsigned char pad_AC[0x28];
    float radius;
    unsigned char pad_D8[0x4];
    int pool;
    float center[4];
} RopeState;

extern float (*D_L03_001FE700[])[4];
extern RopeVector D_L03_00237F40[];
extern float D_00189EA0[];
extern float func_00283B98(float, float);
extern void func_00282CF0(void *, void *, void *);
extern float func_00282DF0(void *);
extern void func_00282F20(void *, void *, float);
extern void func_00282D30(void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern float func_002ABB18(float);
extern float func_00283198(float);
extern float func_00283180(float);
extern float func_00283248(float, float);
