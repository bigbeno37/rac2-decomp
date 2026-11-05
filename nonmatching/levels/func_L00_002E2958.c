/* Tier: opus-1 (v1494). USA v1.01 game compiler, own unit with
 * -O2 -G0 -ffunction-sections -frac2-keep-block-order (needed: without it the
 * prologue and dispatch order change).
 * Best check.py: 608/608 bytes, 0/27 places. Every instruction agrees except ONE:
 * retail has a hazard nop between `mtc1 a0,f0` and `cvt.s.w f0,f0` at +0x104
 * (L00 0x002E2A58/5C, the first instruction after the case-0 `beqz v0` whose delay
 * slot gas fills with a nop). This toolchain's reconstructed transfer-delay rule
 * (u4_mtc1_helper.py + restrict_mtc1_exemption.py: prev insn mtc1 $4->$f0,
 * prev-prev insn the dummy left by a gas-filled branch slot, reader cvt.s.w f0,f0)
 * omits that nop, so ours is one word short there and the aligned case-1/2 loop head
 * then takes a pad nop at +0x194 instead; past that point the bytes agree again.
 * No C wording can change it: the slot must stay a nop and mtc1 a0,f0 must follow,
 * so gas sees exactly the exempted history. Retail is a counterexample to the
 * exemption (docs/COMPILER-NOTES.md "Transfer hazards": 4 of 8 first-transfer
 * witnesses need the nop). It matches if the assembler emits that nop.
 * What fixed the rest (vs the 604-byte sol-high draft): a separate counter for the
 * case-1/2 loop (sharing `i` with the case-0 loop made it cross a call, so sched1
 * hoisted the `i = next` copy above func_L00_002E2C30 and the counter took s0) and an
 * explicit `next = j + 1` that crosses the call while `j` stays in a1; that also put
 * the cached high half/state base in a2/a3 and the upload arguments in retail order.
 * Third Opus attempt (v2588): C unchanged, no wording found; the blocker is now proven.
 * Passing this file's unchanged cc1 output (no -g; -g gives the same result) through SN's
 * toolchain/sn-prodg-2.0/ee/bin/Ps2EeAs.exe gives 608 bytes with 0 of 152 words different
 * from retail L00 (relocated fields masked): that assembler emits the nop, ours does not.
 * Why C cannot reach it: rac2_mtc1_nop_ok only keeps the nop if prev_prev_insn is a real
 * opcode, and a reorder-mode branch leaves &dummy_opcode, so a real nop in the slot would
 * need a filled SEQUENCE, which cc1 never makes (no RTL nop exists before dbr at -O2).
 * Retail census: the a0/cvt.s.w exemption has one no-nop witness, func_00283CE0's first
 * instruction, and the zero/c.lt.s role has three, none right after a branch group;
 * the only sites after a branch and its nop are this function's 27, all with the nop.
 * Caveat: Ps2EeAs also emits the nop in func_00283CE0, where retail has none, so it is not
 * a complete model of the exemption. Untested fix: skip the exemption right after a
 * gas-filled branch slot.
 * Declarations needed are below (catalogue names of the boot helpers placed in
 * every level: func_0027C540, func_002FCB00, func_0027C660, func_00283CF0,
 * func_0027DDC0).
 */
#include "common.h"

typedef struct { int key, base, a08, a0c, width, top, mode, count, step, duration; } DrawRec_2958;
typedef struct { int buffers[7]; short buffer, pad1e, index, pad22, active; short pad26; int timer; char pad2c[0x36c-0x2c]; int delay, fade; char pad374[0x384-0x374]; int style; } DrawState_2958;
extern DrawState_2958 D_L00_001BC200;
extern DrawRec_2958 D_L00_001BC588[];
extern void func_0027C540(int);
/* The upload helper (src/game/002FC8D0.c) returns a packed 64-bit texture descriptor. */
extern long long func_002FCB00(int,int,int,int);
extern int D_001A72DC __attribute__((section(".sdata")));
extern float D_001B18E0 __attribute__((section(".sdata")));
extern void func_L00_002E2C30(int,float);
extern void func_L00_002E3340(int,float);
extern void func_L00_002E3520(void);
extern void func_0027C660(void);
extern int func_00283CF0(float);
extern void func_0027DDC0(int,int,int,int);
void func_L00_002E2958(void) {
 int i; float alpha;
 if(D_L00_001BC200.active) {
  func_0027C540(0);
  func_002FCB00((int)((char *)D_L00_001BC200.buffers[(short)(D_L00_001BC200.buffer^1)]+16),D_001A72DC,512,416);
  func_L00_002E3520();
  switch(D_L00_001BC588[D_L00_001BC200.index].mode) {
  case 0:
   for(i=0;i<D_L00_001BC588[D_L00_001BC200.index].count;i++) {
    if(D_L00_001BC588[D_L00_001BC200.index].duration<D_L00_001BC200.timer*2)
     alpha=(1.0f-(float)D_L00_001BC200.timer/(float)D_L00_001BC588[D_L00_001BC200.index].duration)*2.0f;
    else alpha=1.0f;
    func_L00_002E3340(i,alpha);
   }
   break;
  case 1: case 2:
   {
   int j=0;
   if(j<D_L00_001BC588[D_L00_001BC200.index].count) do {
    int next=j+1;
    func_L00_002E2C30(j,1.0f-((float)D_L00_001BC200.timer-(float)(D_L00_001BC588[D_L00_001BC200.index].count-next)*(float)D_L00_001BC200.delay)/(float)D_L00_001BC200.fade);
    j=next;
   } while(j<D_L00_001BC588[D_L00_001BC200.index].count);
   }
   break;
  }
  func_0027C660();
 }
 func_0027DDC0(0,0,0,func_00283CF0(D_001B18E0*128.0f));
}
