/* opus-2 (Claude Opus 5.5, v2612) retained draft, USA v1.01, game compiler -O2 -G8.
 * Belongs in src/levels/25_wupash_nebula/L25_00373ED0-g8.c, after func_L25_00377608
 * (it calls the three same-file voids defined there; the file then needs
 * #include "common.h" in place of its own u8/s32/f32 typedefs).
 * check.py: 1128/1128 bytes, 0 of 1 places; 3 instructions differ, all the same cause:
 * the second quadword clear's reload register is v0, retail v1 (+0x304/+0x358/+0x35C).
 *
 * What changed against opus-1's draft (same bytes, more natural source):
 * - No `int active = 1` at entry and no re-set. `s2` at entry is the switch's compare
 *   constant: case 0 and case 3 write literal 1s and cse replaces them with that pseudo
 *   (case 3's use crosses a call, hence the saved register). `active = 1` before
 *   func_L05_00379B60 is the same `one`/`on` as L05, L10 and L15.
 * - A single use of `active` is sunk by update_equiv_regs (retail keeps the addiu at
 *   +0x158). The sink needs REG_N_REFS == 2, depth == 0 and INSNs at both ends, and flow
 *   weights refs by loop depth, so the one-pass group around the store keeps it. In
 *   L05/L10/L15 `on` has several uses; a second real use here would show as `s2`, and
 *   retail has only the one, so the original form of this is still unknown.
 *
 * The remaining difference, measured from reload1.c and the .greg "Spilling reg" lines:
 * spills here are insn fix_truncsfsi2 (reg 2, reg 33) and the four TImode zero stores
 * (reg 2). The round-robin order fits retail exactly if $v1 is in used_spill_regs and
 * no GPR reload sits between the fix and the first clear: scratch v0, f1; clear a v0,
 * inherited; clear c v1, inherited. So retail has one more GPR need, somewhere else, at
 * an insn where a pseudo or hard reg occupies $v0 and $v1 is free
 * (order_regs_for_reload: uses == 0 first, in REG_ALLOC_ORDER).
 * Ruled out by reading the code and dumps, so do not repeat:
 * - A pseudo left without a hard reg (the REG_EQUIV reload lead for case 3): that happens
 *   in func_L00_003B14D0 only because s0-s7 and fp are all taken; here s5+ are free.
 * - A constraint reload elsewhere: only a TImode zero store survives to reload with a
 *   constant source. Stores of other constants are refused by the mov patterns' condition.
 * - An occupant of $v0 at a clear or at the fix: retail has no v0 write or read in either
 *   block, and a value cannot cross the calls around them. In matched func_L04_003F9C60,
 *   which also goes por v0 then por v1, the occupant is a visible call value in $v0.
 * - qcopy's inputs: both are pseudos, the asm needs no reload; -frac2-qcopy declares
 *   $v0 as a fixed clobber, so it adds no need either (read, not compiled).
 * - Cases 0 and 3 written with a ctx pseudo (opus-2, v2478): worse.
 * Open: a hidden need in code deleted after reload, the only places left being insns
 * that flow2 or jump2 remove. The empty wait loop is the obvious suspect, since $v0 is
 * live there and $v1 free, but no body that survives flow1 and dies later could be
 * thought of; this was reasoned, no such body was compiled.
 * Same bytes: L05's `typedef union { TI128 q; float f[4]; } Vector;` for the clears.
 */
#include "common.h"
extern void func_L25_003752D8(u8 *);
extern void func_L25_00376FE0(u8 *, f32);
extern void func_L25_00377608(u8 *, f32);

typedef unsigned int Quad __attribute__((mode(TI)));

typedef struct MissionContext {
    float *health;
    char pad04[0xC];
    int paused;
    int completed;
    char pad18[0xC];
    int enabled;
    int pad28;
    int timerVisible;
    float scale;
    char pad34[0x20];
    int (*deathCallback)(char *, int);
    char pad58[0x20];
    char *owner;
} MissionContext;
typedef struct InventoryView {
    char pad00[0x51c];
    unsigned short dialogue;
    char pad51e[0x186];
    unsigned int completed;
} InventoryView;
extern MissionContext D_L25_00221850;
extern InventoryView D_0018C958;
extern char *D_L25_001B0CE0 __attribute__((sda));
extern int D_L25_001B0CD0 __attribute__((sda)), D_L25_001B0CD4 __attribute__((sda));
extern int D_L25_001B0CD8 __attribute__((sda)), D_L25_001B0CDC __attribute__((sda));
extern int D_L25_001B0CC0 __attribute__((sda)), D_L25_001B0CB0 __attribute__((sda));
extern int D_L25_001B0CF8 __attribute__((sda));
extern char *D_0018C0B0 __attribute__((nosda));

extern int D_L25_001B3060 MACRO_ADDR, D_L25_001B1BC8 MACRO_ADDR;
extern int D_L25_001B209C MACRO_ADDR;
extern int D_00138324 __attribute__((nosda));
extern char *D_L25_001A904C MACRO_ADDR;
extern float D_L25_001B2904 MACRO_ADDR;
extern unsigned char D_001A7C01 MACRO_ADDR;
extern int D_001A79F0 MACRO_ADDR;
extern int func_L25_0040DCD0(char *,int);
extern void func_00288E18(int,int);
extern int func_00282948(void *);
extern void func_L05_00379B60(void);
extern void func_L05_0030C930(int,int);
extern int func_002E5DA0(int,int,char *,int);
extern int func_L25_0040D208(void);
extern void func_0029B758(int,void *);
extern void func_L05_00442828(void *,void *,int);
extern void func_L05_00445B58(void *,void *);
extern void func_L05_0037B210(void);
extern void func_00288CB0(int,int);
extern void func_L05_0037B108(void);
extern int func_L05_00442860(void);
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define B(p,o) (*(unsigned char *)((char *)(p)+(o)))
#define H(p,o) (*(unsigned short *)((char *)(p)+(o)))



void func_L25_0040CDA0(char *obj)
{
    char *state = *(char **)(obj+0x68);
    float a[4] __attribute__((aligned(16)));
    float b[4] __attribute__((aligned(16)));
    float c[4] __attribute__((aligned(16)));
    float d[4] __attribute__((aligned(16)));
    int i;
    int active;
    MissionContext *ctx;
    switch (B(obj,0x20)) {
    case 0:
        ctx = &D_L25_00221850;

        ctx->scale=8.0f;
        ctx->deathCallback=func_L25_0040DCD0;
        D_L25_001B0CE0=obj;
        D_L25_001B0CD0=0; D_L25_001B0CD4=0;
        D_L25_001B0CD8=0; D_L25_001B0CDC=0;
        H(obj,0x32)=0x1ff; B(obj,0x30)=0xff; B(obj,0x20)=1;
        I(state,0x2c)=150; I(state,0x30)=300;
        I(state,0x38)=180; I(state,0x34)=720; I(state,0x24)=0;
        ctx->health=*(float **)state;
        ctx->enabled=1;
        D_L25_001B3060=540;
        ctx->owner=obj;
        if (D_0018C0B0) H(D_0018C0B0,0x34)|=0x41;
    case 1:

        if (!D_0018C958.dialogue) func_00288E18(0xa16,0x6d);

        if (func_00282948(state+0x34)) D_L25_00221850.timerVisible=0;
        else D_L25_00221850.timerVisible=1;
        active = 1;
        func_L05_00379B60();
        func_L05_0030C930((int)(D_L25_001B2904*-224.0f+0.5f),2);
        qcopy(obj+0x10,(void *)((I(state,0x20)<<8) + D_L25_001B209C + 0x10));
        if (D_00138324&0x10) func_002E5DA0(0,0,obj,0xeee);
        if (D_L25_001A904C && !D_L25_001B0CDC) {
            func_0029B758(0,func_L25_0040D208);
            do { D_L25_001B0CDC=active; } while (0);
        }
        for (i=6;i>=0;i--) {}
        if (D_L25_00221850.paused || D_L25_001B1BC8<6) break;
        if (D_L25_00221850.completed<=0) {
            D_L25_00221850.timerVisible=1;
            if (!func_00282948(state+0x2c)) break;
            I(state,0x2c)=540;
            func_L05_00442828(&D_L25_001B0CC0,&D_L25_001B0CB0,240);
            *(Quad *)a=0; *(Quad *)b=0;
            B(obj,0x20)=3;
            a[0]=359.0f; a[1]=440.0f; a[2]=524.0f;
            b[0]=403.0f; b[1]=456.0f; b[2]=524.0f;
            func_L25_003752D8((u8 *)1);
            func_L25_00376FE0((u8 *)a,10.0f); func_L25_00377608((u8 *)b,10.0f);
            *(Quad *)d = *(Quad *)c = 0;
            c[0]=417.18f; c[1]=454.03f; c[2]=532.6f;
            d[0]=420.93f; d[1]=413.79f; d[2]=534.54f;
            func_L05_00445B58(c,d);
        } else {
            if (func_00282948(state+0x2c)) {
                func_L05_0037B210(); func_0029B758(0,0);
                D_001A7C01=1; func_00288CB0(2,1);
            } else {
                if (!D_L25_001B0CF8) {
                    func_L05_0037B108(); D_L25_001B0CF8=1;
                }
                { u32 seen = D_0018C958.completed & (1 << D_001A79F0);
                if (!seen) func_00288E18(0xa36,0x8d); }
            }
        }
        break;
    case 2:
        break;
    case 3:

        if (!func_L05_00442860()) {
            D_L25_00221850.timerVisible=0;
            D_L25_00221850.completed++;
            B(obj,0x20)=1;
        }
        break;
    }
}
