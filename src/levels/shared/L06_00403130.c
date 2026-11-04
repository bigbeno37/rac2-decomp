#include "common.h"

/* func_L06_00403130 updates the level's water sheet; RAC1's ocean_pokitaru_update
 * (homing_missile_and_water_sheet.c) is its counterpart. Its draw callback is
 * func_L06_00402570 (L06_00402570.c), which reads the same layer tables. */

typedef int OceanQuad __attribute__((mode(TI)));
typedef union { OceanQuad q; float f[4]; } OceanVector;
typedef struct { float u, v; } OceanUV;

typedef struct {
    int drawMode;          /* 0x00: its sign selects the draw list */
    char pad04[0x10];
    float level;           /* 0x14 */
    float scale;           /* 0x18 */
    char pad1C[0x4];
    float limit;           /* 0x20 */
    char pad24[0xC];
    int outerArea;         /* 0x30 */
    int innerArea;         /* 0x34 */
    int f38;               /* 0x38 */
    float amplitude;       /* 0x3C */
} OceanVars;

typedef struct {
    char pad00[0x20];
    unsigned char state;   /* 0x20 */
    char pad21[0xF];
    unsigned char f30;     /* 0x30 */
    char pad31[0x37];
    OceanVars *vars;       /* 0x68 */
} OceanMoby;

typedef struct {
    char pad000[0x80];
    float position[4];     /* 0x80 */
    char pad090[0x16E];
    short waterState;      /* 0x1FE */
    char pad200[0x2094];
    int action;            /* 0x2294 */
    int pad2298;
    int animation;         /* 0x229C */
} OceanHero;

typedef struct {
    char pad000[0x140];
    OceanVector pos;       /* 0x140 */
    float f150, f154;
    float angle;           /* 0x158 */
} OceanCamera;

extern void func_L00_002E6CF0(void (*)(void *), void *);
extern void func_L00_002E6DB8(void (*)(void *), void *);
extern void func_L06_00402570(void *);
extern int func_002ACBC8(void *, int);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282D30(void *, void *, float);
extern int func_00283CF0(float);
extern float func_00283CE0(int);
extern float func_00283198(float);

extern OceanHero D_00189E20;
extern OceanCamera D_L06_001B9700;
extern int D_L06_001C9DC0;
extern int D_001A79F0 __attribute__((sda));
extern float D_001A7928 __attribute__((sda));
extern int D_L06_001A9010 __attribute__((sda));
extern int D_L06_001A8A48 __attribute__((sda));
extern int D_L06_001B2348 __attribute__((sda));
extern float D_L06_001B3280 __attribute__((sda));
extern float D_L06_001B3284 __attribute__((sda));
extern OceanVector D_L06_001B9A70[];
extern OceanVector D_L06_001B3290[3] __attribute__((sda));
extern OceanUV D_L06_001B3270[] __attribute__((sda));
extern OceanVector D_L06_001B3260 SDATA(D_L06_001B3260);
extern float D_L06_001B0060[] SDATA(D_L06_001B0060);
/* Layer scroll speeds: retail reads the u column through $gp and the v column
 * (0x001B0074) through an absolute address, so the table has both views. */
extern OceanUV D_L06_001B0070[] __attribute__((sda));
extern OceanUV D_L06_001B0070_gp[] SDATA(D_L06_001B0070);
