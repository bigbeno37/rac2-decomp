/* Tier: Claude Opus 5.5 (opus-2, xhigh effort), v2549, 5 November 2026.
 * Best check.py: 1156/1156 bytes, 0/2 USA v1.01 places, first mismatch +0x24C. 10 rows really differ, all
 * in the two preheaders (+0x104, +0x12C, +0x24C..+0x26C); cdiff prints 69 of 289 because it also lists
 * unrelocated symbol rows. All reloads, the near-loop and face-loop bodies and the final bnel now match;
 * the v2145 draft still spilled $v1 and missed the bnel.
 * Profile: -O2 -G8 -ffunction-sections -g, dedicated unit src/levels/shared/L03_003CA4B0-g8.c. -g is
 * required: without it the div.s hazard nops go and 261 rows differ.
 * RAC1 counterpart: draw_gadgebot_spawn_container (overlays/shared/gadgebot_containers_and_frog.c).
 * Fixed here: `float *src=D_L03_0029A920[i]` first in the far loop sets the table up before 240CF0, so
 * its reload no longer overlaps the 240CF0 high. No second spill reg is needed, so reload stays on $v0.
 *
 * Remaining: retail's %hi(D_L03_00240CF0) is in $a0 in both preheaders, ours is $v1 (near) and $v0 (far).
 * Measured cause: in retail that high is one GLOBAL pseudo, gcse's PRE reaching reg T, set in both
 * preheaders and used only there. global.c then picks $a0; a local pseudo gets $v1, or $v0 here.
 * T survives only if the near loop still uses it after the first loop pass. loop.c's move_insn path
 * re-emits a hoisted lo_sum with a fresh HIGH, so if every use is hoisted in pass one then T is deleted by
 * delete_trivially_dead_insns. Retail needs the near loop's lo_sum(T,240CF0) to stay in pass one and be
 * hoisted in the rerun pass; cse2 then merges the fresh HIGHs back into T in both arms.
 * Proof by probe: index-form near loop with `denom+=denom;` and `temp.f[2]+=1;` deleted (61 real insns):
 * the lo_sum hoists in the rerun pass, the giv reduces to s5, and both preheaders come out exactly as in
 * retail (lui v0; lui a0; ... addiu s5,a0 / addiu s1,v0; addiu s0,a0), no $v1 spill.
 * The obstacle: threshold*savings*life >= insn_count with threshold 61 (1+60 non-fixed regs, moved at 61
 * real insns, not at 63), life 1, savings 1. The RAC1 index-form near loop has 75 insns in pass one and 64
 * in the rerun, so it stays. 64 looks minimal for these ten calls: 58 insns that reach the final code
 * plus 6 copies from expand_call that coalesce later (2 x a1=sum, 2 x f12=x+x, 2 x pseudo=f0). The lo_sum must also stay first in the rerun list, and 149 must fail at threshold 58,
 * so retail's count is 59..61.
 * Ruled out: life>=2 hoists in pass one, killing T (a local base pointer set in the body gives this);
 * D+i, &D[i][0], (float *)D+i*4, (float *)D+(i<<2), a struct-typed table and exact RAC1 wording all
 * compile alike or leave 64; reload-only explanations; -g off.
 * Unexplored: how retail reaches <=61 at all. Any fix must keep T alive to cse2 without a forbidden
 * always-false test or a contrived dead self-update.
 */
#include "common.h"
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define I(p,o) (*(int *)((char *)(p)+(o)))
extern void func_00282F20(void *,void *,float),func_002830C0(void *,void *,void *),func_00282CF0(void *,void *,void *),func_00282D30(void *,void *,float),func_00283098(void *,void *,void *);
extern float func_00282C48(float),func_00282DB0(void *,void *),func_00282C10(float);
extern u64 func_0027C878(int);
extern void func_00280B90(void *,int,int);
extern char D_L03_001B9980[];
typedef float ReflectUV[2];
extern ReflectUV D_L03_00240B20[57];
extern float D_L03_00240CF0[57][4];
extern float D_L03_0029A920[57][4],D_L03_0029B3C0[57][4],D_L03_0029B750[42][4],D_L03_0029B120[42][4],D_L03_0029AF50[][2];
extern short D_L03_0029ACB0[42][4][2];
extern int D_L03_001B0330 __attribute__((sda)),D_L03_001B031C __attribute__((sda)),D_L03_001B0320 __attribute__((sda)),D_L03_001B0324 __attribute__((sda)),D_L03_001B0328 __attribute__((sda)),D_L03_001B032C __attribute__((sda)),D_L03_001B0334 __attribute__((sda));
extern float D_L03_001B0338 __attribute__((sda));
typedef struct {
 float position[4][4];
 int color0,color1,color2,color3;
 float uv[4][2];
 u64 zero, texture, mode, blend;
} ReflectPacket;
typedef struct {char pad[0x140];float pos[4];} ReflectCamera;
extern ReflectCamera D_L03_001B9840;

/* Transform src by the matrix into dst (RAC1 TransformPoint). */
static inline void TransformPoint(float *dst,void *src,void *m){func_002830C0(dst,src,m);}

void func_L03_003CA4B0(char *m){
 ReflectPacket packet __attribute__((aligned(16)));
 typedef unsigned int ReflectQ __attribute__((mode(TI)));
 typedef union {float f[4];ReflectQ q;} ReflectVec;
 typedef struct {ReflectVec row[4];} ReflectMatrix;
 ReflectMatrix transform;
 ReflectVec dir,normal,reflected,temp;
#define matrix ((float *)&transform)
 int near=0,i,j,index;float scale,denom;u64 flags;
 scale=F(m,0x2c)/F(*(char **)(m+0x24),0x24);
 func_00282F20(transform.row[0].f,m+0xc0,scale);func_00282F20(transform.row[1].f,m+0xd0,scale);func_00282F20(transform.row[2].f,m+0xe0,scale);
 transform.row[3].f[0]=F(m,0x10);transform.row[3].f[1]=F(m,0x14);transform.row[3].f[2]=F(m,0x18);transform.row[3].f[3]=1;
 if(func_00282C48(D_L03_001B9840.pos[0]-F(m,0x10))<16&&func_00282C48(D_L03_001B9840.pos[1]-F(m,0x14))<16)near=1;
 if(near!=0){
 ReflectUV *uv=D_L03_00240B20;
 float (*pos)[4]=D_L03_00240CF0;
 for(i=0;i<57;i++,uv++,pos++){
 func_002830C0(pos,D_L03_0029A920[i],&transform);
 func_00282CF0(dir.f,pos,D_L03_001B9980);func_00282F20(dir.f,dir.f,1);
 func_002830C0(reflected.f,D_L03_0029B3C0[i],&transform);
 func_00282F20(reflected.f,reflected.f,D_L03_001B0338);
 denom=func_00282DB0(reflected.f,dir.f);func_00282D30(temp.f,reflected.f,denom+denom);
 func_00282CF0(temp.f,dir.f,temp.f);func_00282F20(temp.f,temp.f,1);
 temp.f[2]+=1;
 denom=func_00282C10(temp.f[2]+temp.f[2]);
 denom+=denom;
 {float u=temp.f[0]/denom;
 float v=temp.f[1]/denom;
 (*uv)[1]=v+0.5f;(*uv)[0]=u+0.5f;}
 }
 }else{
 for(i=0;i<57;i++){float *src=D_L03_0029A920[i];func_002830C0(D_L03_00240CF0[i],src,&transform);}
 }
 {u64 texture=func_0027C878(D_L03_001B0330);
 flags=(u64)D_L03_001B031C;
 flags|=(u64)D_L03_001B0320<<2;
 flags|=(u64)D_L03_001B0324<<4;
 flags|=(u64)D_L03_001B0328<<6;
 flags|=(u64)D_L03_001B032C<<32;
 packet.texture=texture;packet.mode=0x0000ff9000000260ULL;packet.blend=flags;}
 packet.zero=0;
 packet.color3=D_L03_001B0334;
 packet.color2=D_L03_001B0334;
 packet.color1=D_L03_001B0334;
 packet.color0=D_L03_001B0334;
 i=0;
 do{
 float *center=normal.f;
 TransformPoint(center,D_L03_0029B750[i],&transform);
 func_00282CF0(dir.f,center,D_L03_001B9980);func_00282F20(dir.f,dir.f,1);
 func_00283098(center,D_L03_0029B120[i],&transform);
 if(!(0.0f<func_00282DB0(dir.f,center))){
 short *idx=D_L03_0029ACB0[i][0];
 for(j=0;j<4;j++,idx+=2){
 index=idx[0];qcopy(packet.position[j],D_L03_00240CF0[index]);
 if(near){packet.uv[j][0]=D_L03_00240B20[index][0];packet.uv[j][1]=D_L03_00240B20[index][1];}
 else {packet.uv[j][0]=D_L03_0029AF50[idx[1]][0];packet.uv[j][1]=D_L03_0029AF50[idx[1]][1];}
 }
 func_00280B90(&packet,0,0);
 }
 i++;
 }while(i<42);
}
