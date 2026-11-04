/*
 * Claude Opus 5.5 (opus-2), task v2454, 2026-11-05. USA v1.01, game -O2 -G8
 * -ffunction-sections (new -g8.c unit, default flags).
 * DIAGNOSTIC, NOT LANDABLE: 1316/1300 bytes, 0 of 1 places, about 177 rows
 * differ (the earlier legitimate draft was 1304 bytes, 250 of 326 rows).
 * The `mode` passed to ui_y is the placeholder `state`, which is not a real
 * display-mode value.
 * Finding: retail has a block boundary that leaves no code right after every
 * screen-scaled y's add.s. ui_y's "float first, then test" form reproduces
 * all seven draw blocks: y in s1, style in s0, the text copy mixed into the
 * conversion, cvt.w.s f1,f0. A "test first" form (whole conversion in each
 * arm) matches the buf block but keeps the text copy out of the conversion.
 * A "test after (int)" form folds before sched1. A by-value int argument adds
 * lw+mtc1; read the global through a pointer so lwc1 stays inside.
 * Header: with ui_y on the height only, the trailing argument loads
 * (a3,t0,t1,s4,t3) match retail. x and y then interleave in sched1. With ui_y
 * on all three, scale and 0.5 are reloaded each time (1348 bytes). Retail
 * loads them once and serialises x/y with cvt temp f3, so x/y look like one
 * block where all pseudos have one death each.
 * Still differs: the placeholder keeps `state` live past the loop (s6 instead
 * of retail's s3..s5 bases); the last three blocks need 0.5 shared in f20
 * (retail), which arm labels stop cse from sharing (plain expressions there
 * restore f20, but style then loads early).
 * Next: find the real register-resident test value (it must already be in a
 * register and must not be live through the loop), e.g. the original
 * menu-layout helper with a mode argument.
 */
#include "common.h"
extern void func_L02_003FDDE8(char *,int);
extern int func_L02_003FE1B0(int), func_L02_003F9B70(int), func_L02_003FCDF8(int);
extern char *func_00288FB8(int);
extern int func_001157AC(char *);
extern int func_00115DA8(char *,char *,...);
extern void func_002802E0(void *,int,int,int,int,int,int,int,int);
extern void func_00280270(void *,u64,char *,int);
extern void func_0027F538(int,int,u64,char *,int);
extern void func_0027F5B8(int,int,u64,char *,int);
extern float D_L02_001B3E04;
extern int D_L02_001B12A4, D_L02_001B12A8, D_L02_001B12AC;
extern char D_L02_001B1228[], D_L02_001B1328[];
extern u8 D_001395B8[], D_00139568[];
extern char *D_L02_001B1190[] SDATA(D_L02_001B1190);
typedef struct { char pad[4]; u8 enabled; char rest[0xDB]; } MenuRec;
extern MenuRec D_L02_00264A70[];
extern int func_00288D58(void);
extern int D_L02_001B1294 __attribute__((sda));
extern int D_L02_001B1298 __attribute__((sda));
extern int D_L02_001B129C __attribute__((sda));
extern int D_L02_001B12A0 __attribute__((sda));
extern int D_L02_001B12B0 __attribute__((sda));
extern int D_L02_001B12B8 __attribute__((sda));
extern int D_L02_001B12BC __attribute__((sda));
extern int D_L02_001B12C0 __attribute__((sda));
extern int D_L02_001B12C8 __attribute__((sda));
extern int D_L02_001B12CC __attribute__((sda));
extern int D_L02_001B12D0 __attribute__((sda));
extern int D_L02_001B12D8 __attribute__((sda));
extern int D_L02_001B12DC __attribute__((sda));
extern int D_L02_001B12E0 __attribute__((sda));
extern int D_L02_001B12E8 __attribute__((sda));
extern int D_L02_001B12EC __attribute__((sda));
extern int D_L02_001B12F0 __attribute__((sda));
extern int D_L02_001B12F8 __attribute__((sda));
extern int D_L02_001B12FC __attribute__((sda));
extern int D_L02_001B1300 __attribute__((sda));
extern int D_L02_001B1308 __attribute__((sda));
extern int D_L02_001B130C __attribute__((sda));
extern int D_L02_001B1310 __attribute__((sda));
extern int D_L02_001B1318 __attribute__((sda));
extern int D_L02_001B131C __attribute__((sda));
extern int D_L02_001B1320 __attribute__((sda));
static __inline__ int ui_y(char *mode, int *v, int adjust)
{
    float f = (float)*v * D_L02_001B3E04 + 0.5f;
    if (mode != 0)
        return (int)f + adjust;
    return (int)f;
}
void func_L02_003FE250(char *obj) {
    char box[32];
    char buf[64];
    char *state;
    char *text, *suffix;
    int value, i;
    int x,y,height;
    float scaled;
    MenuRec *r;
    func_L02_003FDDE8(obj,1);
    state=*(char **)(obj+0x68);
    text=func_00288FB8(func_L02_003FE1B0(*(short *)(state+0x118)));
    x = (int)((float)D_L02_001B12A0 * D_L02_001B3E04 + 0.5f);
    y = (int)((float)D_L02_001B12A4 * D_L02_001B3E04 + 0.5f);
    height = ui_y(state, &D_L02_001B129C, 0);
    func_002802E0(box,x,y,D_L02_001B12A8,D_L02_001B12AC,D_L02_001B1298,height,D_L02_001B12B0,3);
    { u64 style=D_L02_001B1294; int length=func_001157AC(text); func_00280270(box,style,text,length); }
    if (*(short *)(state+0x118)!=0 || D_001395B8[func_L02_003F9B70(0)]) {
        text=func_00288FB8(0x2F44);
        suffix=func_00288FB8(0x2F5E);
        value=func_L02_003FCDF8(*(short *)(state+0x118));
        if(value<10000) func_00115DA8(buf,D_L02_001B1228,text,value,suffix);
        else func_00115DA8(buf,D_L02_001B1328,text,value);
{ int y=ui_y(state,&D_L02_001B12BC,0); u64 style=D_L02_001B12C0; int length=func_001157AC(buf); func_0027F5B8(D_L02_001B12B8,y,style,buf,length); }
    }
    if(D_001395B8[func_L02_003F9B70(*(short *)(state+0x118))]) {
        text=func_00288FB8(0x2F3E);
        func_00115DA8(buf,D_L02_001B1328,text,D_001395B8[func_L02_003F9B70(*(short *)(state+0x118))]);
{ int y=ui_y(state,&D_L02_001B12EC,0); u64 style=D_L02_001B12F0; int length=func_001157AC(buf); func_0027F5B8(D_L02_001B12E8,y,style,buf,length); }
    }
    if (*(short *)(state+0x118)!=0) {
        text=func_00288FB8(0x2F40);
{ int y=ui_y(state,&D_L02_001B12CC,0); u64 style=D_L02_001B12D0; int length=func_001157AC(text); func_0027F538(D_L02_001B12C8,y,style,text,length); }
    }
    for(i=*(short *)(state+0x118)+1;i<14;i++) {
        if(!D_001395B8[func_L02_003F9B70(*(int *)(D_L02_001B1190[i]+8))]) continue;
        if(i>=12 && !func_00288D58()) continue;
        if(i==7) { r=&D_L02_00264A70[D_00139568[0x1E]]; if(!r->enabled) continue; }
        if(i==8) { r=&D_L02_00264A70[D_00139568[0x2A]]; if(!r->enabled) continue; }
        text=func_00288FB8(0x2F41);
{ int y=ui_y(state,&D_L02_001B12DC,0); u64 style=D_L02_001B12E0; int length=func_001157AC(text); func_0027F538(D_L02_001B12D8,y,style,text,length); }
        break;
    }
    text=func_00288FB8(12095);
{ int y=ui_y(state,&D_L02_001B12FC,0); u64 style=D_L02_001B1300; int length=func_001157AC(text); func_0027F538(D_L02_001B12F8,y,style,text,length); }
    text=func_00288FB8(12098);
{ int y=ui_y(state,&D_L02_001B131C,0); u64 style=D_L02_001B1320; int length=func_001157AC(text); func_0027F538(D_L02_001B1318,y,style,text,length); }
    text=func_00288FB8(12099);
{ int y=ui_y(state,&D_L02_001B130C,0); u64 style=D_L02_001B1310; int length=func_001157AC(text); func_0027F538(D_L02_001B1308,y,style,text,length); }
}
