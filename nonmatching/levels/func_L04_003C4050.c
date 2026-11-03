/* opus-1 attempt (Claude Opus 5.5 xhigh), USA v1.01, levels -O2 -G8 -ffunction-sections.
 * Best check: 2060/2052 bytes, 0/2 places; first difference +0xC4.
 * cdiff: 401 of 515 rows differ by position, because two extra instructions
 * shift everything after +0x250. Aligned with difflib (addresses and symbols
 * normalised), 58 instructions differ. The Sol draft this replaces was
 * 2052/2052 bytes, 344 of 513 by position, 122 aligned: it is smaller and
 * lower by position, this one is lower aligned. Sol's is in git history.
 * To build: append after func_L04_003C3BA0 in L04-particle-emitter-g8.c, change
 * that file's extern to (char *), and drop the marker in L04_002CC748.c.
 * Everything needed follows (the g8 preamble, then this function).
 *
 * Still differs:
 * A. +0xC4..+0x184: retail sp+0x20 is s2 and sp+0x60 is s1; ours are swapped.
 *    gcse PRE inserts copies of both before the guard jump and cse2 keeps them
 *    as copies. Priorities are r127 0.48 vs r149 0.435; removing the copies
 *    would probably flip them (not shown).
 * B. Extra `sw $t2,0x148($sp)` at +0x250, with `addiu t2,sp,0x60` too early.
 *    regmove's backward pass pairs op3 with op0 and splits the cmove into a
 *    copy plus cmove, as the loop-invariant source has no REG_DEAD. Reload
 *    stores the copy and does not inherit it (reason not found), so
 *    delete_output_reload never runs; flow2 ignores sp stores. A 3-register
 *    cmove reaching reload would give retail's sequence directly. It needs a
 *    dying source whose set is past a label, jump or loop note; no wording
 *    tried produced one.
 * C. +0x2D0..+0x310 giftag setup: retail builds the constant in v1 and stores
 *    it after the three addiu; ours uses v0 and stores early. Also `lui t2`,
 *    `sw s4` and `addiu s7` order.
 * D. Cursor registers. Middle: retail u s1, v s2, colors s3; ours u s3, v s2,
 *    colors s1. Third: retail u s5, v s1, colors s2; ours u s2, v s1, colors
 *    s5. Post: retail u s1, v s2, colors s3, row s5; ours u s5, v s3, colors
 *    s2, row s1. Instruction order there already agrees.
 * E. Extra nop at +0x324.
 *
 * Helped (aligned count 122 -> 58): shade temp stored after u/v, own u,v per
 * loop, each cursor bumped right after its store, MACRO_ADDR on D_L04_001AF38C
 * (retail's adjacent lui+addiu; SDATA gives the gp form).
 * No gain: explicit colors cursors 67, v-first init 76, giftag temp 58, cursor
 * declaration order 58, MACRO_ADDR on 388 as well 62, other forms of the
 * transform selection 60-69 (none removes the store), indexed packet.uv 182.
 */
#include "common.h"

/* Initializes an emitter and its two slot ranges, then spawns nearby particles
 * and registers its render callback on subsequent updates. */
typedef unsigned int EQ __attribute__((mode(TI)));
typedef union { EQ q; float f[4]; } EVec;
#define EB(p,o) (*(u8 *)((char *)(p)+(o)))
#define EH(p,o) (*(s16 *)((char *)(p)+(o)))
#define EI(p,o) (*(s32 *)((char *)(p)+(o)))
#define EF(p,o) (*(float *)((char *)(p)+(o)))
extern char *D_L04_001B25CC;
extern EVec D_L04_0023BDD0[];
extern u8 D_L04_0023D090[];
extern s16 D_L04_0023D860[];
extern float D_L04_001AF3E0 __attribute__((sda));
extern int D_L04_001AF3BC __attribute__((sda));
extern int D_L04_001AF3C0 __attribute__((sda));
extern int D_L04_001AF3C4 __attribute__((sda));
extern int D_L04_001AF3C8 __attribute__((sda));
extern float D_L04_001AF3B4 __attribute__((sda));
extern float D_L04_001AF3B8 __attribute__((sda));
extern char D_L04_001B9680[];
extern float func_00282DF0(void *);
extern int func_00283DB8(void *,float);
extern float func_00282E80(void *,void *);
extern void func_0029F338(void *);
extern int func_002A77E0(int);
extern int func_002A7820(int,int);
extern float func_002A7878(float,float);
extern float func_002A78D8(float,float);
extern void func_002830C0(void *,void *,void *);
extern char *func_002BDCE8(float,void *,int,float,void *,float,int,float,int);
extern void ref_L04_003C3BA0_00(void *,void *);
extern void func_L04_003C4858(void *,int);
extern void func_L04_003C49C0(void *,int);
extern u32 func_L04_003C70C8(u32);
typedef struct { short first,count; int matrix; char pad08[0x40]; float radius; } EmitterView;
typedef struct { float x,y,z,w; } RPoint __attribute__((aligned(16)));
extern float D_L04_0023D220[],D_L04_0023D540[],D_L04_0023CA50[],D_L04_0023CD70[];
extern u8 D_L04_0023D158[];
extern s16 D_L04_0023D9F0[],D_L04_0023DB80[];
extern float D_L04_001B2E78,D_L04_001B2E80;
extern int D_L04_001AF330 __attribute__((sda));
extern float D_L04_001AF334 __attribute__((sda));
extern EVec D_L04_001AF340[] SDATA(D_L04_001AF340);
extern u8 D_L04_001AF380[] SDATA(D_L04_001AF380);
extern float D_L04_001AF388[] SDATA(D_L04_001AF388);
extern float D_L04_001AF38C[] MACRO_ADDR;
extern u8 D_L04_001AF3A8[] SDATA(D_L04_001AF3A8);
extern u16 D_L04_001B2E7C __attribute__((sda));
extern RPoint D_L04_002AB7A0[], D_L04_002AB6E0[];
extern int D_L04_002AB860[][4];
extern float D_L04_002AB924[],D_L04_002AB920[],D_L04_002AB8C4[],D_L04_002AB8C0[];
extern float D_L04_001AF548;
extern void func_00282CF0(void *,void *,void *);
extern void func_00282F20(void *,void *,float);
extern void func_00282DD8(void *,void *,void *);
extern void func_00282D30(void *,void *,float);
extern u64 func_0027C878(int);
extern void func_002FC560(int,u64);
extern void func_00280B90(void *,int,int);
extern u32 func_00282978(s16 *);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
void func_L04_003C4050(char *self) {
    EVec position,up,basis[4],other[4];
    struct {
        RPoint vertices[4];
        u32 colors[4]; float uv[8]; u64 giftag[4];
    } packet;
    char *state=*(char **)(self+0x68);
    char *matrix=D_L04_001B25CC+EI(state,4)*128;
    u32 alpha=(u32)EB(state,0x5C)<<24;
    u32 rgb=(EB(state,0x5F)<<16)|(EB(state,0x5E)<<8)|EB(state,0x5D);
    int end=EH(state,0)+EH(state,2)+EH(state,0x40);
    int i,j,k,index,opacity;
    RPoint *vertex;
    float speed,size,age,du,dv; u32 shade;
    qcopy(&position,self+0x10);
    up.q=0;
    position.f[3]=1.0f; up.f[2]=1.0f;
    func_00282CF0(&basis[0],D_L04_001B9680,&position);
    func_00282F20(&basis[0],&basis[0],1.0f);
    func_00282DD8(&basis[1],&basis[0],&up);
    func_00282F20(&basis[1],&basis[1],1.0f);
    func_00282DD8(&basis[2],&basis[1],&basis[0]);
    qcopy(&basis[3],&position);
    func_00282D30(&other[0],&basis[0],-1.0f);
    func_00282DD8(&other[1],&other[0],&up);
    func_00282F20(&other[1],&other[1],1.0f);
    func_00282DD8(&other[2],&other[1],&other[0]);
    qcopy(&other[3],&basis[3]);
    packet.giftag[1]=func_0027C878(EI(state,0x54)+0x3F);
    packet.giftag[2]=0xFF9000000260ULL; packet.giftag[3]=0x8000000048ULL;
    packet.giftag[0]=0;
    func_002FC560(0x4A,0); func_002FC560(0x47,0x51001);
    for(i=EH(state,0);i<end;i++) {
        EVec *transform,*chosen;
        age=D_L04_0023D220[i]-D_L04_0023D540[i];
        chosen=D_L04_0023D090[i] ? other : basis;
        transform=chosen;
        D_L04_0023D220[i]=age;
        if(age<=-8.0f) D_L04_0023D220[i]=age+8.0f;
        func_002830C0(&transform[3],&D_L04_0023BDD0[i],matrix);
        speed=D_L04_0023CA50[i]; size=D_L04_0023CD70[i];
        if(D_L04_0023D158[i]) {
            packet.giftag[3]=0x8000000044ULL;
            for(j=2;j<4;j++) {
                float *u,*v;
                u=(float *)((char *)&packet+0x50); v=(float *)((char *)&packet+0x54);
                for(k=0;k<4;k++) {
                    vertex=&packet.vertices[k];
                    index=D_L04_002AB860[j][k];
                    vertex->x=D_L04_002AB7A0[index].x;
                    vertex->y=D_L04_002AB7A0[index].y*speed;
                    vertex->z=D_L04_002AB7A0[index].z*size;
                    vertex->w=1.0f;
                    func_002830C0(&packet.vertices[k],&packet.vertices[k],transform);
                    *u=D_L04_002AB924[index*2]+D_L04_0023D220[i];
                    u=(float *)((char *)u+8);
                    *v=D_L04_002AB920[index*2];
                    v=(float *)((char *)v+8);
                    packet.colors[k]=alpha*D_L04_001AF3A8[index]|rgb;
                }
                func_00280B90(&packet,0,0);
            }
        } else {
            float *u,*v;
            if(D_L04_0023D860[i]) {
                func_00282978(&D_L04_0023D860[i]);
                opacity=func_00283CF0((1.0f-func_00283CE0(D_L04_0023D860[i])*D_L04_001B2E78)*D_L04_001AF334);
            } else if(D_L04_0023D9F0[i]) {
                if(func_00282978(&D_L04_0023D9F0[i])) {
                    func_L04_003C4858(self,i); opacity=0;
                    D_L04_0023D090[i]=func_002A77E0(2);
                } else opacity=func_00283CF0(func_00283CE0(D_L04_0023D9F0[i])*D_L04_001B2E80*D_L04_001AF334);
            } else {
                if(func_00282978(&D_L04_0023DB80[i])) D_L04_0023D9F0[i]=D_L04_001B2E7C;
                opacity=D_L04_001AF330;
            }
            packet.giftag[3]=0x8000000048ULL;
            opacity=(u32)opacity<<24;
            u=(float *)((char *)&packet+0x50); v=(float *)((char *)&packet+0x54);
            for(k=0;k<4;k++) {
                vertex=&packet.vertices[k];
                vertex->x=D_L04_001AF340[k].f[0];
                vertex->y=D_L04_001AF340[k].f[1]*speed;
                vertex->z=D_L04_001AF340[k].f[2]*size;
                vertex->w=1.0f;
                du=D_L04_001AF388[k*2]-D_L04_0023D220[i]; dv=D_L04_001AF38C[k*2];
                shade=(u32)opacity*D_L04_001AF380[k];
                *u=du; *v=dv;
                packet.colors[k]=shade|EI(state,0x38);
                func_002830C0(&packet.vertices[k],&packet.vertices[k],transform);
                u=(float *)((char *)u+8); v=(float *)((char *)v+8);
            }
            func_00280B90(&packet,0,0);
        }
    }
    packet.giftag[1]=func_0027C878(EI(state,0x58)+0x3F);
    packet.giftag[3]=0x8000000044ULL;
    qcopy(&basis[3],matrix+0x30);
    basis[3].f[2]-=EF(state,0x50);
    if(!EH(state,0x42)) {
        for(j=0;j<6;j++) {
            float *u,*v;
            u=(float *)((char *)&packet+0x50); v=(float *)((char *)&packet+0x54);
            for(k=0;k<4;k++) {
                vertex=&packet.vertices[k];
                index=D_L04_002AB860[j][k];
                vertex->x=D_L04_002AB6E0[index].x;
                vertex->y=D_L04_002AB6E0[index].y*EF(state,0x48);
                vertex->z=D_L04_002AB6E0[index].z*EF(state,0x4C);
                vertex->w=1.0f;
                func_002830C0(&packet.vertices[k],&packet.vertices[k],basis);
                *u=D_L04_002AB8C4[index*2]+D_L04_001AF548;
                u=(float *)((char *)u+8);
                *v=D_L04_002AB8C0[index*2];
                v=(float *)((char *)v+8);
                packet.colors[k]=D_L04_001AF3A8[index]*0xFF000000|rgb;
            }
            func_00280B90(&packet,0,0);
        }
    }
}
