/*
 * Tier: opus-2 (Claude Opus 5.5, v2543), USA v1.01; based on opus-1 (v2290).
 * Profile: -O2 -G8 -ffunction-sections -g, separate -g8-dbg.c unit.
 * Best checked result: 1404/1404 bytes, 0/27 places, first mismatch +0x100,
 * 66 of 351 rows differ (was +0x64, 188 rows); most of the 66 are unresolved
 * relocations in the diff. Real differences: the hide block (9 rows) and the
 * $a0/$v0 reload rotation it causes at the scan-pointer reloads (6 rows).
 * Solved here: entry order (no target local: base+176 inline, base/pos/move/
 * state declared in that order); scan and hidden are loop.c givs of i and
 * numhidden (index form), which gives the slots goal 0x60, scan 0x64, hidden
 * 0x68; the first timer call is shared by flag!=0 and the range test, so
 * cse reuses &state->timer in s0 and interblock puts it in the bnez slot;
 * span local keeps the distance in $f12; m is a struct pointer.
 * Hide block facts, each measured:
 *  - The increment reuses $v1 with no second lw. reload never inherits into
 *    this in-out add: a load through the pointer only looked like it because
 *    reload_cse deletes the repeat, and a store between stops that (slot is
 *    not MEM_SCALAR_P, so anti_dependence always conflicts).
 *  - So retail must have (set 278 (plus Y 4)) with Y a pseudo in $v1. That
 *    needs a DEST_REG giv reduced with the store combined into it
 *    (combine_givs skips a DEST_REG giv used in exactly one insn of the
 *    loop), plus cse2 making Y the class leader (Y must outlive the block).
 *  - The dead copy `entry=hidden` in the loop tail below does both: it adds
 *    the use, and flow deletes it so Y stays block-local (it lands in $v0).
 *    `char **hidden=0` and this copy are placeholders for whatever retail
 *    really wrote; no natural second use was found (a re-read of *hidden is
 *    removed by cse1 first).
 *  - Mine schedules Y=copy, numhidden++, store, add, clear; the copy delays
 *    the store a cycle and numhidden++ takes the second issue slot.
 *  - Target: RTL store via the reduced reg, THEN Y=copy, add from Y. sched1
 *    should then issue copy+store together, reload reuse the loaded reg for
 *    the store address (find_equiv_reg), numhidden take $a0 with Y live, and
 *    sched2 raise the addiu. `D[n]=m; hidden=&D[n];` with the copy after the
 *    loop (zero loop uses) gives that RTL at loop.c, but cse2 did not rewrite
 *    the add from Y, so Y dies and the second lw returns (262 rows). Find why
 *    the leader swap fails there (make_regs_eqv in cse.c).
 *  - Do not use: `hidden=&D[n]; *hidden=m; n++; <use of hidden>` in one block
 *    with a biv update inside the lifetime: it emits sw s0,4(v1).
 * Requires the -g8-dbg unit flags above and the declarations below.
 */
#include "include_asm.h"
extern char D_L00_001B8E20[],D_L00_001BF220[];
typedef struct { char pad[404]; char *active; char gap[620]; int counter; } GlobalCamera243;
typedef struct { char pad[256]; float x, y; } Controls243;
extern GlobalCamera243 D_L00_001B8C80;
extern Controls243 D_00138180;
extern char *D_L00_001C3280[], *D_L00_0025B9A0[];
extern char *D_L00_001B184C;
extern float D_L00_001B14CC __attribute__((sda));
typedef struct { char pad[0x2290]; char *owner; int mode; } Player243;
extern Player243 D_L00_00189E20;
extern void func_00282D08(void *,void *,void *,float);
extern int func_00277F48(void *,int,void *,int,float),func_L00_0031BE88(void *);
extern void func_00282CF0(void *,void *,void *),func_00282D30(void *,void *,float),func_00282F20(void *,void *,float);
extern float func_00282DB0(void *,void *),func_002831B0(float),func_00282C48(float);
extern void func_00282DD8(void *,void *,void *),func_002ACEC0(void *,void *,void *,float);
extern int func_L00_004240D8(void *),func_L00_00423BB0(void *),func_L00_004239A8(void *);
extern int func_00282978(short *);
extern float func_00283CE0(int),func_002A7C00(float,float,float);
extern void func_00282CC0(void *,void *,void *);
extern int func_002761F8(void *,void *,void *,void *,int),func_L00_004204A0(void *);
extern void func_L00_002D9D98(void);
#define F(p,n) (*(float *)((char *)(p)+(n)))
#define S(p,n) (*(short *)((char *)(p)+(n)))
#define U(p,n) (*(unsigned short *)((char *)(p)+(n)))
#define P(p,n) (*(char **)((char *)(p)+(n)))
#define I(p,n) (*(int *)((char *)(p)+(n)))
typedef struct { char pad[16]; float pos[4]; char pad20[4]; struct { char pad[16]; int value; } *info; char pad28[112]; int visible; char pad9C[14]; short type; } Moby243;
typedef struct { char pad[44]; float distance; } Move243;
typedef struct { char pad[48]; float offset; short timer; char gap[4]; short flag; char gap2[16]; short counter, locked; } State243;
void func_L00_004243C0(char *obj)
{
    char *base=P(obj,112);
    char *pos=base+96;
    Move243 *move=(Move243 *)(base+352);
    State243 *state=(State243 *)(base+496);
    char *camera=D_L00_001B8E20;
    char a[16],b[16],c[16],d[16],e[16];
    Moby243 *m;
    char *axis;
    int count,n,blocked=0,numhidden=0,type,i;
    float distance=move->distance-state->offset+1.5f;
    float dot,t;
    unsigned short timer;
    char **hidden=0;
    Moby243 *entry;
    char *goal;
    func_00282D08(a,base+176,camera-96,0.5f);
    count=func_00277F48(a,1,D_L00_00189E20.owner,0,distance*0.5f);
    if(count>0) {
        for(i=0;i<count;i++) {
            goal=base+176;
            m=(Moby243 *)D_L00_001C3280[i];
            if(m->visible && (!func_L00_0031BE88(m)||m->type==2923)) {
                type=m->type;
                if(type!=2234 && type!=3335 && type!=4304 && type!=1143 && type!=11) {
                    hidden=&D_L00_0025B9A0[numhidden];
                    *hidden=(char *)m;
                    numhidden++;
                    m->visible=0;
                } else {
                    func_00282CF0(b,m->pos,goal);
                    axis=camera+48;
                    dot=func_00282DB0(axis,b);
                    func_00282D30(c,axis,dot);
                    func_00282CF0(d,b,c);
                    func_00282F20(d,d,1.0f);
                    dot=func_00282DB0(d,move);
                    {
                    float span=move->distance-state->offset;
                    span=dot/span;
                    t=func_002831B0(span);
                    }
                    t=func_00282C48(1.5707963705062866f-t);
                    if(t<0.4363323450088501f) {
                        func_00282DD8(e,move,axis);
                        func_00282F20(e,e,1.0f);
                        if(func_00282DB0(e,b)>0.0f)func_002ACEC0(move,move,camera+32,-0.01745329424738884f);
                        else func_002ACEC0(move,move,camera+32,0.01745329424738884f);
                    }
                }
            }
            entry=(Moby243 *)hidden;
        }
    }
    if(!state->locked) {
        if(func_L00_004240D8(obj)) blocked=1;
        if(func_L00_00423BB0(obj)) blocked=1;
    }
    if(func_L00_004239A8(obj))blocked=1;
    {
    float current_distance=move->distance;
    float minimum=D_L00_001B14CC;
    if(current_distance-state->offset<minimum) {
        state->timer=2000;
        state->offset=current_distance-minimum;
    }
    }
    {
    char *active_camera=D_L00_001B8C80.active;
    if(active_camera && (S(active_camera,134)==4 || S(active_camera,134)==81) && D_L00_001B8C80.counter<10) state->timer=1700;
    }
    if(!state->flag) {
        timer=(unsigned short)state->timer;
        if(blocked || !(((unsigned)(timer-1)<1999) ||
            (D_00138180.x<0.3f && D_00138180.y<0.3f))) goto hold;
    }
    if(func_00282978(&state->timer))state->flag=0;
    if(state->flag) {
        if(func_00282978(&state->timer))state->flag=0;
    }
    t=func_00283CE0(state->timer);
    t=t/func_00283CE0(2000);
    state->offset=func_002A7C00(0.0f,state->offset,t);
    goto scaled;
hold:
    if((short)timer>2000)func_00282978(&state->timer);
    else state->timer=2000;
scaled:
    func_00282F20(move,move,move->distance-state->offset);
    func_00282CC0(b,move,pos+80);
    if(D_L00_00189E20.mode!=49 && !state->locked &&
       func_002761F8(pos+80,b,D_L00_001B184C,D_L00_00189E20.owner,0) &&
       !func_L00_004204A0(D_L00_001BF220)) {
        state->counter++;
        if(state->counter>=30)func_L00_002D9D98();
    } else state->counter=0;
    if(numhidden>0) {
        char **restore=D_L00_0025B9A0;
        int remaining=numhidden;
        do {
            entry=(Moby243 *)*restore++;
            entry->visible=entry->info->value;
        } while(--remaining);
    }
}
