/*
 * Claude Opus 5.5 (xhigh), task v2456, USA v1.01. Body unchanged from v1105 (GPT-6.1 Sol high).
 * Best legit check: in a unit of its own with -O2 -G0 -ffunction-sections -frac2-zero-ti-barrier,
 * 1380/1380 bytes, 0 of 1 places (85 of 345 rows, relocations included). With the default profile
 * (-O2 -G0 -ffunction-sections, as in L06_0033BDC0.c) it is 1388/1380: an extra $s5.
 *
 * Diagnosis (v2456): use the DEFAULT profile; the barrier flag is the wrong model. Retail's case-2
 * clear interleaves with its neighbours, and with default flags case 2 matches retail exactly.
 * Everything still wrong comes from ONE missing basic-block boundary right after the case-3
 * velocity-scale call func_00282F20(&ctx->velocity, &ctx->velocity, ctx->speed):
 *  - without it, sched1 hoists the call-crossing constants (1.0, -1.0, 1/60, 0.0) and
 *    obj+0x10 into the idle cycles of the rotation calls (hence the extra $s5);
 *  - the velocity pointer then stays block-local (local-alloc gives it $s1 and ctx $s4);
 *    used across a boundary it becomes global, so ctx gets $s2 and velocity $s4, as in retail.
 * Probe (NOT landable, placeholder test plus an extra store): add, right after that call,
 *        if ((char *)ctx == obj) obj[0x21]=0;
 * Default profile: 1388 bytes, and after dropping its two words (beql $s2,$s3 / sb $zero,0x21($s3))
 * all 345 words equal retail's, relocations aside: allocation, frame, the rotation stores in the
 * rand delay slots, and the constant placement. An empty asm barrier there instead gives 1380
 * bytes, but velocity stays local, so the case-3 registers and rotation store slots still differ.
 * Also tried: identical arms around position=obj+0x10 (merged before sched1, no boundary), and
 * a float *rot pointer for the rotations (one extra word, no change).
 * Next: find the REAL construct that ends a block there and vanishes by jump2 (WORKER2 "block
 * boundary that leaves no instruction": the test must compare registers that stay live, or a
 * live register with zero, e.g. ctx/obj/velocity, and the arms must differ in source but compile
 * alike, as an inline helper's mode test does). Nothing else needs to change.
 * Returning char * from func_0032DA58 and int from func_002A8C28/func_002E5CF8, plus float from
 * func_002AA548, preserves the implemented callee contracts (FP/GPR banks keep their order).
 * The file's own declarations differ (func_002A9640 returns int, func_002E5CF8 takes Moby *,
 * func_00282948 takes char *), so compile it in a unit of its own. All required declarations are
 * below. This is retained C, not a match.
 */
#include "common.h"
typedef unsigned int Quad __attribute__((mode(TI)));
typedef union { Quad q; float f[4]; } Vec;
extern char *func_002A9640(char *, int, int);
extern int func_002A77E0(int);
extern int func_002A7820(int, int);
extern float func_002A7878(float, float);
extern int func_00282948(void *);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
extern float func_00283B98(float, float);
extern int func_002E5CF8(int, int, char *);
extern char *func_0032DA58(char *, void *, void *, float);
extern int func_002A8C28(char *, float, void *, int, float, float, int, int, int);
extern void func_002B0A68(char *, int, float, void *, float, float, float, unsigned char);
extern void func_0029F338(char *);
extern float func_002AA548(void *, float, void *, float, float, float);
extern char *func_002C16C0(void *, void *, int, int, int, int, float, float, float, float, float);
typedef struct { char pad[0x140]; Vec position; char gap[0x10]; float scale; int unused; int duration, timer; char gap2[0x10]; float speed; int unused2, limit; } Effect;
extern Effect D_L06_001B9700;

extern unsigned char *D_L06_001FF41C;
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define I(p,o) (*(int *)((char *)(p)+(o)))
typedef struct { int timer; float speed, height; int handle; Vec velocity; } UpdateState;
void func_L06_004012B0(char *obj) {
    struct { Vec pos, direction, random; } frame;
    UpdateState *ctx = *(UpdateState **)(obj+0x68);
    char *hit, *particle;
    float speed;

    switch ((unsigned char)obj[0x20]) {
    case 0:
        qcopy(&frame.pos,obj+0x10);
        frame.pos.f[2] += 10.0f;
        func_00282CC0(&ctx->velocity,obj+0x10,&frame.pos);
        ctx->handle=-1;
        obj[0x20]=1;
        break;
    case 1:
        hit=func_002A9640(obj,0x10000,0);
        if (hit) {
          if (*(short *)(*(char **)(hit+0x20)+0xAA)!=0x8B8) {
            if (func_002A77E0(3)==1) {
                ctx->timer=func_002A7820(20,10);
                ctx->height=F(obj,0x18)+20.0f;
                obj[0x20]=3;
            } else {
                ctx->timer=func_002A7820(0,10);
                obj[0x20]=2;
            }

          }
        }
        *(unsigned char *)(obj+0xA8)=255;
        break;
    case 2:
        if (func_00282948(ctx)) {
            float unit = 1.0f;
            func_002E5CF8(0,0,obj);
            frame.random.q=0;
            frame.random.f[2]=0.050000004f;
            func_00282CF0(&frame.direction,&D_L06_001B9700.position,&frame.pos);
            func_00282F20(&frame.direction,&frame.direction,unit);
            func_0032DA58(obj,obj+0x10,&frame.direction,5.0f);
            D_L06_001B9700.speed=0.1f;
            D_L06_001B9700.limit=90;
            D_L06_001B9700.scale=0.2f;
            D_L06_001B9700.duration=60;
            D_L06_001B9700.timer=0;
            func_002A8C28(obj,3.0f,obj+0x10,0x10000,3.0f,1.0f,0,1,0);
            func_002B0A68(obj,0xEF,3.0f,&frame.random,6.0f,3.0f,6.0f,255);
            func_0029F338(obj);
        }
        break;
    case 3: {
        char *position;

        if (func_00282948(ctx)) {obj[0x20]=2; break;}
        func_002AA548(obj+0x18,ctx->height,&ctx->speed,0.027777778f,0.027777778f,0.33333334f);
        F(obj,0xF0)=func_00283B98(F(obj,0xF0),func_002A7820(-10,10)*0.017453294f);
        F(obj,0xF4)=func_00283B98(F(obj,0xF4),func_002A7820(-10,10)*0.017453294f);
        F(obj,0xF8)=func_00283B98(F(obj,0xF8),func_002A7820(-5,25)*0.017453294f);
        func_00282F20(&ctx->velocity,&ctx->velocity,ctx->speed);
        speed=ctx->speed;
        frame.random.q=0;
        position=obj+0x10;
        frame.random.f[0]=func_002A7878(-1.0f,1.0f);
        frame.random.f[1]=func_002A7878(-1.0f,1.0f);
        frame.random.f[2]=func_002A7878(-1.0f,1.0f);
        frame.direction.q=frame.random.q;
        func_00282F20(&frame.direction,&frame.direction,func_002A7878(0.1f,0.2f)*0.016666668f);
        func_00282F20(&frame.pos,&ctx->velocity,-func_002A7878(0.0016666668f,0.016666668f));
        func_00282CC0(&frame.direction,&frame.direction,&frame.pos);
        func_00282F20(&frame.pos,&ctx->velocity,func_002A7878(0.0f,1.0f)*speed);
        func_00282CC0(&frame.pos,&frame.pos,position);
        particle=func_002C16C0(&frame.pos,&frame.direction,20,60,0x606060,3,80000.0f,40000.0f,1.0f,-0.0002f,0.0f);
        if (particle) {
            particle[2]=*D_L06_001FF41C;
            particle[3]=0x44;
            if (func_002A7820(0,1)) {
                particle[3]=0x48;
                I(particle,4)=0x70707F;
                {char *payload=particle+0x20;
                payload[0x11]=0x70;
                payload[0x12]=0x70;
                payload[0x13]=0x70;
                *(short *)(payload+0x1E)=0x7F;}
            }
        }
        obj+=0x10;
        func_002C16C0(obj,&frame.direction,7,127,0xB0B0B0,3,80000.0f,40000.0f,1.0f,-0.0004f,0.0f);
        break;
    }
    }
}
