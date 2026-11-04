/* Claude Opus 5.5 (xhigh), 5 November 2026 (task v2472, opus-2); USA v1.01, default game profile.
 * Best landable form (below, unchanged from v2095): 1336/1336 bytes, 0/1 places, first difference
 * at +0xac, 6 instruction words differ, all one $s6/$s7 swap.
 * Cause (verified): global-alloc priority tie 173/173 between the PRE reaching reg for d+0x2C0
 * (refs 2, sched1 live 115) and the PRE copy of &origin (refs 4, live 231 doubled to 462 by
 * update_equiv_regs). The tie goes to the lower regno, and PRE numbers reaching regs in gcse
 * bucket order. gcse hash = 14029 + regno + offset for (plus reg const) (PLUS 71, SImode 6,
 * REG 57, CONST_INT 52, sp hashes as reg 1): &origin = 14094, d+0x2C0 = 14818 (d is pseudo 85,
 * fixed by its load in the first jal delay slot). With 147 buckets (max_cuid 295) the buckets are
 * 129 vs 118. Table sizes that put &origin first: 129, 135, 139, 143, 153, 163..179
 * (max_cuid 256-259, 268-271, 276-279, 284-287, 304-307, 324-359).
 * EXACT-BYTES FORM (not landable): replace the UV-loop line with
 *   { int i; i=1;i=2;i=3;i=4;i=5;i=6;i=7;i=8;i=9; for(i=0;i<4;i++){...same body...} }
 * Those 9 dead stores reach gcse (flow1 deletes them), so max_cuid becomes 304 (153 buckets).
 * check.py then reports 1 of 1 places for us, pal and us2. 10 stores (305) also match.
 * Repeated `i=0` stores add nothing (cse drops a set to the value already held).
 * Measured as count-neutral (max_cuid stays 295): do-while, while, and float-index (2*i)
 * loops; a TI-copy inline helper for the corners. `int hit = 0;` plus `hit = func_002761F8(..)`
 * gives exactly +1 and no other change, so the five allowed declaration inits reach only
 * max_cuid 300 (151 buckets, no flip).
 * Measured as harmful: an inline DrawGlow(pos, color, size) helper (frame 0x1D0, f24 lost,
 * max_cuid 294); a second induction variable j+=2 (+3, but the loop code changes); a `UV *uv`
 * destination pointer (loop changes).
 * Still open: a natural source change of +9..+12 (or -8..-11) gcse insns that leaves the
 * output alone, or two vanishing sched1 insns between 0x418290 and 0x4183CC (one after 0x4183CC).
 * Earlier notes (v2095): the UV loop matches with `D_L19_001B1C08[] MACRO_ADDR` plus UV struct
 * fields; a plain TI store for the final copy flips $s6 but drops the $s7 frame slot (1324 bytes).
 * Declarations: below, matching the later externs in L19_003E9400.c.
 * Approved helpers qcopy/qcopy_nc come from common.h.
 */
#include "common.h"
typedef int TI128 __attribute__((mode(TI)));
typedef float V[4] __attribute__((aligned(16)));
typedef struct { float u, v; } UV;
typedef struct {
    V p[4];
    int color[4];
    UV uv[4];
    long zero, texture, state, flags;
} Quad;
extern char D_L19_001BFEA0[],D_L19_001B9A40[];
extern UV D_L19_001B1C08[] MACRO_ADDR;
extern void func_0029FC20(void *,int,void *),func_00282F20(void *,void *,float),func_00282CC0(void *,void *,void *),func_002A7988(void *,float,float),func_00282CF0(void *,void *,void *),func_00282DD8(void *,void *,void *),func_00280B90(void *,int,int);
extern float func_002A7940(void);
extern void func_00281EB8(void *,int,int,int,float,float,float);
extern int func_002761F8(void *,void *,int,void *,void *);
extern long func_0027C878(int);
void func_L19_00417F88(char *o) {
 V matrix[4],origin,tmp,jitter,end;
 Quad q;
 V other,factor,impact,ray,corner;
 char *d=*(char **)(o+0x68);
 func_0029FC20(o,0,matrix);qcopy(origin,matrix[3]);
 func_00282F20(tmp,matrix,0.25f);func_00282CC0(origin,origin,tmp);origin[2]+=1;
 func_002A7988(jitter,0.25f,0.45f);func_00282CC0(end,jitter,origin);
 func_00281EB8(origin,0x31,0x60804020,1,*(float *)(d+0x228)*2,0.08f,func_002A7940());
 func_00281EB8(origin,0x31,0x80808080,1,*(float *)(d+0x228)*1.5f,0.08f,func_002A7940());
 func_002A7988(jitter,0.25f,0.45f);func_00282CC0(end,jitter,origin);
 func_00281EB8(end,0x31,0x60804020,1,*(float *)(d+0x228)*2,0.08f,func_002A7940());
 func_00281EB8(end,0x31,0x80808080,1,*(float *)(d+0x228)*1.5f,0.08f,func_002A7940());
 func_002A7988(jitter,0.25f,0.45f);func_00282CC0(end,jitter,origin);
 func_00281EB8(end,0x31,0x60804020,1,*(float *)(d+0x228)*2,0.08f,func_002A7940());
 func_00281EB8(end,0x31,0x80808080,1,*(float *)(d+0x228)*1.5f,0.08f,func_002A7940());
 func_00282CF0(ray,d+0x2e0,origin);func_00282F20(ray,ray,*(float *)(d+0x238)*60);func_00282CC0(ray,ray,origin);
 if(func_002761F8(origin,ray,2,o,0))qcopy_nc(impact,D_L19_001BFEA0);else qcopy_nc(impact,ray);
 factor[0]=0;factor[1]=0;factor[2]=1;factor[3]=1;
 func_00282CF0(jitter,origin,D_L19_001B9A40);func_00282CF0(other,impact,D_L19_001B9A40);
 func_00282F20(jitter,jitter,1);func_00282F20(other,other,1);
 func_00282DD8(jitter,jitter,factor);func_00282F20(jitter,jitter,*(float *)(d+0x234)*2);
 func_00282DD8(other,other,factor);func_00282F20(other,other,*(float *)(d+0x234)*2);
 q.texture=func_0027C878(14);q.state=0xff9000000260L;q.flags=0x8000000048L;q.zero=0;
 q.color[0]=q.color[1]=q.color[2]=q.color[3]=0x80804020;
 { int i; for(i=0;i<4;i++){q.uv[i].u=D_L19_001B1C08[i].u;q.uv[i].v=D_L19_001B1C08[i].v;} }
 func_00282CF0(corner,origin,jitter);*(TI128 *)q.p[0]=*(TI128 *)corner;
 func_00282CC0(corner,origin,jitter);*(TI128 *)q.p[1]=*(TI128 *)corner;
 func_00282CF0(corner,impact,other);*(TI128 *)q.p[2]=*(TI128 *)corner;
 func_00282CC0(corner,impact,other);*(TI128 *)q.p[3]=*(TI128 *)corner;func_00280B90(&q,0,0);
 func_00282F20(jitter,jitter,*(float *)(d+0x234)*1.5f);func_00282F20(other,other,*(float *)(d+0x234)*1.5f);
 q.color[0]=q.color[1]=q.color[2]=q.color[3]=0x80806060;func_00280B90(&q,0,0);
 qcopy(d+0x2c0,impact);
}
