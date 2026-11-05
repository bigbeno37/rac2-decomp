/* func_PAL_L00_003B9E80, the PAL copy of func_L00_003B63D8 (template file
 * src/levels/shared/L00_003B63D8-counter-g8.c). Flags if placed: a unit
 * src/pal/levels/shared/L00_003B9E80-counter-g8.c with
 * -O2 -G8 -ffunction-sections -frac2-zero-ti-por.
 *
 * Tier: Opus (v2914). Best check.py: 1184/1184 bytes, 0/27 places.
 * Everything is settled except 9 instructions in the last arm, at +0x42C..+0x45C, and those
 * differ in registers only. Three values rotate: limit a1 (retail a2), the 6 v1 (retail a1),
 * the old timer a2 (retail v1). The new timer is a0 in both.
 *
 * Values found: 91>76, 0.0666..>0.080000006f, 0.000277..>0.00040000002f, and the last arm's
 * test scales limit with (limit*5+2)/6. PAL has no daddu copy there: use old=U; U=old+1, not
 * the template's timer=U,old=timer++ (that form is one word too long).
 *
 * Why the registers differ: local-alloc priority. With the wording below old has 3 refs,
 * life 8, priority 3/8, below the 6 (4 refs, priority 1) and limit (3/4). Retail needs old
 * ahead of the 6, so 5 refs or a tied qty.
 * `if(U(object,0x96)++<=(limit*5+2)/6)return;` gives retail's exact registers and order, but
 * one extra word: andi v1,v1,0xFFFF from the reg:HI post-increment value. That qty is tied
 * (182 HI + 184 SI, 5 refs). regmove's optimize_reg_copy_3 cannot drop the andi because
 * (plus (subreg:SI (reg:HI)) 1) sits between load and extend and a nested subreg fails
 * validate_replace.
 * Inferred, not reproduced: one source fits both releases if a plain SI copy x=P exists
 * after P=load; new=P+1; mem=new. sched1 moves it up in USA (daddu) and leaves it in PAL
 * (P dies at the copy, ties, no word). No wording kept that copy: cse replaces x by P unless
 * x outlives the block, and it retargets the load when the copy is adjacent to it.
 * Same 68-row bytes from: int/unsigned/unsigned short old, a promoted unsigned short timer,
 * (unsigned short)old (combine folds it into slt), timer=0 or timer++ after the store,
 * old=0 after the test, the load apart from the copy.
 * Worse: a function-scope old shared with case 0 goes global and takes a2 in case 0 as well.
 * Not tried: a bit-field for the 0x96 field; (long) operands, for the empty same-reg
 * extendsidi2 as the tied copy.
 */
#include "common.h"
typedef unsigned int TI128 __attribute__((mode(TI)));
typedef union {TI128 q;float v[4];} Vector;

/* Measured camera prefix: its position precedes the three-row basis. */
typedef struct {
    float position[4];
    u8 reserved10[0x220];
    float basis[3][4];
} CameraBasisPrefix;
typedef struct NODE NODE;
typedef void (*DrawCallback_r002E6CF0)(void *);
extern unsigned int D_00138324[];
extern int D_L00_001A8F00 MACRO_ADDR;
extern int mode_gp SDATA(D_L00_001A8F00);
extern int D_L00_001B1BE8 MACRO_ADDR;
extern float D_L00_001B18E0;
extern CameraBasisPrefix D_L00_001B8DC0;
extern float D_L00_001AFA44 __attribute__((sda));
extern int D_L00_001AFA4C __attribute__((sda));
extern void func_002A7750(unsigned char *,unsigned char,int),func_0029F338(void *);
extern int func_002B49E0(int,int,int,int,unsigned char *),func_002B4B70(int,int);
extern int func_L00_003B6858(void *);
extern void func_L00_003B69D8(void *),func_L00_003B75C8(void *),func_L00_003B72B0(void *),func_L00_003B77A8(void *);
extern void func_L00_003B7908(void *),func_L00_002E6CF0(DrawCallback_r002E6CF0,void *),func_L00_003B6A90(NODE *);
extern float func_002AA2E8(float *,float,float);
extern void func_002833F0(float *,float *),func_00283698(void *,void *),func_00282CF0(void *,void *,void *),func_00283098(void *,void *,void *);
extern void func_L00_003B7190(float *,float *),func_L00_003B7260(float,float,float,float *,float *),func_00282CC0(void *,void *,void *),func_00282F20(void *,void *,float);
extern void func_L00_003B74A8(unsigned char *,void *);
extern int func_002A8C28(void *,float *,float,int,float,float,int,int,int);
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define U(p,o) (*(unsigned short *)((char *)(p)+(o)))
void func_PAL_L00_003B9E80(unsigned char *object)
{
    Vector point;
    /* 00283698 reads three source rows and writes all four destination rows. */
    float inverse[4][4];
    Vector velocity,spawn,offset;
    char *context=*(char **)(object+0x68);
    int mode;
    switch(object[0x20]) { case 0: {
        if(!D_L00_001A8F00) {func_002B49E0(7,1,(int)object,2,0);return;}
        if(D_L00_001A8F00==7 && func_L00_003B6858(object)) {
            point.q=0;point.v[1]=1.5707964f;
            func_002833F0(D_L00_001B8DC0.basis,point.v);
            func_002A7750(object,1,-1);return;
        }
        { unsigned timer=U(object,0x96),old=timer++;
          U(object,0x96)=timer; if(old<6)return; }
        func_0029F338(object);return;
    break; } case 1: {
        D_L00_001B1BE8=1;
        func_L00_003B69D8(object);
        func_L00_003B75C8(object);
        func_L00_003B72B0(object);
        func_L00_003B77A8(object);
        func_L00_002E6CF0(func_L00_003B7908,object);
        if(I(context,0x60) && I(context,0x64)++>=76) {
            func_002AA2E8(&D_L00_001B18E0,1.0f,0.080000006f);
            mode=mode_gp;
            if(D_L00_001B18E0>=1.0f) {
                func_L00_003B6A90((NODE *)object);
                func_L00_003B6858(object);
                I(context,0x64)=0; I(context,0x60)=0;
                mode=D_L00_001A8F00;
            }
        } else {
            func_002AA2E8(&D_L00_001B18E0,0.0f,0.080000006f);
            mode=D_L00_001A8F00;
        }
        if(mode!=7) {
            func_L00_003B6A90((NODE *)object);
            func_0029F338(object);return;
        } else {
            if(!(D_00138324[0]&0x10))return;
            func_002B4B70(1,0);return;
        }
    break; } case 2: {
        func_00283698(inverse,D_L00_001B8DC0.basis);
        func_00282CF0(&point,object+0x10,D_L00_001B8DC0.position);
        func_00283098(&point,&point,inverse);
        func_00283098(&velocity,context,inverse);
        func_L00_003B7190(point.v+1,point.v+2);
        func_L00_003B7260(point.v[1],point.v[2],D_L00_001AFA44*0.00040000002f,velocity.v+1,velocity.v+2);
        velocity.v[0]=0.0f;
        func_00282CC0(&point,&point,&velocity);
        func_00283098(object+0x10,&point,D_L00_001B8DC0.basis);
        func_00282CC0(object+0x10,object+0x10,D_L00_001B8DC0.position);
        func_00283098(context,&velocity,D_L00_001B8DC0.basis);
        if(point.v[1]*point.v[1]+point.v[2]*point.v[2]<0.09f) {func_0029F338(object);return;} else {
            func_L00_003B74A8(object,object+0x10);
            func_00282F20(&offset,D_L00_001B8DC0.basis,-0.1f);
            func_00282CC0(&spawn,object+0x10,&offset);
            func_002A8C28(*(void **)(object+0xB8),spawn.v,0.1f,0x10000,1.0f,1.0f,0,1,0);
            func_00282CF0(&spawn,object+0x10,&offset);
            func_002A8C28(*(void **)(object+0xB8),spawn.v,0.1f,0x10000,1.0f,1.0f,0,1,0);
            func_00282CF0(&spawn,object+0x10,&offset);
            func_002A8C28(*(void **)(object+0xB8),spawn.v,0.1f,0x10000,1.0f,1.0f,0,1,0);
            if(D_L00_001A8F00==7) {int limit=D_L00_001AFA4C; unsigned old=U(object,0x96);
              U(object,0x96)=old+1; if((int)old<=((s32)(limit)*5+2)/6)return;}
        }
    }
    break; default: return; }
    func_0029F338(object);
}
