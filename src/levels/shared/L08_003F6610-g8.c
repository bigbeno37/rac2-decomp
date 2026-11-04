#include "common.h"
typedef int TI128 __attribute__((mode(TI)));
extern u8 D_00189EA0[];
extern void func_00282CF0(void *,void *,void *);
extern void func_00282D30(void *,void *,float);
extern void func_00282CC0(void *,void *,void *);
#define E_I(p,o) (*(int *)((u8 *)(p)+(o)))
#define E_F(p,o) (*(float *)((u8 *)(p)+(o)))
extern void func_L08_003F6348(void *,void *);
extern float func_00282E48(void *,void *);
extern void func_002A93B0(void *);
extern int D_L08_001B2508;
extern u8 D_L08_001B9B00[];
extern u8 *D_L08_001FE7C0[];
extern u8 *D_L08_001B2ACC __attribute__((sda));
extern float D_L08_001B0B5C __attribute__((sda));
extern u8 D_L08_001AC5E0[] MACRO_ADDR;
extern void func_0029F338(void *);
extern int func_002A8600(void **,int,int,int);
extern int func_00282978(void *);
extern int func_002A77E0(int);
extern void func_L04_003429F8(void *,void *,void *,int,void *,int,float);
extern int func_L08_003F6568(void *,void *);
extern void func_002A7470(void *,int,int,int);
extern float func_0029F530(void *);
extern float func_00282DF0(void *);
extern void func_00282FB8(void *,void *,float);
extern void func_00282F20(void *,void *,float);
extern int func_002A7820(int,int);
extern void func_L02_0031E9E8(void *,float);
extern float func_002A7878(float,float);
extern float func_002A89A8(void *,int,float);
extern void func_0029FCC0(void *,int,void *);
extern void func_002ACB48(void *,float);
extern float func_L00_0031CDD0(void *,void *,int,float,float);
extern u8 *func_L08_003F6018(void *,void *,void *);
extern int func_L06_0035EA68(void *,void *);
extern void func_002ABBB8(void *,void *,void *,float,float,int,int,int,float,float,float,float,int,float,int,int,int,int,int);
typedef struct { u8 pad[0x120]; float speed; int path,object; u16 delay, timer; u8 limit,count,nearest,pad2; int children[2]; int pad3; int targets[4]; } FVars;
