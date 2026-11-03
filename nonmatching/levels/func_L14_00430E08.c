/* func_L14_00430E08: near miss, 4 November 2026 (opus-2, Claude Opus 5.5 xhigh).
   Profile: needs its own unit, -O2 -G0 -ffunction-sections -frac2-hazard-labels
   (retail branches land on the hazard nop after bc1fl at 0x430F98).
   Best check.py: 920/920 bytes, 0/1 places, 8 instructions differ: only the
   three pointer spill slots rotate (ours D4=axis, D8=object+16, DC=rotated;
   retail D4=object+16, D8=rotated, DC=axis). Prologue and loops match.
   Cause: reload numbers spill slots by pseudo order, which follows gcse PRE
   reaching-reg bucket order (bucket = (14045 + base regno 85) or fp hash, mod
   N; object+16=14130, rotated=14190, axis=14222). N=(max_cuid/2)|1.
   PROVEN: adding 7 dead stores (color=1..7) after the declarations gives
   max_cuid 213, N=107 and a full 1/1 match. So only 3 more pre-gcse insns
   that vanish later are missing. This body has the five allowed inits
   (i=32, k=0, j=0, color, scale=1.0f; each +1, code unchanged) and reaches
   max_cuid 209, N=105. Tried, code changed: two inline helpers (876 bytes),
   late copy of object + block-local copy (object spilled, 924 bytes),
   ternary scale clamp (+2 but adds a mov.s). The opus-1 draft's count/corners
   variables compiled to 912 bytes here; this body is 920.
   Declarations: the extern block below (D_0018A0E0, not D_L14_0018A0E0).
*/
#include "common.h"
extern char D_L14_001B9500[];
extern char D_0018A0E0[];
extern void func_002FC560(unsigned int, unsigned long);
extern void func_00281DE8(void *, int, int, int);
extern void func_00282F20(void *, void *, float);
extern void func_00282DD8(void *, void *, void *);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00280B90(void *, int, int);
typedef struct {
    float pos[4][4];
    int color[4];
    float uv[4][2];
    long long mode;
    char pad[24];
} PrisonQuad;
extern float D_L14_002602F0[32], D_L14_00260270[32];
extern float func_002A7878(float, float);
extern int func_00283D38(int, int, float);
extern void func_002ACEC0(void *, void *, void *, float);
void func_L14_00430E08(void *arg)
{
    char *object = arg;
    PrisonQuad q;
    float right[4], rotated[4], delta[4], axis[4];
    char *state=*(char **)(object+0x68);
    float scale=1.0f;
    int i=32,k=0,j=0,color=0x80808080;
    for(i=0;i<32;i++) {
        D_L14_002602F0[i]+=1.0f/ *(int *)(state+0xa8);
        if(D_L14_002602F0[i]>1.0f) { D_L14_002602F0[i]=0; D_L14_00260270[i]=func_002A7878(-1.57079637f,1.57079637f); }
    }
    func_00281DE8(&q,*(int *)(state+0xb0),1,128);
    q.mode=5;
    q.uv[0][0]=0; q.uv[0][1]=0; q.uv[1][0]=0; q.uv[1][1]=1;
    q.uv[2][0]=1; q.uv[2][1]=0; q.uv[3][0]=1; q.uv[3][1]=1;
    func_00282CF0(right,D_L14_001B9500,object+16);
    func_00282F20(right,right,*(float *)(state+0x80)*0.5f);
    func_00282F20(axis,D_0018A0E0,-1.0f);
    for(k=0;k<32;k++) {
        if(*(float *)(state+0xac)<D_L14_002602F0[k]) color=*(int *)(state+0x98);
        else color=func_00283D38(*(int *)(state+0x94),*(int *)(state+0x98),D_L14_002602F0[k]/ *(float *)(state+0xac));
        q.color[0]=q.color[1]=q.color[2]=q.color[3]=color;
        func_002ACEC0(rotated,axis,right,D_L14_00260270[k]);
        func_00282DD8(delta,right,rotated);
        func_00282F20(delta,delta,*(float *)(state+0xb4)*(1.0f-D_L14_002602F0[k]));
        scale=*(float *)(state+0xa4)+D_L14_002602F0[k]*(*(float *)(state+0xa0)-*(float *)(state+0xa4));
        if(scale>1.0f) scale=1.0f;
        func_002FC560(0x47,0x53001);
        for(j=0;j<4;j++) {
            qcopy(q.pos[j],object+16);
            if(j>=2) func_00282CC0(q.pos[j],q.pos[j],delta);
            else func_00282CF0(q.pos[j],q.pos[j],delta);
            if(!(j&1)) {
                func_00282F20(rotated,rotated,*(float *)(state+0x80)*(1.0f-scale));
                func_00282CC0(q.pos[j],q.pos[j],rotated);
            } else {
                func_00282F20(rotated,rotated,*(float *)(state+0x80));
                func_00282CC0(q.pos[j],q.pos[j],rotated);
                q.color[j]=*(int *)(state+0xc);
            }
            func_00282CC0(q.pos[j],q.pos[j],right);
        }
        func_00280B90(&q,0,0);
    }
    func_002FC560(0x47,0x5360B);
}
