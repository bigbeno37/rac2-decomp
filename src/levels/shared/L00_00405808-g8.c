#include "common.h"

#define I(p,o) (*(int *)((char *)(p)+(o)))
#define H(p,o) (*(short *)((char *)(p)+(o)))
#define U(p,o) (*(unsigned short *)((char *)(p)+(o)))
#define B(p,o) (*(unsigned char *)((char *)(p)+(o)))
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define P(p,o) (*(char **)((char *)(p)+(o)))
typedef int Quad __attribute__((mode(TI)));
typedef struct { float x,y,z,w; } __attribute__((aligned(16))) Vec;
typedef struct {
    char pad0[0x730]; float basis[12];
    char pad760[0x1230-0x760]; int buttons;
    char pad1234[0x1244-0x1234]; int itemState;
    char pad1248[0x2290-0x1248]; char *object; int current;
    int pad2298; unsigned int control;
    char pad22a0[0x24ec-0x22a0]; short direction;
    char pad24ee[0x2534-0x24ee]; int timer;
} Hero;
typedef struct { char pad[0x360]; unsigned short kills, pad362; unsigned int best, levels; } Stats;
typedef struct { int present; unsigned char type; char pad[219]; } Upgrade;
typedef struct { float color[4]; float pos[3]; float radius; } Light;
typedef struct { float pos[4]; char *target; float total, left; short timer, seen;
    char pad20[0x44-0x20]; int delay; float yaw,pitch; } Ray;
typedef struct { char pad0[0x74]; unsigned char playing; char pad75[0x88-0x75]; char *owner; } Voice;
extern Hero D_00189E20;
extern Stats D_0018C360;
typedef struct { unsigned char pad0[0x10]; unsigned char current; unsigned char pad11[0x48-0x11]; unsigned char equipped; } Inventory;
extern Inventory D_00139568;
extern char D_00188660[];
extern Upgrade D_L00_00262570[];
extern char D_L00_001B8C80[];
extern Light D_L00_001C6F40[];
extern int D_001A7390 MACRO_ADDR, D_001A79F0 MACRO_ADDR;
extern unsigned char D_001A7A60[32] __attribute__((sda));
extern int D_00138320 __attribute__((nosda));
extern int D_L00_001B2C74 MACRO_ADDR;
extern int D_L00_001B2C78 MACRO_ADDR;
extern int D_L00_001B2C7C MACRO_ADDR;

extern int D_L00_001B0E30 __attribute__((sda));
extern float D_L00_001B0E3C __attribute__((sda));
extern float D_L00_001B0E48 __attribute__((sda));
extern int D_L00_001B2C70 __attribute__((sda));
extern int D_L00_001A903C;
extern float func_0029F530(void *);
extern int func_00282948(void *);
extern void func_00282978(void *);
extern void func_002A7470(void *,int,int,int);
extern void func_00283098(void *,void *,void *);
extern void func_00282CC0(void *,void *,void *);
extern void func_L00_002C42E0(int,int);
extern void func_00283678(void *,void *);
extern void func_00283698(void *,void *);
extern void func_002830C0(void *,void *,void *);
extern void func_00282D30(void *,void *,float);
extern float func_00283248(float,float);
extern float func_00282E20(void *);
extern char *func_L00_004051C8(void *,void *,void *,void *);
extern char *func_002AB1F0(void *);
extern float func_00283CE0(int);
extern void func_00290010(int,int,int,float);
extern int func_002E5E58(int,int,void *);
extern void func_002B0A18(int,int);
extern int func_002E5CF8(int,int,void *);
extern void func_L00_00408800(void *);
extern void func_L00_00406150(void *);
extern int func_00291308(void *,float,float,unsigned int);
extern void func_00282F20(void *,void *,float);
extern int func_002E59B0(void *,int);
extern void func_00291450(int);
extern void func_002E59F8(int);
extern void func_002A75E0(void *,int,int,int,int);
