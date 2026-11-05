/* Tier: opus-2 (2026-11-04), from the sol-high draft. USA v1.01: 800/804 bytes,
 * 0/1 places; 1 word differs: the nop at retail +0x1E0 between the $gp load of
 * D_L30_001B185C and the rise div.s. Every other operation and register aligns.
 * Unit: <file>-g8.c with -O2 -G8 -ffunction-sections -g -fno-gcse (own unit).
 * -g gives the two earlier division nops and the line label before the load;
 * -fno-gcse keeps the owner's matrix address inside the loop; qzero_nc keeps
 * the spawn argument addresses; one horizontal-product expression gives the
 * first cosine-result move.
 *
 * Blocker (assembler, tools/compiler/transformers/pad_div_erratum_nops.py):
 * the label-floor counter is bumped at the tail of every append_insn call,
 * including the place != NULL calls that write a relaxable $gp macro's
 * lui/op alternative, so `label; l.s $f12,sym; div.s` counts 2 slots and gets
 * 0 nops, where the SDK assembler gives 1. Controls with this checkout's as
 * -G8: `$LM1: l.s $f12,sym; div.s` -> c78c0000 460cbb03 (no nop);
 * `$LM1: lwc1 $f12,0($28); div.s` -> c78c0000 00000000 460cbb03 (retail).
 * cc1 always prints SDA loads as the `l.s sym` macro (no %gp_rel form), and
 * the frag-distance path cannot see the label across the macro's frag_var.
 * A separate division statement gives two nops; MACRO_ADDR gives an absolute
 * two-word load; -mno-split-addresses is neutral. Fix: count only place == NULL
 * instructions (the emitted slot).
 *
 * Third Opus pass (v2584, 2026-11-05), recompiled unchanged: same 800/804. Proof that the
 * nop is the only difference: in the unit above, replacing the rise statement with this
 * one source line gives 1 of 1 places (forbidden asm, so not landable and not landed):
 *   { float d=D_L30_001B185C; __asm__("nop"); riseLow=func_002A7878(rise/d,rise); }
 * Ruled out: a float literal (the word is 4.0f, 0x40800000, a zero half, so gas builds it
 * with lui/mtc1 instead of a one-word .lit4 load; the value also sits in a mixed int/float
 * settings table, so it is data); a defined or struct-member symbol (same macro and frag);
 * the branch floor (retail's preceding group is a jal, which starts no distance).
 * Label placement alone gives 0 nops (label before the l.s) or 2 (label after it), never 1.
 * Retail census, all levels: 5 functions have $gp lwc1; nop; div.s, all unmatched and all
 * still INCLUDE_ASM: this one, func_L00_00424F98, func_L05_00419CC8, func_L08_0041E790 and
 * func_L08_003C67B8. Drafts of the first three of those record this same missing nop.
 */
#include "common.h"
typedef struct { char pad[0xC0]; float matrix[4][4]; } ParticleOwner;
extern int D_001A79F0;
extern unsigned D_L30_001B1820 __attribute__((sda)), D_L30_001B1824 __attribute__((sda));
extern unsigned D_L30_001B18B8 __attribute__((sda)), D_L30_001B18BC __attribute__((sda));
extern float D_L30_001B1828 __attribute__((sda)),D_L30_001B182C __attribute__((sda)),D_L30_001B1830 __attribute__((sda)),D_L30_001B1834 __attribute__((sda)),D_L30_001B1838 __attribute__((sda)),D_L30_001B183C __attribute__((sda));
extern float D_L30_001B1858 __attribute__((sda)),D_L30_001B185C __attribute__((sda)),D_L30_001B18AC __attribute__((sda)),D_L30_001B18B0 __attribute__((sda));
extern int func_00278E78(void),func_001163B0(void),func_00283CF0(float);
extern float func_002A7878(float,float),func_00283180(float),func_00283198(float),func_00282DF0(void *);
extern void func_00283098(void *,void *,void *),func_00282F20(void *,void *,float),func_00282CC0(void *,void *,void *);
extern unsigned char *func_002BAA28(void *,void *,void *,unsigned,unsigned,int,int,int,int);
void func_L30_0040B6B0(ParticleOwner *obj,char *origin,int count,float spread,float rise,float speedMax,float speedMin,float size,float life) {
    struct { float p[4], v[4], extra[4], base[4]; } f;
    char *st=*(char **)((char *)obj+0x68);
    unsigned color,kind;
    int i;
    if(D_001A79F0==21 || D_001A79F0==30) {color=0x30301010;kind=D_L30_001B1824;}
    else {color=D_L30_001B1820;kind=D_L30_001B1824;}
    if(!func_00278E78()) {color=D_L30_001B18B8;kind=D_L30_001B18BC;}
    if(count>0) { i=count; do {
        float t, riseLow;
        float horizontal, magnitude;
        int a,b,c;
        unsigned char *p;
        horizontal=func_00283180(func_002A7878(-3.1415927f,3.1415927f))*func_002A7878(speedMin,speedMax);
        f.p[0]=horizontal;
        t=func_00283198(func_002A7878(-3.1415927f,3.1415927f));
        t *= func_002A7878(speedMin,speedMax); f.p[1]=t;f.p[2]=0;
        func_00283098(f.p,f.p,obj->matrix);
        t=func_00282DF0(f.p);
        { float range=spread*((speedMax-t)*0.016666668f)/speedMax;
        func_00282F20(f.v,f.p,func_002A7878(range/D_L30_001B1858,range)); }
        qcopy(f.base,st); f.base[2]*=D_L30_001B18AC;
        func_00282CC0(f.v,f.v,f.base);
        riseLow=func_002A7878(rise/D_L30_001B185C,rise);
        f.v[2]+=riseLow*0.016666668f;
        qzero_nc(f.extra);
        func_00282CC0(f.p,f.p,origin);
        f.p[2]+=D_L30_001B18B0;
        f.v[3]=size;f.extra[3]=life;
        a=func_00283CF0(func_002A7878(D_L30_001B1828,D_L30_001B182C));
        b=func_00283CF0(func_002A7878(D_L30_001B1830,D_L30_001B1834));
        c=func_00283CF0(func_002A7878(D_L30_001B1838,D_L30_001B183C));
        p=func_002BAA28(f.p,f.v,f.extra,color,kind,a,b,c,-1);
        if(p && (func_001163B0()&1))p[3]=0x48;
    } while(--i); }
}
