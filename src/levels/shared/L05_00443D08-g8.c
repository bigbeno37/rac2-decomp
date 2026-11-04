#include "common.h"
typedef unsigned int ShotQuad __attribute__((mode(TI)));
typedef union { ShotQuad q; float f[4]; } ShotVector;
extern int D_L05_001B2048, D_00138320[], D_L05_002A1940[];
typedef struct { int upgrade; char pad[12]; int maximum, ammo; char tail[16]; } WeaponSlot;
extern WeaponSlot D_L05_002A1800[];
extern char D_00188660[],D_L05_002B8770[];
extern float D_L05_001B1A9C __attribute__((sda)), D_L05_001B1B70 __attribute__((sda));
extern unsigned short D_L05_001B1AA0 __attribute__((sda)),D_L05_001B1AA4 __attribute__((sda)),D_L05_001B1AA8 __attribute__((sda));
extern int D_L05_001B1AAC __attribute__((sda)),D_L05_001B1AB0 __attribute__((sda)),D_L05_001B1AFC __attribute__((sda));
extern float D_L05_001B1B30 __attribute__((sda)),D_L05_001B1B40 __attribute__((sda)),D_L05_001B1B50 __attribute__((sda)),D_L05_001B1B60 __attribute__((sda));
extern int func_00282978(void *),func_00282948(void *),func_L05_0037AA88(int);
extern void func_00282CF0(void *,void *,void *),func_00282CC0(void *,void *,void *),func_00282F20(void *,void *,float),func_00283098(void *,void *,void *),func_00282D30(void *,void *,float);
/* The sound call and the four spawners below return a value this function ignores. Declaring them
   void changes the code: their call_value is what ends the zero register's dependence on the later
   `v0 = 5`, which orders each clear's por after its constant and keeps the upgrade test in v1. */
extern int func_002E5CF8(int,int,void *);
extern void func_L05_0037AB18(int,int);
extern char *func_L05_00419CC8(void *,void *,float,void *,int,int,int,int,int);
extern char *func_L05_003D0960(void *,void *,int,void *,int),*func_L05_003979B0(void *,void *,int,void *,int),*func_L05_00427090(void *,void *,int,void *,int,int),*func_L05_00427988(void *,void *,int,void *,int);
extern int func_002E5DA0(int,int,void *,int),func_L00_002D6998(void *);
extern void func_002E59F8(int);
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define H(p,o) (*(short *)((char *)(p)+(o)))
#define B(p,o) (*(unsigned char *)((char *)(p)+(o)))
#define P(p,o) (*(char **)((char *)(p)+(o)))
/* Fires the ship's weapons for one frame: a pair of shots on input bit 0x40, four weapons on
   input bit 0x20, each behind its own timer, and a held weapon on 0x80, whose ammunition
   recharges every third frame while it is released. */
