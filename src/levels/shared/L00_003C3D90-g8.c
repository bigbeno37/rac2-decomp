#include "common.h"
extern int D_L00_001A8F00;
extern char D_00189EA0[];
extern char D_00188660[];
extern unsigned char D_00139568[];
typedef struct { char pad0[4]; unsigned char active; char pad5[0x6b]; float strength; char pad74[0x6c]; } WeaponEntry;
extern WeaponEntry D_L00_00262570[];
typedef struct { char pad[0xb4]; int changed; } ChangedState;
extern ChangedState D_00139868;
typedef struct { char pad0[0x80]; float position[4]; char pad90[0x11b0]; int upgrades; char pad1244[0x104c]; char *moby; char pad2294[0x28]; unsigned char busy; } HeroState;
extern HeroState D_00189E20_13;
extern char D_L00_001B8DC0[];
extern char D_L00_0023F040[];
typedef struct { float v[4]; } EffectVector;
extern EffectVector D_L00_00240080[][10] __attribute__((sda));
extern int D_L00_001B2950[] __attribute__((sda));
extern int D_L00_001B2970[] __attribute__((sda));
extern short D_L00_001B2990[][8] __attribute__((sda));

typedef struct { char pad[0xf8]; unsigned char *f8; } ParticleDefs;
extern ParticleDefs D_L00_001FE940;
extern int D_L00_001AFF98 __attribute__((sda));
extern int D_L00_001AFF44 __attribute__((sda));
extern int D_L00_001AFF90 __attribute__((sda));
extern int D_L00_001AFF10 __attribute__((sda));
extern int D_L00_001AFF14 __attribute__((sda));
extern int D_L00_001AFF18 __attribute__((sda));
extern int D_L00_001AFF1C __attribute__((sda));
extern int D_L00_001AFF24 __attribute__((sda));
extern float D_L00_001AFF2C __attribute__((sda));
extern float D_L00_001AFF40 __attribute__((sda));
extern float D_L00_001AFF4C __attribute__((sda));
extern int D_L00_001AFF64 __attribute__((sda));
extern int D_L00_001AFF68 __attribute__((sda));
extern float D_L00_001AFF6C __attribute__((sda));
extern int D_L00_001AFF70 __attribute__((sda));
extern float D_L00_001AFF7C __attribute__((sda));
extern void func_L00_0037E640(void);
extern void func_00282978_1(void *);
extern int func_002E59B0(void *,int);
extern int func_002E5CF8(int,int,void *);
extern void func_002E59F8(int);
extern int func_L00_003C3A70(int);
extern float func_L00_0037E668(void);
extern int func_L00_003C5B18(float);
extern void func_L00_0037E698(void);
extern void func_0029F338(void *);
extern float func_00282C48(float);
extern int func_00283CF0(float);
extern float func_00283CE0(int);
extern int func_00283D38(float,int,int);
extern int func_00282978(void *);
extern void func_00282D30(void *,void *,float);
extern void func_00282CC0(void *,void *,void *);
extern void func_0029FCC0(void *,int,void *);
extern int func_002A7820(int,int);
extern float func_002A7940(void);
extern void func_002AF000(void *,float,float,float);
extern char *func_002C36D0(void *,int,int,int);
extern int func_00282948(void *);
extern int func_002829D8(void *);
extern int func_00277F48(void *,int,void *,int,float);
extern float *func_002AB1F0(void *);
extern void func_00282F20(void *,void *,float);
extern void func_00282CF0(void *,void *,void *);
extern void func_002A8E20(void *,void *);
extern void func_L00_003C58E8(void *,int);
extern char *func_L00_00412FF8(void *,int);
extern void func_002A7988(void *,float,float);
extern float func_002A7878(float,float);
extern void func_002ACEC0(void *,void *,void *,float);
extern void func_L00_002E6CF0(void *,void *);
extern void func_L00_003C4850(void *);
#define WORD(p,o) (*(int *)((char *)(p)+(o)))
#define FLOAT(p,o) (*(float *)((char *)(p)+(o)))
#define HALF(p,o) (*(short *)((char *)(p)+(o)))
#define BYTE(p,o) (*(unsigned char *)((char *)(p)+(o)))
typedef struct { char pad0[0x46]; short category; } MobyInfo;
typedef struct EffectContext EffectContext;
typedef struct Moby { char pad0[0x10]; float x,y,z,w; unsigned char state; char pad21[3]; MobyInfo *info; char pad28[9]; unsigned char enabled; char pad32[0x68-0x32]; EffectContext *ctx; char pad6c[0xaa-0x6c]; short oclass; char padac[0xe0-0xac]; float forward[4]; } Moby;
typedef struct { char pad0[4]; Moby *moby; char pad8[8]; float dx,dy,dz; } ParticleOwner;
typedef struct { char pad0[2]; unsigned char sprite; char pad3; int colour; char pad8[4]; float size; float x,y,z; char pad1c[4]; ParticleOwner owner; char pad3c[0xb8-0x3c]; Moby *source; } Particle;
typedef struct { float x,y,z; float damage; Moby *source; int flags; unsigned char kind; unsigned char mode; short oclass; float strength; int one; char pad24[0xc]; } HitPacket;
struct EffectContext { float value; short timer; short pad6; int colour; int sound; float cooldown; int lock; short timers[4]; Moby *targets[4]; };
extern Moby *D_L00_001C3280[];
#define TARGET(c,i) (((Moby **)((char *)(c)+((i)<<2)))[8])
static __inline__ void clearSound(Moby *self,EffectContext *ctx)
{
    int id=ctx->sound;
    if(id!=-1) {
        char *sound=D_00188660+id*112;
        if(*(Moby **)(sound+0x88)==self && BYTE(sound,0x74))
            func_002E59F8(id);
    }
    ctx->sound=-1;
}
