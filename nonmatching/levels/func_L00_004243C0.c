/*
 * Tier: opus-1 (Claude Opus 5.5, v2290), USA v1.01; based on sol-high (v1110).
 * Profile: -O2 -G8 -ffunction-sections -g, separate -g8.c unit.
 * Best checked result: 1404/1404 bytes, 0/27 places, first mismatch +0x64,
 * 188 of 351 rows differ (sol-high draft: +0x8, 309 rows).
 * New: `goal=base+176` assigned at the top of the scan loop (not `target`)
 * goes through gcse PRE/cprop as a copy of the target pseudo and loop.c
 * hoists it, so target lives in s3 with a preheader store, as in retail.
 * Remaining: slot order is scan 0x60, hidden 0x64, goal copy 0x68; retail
 * has goal copy 0x60, scan 0x64, hidden 0x68 (its scan/hidden look like
 * late-numbered loop.c givs, but a for(i) array-index loop spills the
 * fp+16 address of b at an extra slot). The hide path reloads hidden after
 * the numhidden update instead of incrementing it right after the store,
 * so its branch is one instruction longer. Also differs: base+... address
 * order at entry (s1/s3/s2) and later call/timer scheduling.
 * Required declarations are included below.
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
typedef struct { char pad[44]; float distance; } Move243;
typedef struct { char pad[48]; float offset; short timer; char gap[4]; short flag; char gap2[16]; short counter, locked; } State243;
void func_L00_004243C0(char *obj)
{
    char *base=P(obj,112),*target=base+176,*pos=base+96;
    Move243 *move=(Move243 *)(base+352);
    State243 *state=(State243 *)(base+496);
    char *camera=D_L00_001B8E20;
    char a[16],b[16],c[16],d[16],e[16];
    char *m,*axis;
    int count,n,blocked=0,numhidden=0,type,i;
    float distance=move->distance-state->offset+1.5f;
    float dot,t;
    unsigned short timer;
    char *goal;
    char **scan,**hidden;
    func_00282D08(a,target,camera-96,0.5f);
    count=func_00277F48(a,1,D_L00_00189E20.owner,0,distance*0.5f);
    if(count>0) {
        scan=D_L00_001C3280;
        hidden=D_L00_0025B9A0;
        n=count;
        do {
            goal=base+176;
            m=*scan;
            if(I(m,152) && (!func_L00_0031BE88(m)||S(m,170)==2923)) {
                type=S(m,170);
                if(type!=2234 && type!=3335 && type!=4304 && type!=1143 && type!=11) {
                    *hidden++=m;
                    numhidden++;
                    I(m,152)=0;
                } else {
                    func_00282CF0(b,m+16,goal);
                    axis=camera+48;
                    dot=func_00282DB0(axis,b);
                    func_00282D30(c,axis,dot);
                    func_00282CF0(d,b,c);
                    func_00282F20(d,d,1.0f);
                    dot=func_00282DB0(d,move);
                    t=move->distance-state->offset;
                    t=dot/t;
                    t=func_002831B0(t);
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
        } while(--n);
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
        if(func_00282978(&state->timer))state->flag=0;
    }
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
        char *entry;
        do {
            entry=*restore++;
            I(entry,152)=I(P(entry,36),16);
        } while(--remaining);
    }
}
