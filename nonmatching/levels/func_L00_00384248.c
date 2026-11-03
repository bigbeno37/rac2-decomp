/*
 * Tier: opus-1 (Claude Opus 5.5, xhigh), task v2321, 4 November 2026.
 * Current US check.py: 1072/1072 bytes, 0/27 placements; first differing
 * instruction +0x30C (all earlier rows match apart from relocations).
 * Profile: -O2 -G8 -ffunction-sections -frac2-qcopy -frac2-qcopy-barrier
 * (src/levels/shared/L00_0037E898-trail-g8.c).
 * Fixed since v0943: use work->objects[i] in both loops (the slots local
 * commuted three pointer adds); the button test goes through an inline
 * pad_buttons() whose two returns give per-arm loads that jump2 cross-jumps,
 * which reproduces retail's lui/b/addiu v1 arms and the shared lw 0x1A0(v1).
 * Remaining (+0x30C..+0x31C): the timer store and vector copy. Retail has
 * 45=v1, &timers[i]=a0, and the copy source rematerialised into v1; ours has
 * the address in v0, 45 in v1, and the source in a0. Local-alloc's fake
 * lifetimes keep 45 and a block-local copy-source pseudo from sharing v1.
 * Retail fits a loop-hoisted source address that was spilled with REG_EQUIV
 * and reloaded at the asm (then v0 is clobbered within both fake ranges).
 * qcopy() and the .bits assignment tie byte for byte.
 * Needs the declarations below. Not linked or placement-verified.
 */
#include "common.h"

typedef int Quadword __attribute__((mode(TI)));
typedef union { Quadword bits; float f[4]; } Vector4;
extern void func_002A0F48(void *);
extern void func_00282F20(void *, void *, float);
extern float func_00283198(float);
#define FIELD(type, p, off) (*(type *)((char *)(p) + (off)))

extern void func_00283098(void *, void *, void *);
extern void func_00282CC0(void *, void *, void *);
extern void func_002AEC18(void *, int);
extern void func_002A05A8(char *);
extern float func_002AA2E8(float *, float, float);
extern int func_00282948(int *);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern void *func_L00_00384CA8(void *, void *, void *, unsigned int);
extern int func_002E5CF8(int, int, char *);

extern char D_L00_001B8DC0[];
extern float D_L00_001AEDA0[2] __attribute__((sda));
extern int D_L00_001AEDA8[2] __attribute__((sda));
extern float D_L00_001AEDB0 __attribute__((sda));
extern float D_L00_001AEDB4 __attribute__((sda));
extern float D_L00_001AEDBC __attribute__((sda));
extern int D_L00_001AEDD0[2] __attribute__((sda));
extern Vector4 D_L00_001AEDC0 SDATA(D_L00_001AEDC0);
extern int D_L00_001B19C8;

/* Measured input fields; other fields are not yet identified. */
typedef struct {
    char pad[0x10C];
    float axis;
    char pad110[8];
    float negative, positive;
    char pad120[0x80];
    int buttons;
} PlayerControl;
extern PlayerControl D_00138500, D_00138180;

typedef struct {
    Vector4 basis[2];
    Vector4 motion[2];
    int timers[2];
    char *objects[2];
} AttachmentWork;

static inline int pad_buttons(int i)
{
    if(i)return D_00138180.buttons;
    return D_00138500.buttons;
}

void func_L00_00384248(char *owner)
{
    AttachmentWork *work=FIELD(AttachmentWork *,owner,0x68);
    Vector4 position,velocity;
    float scale,step,input,target,phase,period;
    int i,alpha;
    for(i=0;i<2;++i) {
        func_00283098(work->objects[i]+0x10,&work->basis[i],(D_L00_001B8DC0+0x230));
        func_00282CC0(work->objects[i]+0x10,work->objects[i]+0x10,D_L00_001B8DC0);
        FIELD(float,work->objects[i],0x2C)=FIELD(float,FIELD(char *,work->objects[i],0x24),0x24)*D_L00_001AEDA0[i];
        func_002AEC18(work->objects[i],D_L00_001AEDA8[i]);
        func_002A05A8(work->objects[i]);
        func_002A0F48(work->objects[i]);
    }
    scale=D_L00_001AEDB0*0.016666668f;
    step=D_L00_001AEDBC*0.00027777778f;
    for(i=0;i<2;++i) {
        if(i)input=D_00138180.axis;else input=D_00138500.axis;
        if(input==0) { if(i)input=D_00138180.positive-D_00138180.negative; else input=D_00138500.positive-D_00138500.negative; }
        target=-input*scale;
        if(target!=0)func_002AA2E8(&work->motion[i].f[2],target,step);
        else func_002AA2E8(&work->motion[i].f[2],0,step+step);
        work->basis[i].f[2]+=work->motion[i].f[2];
        if(work->basis[i].f[2]>0.35f) {
            work->basis[i].f[2]=0.35f;
            if(work->motion[i].f[2]>0)work->motion[i].f[2]=0;
        }else if(work->basis[i].f[2]<-0.45f) {
            work->basis[i].f[2]=-0.45f;
            if(work->motion[i].f[2]<0)work->motion[i].f[2]=0;
        }
        if(func_00282948(&D_L00_001AEDD0[i]))FIELD(unsigned char,work->objects[i],0x23)=128;
        else {
            phase=func_00283CE0(D_L00_001B19C8%60);
            period=func_00283CE0(60);
            phase*=6.2831855f;
            phase/=period;
            alpha=func_00283CF0(func_00283198(phase-3.1415927f)*10)+20;
            FIELD(unsigned char,work->objects[i],0x23)=alpha;
        }
        if(func_00282948(&work->timers[i]) &&
           FIELD(unsigned char,work->objects[i],0x23)==128 && (pad_buttons(i)&0xE0)) {
            work->timers[i]=45;
            position.bits=D_L00_001AEDC0.bits;
            if(!i)position.f[1]-=0.05f;
            func_00283098(&position,&position,work->objects[i]+0xC0);
            func_00282CC0(&position,&position,work->objects[i]+0x10);
            func_00282F20(&velocity,work->objects[i]+0xC0,D_L00_001AEDB4*0.016666668f);
            func_L00_00384CA8(work->objects[i],&position,&velocity,i);
            func_002E5CF8(0,0,work->objects[i]);
        }
    }
}
