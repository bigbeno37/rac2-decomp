/* Tier: opus-2 (v2528), USA v1.01; best 2752/2752 bytes, 0/27 places; 51 of 688 instructions differ with
 * relocations masked (opus-1 left 60, with a 2752-byte draft that had the wrong structure in four places).
 * Unit: src/levels/shared/L00_003C3D90-g8.c, profile -O2 -G8 -ffunction-sections -g -frac2-hazard-labels.
 * All declarations it needs are below.
 *
 * REVIEW, must be ruled on before this can land: func_00282978_1 is a void alias of FastDecTimer, which returns int
 * (two other sites in this function read the result, so one prototype cannot serve all three). It is probably a false
 * prototype. Without it C4020 is `bne` and C4040 has the two insns swapped. No legal wording found. See 1.
 *
 * Settled by measurement here; keep these:
 * 1. C4020/C4040 are one problem. Colour selection is a plain if/else (not two ternaries). sched2 breaks the
 *    `sw v0,8(s3)` / `a0=s3+4` priority tie on dependent count: a0 has 2 (the call, the next a0 set), the store 1,
 *    because a call_value is v0's next explicit set. A plain `call` leaves v0's use list open, so the store also
 *    depends on the next v0 set, the tie falls to luid and retail's order results. a0 then goes into the jal slot,
 *    update_block's USE makes a0 look live in reorg's walk, and the first branch becomes retail's annulled `bnel`.
 *    So the retail call looks like a `call`, not a `call_value`. Not compiled, from expand_call: a (void) cast or an inline
 *    wrapper keeps the CALL_EXPR's int type, so still call_value. Untested: a real block boundary between this call and `a0=sp`, which would drop a0's
 *    second dependent instead.
 * 2. C45C8: the a0 preheader init must not be PRE'd above the first test. ctx->targets[k] forces ctx+0x20 into a
 *    register (expand_assignment's alignment test), which gcse then hoists. TARGET() below gives (ctx+k*4)+0x20.
 * 3. C4618: `<<2` in TARGET(), not `*4`: a MULT sorts first in expand_expr's both_summands, a shifted register does
 *    not, so this gives retail's `addu v0,s3,v0`. `*4` fixes item 2 and not this one.
 * 4. The search loop may be written with the break or with `k<4 && x!=target` in the for: the diff did not change.
 * 5. Spill slots 0x84..0x94 match in this form, and the two ternaries are no longer needed for them.
 * Earlier findings still true: tables 2950/2970/2990/00240080 are incomplete `sda` arrays; the short tables are one
 * array D_L00_001B2990[][8]; every loop counter is its own scoped variable; typed Particle/HitPacket/Moby views;
 * `weapon` pointer, chained slot store, `object` local in the acquisition block. Reload registers are round-robin.
 *
 * Still differs (51 rows, three places):
 * A. C4058-C4084: `addiu v1,s3,0x10` + its spill go before the func_00282D30 call in retail, after it here. sched1
 *    issues the PRE copies 2101/2104 by priority; 2289 (r666=2970, prio 2 through its s7 copy) beats them, so 2104 lands
 *    after the call and $3 then depends on that call in sched2. Retail needs them ahead of 2289.
 * B. C46C4-C46EC: first table address must get v1, second a0, 64 a1 (a reload: the a0 slot is busy, round-robin
 *    moves on). Local-alloc gives the second v1 because its qty is one insn shorter (gap 4 vs 5: `r494=v0` and the
 *    spread load sit inside the first). Equal gaps should tie in the first's favour (not reached). Results of trying:
 *    `D[1][r]=64` literal: v1/a0 right, 46 rows, but 64 lands in v1 and `sh v0,(s0)`/`a0=sp+0x30` swap at C46F0.
 *    Passing D_L00_001AFF7C directly: second goes to a1, order worse. `short id`: adds a sign extension (690 rows).
 *    C4700 (`lw a0,0x84(sp)` order) probably follows from B or C.
 * C. C4720-C4784, ribbon preheader. Retail: three givs s0 [n-1], s1 [n], s2 [n].w with `swc1 f21,0(s2)`, inits
 *    `v0=D+mult; v1=v0; s2=v0+0x1C; s1=v1+0x10`. So .w is a REGISTER giv: the +12 is applied before n*16 is added
 *    (otherwise it is mem(r+12) and combine_givs folds it into s1, losing a saved register, so D wins $fp), and the
 *    add_val must reach sge_plus_constant as (plus D mult), D first, so the emitted tree should be (D+mult)+28 and cse2
 *    can merge it with [n]'s init, leaving the daddu (reasoned from loop.c and expr.c, not reached). The mult must be loop.c's own, or cse merges it with the madd.
 *    Measured forms: `row` local (here): s2 from the madd, one insn short (the nop at C4784 is loop alignment).
 *    aligned(16) on EffectVector with natural D[r][n].v[3]: no fold into the symbol, add ((mult+12)+D), 690 rows.
 *    `float *w=D[r][n].v; w[3]`, and `v+3` through a second local: cse merges with [n], combined, D takes $fp.
 *    ((float *)&D[r][n])[3]: 12 folds into the symbol. Pointer `table[r][n].v[3]` or `*(table[r][n].v+3)`: flattened
 *    by the front end to (mult+table)+n*16, D+12 appears, 690 rows.
 *    ((Ribbon *)D+r)->pts[n].v[3], struct Ribbon { EffectVector pts[10]; }: right register giv, but add
 *    ((mult+12)+D) emits (mult+0x10)+(D+0xC), 691 rows. Closest in structure. Next: the same with D first, for which
 *    the row base must be a register plus a non-MULT index (a MULT always sorts first).
 * About 30 compiles. Order of work next: rule on the REVIEW item, then C, then B and A (C changes insn counts, hence gcse
 * buckets and slots).
 */
#include "common.h"
extern int D_L00_001A8F00;
extern char D_00189EA0[];
extern char D_00188660[];
extern unsigned char D_00139568[];
typedef struct { char pad0[4]; unsigned char active; char pad5[0x6b]; float strength; char pad74[0x6c]; } WeaponEntry;
extern WeaponEntry D_L00_00262570[];
typedef struct { char pad[0xb4]; int changed; } ChangedState;
extern ChangedState D_00139868;
typedef struct { char pad0[0x80]; float position[4]; char pad90[0x11b0]; int upgrades; char pad1244[0x104c]; char *moby; char pad2294[0x28]; unsigned char busy; } HeroState;
extern HeroState D_00189E20_13;
extern char D_L00_001B8DC0[];
extern char D_L00_0023F040[];
typedef struct { float v[4]; } EffectVector;
extern EffectVector D_L00_00240080[][10] __attribute__((sda));
extern int D_L00_001B2950[] __attribute__((sda));
extern int D_L00_001B2970[] __attribute__((sda));
extern short D_L00_001B2990[][8] __attribute__((sda));

typedef struct { char pad[0xf8]; unsigned char *f8; } ParticleDefs;
extern ParticleDefs D_L00_001FE940;
extern int D_L00_001AFF98 __attribute__((sda));
extern int D_L00_001AFF44 __attribute__((sda));
extern int D_L00_001AFF90 __attribute__((sda));
extern int D_L00_001AFF10 __attribute__((sda));
extern int D_L00_001AFF14 __attribute__((sda));
extern int D_L00_001AFF18 __attribute__((sda));
extern int D_L00_001AFF1C __attribute__((sda));
extern int D_L00_001AFF24 __attribute__((sda));
extern float D_L00_001AFF2C __attribute__((sda));
extern float D_L00_001AFF40 __attribute__((sda));
extern float D_L00_001AFF4C __attribute__((sda));
extern int D_L00_001AFF64 __attribute__((sda));
extern int D_L00_001AFF68 __attribute__((sda));
extern float D_L00_001AFF6C __attribute__((sda));
extern int D_L00_001AFF70 __attribute__((sda));
extern float D_L00_001AFF7C __attribute__((sda));
extern void func_L00_0037E640(void);
extern void func_00282978_1(void *);
extern int func_002E59B0(void *,int);
extern int func_002E5CF8(int,int,void *);
extern void func_002E59F8(int);
extern int func_L00_003C3A70(int);
extern float func_L00_0037E668(void);
extern int func_L00_003C5B18(float);
extern void func_L00_0037E698(void);
extern void func_0029F338(void *);
extern float func_00282C48(float);
extern int func_00283CF0(float);
extern float func_00283CE0(int);
extern int func_00283D38(float,int,int);
extern int func_00282978(void *);
extern void func_00282D30(void *,void *,float);
extern void func_00282CC0(void *,void *,void *);
extern void func_0029FCC0(void *,int,void *);
extern int func_002A7820(int,int);
extern float func_002A7940(void);
extern void func_002AF000(void *,float,float,float);
extern char *func_002C36D0(void *,int,int,int);
extern int func_00282948(void *);
extern int func_002829D8(void *);
extern int func_00277F48(void *,int,void *,int,float);
extern float *func_002AB1F0(void *);
extern void func_00282F20(void *,void *,float);
extern void func_00282CF0(void *,void *,void *);
extern void func_002A8E20(void *,void *);
extern void func_L00_003C58E8(void *,int);
extern char *func_L00_00412FF8(void *,int);
extern void func_002A7988(void *,float,float);
extern float func_002A7878(float,float);
extern void func_002ACEC0(void *,void *,void *,float);
extern void func_L00_002E6CF0(void *,void *);
extern void func_L00_003C4850(void *);
#define WORD(p,o) (*(int *)((char *)(p)+(o)))
#define FLOAT(p,o) (*(float *)((char *)(p)+(o)))
#define HALF(p,o) (*(short *)((char *)(p)+(o)))
#define BYTE(p,o) (*(unsigned char *)((char *)(p)+(o)))
typedef struct { char pad0[0x46]; short category; } MobyInfo;
typedef struct EffectContext EffectContext;
typedef struct Moby { char pad0[0x10]; float x,y,z,w; unsigned char state; char pad21[3]; MobyInfo *info; char pad28[9]; unsigned char enabled; char pad32[0x68-0x32]; EffectContext *ctx; char pad6c[0xaa-0x6c]; short oclass; char padac[0xe0-0xac]; float forward[4]; } Moby;
typedef struct { char pad0[4]; Moby *moby; char pad8[8]; float dx,dy,dz; } ParticleOwner;
typedef struct { char pad0[2]; unsigned char sprite; char pad3; int colour; char pad8[4]; float size; float x,y,z; char pad1c[4]; ParticleOwner owner; char pad3c[0xb8-0x3c]; Moby *source; } Particle;
typedef struct { float x,y,z; float damage; Moby *source; int flags; unsigned char kind; unsigned char mode; short oclass; float strength; int one; char pad24[0xc]; } HitPacket;
struct EffectContext { float value; short timer; short pad6; int colour; int sound; float cooldown; int lock; short timers[4]; Moby *targets[4]; };
extern Moby *D_L00_001C3280[];
#define TARGET(c,i) (((Moby **)((char *)(c)+((i)<<2)))[8])
static __inline__ void clearSound(Moby *self,EffectContext *ctx)
{
    int id=ctx->sound;
    if(id!=-1) {
        char *sound=D_00188660+id*112;
        if(*(Moby **)(sound+0x88)==self && BYTE(sound,0x74))
            func_002E59F8(id);
    }
    ctx->sound=-1;
}
void func_L00_003C3D90(Moby *self)
{
    float offset[4],origin[4],position[4];
    EffectContext *ctx=self->ctx;
    float value,ratio,sign;
    int i,j,k,slot,col1,col2;
    if(D_L00_001A8F00) return;
    qcopy(&self->x,D_00189EA0);
    func_L00_0037E640();
    if(!D_L00_001A8F00) {
        if(!func_002E59B0(self,ctx->sound))
            ctx->sound=func_002E5CF8(3,4,self);
    } else if(func_002E59B0(self,ctx->sound)) clearSound(self,ctx);
    if(D_L00_00262570[D_00139568[0x2d]].active && self->oclass==0xa5c) {
        int saved=D_L00_001AFF98;
        D_L00_001AFF98=0;
        if(func_L00_003C3A70(1)) {
            clearSound(self,ctx);
            func_002E5CF8(2,0,self);
            D_L00_001AFF98=0;
            func_0029F338(self);
            return;
        }
        D_L00_001AFF98=saved;
    }
    value=func_L00_0037E668();
    if(!func_L00_003C5B18(value) || D_L00_001AFF44==40) {
        clearSound(self,ctx);
        D_L00_001AFF44=0;
        func_L00_0037E698();
        func_002E5CF8(2,0,self);
        D_L00_001AFF98=0;
        func_0029F338(self);
        return;
    }
    if(value!=ctx->value) {
        D_00139868.changed+=func_00283CF0(func_00282C48(ctx->value-value));
        ctx->value=value;
        ctx->timer=D_L00_001AFF90;
    }
    ratio=func_00283CE0(ctx->timer)/func_00283CE0(D_L00_001AFF90);
    if(self->oclass==0xa5c) {
        col1=D_L00_001AFF10;
        col2=D_L00_001AFF14;
    } else {
        col1=D_L00_001AFF18;
        col2=D_L00_001AFF1C;
    }
    ctx->colour=func_00283D38(ratio,col1,col2);
    func_00282978_1(&ctx->timer);
    func_00282D30(offset,D_00189E20_13.moby+0xe0,0.75f);
    func_00282CC0(&self->x,D_00189E20_13.position,offset);
    func_0029FCC0(D_00189E20_13.moby,10,origin);
    for(i=0;i<8;i++) {
        if(!D_L00_001B2950[i]) {
            Particle *effect;
            int id=func_002A7820(D_L00_001AFF64,D_L00_001AFF68);
            value=func_002A7940();
            ratio=func_002A7940();
            func_002AF000(position,D_L00_001AFF4C,value,ratio);
            func_00282CC0(position,position,origin);
            effect=(Particle *)(D_L00_001B2950[i]=(int)func_002C36D0(position,id,2,1));
            if(effect) {
                ParticleOwner *sub=&effect->owner;
                sub->dx=effect->x-self->x;
                sub->dy=effect->y-self->y;
                sub->dz=effect->z-self->z;
                sub->moby=self;
                effect->colour=D_L00_001AFF70|ctx->colour;
                effect->size=D_L00_001AFF6C*210000.0f;
                effect->sprite=D_L00_001FE940.f8[1];
                D_L00_001B2970[i]=id;
            }
            effect=(Particle *)func_002C36D0(position,id,2,1);
            if(effect) {
                ParticleOwner *sub=&effect->owner;
                sub->dx=effect->x-self->x;
                sub->dy=effect->y-self->y;
                sub->dz=effect->z-self->z;
                sub->moby=self;
                effect->colour=0x307f7f7f;
                effect->size=D_L00_001AFF6C*63000.00390625f;
                effect->sprite=D_L00_001FE940.f8[1];
            }
        } else if(func_00282948(D_L00_001B2970+i)) D_L00_001B2950[i]=0;
    }
    slot=0;
    func_00282948(&ctx->lock);
    {
    int t;
    for(t=0;t<4;t++) if(func_00282978(&ctx->timers[t])) {
        ctx->targets[t]=0;
        slot=t;
    }
    }
    if(func_002829D8(&ctx->cooldown)) {
        int count=func_00277F48(&self->x,1,D_00189E20_13.moby,0,D_L00_001AFF2C*0.3f);
        if(count) {
            HitPacket packet;
            float scaled[4],output[4];
            int p;
            WeaponEntry *weapon=&D_L00_00262570[D_00139568[0x2d]];
            packet.flags=65536;
            packet.one=1;
            packet.damage=5627.9248046875f;
            packet.kind=3;
            packet.strength=weapon->strength;
            packet.mode=2;
            packet.oclass=self->oclass;
            packet.source=self;
            for(p=0;p<count;p++) {
                Moby *object=D_L00_001C3280[p];
                float *health;
                if(!object->enabled || !object || object->state==0xfe || object->state==0xfd || !object->info || object->info->category!=5) continue;
                health=func_002AB1F0(object);
                if(health) {
                    if(health[0]<=0) continue;
                    func_00282D30(scaled,self->forward,health[4]);
                    func_00282CC0(position,&object->x,scaled);
                } else qcopy(position,&object->x);
                func_00282CF0(&packet,position,&self->x);
                func_00282F20(&packet,&packet,1.0f);
                func_002A8E20(object,&packet);
                func_00282D30(scaled,&packet,D_L00_001AFF2C*0.3f);
                func_00282CC0(output,&self->x,scaled);
                func_L00_003C58E8(output,ctx->colour);
                ctx->cooldown=0.5f;
            }
        }
    }
    if(!D_L00_001A8F00 && slot && self->oclass==0x1192 && !ctx->lock && !D_00189E20_13.busy && D_00189E20_13.upgrades>=23) {
        int hits=func_00277F48(&self->x,1,D_00189E20_13.moby,0,D_L00_001AFF40);
        if(hits) for(j=0;j<hits;j++) {
            float *health;
            int freeSlot;
            if(!D_L00_001C3280[j]->enabled || !D_L00_001C3280[j] || D_L00_001C3280[j]->state==0xfe || D_L00_001C3280[j]->state==0xfd || !D_L00_001C3280[j]->info || D_L00_001C3280[j]->info->category!=5) continue;
            health=func_002AB1F0(D_L00_001C3280[j]);
            if(health && health[0]<=0) continue;
            freeSlot=-1;
            for(k=0;k<4 && D_L00_001C3280[j]!=TARGET(ctx,k);k++) {
                if(!TARGET(ctx,k)) freeSlot=k;
            }
            if(k==4) {
                Particle *effect;
                Moby *object;
                if(freeSlot<0) break;
                object=D_L00_001C3280[j];
                TARGET(ctx,freeSlot)=object;
                effect=(Particle *)func_L00_00412FF8(object,D_L00_001AFF24);
                if(effect) {
                    effect->source=self;
                    D_L00_001AFF44++;
                    func_002E5CF8(5,0,self);
                    ctx->timers[slot]=func_002A7820(20,60);
                }
                break;
            }
        }
    }
    qcopy(position,origin);
    {
    int r;
    for(r=0;r<5;r++) {
        if(func_00282978(&D_L00_001B2990[0][r])) D_L00_001B2990[0][r]=0;
        if(!D_L00_001B2990[0][r]) {
            float velocity[4],scaled[4];
            int id=func_002A7820(3,45);
            float spread=D_L00_001AFF7C;
            int fade=64;
            D_L00_001B2990[2][r]=id;
            D_L00_001B2990[1][r]=fade;
            D_L00_001B2990[0][r]=id;
            func_002A7988(velocity,spread,spread);
            qcopy(D_L00_00240080[r],position);
            D_L00_00240080[r]->v[3]=0.04f;
            sign=1.0f;
            {
            int n;
            EffectVector *row=D_L00_00240080[r];
            for(n=1;n<10;n++) {
                func_00282CF0(scaled,D_L00_00240080[r][n-1].v,D_L00_001B8DC0);
                func_002ACEC0(velocity,velocity,scaled,func_002A7878(0.0872664749622345f,1.3089970350265503f)*sign);
                sign=-sign;
                func_00282CC0(D_L00_00240080[r][n].v,D_L00_00240080[r][n-1].v,velocity);
                row[n].v[3]=0.04f;
            }
            }
        }
    }
    }
    func_L00_002E6CF0(func_L00_003C4850,self);
}
