/*
 * opus-1, Claude Opus 5.5 (xhigh), task v2292; retained C, not linked or counted.
 * Profile: -O2 -G8 -ffunction-sections -frac2-zero-ti-por -g (own -g8.c file).
 * USA v1.01: 1336/1336 bytes, 0/7 places, 56 of 334 rows differ, most of them
 * only $gp/%hi relocation display (3 November 2026).
 * Matches retail: 0x360 frame and saves, the prologue invariants, both loops'
 * registers, inner-loop givs s2-s7/fp, j in $a2 with sq/lq caller-save, the
 * spilled i<2 and i+1, the D+0x100 resident derivation and every FPR.
 * Keys: uv[4][2] with indexed sources (j), source 1 as a MACRO_ADDR absolute
 * view (an unsplit la, so loop.c reduces it as a giv), an index-based first
 * loop (biv elimination keeps the sp+0x240 end in a register), the resident
 * pointer set after the first call, and a block-local x for the inner time.
 * Still different: (1) spill slots of sp+0x40, sp+0x280, sp+0x2B0 and i+1
 * (ours 0x2CC/0x2DC/0x2C8/0x2D8, retail 0x2C8/0x2D8/0x2DC/0x2CC): pseudo or
 * PRE order, not yet tried with declaration initialisers; (2) switch giv base
 * is packet[0] (s0=sp) where retail uses packet[3] (s0=sp+0x1B0): combine_givs
 * gives +1 to a zero-addend giv, so something here has add_val sp+16j that
 * retail lacks. Next: read the -dL combine statistics for the j loop.
 * D_L02_001B1A8C must become a per-level name/alias before linking.
 */

#include "common.h"

typedef int Quad __attribute__((mode(TI)));
typedef union { Quad q; float f[4]; } Vector;
typedef struct {
    Vector point[4];
    unsigned int color[4];
    float uv[4][2];
    u64 reserved, texture, setup, state;
} EffectPacket;
extern int ref_L02_00410F68_0 __attribute__((sda));
extern int ref_L02_00410F68_1 __attribute__((sda));
extern int ref_L02_00410F68_2 __attribute__((sda));
extern int ref_L02_00410F68_3 __attribute__((sda));
extern int ref_L02_00410F68_4 __attribute__((sda));
extern float ref_L02_00410F68_5 __attribute__((sda));
extern unsigned int ref_L02_00410F68_6[2] __attribute__((sda));
extern float ref_L02_00410F68_7[][2] SDATA(ref_L02_00410F68_7);
extern float ref_L02_00410F68_8 __attribute__((sda));
extern float ref_L02_00410F68_9 __attribute__((sda));
extern u8 D_00189E20[];
extern float D_L02_001B1A8C[][2] MACRO_ADDR;
extern void func_00283658(void *,void *);
extern u64 func_0027C878(int);
extern void func_00282CF0(void *,void *,void *);
extern void func_002AC9F8(void *,float);
extern unsigned int func_00283D38(unsigned int,unsigned int,float);
extern void func_00282C90(void *);
extern void func_002833F0(void *,void *);
extern void func_L01_002E1510(void *,int,void *,int,void *);

/* Assemble four animated strips in the object's coordinate frame. */
void func_L02_00410F68(u8 *object)
{
    EffectPacket packet[4];
    Vector frame[4];
    Vector rotation[3];
    Vector angle;
    u8 *effect=*(u8 **)(object+0x68);
    u8 *resident;
    float alpha,spread,shrink,time;
    int i,j;
    func_00283658(frame,object+0xC0);
    qcopy(&frame[3],object+0x10);
    for(i=0;i<4;i++) {
        packet[i].texture=func_0027C878(ref_L02_00410F68_0);
        packet[i].setup=0xFF9000000260ULL;
        packet[i].reserved=0;
        {
            u64 state=(long long)ref_L02_00410F68_1;
            state |= (u64)(long long)ref_L02_00410F68_2<<2;
            state |= (u64)(long long)ref_L02_00410F68_3<<4;
            state |= (u64)(long long)ref_L02_00410F68_4<<6;
            state |= 0x8000000000ULL;
            packet[i].state=state;
        }
    }
    spread=0.1f;
    alpha=(1.95f-*(float *)(effect+0x34))*2.0f;
    if(alpha>1.0f) alpha=1.0f;
    else if(alpha<0.0f) alpha=0.0f;
    if(*(float *)(effect+0x34)<1.15f) {
        func_00282CF0(D_00189E20+0x100,object+0x10,D_00189E20+0x80);
        resident=D_00189E20;
        *(float *)(resident+0x108)=0;
        func_002AC9F8(D_00189E20+0x100,0.016666668f);
        time=*(float *)(effect+0x34);
        shrink=1.15f-time;
        if(shrink<0) shrink=0;
        {
            float fade=(0.2f-time)*5.0f;
            if(fade<0) fade=0;
            if(*(int *)(effect+0x38)<0) ref_L02_00410F68_5=fade;
        }
        spread=shrink+0.1f;
        if(time<0.65f) {
            float scale=time*1.538461566f;
            *(short *)(resident+0x2500)=1;
            *(float *)(resident+0x2024)=scale;
            *(float *)(resident+0x2020)=scale;
            *(float *)(resident+0x2028)=2.0f-scale;
        } else *(short *)(resident+0x2500)=0;
    }
    for(i=0;i<4;i++) {
        j=0;
        do {
            unsigned int color;
            float x;
            packet[i].uv[j][0]=ref_L02_00410F68_7[j][0];
            packet[i].uv[j][1]=D_L02_001B1A8C[j][0];
            color=ref_L02_00410F68_6[i!=0];
            packet[i].color[j]=color;
            packet[i].color[j]=func_00283D38(color&0xFFFFFF,color,alpha);
            func_00282C90(&packet[i].point[j]);
            x=*(float *)(effect+0x34);
            if(i!=0) x+=0.01f;
            packet[i].point[j].f[0]=x;
            if(j&1) {
                switch(i) {
                case 0: packet[0].point[j].f[2]+=ref_L02_00410F68_8+spread; break;
                case 1: packet[1].point[j].f[2]+=ref_L02_00410F68_8+ref_L02_00410F68_9; break;
                case 2: packet[2].point[j].f[2]+=(ref_L02_00410F68_8-spread*0.8f)+ref_L02_00410F68_9; break;
                case 3: packet[3].point[j].f[2]+=(ref_L02_00410F68_8+spread*0.8f)+ref_L02_00410F68_9; break;
                }
            } else {
                switch(i) {
                case 0: packet[0].point[j].f[2]+=ref_L02_00410F68_8-spread; break;
                case 1: packet[1].point[j].f[2]+=ref_L02_00410F68_8-ref_L02_00410F68_9; break;
                case 2: packet[2].point[j].f[2]+=(ref_L02_00410F68_8-spread*0.8f)-ref_L02_00410F68_9; break;
                case 3: packet[3].point[j].f[2]+=(ref_L02_00410F68_8+spread*0.8f)-ref_L02_00410F68_9; break;
                }
            }
            j++;
        } while (j<4);
    }
    angle.q=0; angle.f[2]=0.19634955f;
    func_002833F0(rotation,&angle);
    func_L01_002E1510(packet,4,rotation,32,frame);
}
