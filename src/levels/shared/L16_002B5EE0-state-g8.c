#include "common.h"
typedef struct {
    int tableIndex; u8 pad04[12]; float origin[4];
    float angle, angularSpeed; int counter, timer; u8 pad30[16];
    int flags, sound; float amount;
} ObjectState;
#define F(p,o) (*(float *)((u8 *)(p)+(o)))
#define I(p,o) (*(int *)((u8 *)(p)+(o)))
extern u8 D_0019B2E8[];
extern u8 D_00188660[];
extern int D_001A79F0 MACRO_ADDR;
extern u16 D_0018CC7C NOT_SDA;
extern int D_L16_001A8FF0 MACRO_ADDR;
extern u8 *D_L16_001B2B1C __attribute__((sda));
extern float D_L16_001B02B0 __attribute__((sda));
extern float D_L16_001B02B8 __attribute__((sda));
extern u32 D_L16_001B02C0[] MACRO_ADDR;
extern u32 colorFirst[] SDATA(D_L16_001B02C0);
extern float D_L16_001B02BC MACRO_ADDR;

extern char D_L16_001B02D0[];
extern int D_L16_001B2648 MACRO_ADDR;
extern int D_L16_001B3814 MACRO_ADDR;
typedef struct {
    u8 pad[0x160]; float strength; int pad164;
    int duration, timer; u8 pad170[0x10];
    float damping; int pad184, lifetime;
} EffectControls;
extern EffectControls D_L16_001B99C0;
extern u8 D_L16_001A8F20[] MACRO_ADDR;
extern float D_L16_001B9B00[4];
extern void func_002A7750(u8 *,s32,s32);
extern float func_L16_003960E8(const u8 *);
extern void func_L14_004272D8(void *);
extern void func_00282F20(void *,void *,float);
extern void func_00282CC0(void *,void *,void *);
extern void func_002A8C28(u8 *,void *,int,int,int,int,float,float,float);
extern int func_002E5CF8(int,int,char *);
extern void func_002E59F8(s32);
extern int func_00288E18(int,int);
extern void func_L00_002E6CF0(void (*)(void *),void *);
extern void func_L16_003C32F8(void *);
extern void *func_002A9640(void *,u32,s32);
extern void func_00282CF0(void *,void *,void *);
extern void func_0032DA58(void *,void *,void *,float);
extern void func_002AB6D8(float *,float *);
extern void func_002B0A68(char *,int,float *,u8,float,float,float,float);
extern int func_002A8508(int,int);
extern char *func_00288FB8(int);
extern int func_00115DA8(char *,char *,...);
extern void func_L00_0030DD70(char *,int);
extern int func_00282948(int *);
extern void func_002A0430(unsigned int);
extern void func_0029F338(void *);
extern float func_00283B98(float,float);
extern int func_L01_0031BC68(int,int,float);

/* Object state machine: it moves along its direction as the level value from
 * func_L16_003960E8 rises, bursts once that value reaches 1 (effects, shake, a
 * formatted message with a count), then deletes itself when its timer runs out.
 * The idle colour pulses while the object is visible. */
