/*
 * Tier: Claude Opus 5.5, xhigh effort; USA v1.01 game compiler, -O2 -G8
 * -ffunction-sections (put it in a file ending in -g8.c for the -G8 profile).
 * Best check.py: 1468/1476 bytes, 0/27 places (about 15 compiles this task).
 * Matches now: D_L00_00262570 read as a 0xe0-byte struct array (mult/addu/lwc1
 * 0x54); one list[i] load per use via qcopy_nc in the loop; active=1 after
 * the query call; reaction test (p && st==1) || st==2 gives retail's beqz p /
 * lbu-in-slot / beql / andi 0xff / bne; constants 7/2/1 held in saved regs.
 * Missing 8 bytes:
 *  1. Loop guard: retail tests a separate zero register (s1, set before the
 *     three calls: slt s1,s1,v0; beqz) while the counter is s6=0 in a jal
 *     slot. Here one i=0 is shared and combine folds the guard into blez.
 *     A second zero local (n=0) keeps the guard on n through cse1/gcse/loop,
 *     but gcse cprop turns its later uses into 0, so cse2 makes i canonical
 *     again (make_regs_eqv last-use rule) and the fold returns.
 *  2. Retail has a nop between lwc1 $f0,0x54 and div.s. gcc only prints
 *     #nop comments, so this nop comes from the assembler; the cause is unknown.
 * Saved registers also differ: retail o=s4, q=s3, o+16=s5, plus prologue order.
 * All required declarations follow.
 */
#include "common.h"
extern void func_L00_00409420(char *);
extern void ref_L00_00408E58_00(int,int);
extern void func_002A75E0(char *,int,int,int,int);
extern void func_00282CC0(void *,void *,void *);
extern void func_002829D8(void *);
extern void func_00282A88(void *,int);
extern int func_00282948(void *);
extern int func_L00_002D4040(int);
extern void func_L00_002AF568(void);
extern int func_L00_002D6998(void *);
extern void func_002E5CF8(int,int,char *);
extern void func_00282F20(void *,void *,float);
extern void func_L00_00416F68(void *,int,char *);
extern void func_002E6EF8(void (*)(char *),char *);
extern void func_L00_00409480(char *);
extern void func_L00_0037E000(char *);
extern void func_002AF580(void *,void *,int *,void *,int,int,float,float,float,float,float,int,int);
extern char *func_002AB1F0(char *);
extern void func_002A6C58(char *,int,int);
extern void func_00310440(char *);
extern void func_L00_002D3F30(int,int);
extern struct {
    char pad0[0x1230]; int mask;
    char pad1234[0xc]; int level;
    char pad1244[0x104c]; char *object; int state;
    char pad2298[0x24]; unsigned char disabled;
} D_00189E20;
extern int D_L00_001A8F00 __attribute__((nosda));
extern int D_00138324 __attribute__((nosda));
extern int D_L00_001B0F90 __attribute__((sda));
extern unsigned char D_00139593 __attribute__((nosda));
extern struct { char pad[0x54]; float f54; char pad58[0xe0-0x58]; } D_L00_00262570[];
extern float D_L00_0025C64C __attribute__((nosda));
extern char D_L00_0029FBE0[];
extern char D_L00_001B8FF0[];
#define I(p,n) (*(int *)((char *)(p)+(n)))
#define F(p,n) (*(float *)((char *)(p)+(n)))
#define B(p,n) (*(unsigned char *)((char *)(p)+(n)))
void func_L00_00408E58(char *o)
{
    char *q = *(char **)(o+104);
    struct {
        int base[4] __attribute__((aligned(16)));
        int v[4] __attribute__((aligned(16)));
        int out[4] __attribute__((aligned(16)));
        char *list[32];
        int count;
        int *timer;
    } frame;
#define base frame.base
#define v frame.v
#define out frame.out
#define list frame.list
#define count frame.count
#define timer frame.timer
    int i;
    func_L00_00409420(o);
    if (D_00189E20.state==1) ref_L00_00408E58_00(29,1);
    if(B(o,0x31)) D_L00_0025C64C=0.1f;
    if(B(o,0x42)==B(o,0x43) && (B(o,0x60)&2)) func_002A75E0(o,1,0,2,0);
    func_00282CC0(base,base,o+16);
    func_002829D8(q);
    if(B(o,32)==1) {
        if(D_L00_001A8F00 || D_00189E20.disabled || !(D_00138324&D_00189E20.mask) || D_00189E20.level<23) return;
        if(func_L00_002D4040(-1)) {
            func_L00_002AF568();
            I(q,24)=func_L00_002D6998(D_L00_0029FBE0);
            B(o,32)=2;
            I(q,4)=D_L00_001B0F90;
        } else func_002E5CF8(4,0,o);
        F(q,0)=1.0f/D_L00_00262570[D_00139593].f54;
    } else if(B(o,32)==2) {
        if(func_00282948(q+16)) { B(o,32)=4; I(q,16)=30; I(q,20)=0; }
        if(I(q,16)==10) {
            func_002A75E0(o,2,0,3,0);
            func_002E5CF8(1,0,o);
        }
    } else if(B(o,32)==4) {
        if(func_00282948(q+16)) {
            B(o,32)=5; I(q,20)=0; I(q,16)=30;
            func_002A75E0(o,3,0,3,0);
        } else if(!I(q,20)) {
            qcopy(v,D_00189E20.object+0xc0);
            func_00282F20(v,v,1.0f);
            func_00282CC0(out,base,v);
            func_L00_00416F68(out,1,o);
            I(q,20)=1;
        }
    } else if(B(o,32)==5) {
        if(func_00282948(q+8)) {
            B(o,32)=3; I(q,8)=64;
            func_002A75E0(o,4,0,3,0);
        } else func_002E6EF8(func_L00_00409480,o);
    } else if(B(o,32)==3) {
        i=0;
        qcopy(v,D_L00_001B8FF0);
        timer=(int *)(q+4);
        func_00282A88(list,128);
        func_L00_0037E000(o);
        func_002AF580(o+16,v,&count,list,32,0,3.1415929794311523f,1.047197699546814f,40.0f,0.0f,0.0f,0,0);
        for(;i<count;i++) {
            char *p;
            int active;
            qcopy_nc(out,list[i]+16);
            p=func_002AB1F0(list[i]);
            active=1;
            if(p) {
                F(out,8)+=F(p,16)*1.5f;
                if(F(p,0)>100.0f) active=0;
            }
            if(list[i] && B(list[i],32)!=254 && B(list[i],32)!=253) {
                if(*(short *)(list[i]+0xaa)==0xa8d) B(list[i],0x95)=9;
                if(*(short *)(list[i]+0xaa)==0xa8e) B(list[i],0x95)=7;
                if(*(short *)(list[i]+0xaa)==0x1002) B(list[i],32)=7;
            }
            if(active) {
                if((p && B(p,14)==1) || B(p,14)==2) {
                    func_L00_00416F68(out,0,0);
                    I(q,28)++;
                    func_002A6C58(list[i],0,3);
                    B(p,14)=2;
                } else if(p && B(p,14)!=3) {
                    func_L00_00416F68(out,0,0);
                    I(q,28)++;
                    func_002A6C58(list[i],0,3);
                    func_00310440(list[i]);
                }
            } else {
                F(p,0)-=20.0f;
                if(F(p,0)<0.0f) F(p,0)=0.0f;
            }
        }
        if(func_00282948(timer)) {
            if(I(q,28)) func_002E5CF8(2,0,o);
            else func_002E5CF8(3,0,o);
            I(q,28)=0;
            B(o,32)=0;
            func_L00_002D3F30(-1,1);
        }
    }
}
