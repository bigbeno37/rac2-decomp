#include "common.h"



typedef int Rac2TI128 __attribute__((mode(TI)));

typedef struct Vec_L00_003C27A0 {
    Rac2TI128 v;
} Vec_L00_003C27A0;

typedef struct State_L00_003C27A0 {
    f32 home[4];        /* 0x00 */
    s32 group;          /* 0x10 */
    s32 group_b;        /* 0x14 */
    s32 x18;            /* 0x18 */
    f32 spin;           /* 0x1C */
    f32 x20;            /* 0x20 */
    f32 x24;            /* 0x24 */
    f32 x28;            /* 0x28 */
    f32 x2C;            /* 0x2C */
    f32 x30;            /* 0x30 */
    f32 x34;            /* 0x34 */
    f32 x38;            /* 0x38 */
    f32 x3C;            /* 0x3C */
    s32 gate;           /* 0x40 */
    f32 x44;            /* 0x44 */
    s32 spawn;          /* 0x48 */
    u8 *moby;           /* 0x4C */
    u8 pad50[0x58 - 0x50];
    f32 angle;          /* 0x58 */
    u8 pad5C[0x70 - 0x5C];
    u8 track[0x1FB0 - 0x70];    /* 0x70 */
    u8 *target;         /* 0x1FB0 */
} State_L00_003C27A0;

extern s32 D_L00_001C9340 __attribute__((nosda));
extern s32 D_001A79F0[] __attribute__((sda));
extern u16 **D_L00_001A9014;
extern u16 *D_L00_001FA920[];
extern u8 *D_L00_001B1E9C;
extern u8 *D_L00_001B251C;
extern u8 *D_L00_001B2514;
extern s16 D_L00_00226900[];
extern Vec_L00_003C27A0 D_L00_001AFEB0[] __attribute__((sda));
extern Vec_L00_003C27A0 D_L00_001AFEA0 __attribute__((sda));
extern s32 D_L00_001AFE98[2] __attribute__((sda));
extern s32 D_L00_001AFE90[2] __attribute__((sda));

extern u8 *func_0029EFD0(s32);
extern void func_002B37E8(void *, void *);
extern void func_L00_00323448(void *, void *, s32, s32, s32, s32, s32, s32);
extern void func_L00_00323E48(void *, int, int, void *);
extern void func_00282C88(void *);
extern float func_00283248(float, float);
extern void *func_002A9640(void *, s32, s32);
extern void func_002833F0(void *, void *);
extern float func_00282C48(float);
extern void func_L00_0032A530(void *, void *, void *, void *, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_002A7820(s32, s32);
extern void func_L00_00321878(void *, s32, s32, s32, s32, s32, float);
extern s32 func_002E5DA0(s32, s32, void *, s32);
extern void func_L00_003C2DE8(char *, char *, float, float);
extern void *func_0032C700(void *, int, int, int, int, int, int, int, float, float, int, int, int, int);
extern s32 func_002E5CF8(s32, s32, void *);
extern void func_0029F338(void *);
extern s32 func_00283DB8(void *, float);
extern void func_002AEE70(void *, void *, void *, float, float, float);
extern void func_002AEF28(void *, void *, void *, float, float, float);
extern float func_00283B98(float, float);
extern void func_L00_00323958(void *, void *);
