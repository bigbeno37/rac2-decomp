/* opus-2 (Claude Opus 5.5, xhigh), task v2462, USA v1.01, 5 November 2026: 2440/2440 bytes,
 * 0/1 places (check.py: "instruction at +0x68 differs"); cdiff 187 of 610 rows differ, 102 of
 * them not just relocations.  The previous draft (opus-1) recompiled here gave 306 rows.
 * Profile (own unit, e.g. src/levels/08_tabora/L08_00429560-g8.c plus a cflags line):
 *   -O2 -G8 -ffunction-sections -g -frac2-qcopy -frac2-qcopy-barrier -frac2-hazard-labels
 * Do NOT use -frac2-zero-ti-barrier: with it the velocity clear blocks sched1, so the
 * D_L08_002603E0[index] / D_L08_001B3A30[index] addresses and -1.0 are not hoisted above
 * func_002A7878(0.03,0.12) as in retail (a nop shows in its delay slot).
 * Counterpart: RAC1 update_slot_panes_and_effects (src/overlays/shared/follow_camera.c in
 * rac1-decomp): same two 20-point strips, phases, sparks and lights.
 *
 * Fixed in this draft (measured):
 *  - Loop 2 hoisting (loop.c rerun pass, thr 61, -3 per move, moves if thr*life >= insns):
 *    0.3 is a local set before loop 2 (sway), so cse2 turns it into retail's mov.s f29,f27
 *    from loop 1's 0.3; the first-row point is a per-iteration pointer q (RAC1 loop-1 style),
 *    which leaves 50 insns with 4 moves before 1.0, so 1.0 stays in the loop as in retail.
 *    Loop 1 and the loop-2 body now match apart from p/q registers.
 *  - Light blocks: a block-local pointer (RAC1's Snd *s) puts the light record in a0.
 *  - No unused/shadowed locals remain (the old top-level p, q, t were unused).
 *
 * Still differs:
 *  1. Spill slots: retail 0xC8=&end, 0xCC=index*16, 0xD0=index*64; here 0xC8=index*16,
 *     0xCC=index*64, 0xD0=&end.  gcse hashes (at+N) as 42+N and (ashift index k) as R+k+53
 *     (R = index's pseudo, now 105), so &end (170) must sort before index<<4: R >= 114.
 *     No natural set of extra locals found; dummies are not acceptable.
 *  2. Loop-2 pointer init: retail q = first*320 + (base+8) with p reusing base+0x120's s0
 *     (320 in s1, first*320 in s2), as the direct D_L08_002604A0[first][k][2] form gives
 *     (giv reduced in pass 1).  But that form leaves 49 insns in pass 2, so 1.0 is hoisted
 *     too (49 >= 49).  Needed: the direct form plus one more non-movable loop-2 insn, or a
 *     per-iteration form whose giv init is first*320 + (base+8).
 *  3. Entry: index*16/index*64 order and spills; first/second share v0 (retail v1 for second).
 *  4. Temp registers: init loop (retail 320 in a0, base in v1), before loop 1, loop 1's
 *     D_00189E20/D_L08_001BFF40 addresses (retail a0/a1), and the arg order of
 *     func_00282D30/func_00282CC0 after loop 2.
 */
#include "common.h"
typedef unsigned int Quadword __attribute__((mode(TI)));
extern s32 func_L02_002F1658(void);
extern f32 D_L08_001B3A40[3][4],D_L08_002603E0[3][4][4],D_L08_002604A0[6][20][4];
extern s32 D_L08_001B39A0[3] MACRO_ADDR,D_L08_001B3A30[3] MACRO_ADDR,D_L08_001B3A10[3] MACRO_ADDR,D_L08_001B3A00[3] MACRO_ADDR;
extern s32 D_L08_001B39E0[3] MACRO_ADDR,D_L08_001B39F0[3] MACRO_ADDR;
extern f32 D_L08_001B39B0[3] MACRO_ADDR,D_L08_001B39C0[3] MACRO_ADDR,D_L08_001B39D0[3] MACRO_ADDR,D_L08_001B3A70[3][4];
extern f32 D_0018A0E0[],D_L08_001A8F20[];
typedef struct { char pad[0x1830]; char *owner; } BeamOwnerGlobals;
typedef struct { char pad[0x18]; char *owner; } BeamOwnerState;
extern BeamOwnerGlobals D_00189E20;
extern BeamOwnerState D_L08_001BFF40;
extern char D_L08_001C7C80[];
extern f32 D_L08_001B1D24 __attribute__((sda)),D_L08_001B1D20 __attribute__((sda));
extern void func_00282CF0(void *,void *,void *),func_00282CC0(void *,void *,void *);
extern void func_00282DD8(void *,void *,void *),func_002830C0(void *,void *,void *);
extern void func_00283098(void *,void *,void *),func_00282F20(void *,void *,f32),func_00282D30(void *,void *,f32);
extern f32 func_00282DF0(void *),func_00283B98(f32,f32),func_00283BE0(f32,f32),func_00283CE0(s32);
extern f32 func_002A89A8(void *,s32,f32),func_002ABB18(f32),func_00283198(f32),func_00283180(f32);
extern f32 func_002A7878(f32,f32),func_002A78D8(f32,f32);
extern s32 func_002A77E0(s32);
extern void func_002A7988(void *,f32,f32);
extern char *func_L08_0034A078(void *,void *,s32,s32);
extern s32 func_L00_002FCF18(void *,f32,f32,f32,f32,f32);
void func_L08_00429560(s32 unused,s32 slot) {
    f32 delta[4],point[4],step[4],basis[4][4],enddelta[4],end[4],velocity[4],temp[4];
    s32 first,second,i,k,light;
    f32 spacing,previous,advance,height,half,angleStep,weight,a,b,c,r;
    char *effect,*data,*owner;
    s32 index;
    index=slot;
    if(!func_L02_002F1658()) return;
    func_00282CF0(delta,D_L08_001B3A40[index],D_L08_002603E0[index][3]);
    second=index*2+1;first=index*2;
    spacing=func_00282DF0(delta)*0.05f;
    if(D_L08_001B39A0[index]==2){

        D_L08_001B39A0[index]=3;
        { s32 j; for(j=0;j<20;j++){
            D_L08_002604A0[first][j][1]=D_L08_002604A0[first][j][2]=0;
            D_L08_002604A0[first][j][3]=1;D_L08_002604A0[first][j][0]=spacing*(f32)j;
            D_L08_002604A0[second][j][0]=D_L08_002604A0[first][j][0];
            D_L08_002604A0[second][j][1]=D_L08_002604A0[second][j][2]=0;D_L08_002604A0[second][j][3]=1;
        }}
    }
    D_L08_001B39B0[index]=func_00283B98(D_L08_001B39B0[index],0.17453295f);
    D_L08_001B39C0[index]=func_00283BE0(D_L08_001B39C0[index],0.80285156f);
    D_L08_001B39D0[index]=func_00283B98(D_L08_001B39D0[index],0.05236f);
    *(Quadword *)(point)=*(Quadword *)(D_L08_002603E0[index][3]);
    func_00282F20(step,delta,spacing);
    previous=0;advance=0;
    angleStep=(360.0f/(func_00283CE0(20)*0.5f))*0.017453294f;
    { f32 (*p)[4]=D_L08_002604A0[first];
    f32 t;
    for(i=0;i<19;i++){
        half=(advance+spacing)*0.5f;height=0;
        if(i>0){
            height=func_002A89A8(point,0,0.5f)+D_L08_001B1D24;
            owner=D_00189E20.owner;
            if(owner && D_L08_001BFF40.owner==owner) height=*(f32 *)(owner+0x18);
            height=point[2]-height;
            if(previous+half<height)height=previous+half;
            else if(height>0)height=(height+previous)*0.5f;
            advance=height-previous;previous=height;
        }
        func_00282CC0(point,point,step);
        t=(f32)i;
        weight=(func_00283CE0(i)/func_00283CE0(20))*0.3f+0.1f;
        a=func_002ABB18(D_L08_001B39B0[index]+t*angleStep);
        weight=weight*func_00283198(a);
        (*p)[1]=0;(*p)[3]=1;(*p)[0]=spacing*t;(*p)[2]=weight-height;p++;
    }
    }
    *(Quadword *)(basis[3])=*(Quadword *)(D_L08_002603E0[index][3]);
    func_00282CF0(enddelta,D_L08_001B3A40[index],basis[3]);
    func_00282F20(basis[0],enddelta,1.0f);
    func_00282DD8(basis[1],basis[0],D_0018A0E0);
    func_00282F20(basis[1],basis[1],-1.0f);
    func_00282DD8(basis[2],basis[1],basis[0]);
    func_002830C0(end,D_L08_002604A0[first][18],basis);
    *(Quadword *)(D_L08_001B3A70[index])=*(Quadword *)(end);
    { f32 (*p)[4]=D_L08_002604A0[second];

    f32 t,a,sway;
    sway=0.3f;
    for(k=0;k<19;k++){
        f32 *q=D_L08_002604A0[first][k];
        t=(f32)k;(*p)[2]=0;
        a=func_002ABB18(D_L08_001B39C0[index]+t*0.99483776f);
        b=func_002ABB18(D_L08_001B39D0[index]+t*0.20944f);
        b=func_00283198(b)*0.5f;
        c=func_00283198(a);
        r=func_002A78D8(0,0.2f);
        (*p)[2]=q[2]+b*c+r;
        (*p)[1]=func_00283180(a)*sway;(*p)[3]=1;(*p)[0]=spacing*t;
        p++;
    }
    }
    r=func_002A7878(0.03f,0.12f);
    *(Quadword *)velocity=0;velocity[0]=1;velocity[3]=1;
    velocity[1]=func_002A7878(-1,1);velocity[2]=func_002A7878(-1,1);
    func_00282F20(velocity,velocity,r);
    func_00283098(velocity,velocity,D_L08_002603E0[index]);
    func_L08_0034A078(D_L08_002603E0[index][3],velocity,127,D_L08_001B3A30[index]);
    if(!func_002A77E0(3)){
        func_00282D30(temp,D_L08_002603E0[index],0.3f);
        func_00282CC0(temp,temp,D_L08_002603E0[index][3]);
        effect=func_L08_0034A078(temp,D_L08_001A8F20,127,D_L08_001B3A30[index]);
        if(effect){
            *(s16 *)(effect+10)=4;*(f32 *)(effect+12)=120000.0f;data=effect+32;
            *(f32 *)(data+16)=1.0f/func_00283CE0(4);*(s32 *)(data+24)=0x7F7F7F;
            if(D_L08_001B3A10[index]==1)*(s16 *)(data+22)=1;
        }
    }
    r=func_002A7878(0.01f,0.05f);
    func_002A7988(velocity,0.01f,0.05f);
    func_L08_0034A078(end,velocity,127,D_L08_001B3A30[index]);
    if(!func_002A77E0(3)){
        effect=func_L08_0034A078(end,D_L08_001A8F20,127,D_L08_001B3A30[index]);
        if(effect){
            *(s16 *)(effect+10)=4;*(f32 *)(effect+12)=120000.0f;data=effect+32;
            *(f32 *)(data+16)=1.0f/func_00283CE0(4);*(s32 *)(data+24)=0x7F7F7F;
            if(D_L08_001B3A10[index]==1)*(s16 *)(data+22)=2;
        }
    }
    if(D_L08_001B3A00[index]){
        if(D_L08_001B3A00[index]&1){
            *(Quadword *)(temp)=*(Quadword *)(D_L08_002603E0[index][3]);temp[2]-=0.25f;
            if(D_L08_001B39E0[index]==-1)D_L08_001B39E0[index]=func_L00_002FCF18(temp,D_L08_001B1D20,0,2,1,2);
            light=D_L08_001B39E0[index];
            if(light>=0){ char *s=D_L08_001C7C80+light*32;*(Quadword *)(s+16)=*(Quadword *)(temp);*(f32 *)(s+28)=D_L08_001B1D20; }
        }
        if(D_L08_001B3A00[index]&2){
            end[2]+=0.5f;
            if(D_L08_001B39F0[index]==-1)D_L08_001B39F0[index]=func_L00_002FCF18(end,D_L08_001B1D20,0,2,1,2);
            light=D_L08_001B39F0[index];
            if(light>=0){ char *s=D_L08_001C7C80+light*32;*(Quadword *)(s+16)=*(Quadword *)(end);*(f32 *)(s+28)=D_L08_001B1D20; }
        }
    }
}
