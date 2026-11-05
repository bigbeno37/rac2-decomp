/*
 * Tier: opus-2 (Claude Opus 5.5, v2621), USA v1.01; based on v2543 and v2290.
 * Profile: -O2 -G8 -ffunction-sections -g -fstrict-aliasing, a separate
 * L00_004243C0-g8-dbg.c unit (same flags as its neighbour L00_004240D8-g8-dbg.c).
 * Best checked result: 1404/1404 bytes, 0/27 places, first mismatch +0x154, 55 of
 * 351 rows differ. 51 of the 55 are unresolved relocations in the diff. The only
 * real difference is where `addiu $v1,$v1,4` sits (4 rows at +0x154):
 *   mine:   lw v1,hidden; sw s0,0(v1); lw a0,n; addiu a0; addiu v1,4; sw a0,n; ...
 *   retail: lw v1,hidden; sw s0,0(v1); addiu v1,4; lw a0,n; addiu a0; sw a0,n; ...
 * This draft does NOT land as it stands: the do/while group and using `type` as a
 * pointer are measured means to an end, not known retail source. They need review
 * even if the last row closes.
 *
 * New in v2621 (compiled and read, unless marked as inferred):
 *  - -fstrict-aliasing belongs on this unit: with the reused `type` it alone fixes the
 *    camera test and the registers after it. The neighbouring unit has the same flag.
 *  - scan and hidden can be plain pointers declared in the if block. With goal still
 *    assigned inside the loop (declared at the top, so hoisted, low pseudo number) the
 *    slots stay goal 0x60, scan 0x64, hidden 0x68. That drops the index givs, the dead
 *    `entry=hidden` copy and `hidden=0` that v2543 needed.
 *  - One `type` local read at the ladder, used for the pointer and read at the camera
 *    test gives $v1 with no other help (inferred: a global pseudo that conflicts with
 *    the ladder's $v0 constants). A block-local pointer lands in $v0.
 *  - Plain `D[n++]=m` already gets every register and the rotation right (v1, a0, then
 *    v0 for scan) and differs by one insn: a second `lw v1,hidden` before the add.
 *    Reload never inherits into this in-out add. reload_cse deletes the repeat, but
 *    only if no store through an unknown pointer intervenes (test: K=(int)p; p++ has
 *    no second lw, *p=x; p++ and K=(int)p; *Q=1; p++ do). The slot's alias set is 0
 *    and it is not MEM_SCALAR_P, so the store always conflicts and -fstrict-aliasing
 *    cannot help. So plain D[n++]=m cannot match on this compiler.
 *
 * What retail needs (inferred from reload rotation and store dependences):
 *  - sched1 output must have store < n++ < clear. At sched2 a store through an unknown
 *    register conflicts with every slot access, so that order cannot change there.
 *  - The pointer register must be busy at n++ (a0, not v1) or the rotation must already
 *    have advanced. Either the add follows n++ with Y in v1, or (store via R) the store's
 *    own address reload advances it.
 * Why each route stops:
 *  - store via R, Y=R, R=Y+4 (loop.c gives this order, cse2 rewrites the add, with a
 *    local that outlives the block): combine folds the copy into the add, because the
 *    add is Y's first use and Y dies there. Back to the in-out add and the second lw.
 *  - Y=R, store via Y: no fold, but the store waits a cycle, so n++ takes the second
 *    issue slot at t0 and goes first (with -fstrict-aliasing the clear can take it, and
 *    then the clear goes first).
 *  - A no-op copy `axis=(char *)m` (tied to s0, prints nothing) takes that slot, so the
 *    store now precedes n++, but the add goes before n++ and n++ gets $v1 (inferred:
 *    reg weight 0 vs +1, Y dies at the add).
 *  - A do/while(0) group is a barrier at sched1 and also at sched2: it fixes sched1 but
 *    keeps the add below it. Here combine folds the in-place `type+1` into
 *    `hidden=type`, after the group (seen in the combine dump), so the add stays below
 *    the barrier and behind `lw a0`.
 * Untried leads: a barrier or dependence that exists at sched1 and is gone at sched2; a
 * second use of the in-place add's result that survives combine and prints nothing.
 * Not usable: `hidden=&D[n]; ...; *hidden=m` with the store after the biv update (giv
 * init -4 or sw s0,-4(v1)); a reused local multi-set inside the loop as the giv itself
 * (no reduction, 1396 bytes).
 * Requires the unit flags above and the declarations below.
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
    Moby243 *entry;
    char *goal;
    func_00282D08(a,base+176,camera-96,0.5f);
    count=func_00277F48(a,1,D_L00_00189E20.owner,0,distance*0.5f);
    if(count>0) {
        char **scan,**hidden;
        scan=D_L00_001C3280;
        hidden=D_L00_0025B9A0;
        for(i=0;i<count;i++) {
            goal=base+176;
            m=(Moby243 *)*scan;
            if(m->visible && (!func_L00_0031BE88(m)||m->type==2923)) {
                type=m->type;
                if(type!=2234 && type!=3335 && type!=4304 && type!=1143 && type!=11) {
                    do {
                        type=(int)hidden;
                        *(char **)type=(char *)m;
                        type=(int)((char **)type+1);
                    } while(0);
                    numhidden++;
                    m->visible=0;
                    hidden=(char **)type;
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
            scan++;
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
    if(active_camera && ((type=S(active_camera,134))==4 || type==81) && D_L00_001B8C80.counter<10) state->timer=1700;
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
