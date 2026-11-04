#include "common.h"
typedef int VectorBits __attribute__((mode(TI)));
struct Rac2Native_5f824a0e369ccf7b_src_record { u8 b0,b1,b2,b3; u16 s4,s6,s8,s10,s12; u8 e,f; };
extern struct Rac2Native_5f824a0e369ccf7b_src_record D_L00_00294900[];
extern u8 D_00139568[];
extern float D_L00_001AF670[4] __attribute__((sda));
extern unsigned int D_L00_001AF680 __attribute__((sda));
extern unsigned int D_L00_001AF684 __attribute__((sda));
extern unsigned int D_L00_001AF688 __attribute__((sda));
extern unsigned int D_L00_001AF68C __attribute__((sda));
extern unsigned int D_L00_001AF690 __attribute__((sda));
extern int D_L00_001AF6B4 __attribute__((sda));
typedef struct {
    char pad0[0x730];
    float a550[4];
    float a560[4];
    float a570[4];
    float a580[4];
    char pad4[0x488];
    char * aa18;
    char pad5[0x634];
    unsigned int b050;
    char pad6[0xC];
    int b060;
    char pad7[0x20C];
    int b270;
    char pad8[0xE3C];
    char * c0b0;
    int c0b4;
    char pad10[0x4];
    int c0bc;
    char pad11[0x18];
    u8 c0d8;
    char pad12[0x3];
    u8 c0dc;
    char pad13[0x22F];
    short c30c;
    char pad14[0x46];
    int c354;
} GameView;
typedef struct { char pad[0x1A0]; unsigned int b320, b324; } InputView;
extern GameView D_00189E20 NOT_SDA;
extern InputView D_00138180 NOT_SDA;
/* Only this measured prefix of the level's pulse-curve state is used here. */
typedef struct { float start; int reserved,ticks,mode; float next; int reserved2,nextTicks; } PulseCurvePrefix;
typedef struct { char pad[0x160]; PulseCurvePrefix pulse; } CameraView;
extern CameraView D_L00_001B8C80 NOT_SDA;
typedef struct { char p0[0x28]; unsigned int animation; char p1[0x28]; float rate; char p2[0x18]; float power; char tail[0x6C]; } WeaponEntry;
extern WeaponEntry D_L00_00262570[];
extern char *D_L00_001C3280[];
extern int func_00282978(void *),func_00282948(void *),func_002829D8(void *);
extern int func_002E5CF8(int,int,char *);
extern void func_002A75E0(void *,int,int,int,int),func_002A7470(void *,int,int,int);
extern int func_L00_002C42E0(int,int);
extern int func_L00_002D4040(int),func_L00_002D3F30(int,int);
extern void func_00273318(void *,unsigned int,void *,int,int,int,float,float,float);
extern void func_L00_002AF568(void);
extern void func_L00_002D6B70(int);
extern int func_L00_002D6998(struct Rac2Native_5f824a0e369ccf7b_src_record *);
extern float func_002A7940(void),func_00283CE0(int),func_002A7878(float,float),func_00283B98(float,float);
extern void func_L00_003A6538(void *);
/* The binder: func_L00_002E6EF8 in 23 programs, its twin func_L08_002F2D10 in levels 08, 09,
   19 and 30, so the call carries a name without an address. */
extern void ref_L00_003A53E8_00(void (*)(void *),void *);
extern void func_L00_0032A1A8(char *,char *,void *,float);
extern void func_0029FCC0(char *,int,char *),func_L00_003A5FC0(void *,void *);
extern void func_00282D30(void *,void *,float),func_00282CC0(void *,void *,void *);
extern int func_00277F48(float,void *,int,void *,int);
extern void func_002A8E20(void *,void *),func_0032D758(void *,int,void *);
extern void func_00283410(void *,void *),func_00283678(void *,void *),func_00283738(void *,void *,void *);
extern void func_002AB668(void *,int,float),func_002ACDE8(void *,void *,void *);
extern char *func_L00_003A5DC0(void *,void *,void *,void *);

static inline void SetCameraPulse(CameraView *camera, float amount, int ticks)
{
    camera->pulse.start = amount;
    camera->pulse.ticks = ticks;
    camera->pulse.next = amount;
    camera->pulse.nextTicks = ticks;
    camera->pulse.mode = 0;
}

/* Control the two-stage blast and trace its individual fan rays. */
