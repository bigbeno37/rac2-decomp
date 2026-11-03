/* Complete C near miss; not linked and no matching credit.
 * 2026-11-03 (opus-1): game -O2 -G8; must live in src/levels/shared/L05_003D33E8-g8.c
 * (the radius floats are $gp-relative).
 * check.py: us 0/4, 1188/1196 bytes. Frame 0xB0 with retail's saved set and register assignment
 * (s1=o, s0=o+0x10 reaching reg, s2=state, s3=hit, s4=&old, f20=2.0); trail and loop identical.
 * Key shapes: first branch's no-hit/no-flag path joins the st+0x2c effects (finish); the impact,
 * finish and destroy code sits inside the second branch so the two 0xFBF calls are not cross-jumped;
 * D_L05_001FF240[97][7]; reusing r for the 2.0 bound gives f20.
 * Differs (4 local spots): retail loads x before the 2.0 bound (+mtc1 nop); first-branch o+0x10
 * recompute goes through v0 (retail addiu a0 + daddu s0,a0); retail keeps &D_L05_001BFB00 in s3 for
 * the hit-flag read (a struct cast did not change it); retail's `b impact` slot is a nop with a doubled
 * `lui v1,0x1C` before the impact label (here the lui fills the slot).
 * Needs common.h (qcopy, u8, NOT_SDA) and the -g8 file's func_002E5DA0/func_0029F338 (repeated below).
 */
#include "common.h"
extern int func_002E5DA0(int, int, char *, int);
extern void func_0029F338(void *);
extern void func_00282CC0(void *,void *,void *);
extern void func_00282CF0(void *,void *,void *);
extern void func_00282D30(void *,void *,float);
extern int func_00282948(void *);
extern int func_00277258(void *, int, void *, float);
extern int func_002761F8(void *, void *, int, void *, void *);
extern int func_00278E78(void);
extern void func_L05_004276E8(void *,void *,void *);
extern void *func_L05_00424990(void *,int,int,int,int,int,int,int,int,int,int,float);
extern void func_L05_003D1E60(void *,int,int);
extern float func_002A7940(void);
extern void func_002AF000(void *,float,float,float);
extern char *func_002BD8B0(void *,void *,int,int,int,int,float);
extern float D_L05_001B1534 __attribute__((sda));
extern float D_L05_001B1530 __attribute__((sda));
extern char D_L05_001BFB30[], D_L05_001BFB20[], D_L05_001BFB00[];
extern u8 *D_L05_001FF240[] NOT_SDA;
void func_L05_00427238(char *o)
{
    float old[4], pos[4], step[4], random[4];
    char *st=*(char **)(o+0x68);
    int hit;
    int i;
    char *p;
    float r;
    float angle;
    float radius;
    qcopy(old,o+0x10);
    func_00282CC0(o+0x10,o+0x10,st);
    *(float *)(o+0x2c)+=0.07f;
    if (*(float *)(o+0x2c)>*(float *)(*(char **)(o+0x24)+0x24))
        *(float *)(o+0x2c)=*(float *)(*(char **)(o+0x24)+0x24);
    if (func_00282948(st+0x18))
        goto destroy;
    r=2.0f;
    if (*(float *)(o+0x10)<r || *(float *)(o+0x10)>1021.0f || *(float *)(o+0x14)<r || *(float *)(o+0x14)>1021.0f || *(float *)(o+0x18)<r || *(float *)(o+0x18)>1021.0f)
        goto destroy;
    if (func_00282948(st+0x28)) {
        hit=0;
        radius=*(int *)(st+0x24)?D_L05_001B1534:D_L05_001B1530;
        if (func_00277258(o+0x10,16,*(void **)(st+0x20),radius)) {
            hit=1; qcopy(o+0x10,D_L05_001BFB30);
        }
        if ((hit || func_002761F8(old,o+0x10,16,*(void **)(st+0x20),0)) && *(int *)(D_L05_001BFB00+0x18)) {
            func_002E5DA0(7,0,o,0xFBF);
            goto impact;
        }
        goto finish;
    }
    if (func_00278E78()!=6) {
        hit=0;
        radius=1.0f;
        if (*(int *)(st+0x24)) radius=1.5f;
        if (func_00277258(o+0x10,16,*(void **)(st+0x20),radius)) {
            hit=1; qcopy(o+0x10,D_L05_001BFB30);
        }
        if (hit || func_002761F8(old,o+0x10,16,*(void **)(st+0x20),0)) {
            func_002E5DA0(7,0,o,0xFBF);
impact:
            qcopy(o+0x10,D_L05_001BFB20);
            func_L05_004276E8(o,st,o+0x10);
finish:
            if (*(int *)(st+0x2c)) {
                if (*(int *)(st+0x24))
                    func_L05_00424990(o+0x10,0,50,30,30,30,40,1,1,1,1,2.0f);
                func_L05_003D1E60(o+0x10,0,0);
            }
destroy:
            func_0029F338(o);
            return;
        }
    }
    qcopy(pos,old);
    func_00282CF0(step,o+0x10,old);
    func_00282D30(step,step,0.333f);
    for(i=2;i>=0;i--) {
        func_00282CC0(pos,pos,step);
        r=func_002A7940();
        angle=func_002A7940();
        func_002AF000(random,0.012500001f,r,angle);
        if (*(int *)(st+0x24)) {
            p=func_002BD8B0(pos,random,0x4FFF0040,0x1FFF0040,10,1,30000.0f);
            if(p) p[2]=D_L05_001FF240[97][7];
            p=func_002BD8B0(pos,random,0x4FFF2040,0x1FFF2040,10,1,15000.0f);
        } else {
            p=func_002BD8B0(pos,random,0x4F00FF20,0x1F00FF20,10,1,30000.0f);
            if(p) p[2]=D_L05_001FF240[97][7];
            p=func_002BD8B0(pos,random,0x4F20FF20,0x1F20FF20,10,1,15000.0f);
        }
        if(p) p[2]=D_L05_001FF240[97][7];
    }
}
