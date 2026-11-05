/* func_PAL_L00_004007B0, the PAL copy of func_L00_003FBF78: the same C, where the duration
   D_L00_001B0B60 becomes (D_L00_001B0B60 * 5 + 2) / 6.
   Tier: opus-1 (Claude Opus 5.5, xhigh), task v2718.
   Unit flags: the template's, -O2 -G8 -ffunction-sections -g, as a -g8-dbg unit of its own
   (src/pal/levels/shared/L00_004007B0-g8-dbg.c).
   Best check.py --version pal: 424/428 bytes, 0/27 places, one instruction out of place.
   What differs: `lui $s0,%hi(D_L00_001BD2C0)` lands in the slot of the first jal
   (func_00282948, +0x30). Retail has a nop there and the lui at +0x9C, right after the first
   func_00283CF0 call; everything else keeps its order, shifted one word. sched2 already
   has the lui at retail's place (insn 86); only reorg moves it.
   Cause: fill_simple_delay_slots' forward scan from that call. The division guard (trap_if)
   sets needed.volatil, which should stop the scan, but mark_referenced_resources' MEM case in
   this resource.c assigns res->volatil, so the next ordinary load (the two byte reads of
   D_L00_001B1980) clears it and the lui passes the guard.
   Measured: this exact C through compiler-lab's sticky-volatile-opt cc1 with
   -frac2-sticky-volatile-refs (the |= rule, patch 0061 on orch/main) puts a bare jal and the lui
   at retail's place, nothing else changed. That option is not in this compiler.
   Exact bytes, FORBIDDEN (invented volatile), 27 of 27 places: read the second byte of the first
   interpolation as func_00283CE0(*(volatile unsigned char *)D_L00_001B1980). No upstream C
   declares that symbol volatile. It only proves which reference matters.
   Tried: a local record pointer and a local scaled duration: the lui stays in the slot and the
   loads reorder, worse. There is no legal stop in the scan's range: retail has no other branch,
   so no label can survive, and $s0 is untouched before the lui.
   To unblock: land a compiler rule equivalent to 0061. Then the C below should need no change.
   Needs: only the declarations below. */
#include "common.h"

/* The counter pair gives a ratio; the three bytes and four floats are interpolated with it and
   stored in the record. Retail reaches the byte and float tables through the assembler's
   absolute macro form (MACRO_ADDR arrays), also when the load lands in a call's delay slot. */
extern int D_L00_001B0B64 __attribute__((sda));
extern int D_L00_001B0B60 __attribute__((sda));
extern float D_L00_001B0BB0 __attribute__((sda));

extern unsigned char D_L00_001B1980[] MACRO_ADDR;
extern unsigned char D_L00_001B1981[] MACRO_ADDR;
extern unsigned char D_L00_001B1982[] MACRO_ADDR;
extern float D_L00_001B2BE0[] MACRO_ADDR;
extern float D_L00_001B1984[] MACRO_ADDR;
extern float D_L00_001B1988[] MACRO_ADDR;
extern float D_L00_001B198C[] MACRO_ADDR;
extern float D_L00_001B1990[] MACRO_ADDR;

/* Measured fields of the record the fade writes. */
typedef struct {
    char pad_00[0x218];
    float f218, f21C;
    char pad_220[8];
    float f228, f22C;
    int i230, i234, i238;
} L00FadeRecord;
extern L00FadeRecord D_L00_001BD2C0;

extern void func_00282948(void *);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern void func_0027AB40(void);

void func_PAL_L00_004007B0(void)
{
    float ratio;

    if (D_L00_001B0B64 <= 0) {
        return;
    }
    func_00282948(&D_L00_001B0B64);
    ratio = func_00283CE0(D_L00_001B0B64) / func_00283CE0((((int)(D_L00_001B0B60) * 5 + 2) / 6));
    D_L00_001BD2C0.i230 =
        func_00283CF0(func_00283CE0(D_L00_001B1980[0]) + (255.0f - func_00283CE0(D_L00_001B1980[0])) * ratio);
    D_L00_001BD2C0.i234 =
        func_00283CF0(func_00283CE0(D_L00_001B1981[0]) + (255.0f - func_00283CE0(D_L00_001B1981[0])) * ratio);
    D_L00_001BD2C0.i238 =
        func_00283CF0(func_00283CE0(D_L00_001B1982[0]) + (255.0f - func_00283CE0(D_L00_001B1982[0])) * ratio);
    D_L00_001BD2C0.f218 = D_L00_001B1984[0] + -D_L00_001B1984[0] * ratio;
    D_L00_001BD2C0.f21C =
        D_L00_001B1988[0] + (D_L00_001B2BE0[0] * (D_L00_001B0BB0 * 1024.0f) - D_L00_001B1988[0]) * ratio;
    D_L00_001BD2C0.f228 = D_L00_001B198C[0] + (255.0f - D_L00_001B198C[0]) * ratio;
    D_L00_001BD2C0.f22C = D_L00_001B1990[0] + (100.0f - D_L00_001B1990[0]) * ratio;
    func_0027AB40();
}
