#include "common.h"
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define H(p,o) (*(short *)((char *)(p)+(o)))
typedef struct { int first,second,third; } AllocationOffsets;
typedef struct { void *data; int first,second; float cooldown; int timer; void *target,*previous; short acquire,release; } WeaponContext;
typedef struct HeroView {
char pad0[1180];
unsigned char field49c;
char pad49d[1935];
int fieldc2c;
char padc30[1536];
int field1230;
char pad1234[12];
int field1240;
char pad1244[700];
int field1500;
char pad1504[3468];
unsigned char * field2290;
int field2294;
char pad2298[4];
int field229c;
char pad22a0[528];
int field24b0;
char pad24b4[128];
int field2534;
} HeroView;
extern HeroView D_00189E20;
extern unsigned int D_00138320;
extern unsigned char D_0013957E;
typedef struct { char pad[0x54]; float rate; char rest[0x88]; } WeaponEntry;
extern WeaponEntry D_L00_00262570[];
extern void func_L00_003B6158(void *), func_L00_003B60E8(void);
extern int func_L00_002D4040(int);
extern void func_00273318(void *,float,unsigned int,void *,int,float,int,float,int);
extern void *func_L00_00329E90(void *,void (*)(void),int *);
extern char *func_L00_0032A088(int,void *),*func_L00_00329FF0(void *);
extern AllocationOffsets *func_L00_0032A0B8(int,void *);
extern void func_00282A88(void *,int);
extern int func_00282948(int *),func_002829D8(float *);
extern int func_L00_002B3998(float);
extern void func_L00_002AF568(void),func_L00_00311080(void *,void *,int,int,int,int);
extern void func_L00_002D3F30(int,int),func_002E5CF8(int,int,void *);
extern char *func_L00_0041C2D0(void *,void *,int,void *,int,int);
