#include "include_asm.h"

/* Bodies whose retail bytes reach their globals through $gp: this file is built -G8.
   Measured here: cc1 emits `sw $r,SYM` as a macro and GAS expands it. Outside a delay
   slot it always becomes `lui $at,%hi ; sw %lo($at)` (HI16/LO16) whatever the attribute;
   inside a delay slot (.set nomacro: one instruction only) it becomes the $gp-relative
   form with a GPREL16 relocation, whose symbol must therefore be named GP + the retail
   displacement. `sda` only adds `.extern SYM,size`. */

extern unsigned int D_L04_001A8ED0;

extern unsigned int D_L04_001B0908 __attribute__((sda));

void func_L04_0040AC80(void)
{
    D_L04_001A8ED0 = 2;
    D_L04_001B0908 = 1;
}

extern unsigned int D_L04_001B0910 __attribute__((sda));

void func_L04_0040AC98(void)
{
    D_L04_001B0910 = 1;
}

/* func_L04_0040AC30 reaches its globals off $gp too, but its body is not matched
   yet; its marker stays in L04_00402D90.c and the attempt is in the wave-8 drafts. */

/* 0x001A8ED0 is absolute in func_L04_0040AC80 and $gp-relative here, so it is
   reached under a second name, declared small data.  The store to 0x001A8EE4 is
   written last in the source: cc1 emits the last store of a straight-line run
   first and then the source order, which is the order retail has. */
extern unsigned int D_L04_001A8ED0_1 __attribute__((sda));

extern unsigned int D_L04_001A8EE4;

void func_L04_0040ACA8(void)
{
    D_L04_001A8EE4 = 1;
    D_L04_001A8ED0_1 = 2;
    D_L04_001B0908 = 2;
}

/* The record func_L04_004044B8 walks: the fields it uses, at the retail's offsets. */
typedef struct {
    unsigned char pad0[0x84];
    int f84;
    int f88;
    char *f8c;
    unsigned char pad1[0x15C - 0x90];
    int f15c;
    int f160;
    unsigned char *f164;
    unsigned char pad2[0x1A0 - 0x168];
    int f1a0;
    unsigned char pad3[0x1B0 - 0x1A4];
    float f1b0;
    unsigned char pad3b[4];
    int f1b8;
    unsigned char pad4[0x2D0 - 0x1BC];
    float f2d0;
} L04_004044B8_Record;

extern int func_002ACBC8(void *, int);

extern void func_002A7470(char *, int, int, int);

/* 0x001A7B90 is the boot's resident data, reached off $gp in the delay slot. */
extern int D_001A7B90 __attribute__((sda));

extern int D_L04_001A8FE8;

/* Updates the record's flag from the two point tests, prints the message when the
   record has one and its counter runs, then advances the counter and the value. */
void func_L04_004044B8(void *a0, L04_004044B8_Record *s0)
{
    char *s1 = (char *)a0 + 0x10;
    int v;

    if (func_002ACBC8(s1, s0->f88)) {
        s0->f15c = 1;
    }
    if (!func_002ACBC8(s1, s0->f84)) {
        return;
    }
    if (s0->f15c == 0) {
        return;
    }
    v = s0->f160;
    if (s0->f8c && v > 0) {
        func_002A7470(s0->f8c, 9, 0, 0xC);
        v = s0->f160;
    }
    if (v < 4) {
        float f1 = (float)s0->f1b8;
        float f2;
        float f0;

        if (D_001A7B90 != 0) {
            f2 = f1 * 0.02f;
        } else {
            f2 = f1 * 0.016666667f;
        }
        f0 = s0->f2d0 + s0->f1b0 - f2;
        s0->f2d0 = f0;
        if (f0 < 0.0f) {
            s0->f2d0 = 0.0f;
        }
        s0->f1b8 = 0;
        s0->f15c = 0;
        s0->f160 = s0->f160 + 1;
        if (s0->f160 != 3) {
            return;
        }
        if (*(unsigned char *)(s0->f164 + 0x20) == 2) {
            s0->f1a0 = D_L04_001A8FE8;
        }
    }
}

#include "common.h"
#define I(p,o) (*(int *)((p)+(o)))
#define F(p,o) (*(float *)((p)+(o)))
#define P(p,o) (*(unsigned char **)((p)+(o)))
#define H(p,o) (*(short *)((p)+(o)))
typedef unsigned int AttackQuad __attribute__((mode(TI)));
typedef struct { int v[5]; } AttackChoices;
extern AttackChoices D_L04_001B0A10;
extern struct { unsigned char pad[0x80]; float position[4]; unsigned char pad90[0x2290-0x90]; unsigned char *hero; } D_00189E20;
#define D_00189F00 ((unsigned char *)&D_00189E20 + 0xe0)
#define D_L04_0018C0B0 D_00189E20.hero
typedef struct {
 unsigned char pad0[0x260]; int triggerList;
 unsigned char pad264[0x10]; short elevated;
 unsigned char pad276[6]; float height;
 unsigned char pad280[4]; float tilt;
 unsigned char pad288[8]; float offset[4];
 unsigned char pad2a0[0x6c]; float length;
 unsigned char pad310[0x50]; int state, duration, timer;
 unsigned char pad36c[0x16c]; int alternate, alternateTimer;
 unsigned char pad4e0[0x284]; int cooldown, ready;
} AttackState;
typedef struct { int unused, flags; unsigned char pad8[0x38]; } AttackSettings;
extern AttackSettings D_L04_002C63C0[];

extern unsigned char *D_L04_001C3B40[];
extern int D_L04_001B090C __attribute__((sda));
extern int D_00138320 NOT_SDA,D_00138324 NOT_SDA;
extern void func_00282D30(void *,void *,float);
extern void func_00282CC0(void *,void *,void *);
extern int func_00277F48(void *,float,int,void *,int);
extern int func_002ABB78(void *);
extern void func_002A8F70(void *,void *,int,void *,void *,float);
extern void func_L04_0040E460(void *);
extern int func_002A8600(void *,int,int,int);
extern int func_002A86E8(void *,void *,int,int);
extern void func_00282CF0(void *,void *,void *);
extern float func_L00_002EE290(void *);
extern int func_002A7820(int,int);
extern int func_002E5DA0(int,int,void *,int);
extern void func_L04_0040C0C0(void *,void *,int,int);
extern int func_00282948(void *);
extern void func_0029FCC0(void *,int,void *);
extern void func_002AF000(void *,float,float,float);
extern void func_00282CD8(void *,void *,void *);
extern void func_L04_003F9320(void *,void *,void *,int);
extern int func_002E5CF8(int,int,void *);
extern unsigned char *func_L04_0040EC80(void *,void *,void *,void *,void *,void *,float,float);
extern int func_L00_002D6900(int);
extern void * func_L04_003FA5A8(void *,void *,void *,float,float,float,float,float);
extern void * func_L04_003FBD80(void *,void *);
