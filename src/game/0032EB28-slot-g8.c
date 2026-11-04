#include "common.h"

/* func_00334160: retail keeps the owned-flag mask 2 in a register from the first block and
   reuses it for the weapon type==2 test; the type tests are one || whose true arm jumps to a
   shared return, and D_001A7A80 is reached by an unsplit lui/addiu pair. */
typedef int Quad341 __attribute__((mode(TI)));
typedef struct {
    float vector[4];
    char pad10[8];
    unsigned char type;
    unsigned char source;
    short oclass;
    char pad1C[4];
    int flag;
    char pad24[0xC];
} Hit334160;
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(char **)((char *)(p)+(o)))
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define B(p,o) (*(unsigned char *)((char *)(p)+(o)))
#define H(p,o) (*(short *)((char *)(p)+(o)))
typedef struct { int active; unsigned char type; char pad[0x47]; short map; char pad2[0x92]; } Weapon1271;
typedef struct { float cost,a; int frames; float b; } Effect1271;
typedef struct { int timer,color,alpha; float zero,a,b; char *object; int state,owner,active; char pad[8]; } Slot1271;
extern Weapon1271 D_00239BA0[];
extern unsigned char D_001A7A80[] MACRO_ADDR;
extern unsigned char D_00139568[];
extern Effect1271 D_00237DA0[2][119];
extern Slot1271 D_00237800[30];
typedef struct { char pad[0xb8]; int owner; } Attacker341;
extern Attacker341 *D_001B1000 __attribute__((sda));
extern float *func_002AB1F0(void *);
extern int func_00334100(int);
extern void func_002A04B8(void *,int *,int *,int *);
extern void func_00282D30(void *,void *,float),func_00282F20(void *,void *,float);
extern void func_002A8C00(void *,void *,int,void *,float),func_002A8E20(void *,void *);
static __inline__ int IsSpecial1271(int id)
{
    int special=0;
    switch(id) {
    case 12:case 14:case 17:case 18:case 53:special=1;break;
    }
    return special;
}
static __inline__ int IsEffect1271(int id)
{
    Weapon1271 *weapon=&D_00239BA0[D_00139568[id]];
    if(weapon->active) {
        int type=weapon->type;
        if(type==2 || (type==1 && IsSpecial1271(id))) return 1;
    }
    return 0;
}
static __inline__ int IsEffect3_1271(int id)
{
    Weapon1271 *weapon=&D_00239BA0[D_00139568[id]];
    int active=0;
    if(weapon->active) active=weapon->type==3;
    return active;
}
