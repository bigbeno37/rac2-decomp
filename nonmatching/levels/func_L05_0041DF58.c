/*
 * Opus 5.5 xhigh (opus-2), task v2241, 3 November 2026. Builds on opus-1's v1866 draft.
 * Best check.py, USA v1.01 (level 05): 3520/3516 bytes, 0/1 places; 839 of 880 rows differ (most
 * only because the listing is shifted; v1866 had 841). Compile as an isolated -g8.c unit
 * (-O2 -G8 -ffunction-sections); the declarations it needs follow. No linked build was run.
 * Closeness by a private masked alignment (relocated halves, call targets and gp/%lo offsets
 * masked; not a check.py result): 742 of 879 retail rows line up, v1866 has 730 by the same count.
 *
 * New over v1866 (keep these):
 * - `selected = 1` as its declaration initializer (NOTE REVIEW: one initializer on a used local).
 *   The switch's `== 1` reuses that older pseudo, so the opening now matches: li 1 in s0 after
 *   sd s1, beq $v1,$s0. (A plain `int index = 1` puts the 1 in s2 and moves D/fmt instead.)
 * - if (D_L05_001A904C) closes before func_L00_0030CF60 (retail beqz at 0041E3A4 targets the
 *   jal at 0041EB5C); the three last sprites are unconditional. This gives retail's 0x8710 in the
 *   beqz delay slot and 4.0f/0.5f hoisted into f20/f23 after that call.
 * - The second sprite handle reuses `state` (L15 twin's finding): it lands in s3 as in retail.
 *
 * Still different, all from one missing construct: retail has a block boundary that leaves no
 * instruction right after each text y conversion (mfc1 $a1) and each sprite y conversion
 * (cvt.s.w $f13). Its effects: x is built in v0, then daddu $a0,$v0 after mfc1 $a1;
 * a2/a3/t0 are set after mfc1 $a1, .8f/0.0f are set after the first sprite's y (f21/f22, here
 * f22/f21 and hoisted). In case 0 D/fmt are s2/s1 in retail but s0/s1 here: something keeps the
 * 1 (s0) live through the arms, which fits a vanished `X == 1` test at that boundary using the
 * cse'd 1, with X already in a register. No real X was found.
 * Tried without effect: a helper testing the switch variable `mode == 1` (cse folds it in each
 * arm); a one-armed `if (drawX == 1) label = text;` before each draw (gone before combine, no
 * boundary; diagnostic only, not kept).
 */
#include "common.h"

typedef struct { int active; char pad[8]; int maximum, capacity, current; char gap[4]; short icon; char tail[10]; } HudItem;
typedef struct { int v[4]; } HudRect;
extern char *D_L05_001B13B0 __attribute__((sda));
extern int D_L05_001B1360 __attribute__((sda));
extern int D_L05_001B1390 __attribute__((sda));
extern int D_L05_001B1394 __attribute__((sda));
extern int D_L05_001B1398 __attribute__((sda));
extern int D_L05_001B139C __attribute__((sda));
extern int D_L05_001B13A0 __attribute__((sda));
extern int D_L05_001B13A4 __attribute__((sda));
extern int D_L05_001B13D0 __attribute__((sda));
extern int D_L05_001B13DC __attribute__((sda));
extern int D_L05_001B1428 __attribute__((sda));
extern int D_L05_001B1438 __attribute__((sda));
extern int D_L05_001B1440 __attribute__((sda));
extern char *D_L05_001A904C MACRO_ADDR;
extern int D_001A7340[] __attribute__((sda));
extern int D_L05_001B143C[] __attribute__((sda)), D_L05_001B1444[] __attribute__((sda)), D_L05_001B142C[] __attribute__((sda));
extern int D_L05_001B3550[] __attribute__((sda)), D_L05_001B2D80[] __attribute__((sda));
extern float D_L05_001B2D84[] __attribute__((sda)), D_L05_001AC4E0[] __attribute__((sda)), D_L05_001AC4E4[] __attribute__((sda));
extern char D_L05_001B1448[], D_L05_001B1450[], D_L05_001B1498[];
extern HudRect D_L05_001B1468, D_L05_001B1478, D_L05_001B1488;
typedef struct { int unused[4]; int objectives; int wave; int more[4]; int health; } SpaceHudStats;
extern SpaceHudStats D_L05_00221D10;
extern HudItem D_L05_002A1800[];
extern void func_L05_00421B98(int,int,int,int);
extern char *func_00288FB8(int);
extern int func_00115DA8(char *, const char *, ...);
extern void func_0027F5B8(int,int,unsigned long,char *,int);
extern void func_0027F538(int,int,unsigned long,char *,int);
extern void func_002ED578(int,int *,int *,int *);
extern int func_00336D50(void *,int);
extern float func_00283CE0(int);
/* Integer and floating arguments preserve their independent EE ABI bank orders. */
extern void func_00300968(int,float,float,int,float,float);
extern int func_002ED8A0(int);
/* Integer arguments before the float: each EE argument bank keeps its order. */
extern int func_00283D38(int,int,float);
extern int func_00282948(int *);
extern void func_0027EAE8(int,int,int,int,int,int *);
extern int func_0028E588(int,int);
extern int func_0028EA58(int,int,int,int,int,HudRect *);
extern void func_L05_003263F8(int);
extern int func_L05_00324DB8(void);
extern void func_L00_0030CF60(void);
extern void func_L05_0037A5F8(void);

/* A screen coordinate scaled for the display; mode 1 also takes an offset in unscaled pixels. */
static __inline__ int scaled_screen_coord(int mode, float value, float offset)
{
    float scaled = value * D_L05_001B2D84[0] + 0.5f;
    if (mode == 1)
        return (int)(scaled + offset);
    return (int)scaled;
}
int func_L05_0041DF58(void) {
    char text[128];
    int colors[4], colors2[4];
    HudRect r0, r1, r2;
    int h,m,s;
    int drawX, drawY;
    char *label;
    char *state = *(char **)(D_L05_001B13B0 + 0x68);
    int index, color, selected = 1;
    float x,y,ratio,a,b,health;
    float segmentColor, right;
    int i0,i1,i2;
    char *currentBase, *capacityBase;
    HudItem *items;
    { int mode = D_L05_001B1360;
    switch (mode) {
    case 0:
        func_L05_00421B98(0,1,1,1);
        label = func_00288FB8(0x31D1);
        func_00115DA8(text,D_L05_001B1448,label,D_L05_00221D10.objectives);
        drawY = (int)((D_L05_001B143C[0]-8)*D_L05_001B2D84[0]+.5f);
        drawX = D_L05_001B1438-60;
        func_0027F5B8(drawX,drawY,0x80808080UL,text,128);
        label = func_00288FB8(0x31D2);
        func_00115DA8(text,D_L05_001B1448,label,D_L05_00221D10.wave+1);
        drawY = (int)((D_L05_001B1444[0]-8)*D_L05_001B2D84[0]+.5f);
        drawX = D_L05_001B1440-60;
        func_0027F5B8(drawX,drawY,0x80808080UL,text,128);
        break;
    case 1:
        func_L05_00421B98(1,1,0,1);
        label = func_00288FB8(0x31D4);
        func_00115DA8(text,D_L05_001B1448,label,D_L05_00221D10.objectives);
        drawY = (int)((D_L05_001B1444[0]-8)*D_L05_001B2D84[0]+.5f);
        drawX = D_L05_001B1440-60;
        func_0027F5B8(drawX,drawY,0x80808080UL,text,128);
        func_002ED578(*(int *)(state+0x44),&h,&m,&s);
        label = func_00288FB8(0x2F83);
        func_00115DA8(text,D_L05_001B1450,label,h,m,s);
        drawY = (int)((D_L05_001B142C[0]-8)*D_L05_001B2D84[0]+.5f);
        drawX = D_L05_001B1428-80;
        func_0027F5B8(drawX,drawY,0x80808080UL,text,128);
        break;
    case 2:
        func_L05_00421B98(0,1,0,1);
        label = func_00288FB8(0x31D1);
        func_00115DA8(text,D_L05_001B1448,label,D_L05_00221D10.objectives);
        drawY = (int)((D_L05_001B1444[0]-8)*D_L05_001B2D84[0]+.5f);
        drawX = D_L05_001B1440-60;
        func_0027F5B8(drawX,drawY,0x80808080UL,text,128);
        break;
    case 3:
        func_L05_00421B98(1,1,0,1);
        label = func_00288FB8(0x31D7);
        func_00115DA8(text,D_L05_001B1448,label,D_L05_001B13D0);
        drawY = (int)((D_L05_001B1444[0]-8)*D_L05_001B2D84[0]+.5f);
        drawX = D_L05_001B1440-60;
        func_0027F5B8(drawX,drawY,0x80808080UL,text,128);
        func_002ED578(D_L05_001B3550[0],&h,&m,&s);
        label = func_00288FB8(0x2F83);
        func_00115DA8(text,D_L05_001B1450,label,h,m,s);
        drawY = (int)((D_L05_001B142C[0]-8)*D_L05_001B2D84[0]+.5f);
        drawX = D_L05_001B1428-80;
        func_0027F5B8(drawX,drawY,0x80808080UL,text,128);
        break;
    }
    }
    if (D_L05_001A904C) {
        D_L05_001B1390=func_00336D50(D_L05_001A904C+0x8710,128);
        D_L05_001B1394=func_00336D50(D_L05_001A904C+0x8710,129);
        D_L05_001B1398=func_00336D50(D_L05_001A904C+0x8710,130);
        D_L05_001B139C=func_00336D50(D_L05_001A904C+0x8710,131);
        D_L05_001B13A0=func_00336D50(D_L05_001A904C+0x8710,132);
        D_L05_001B13A4=func_00336D50(D_L05_001A904C+0x8710,133);
        x=func_00283CE0(D_001A7340[0])*.5f;
        y=scaled_screen_coord(D_L05_001B2D80[0],200.0f,4.0f);
        func_00300968(D_L05_001B1390,x,y,0x60442D00,1.0f,0.0f);
        func_00300968(D_L05_001B1394,x,y,0x55F0C070,1.0f,0.0f);
        index=func_002ED8A0(6);
        ratio=func_00283CE0(D_L05_002A1800[index].current);
        ratio/=func_00283CE0(D_L05_002A1800[index].maximum);
        switch(index) {
        case 6:
            if(ratio>0.0f) {
                segmentColor=(float)func_00283D38(0x800000FF,0x8000FF00,ratio);
                func_00300968(D_L05_001B139C,x,y,segmentColor,1.0f,0.0f);
            }
            break;
        case 12:
            if(ratio>.5f) {
                ratio-=.5f;
                segmentColor=(float)func_00283D38(0x800000FF,0x8000FF00,ratio+ratio);
                func_00300968(D_L05_001B13A0,x,y,segmentColor,1.0f,0.0f);
                func_00300968(D_L05_001B139C,x,y,0x8000FF00,1.0f,0.0f);
            } else if(ratio>0.0f) {
                segmentColor=(float)func_00283D38(0x800000FF,0x8000FF00,ratio+ratio);
                func_00300968(D_L05_001B139C,x,y,segmentColor,1.0f,0.0f);
            }
            break;
        case 14:
            if(ratio>0.6666667f) {
                ratio-=0.6666667f;
                segmentColor=(float)func_00283D38(0x800000FF,0x8000FF00,ratio*3.0f);
                func_00300968(D_L05_001B13A4,x,y,segmentColor,1.0f,0.0f);
                func_00300968(D_L05_001B13A0,x,y,0x8000FF00,1.0f,0.0f);
                func_00300968(D_L05_001B139C,x,y,0x8000FF00,1.0f,0.0f);
            } else if(ratio>0.33333334f) {
                ratio-=0.33333334f;
                segmentColor=(float)func_00283D38(0x800000FF,0x8000FF00,ratio*3.0f);
                func_00300968(D_L05_001B13A0,x,y,segmentColor,1.0f,0.0f);
                func_00300968(D_L05_001B139C,x,y,0x8000FF00,1.0f,0.0f);
            } else if(ratio>0.0f) {
                segmentColor=(float)func_00283D38(0x800000FF,0x8000FF00,ratio*3.0f);
                func_00300968(D_L05_001B139C,x,y,segmentColor,1.0f,0.0f);
            }
            break;
        }
        health=D_L05_00221D10.health*.01f;
        if(health>.3f) *(int *)(state+0x38)=1;
        else if(func_00282948((int *)(state+0x34))) {
            *(int *)(state+0x34)=5;
            *(int *)(state+0x38)^=1;
        }
        colors[1]=func_00283D38(0x800000FF,0x8000FF00,health);
        colors[0]=0x800000FF;
        right=health*200.0f+156.0f;
        if(*(int *)(state+0x38)) func_0027EAE8(156,14,(int)right,27,1,colors);
        index=func_002ED8A0(7);
        items=D_L05_002A1800;
        currentBase=(char *)&items[0].current;
        capacityBase=(char *)&items[0].capacity;
        ratio=func_00283CE0(*(int *)(currentBase + index*sizeof(HudItem)));
        health=ratio/func_00283CE0(*(int *)(capacityBase + index*sizeof(HudItem)));
        color=func_00283D38(0x800000FF,0x8000FF00,health);
        a=func_00283CE0(450);
        b=func_00283CE0(386);
        b-=func_00283CE0(450);
        colors2[0]=0x800000FF; colors2[1]=color;
        b*=health;
        right=a+b;
        if(health>0.0f) func_0027EAE8(450,8,(int)right,19,1,colors2);
        r0=D_L05_001B1468; r1=D_L05_001B1478; r2=D_L05_001B1488;
        func_0028EA58(func_0028E588(0xEA97,1),21,7,30,30,&r1);
        func_0028EA58(func_0028E588(0xEA97,0),21,7,30,30,&r0);
        func_0028EA58(func_0028E588(0xEA97,items[func_002ED8A0(D_L05_001B13DC)].icon),21,7,30,30,&r2);
        func_0028EA58(func_0028E588(0xEA97,1),462,9,30,30,&r1);
        func_0028EA58(func_0028E588(0xEA97,items[func_002ED8A0(7)].icon),462,9,30,30,&r2);
        func_00115DA8(text,D_L05_001B1498,*(int *)(currentBase + sizeof(HudItem)*func_002ED8A0(D_L05_001B13DC)),*(int *)(capacityBase + sizeof(HudItem)*func_002ED8A0(D_L05_001B13DC)));
        func_0027F538(67,5,0x80808080UL,text,128);
        func_L05_003263F8(2);
        selected=func_L05_00324DB8();
        if(selected>=0) {
            items[func_002ED8A0(D_L05_001B13DC)].active=0;
            D_L05_001B13DC=selected;
            items[func_002ED8A0(selected)].active=1;
        }
        func_00300968(D_L05_001B1398,x,y,0x80F0F0F0,1.0f,0.0f);
    }
    func_L00_0030CF60();
    {
        i0=func_00336D50(D_L05_001A904C+0x8710,186);
        state=(char *)func_00336D50(D_L05_001A904C+0x8710,187);
        i2=func_00336D50(D_L05_001A904C+0x8710,188);
        func_00300968(i0,D_L05_001AC4E0[0]+4.0f,(float)(int)((D_L05_001AC4E4[0]+4.0f)*D_L05_001B2D84[0]+.5f),0x60442D00,.8f,0.0f);
        func_00300968((int)state,D_L05_001AC4E0[0]+4.0f,(float)(int)((D_L05_001AC4E4[0]+4.0f)*D_L05_001B2D84[0]+.5f),0x60241700,.8f,0.0f);
        func_00300968(i2,D_L05_001AC4E0[0]+4.0f,(float)(int)((D_L05_001AC4E4[0]+4.0f)*D_L05_001B2D84[0]+.5f),0x55F0C070,.8f,0.0f);
        func_L05_0037A5F8();
    }
    return 1;
}
