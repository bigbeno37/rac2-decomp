/* Claude Opus 5.5 (opus-2, xhigh), 3 November 2026 (v2253); retained near miss, closer than the drafts below.
 * USA v1.01, game -O2 -G8 -ffunction-sections in a dedicated *-g8.c unit (e.g. L00_002AE388-blend-g8.c):
 * 1636 bytes / 1644 retail; tools/check.py: 0 of 27 places match; 365 of 411 retail instructions in
 * sequence (previous draft 345). All declarations it needs are below (own State/Slot types: the existing
 * L00_002AE388-g8.c declares D_00189E20_26 with a different type).
 * What it fixes: function-scope g (a loop-top g lets combine_movables merge the 0x22BF block's h into it),
 * the !different arm written out (jump2 cross-jumps it back; flow's extra refs give g priority over the
 * o+0x38 PRE pseudo, so g gets $fp), and literal .05f/.3f (retail's prologue order 0, .05, .3, 1.0; f23=.05).
 * Still differs: slot-base lui register (v1/a0 swapped) and its copy into the delay slots of the two bc1f
 * and the arm-end b; the order of i+1/base insns in the allocate and fade arms; spill slots (retail o+6 at
 * 4, o+0x38 at 8, i+1 at 0xC); transition sltu+daddu; loop back-edge bnez+lui vs bnel+sll.
 * Tried without gain: a block-scoped `StateAED68 *h` around the join (moves the state lui before the
 * beqz as in retail, but same score). Declaration initialisers used: g and three block h locals.
 *
 * Earlier: GPT-6.1 Sol (high effort), 29 October 2026; retained near miss.
 * USA v1.01, game -O2 -G8 -ffunction-sections: 1620 bytes / 1644 retail;
 * tools/check.py func_L00_002AED68 --diff: 0 of 27 places match.
 * All required declarations are below. Requires a dedicated *-g8.c unit.
 *
 * Typed slots, indexed 0x600/0x400-byte buffer records and separate loop
 * counters recover the 0x80 frame, saved f20-f23 set and retail's buffer-loop
 * instruction pattern. Explicit .05/.3 locals retain the constants across calls.
 * Remaining: constant setup order, repeated state-base materializations,
 * six float-store ordering, transition-result allocation and active/fading joins.
 * A function check has not yet qualified these extern identities for linking.
 *
 * Correction to the earlier sol-low reconstruction: the ID copy condition is
 * modes[0] == modes[1] || progress > 0.5f. Retail's beql at 00:002AF0B0
 * targets the copy at 002AF0DC when the modes agree; it skips the FP test.
 * The previous different-modes AND progress condition changes behavior.
 *
 * Measured: explicit state pointer gives 1604 bytes but wrong base lifetimes;
 * pointer-walking buffers produce a 0xA0 frame and 128-bit spills. Indexed
 * buffer references and separately scoped counters remove these spills.
 * Moving constant initialization inside the loop gives 1612 bytes; naming a
 * zero comparison local gives 1620 with f20/f21 interchanged and rematerialized
 * zero call arguments. Keep the retained literal-zero form as the starting point.
 * -fno-gcse worsens to 1528 bytes; -frac2-gcse-unsplit-edges and
 * -fno-thread-jumps tie their 1612-byte default-profile input. Nested versus
 * combined active initialization guards also tie. No linked match claimed.
 * Next: investigate state-object access forms that retain retail's separate
 * address lifetimes and join layout; do not repeat these variants unchanged.
  * 2026-11-01 (closer B): `$fp` holds D_00189E20 for the 0x22BB reads and the 0x2290 reads at the arm ends, while
 * 0x22BF, the enabled arm, the fade arm and the block after the join rebuild it with lui/addiu. One name hoists
 * all of them to one register; a pointer local set at the top is rematerialised at every use. Unsolved.
 * 2026-11-03 (v2230, RAC1 form): RAC1 update_hero_item_slot_animation_blends (hero_animation_slots.c) is the
 * counterpart. Its `Hero *g = GB` at the loop top plus block-scoped `Hero *h = GB` locals reproduce retail's split
 * (g hoisted, h rebuilt per block) and the control flow, but g (12 refs, 606 live) then loses $fp to the o+0x38 PRE
 * pseudo (4 refs, 47 live); it needs about 26 refs. 1700 bytes, 262 of 411 exact versus 286 for this draft.
 * The six table stores follow sched1's register-death preference: the last `out` store goes first. Source order
 * 6D4, 6D8, 694, 5E4, 624, 628 gives retail's 5E4, 6D4, 6D8, 694, 624, 628. Retail's prologue order
 * (0.0, 0.05, 0.3, 1.0; 0.3 in f22) fits loop.c hoisting .05/.3 in pass 1, but one in-block load is not desirable
 * (savings 1, life 4 against 445 insns). Function-scope locals give the order .05, .3, 0.0, 1.0.
 */
#include "include_asm.h"
typedef struct SlotAED68 {
 unsigned char pad0[6];
 unsigned char flags[2];
 float opacity;
 unsigned char padc[12];
 int icon;
 unsigned char pad1c[4];
 unsigned char ids[2];
 unsigned char modes[2];
 float progress, scale, speed;
 short transition;
 unsigned char active, fading;
 int destroy;
 void *buffers[2];
} SlotAED68;
typedef struct PlayerAED68 {
 unsigned char pad0[0x40];
 unsigned char ids[2], modes[2];
 float progress;
} PlayerAED68;
typedef struct StateAED68 {
 char pad0[3116];
 int fc2c;
 char padc30[616];
 int fe98;
 char pade9c[8];
 int fea4;
 char padea8[5096];
 PlayerAED68 *f2290;
 char pad2294[39];
 unsigned char f22bb;
 char pad22bc[3];
 unsigned char f22bf;
} StateAED68;
extern StateAED68 D_00189E20_26;
extern struct {
 char pad0[1508];
 float f5e4;
 char pad5e8[60];
 float f624;
 float f628;
 char pad62c[104];
 float f694;
 char pad698[60];
 float f6d4;
 float f6d8;
} D_L00_0025BE10;
extern SlotAED68 *D_0018ACB0[2];
extern int D_L00_001A7D00[2] __attribute__((sda));
extern unsigned char D_L00_001DA860[];
extern int D_L00_001D9F00[];
typedef struct { char data[0x600]; } LargeBufferAED68;
typedef struct { char data[0x400]; } SmallBufferAED68;
extern LargeBufferAED68 D_L00_001B5C00[];
extern SmallBufferAED68 D_L00_001B3400[];
extern float func_002AA2E8(float *,float,float);
extern void func_L00_00310C88(void *,void *);
extern SlotAED68 *func_L00_00310BC8(void *,int);
extern void func_L00_00310D30(void *,void *);
extern int func_L00_002A4EF0(int);
extern int func_L00_002ADFE8(int,float *);
extern int func_L00_002ADFD0(void *,int);
void func_L00_002AED68(void)
{
 int i,j,enabled,different,active; short transition; float amount; SlotAED68 *p;
 StateAED68 *g=&D_00189E20_26;
 for(i=0;i<2;i++) {
  if(D_0018ACB0[i] && D_0018ACB0[i]->destroy) {
   if(func_002AA2E8(&D_0018ACB0[i]->opacity,0.0f,0.1f)==0.0f)
    func_L00_00310C88(g->f2290,&D_0018ACB0[i]);
  } else {
   enabled=0;
   if(g->f22bb && (i==0 || g->f22bb==2)) enabled=1;
   { StateAED68 *h=&D_00189E20_26;
   if(h->f22bf) {
    enabled=0;
    if(D_0018ACB0[i]) D_0018ACB0[i]->destroy=1;
   } }
   if(enabled) {
    StateAED68 *h=&D_00189E20_26;
    if(!D_0018ACB0[i]) {
     D_0018ACB0[i]=func_L00_00310BC8(h->f2290,D_L00_001A7D00[i]);
     D_0018ACB0[i]->opacity=0.0f;
     D_0018ACB0[i]->icon=D_L00_001D9F00[D_L00_001DA860[i+1]];
    }
   } else {
    StateAED68 *h=&D_00189E20_26;
    if(!D_0018ACB0[i]) continue;
    if(func_002AA2E8(&D_0018ACB0[i]->opacity,0.0f,0.15f)==0.0f)
     func_L00_00310C88(h->f2290,&D_0018ACB0[i]);
   }
  }
  p=D_0018ACB0[i]; if(!p) continue;
  different=D_00189E20_26.f2290->modes[0]!=D_00189E20_26.f2290->modes[1];
  if(D_00189E20_26.fe98 && !D_00189E20_26.fea4)
   func_002AA2E8(&p->opacity,0.0f,0.15f);
  else if(!p->destroy) func_002AA2E8(&p->opacity,1.0f,0.25f);
  transition=0;
  if(different && D_00189E20_26.fc2c &&
     func_L00_002A4EF0(p->modes[1])!=func_L00_002A4EF0(D_00189E20_26.f2290->modes[1]))
   transition=p->modes[0]!=p->modes[1];
  p->transition=transition;
  active=func_L00_002ADFE8(g->f2290->modes[1],&amount);
  if(active || p->active) {
   if(amount!=0.0f) {
    D_L00_0025BE10.f6d4=0.05f; D_L00_0025BE10.f6d8=0.3f;
    D_L00_0025BE10.f694=amount; D_L00_0025BE10.f5e4=amount;
    D_L00_0025BE10.f624=0.05f; D_L00_0025BE10.f628=0.3f;
   }
   if(active && (!p->active || p->modes[1])) {
     if(p->modes[0]==p->modes[1] || p->progress>0.5f) { p->modes[0]=p->modes[1]; p->ids[0]=p->ids[1]; }
     p->progress=0.0f; p->speed=0.06666667f; p->modes[1]=0; p->ids[1]=0;
     p->scale=1.0f; p->active=1; p->fading=0;
   }
   if(!active) {
    if(!p->fading) {
     p->modes[1]=func_L00_002ADFD0(g->f2290,g->f2290->modes[1]);
     p->ids[1]=0; p->fading=1; p->progress=0; p->scale=0;
    } else {
     p->progress+=0.1f; if(p->progress>1.0f) p->progress=1.0f;
     if(p->progress==1.0f) p->active=0;
    }
   } else p->scale=1.0f;
   { int k;
   for(k=0;k<2;k++) {
    if(p->modes[k]<23) { p->flags[k]=1; p->buffers[k]=&D_L00_001B3400[i*2+k]; }
    else { p->flags[k]=0; p->buffers[k]=&D_L00_001B5C00[i*2+k]; }
   }
   }
   func_L00_00310D30(g->f2290,p);
  } else {
   if(!p->transition) for(j=0;j<2;j++) {
    if(different && j!=1) continue;
    p->modes[j]=func_L00_002ADFD0(D_00189E20_26.f2290,D_00189E20_26.f2290->modes[j]);
   }
   if(p->transition) {
    p->progress+=0.15f;
    if(p->progress>=1.0f) { p->modes[0]=p->modes[1]; p->ids[0]=p->ids[1]; p->transition=0; }
   } else {
    if(!different) {
     p->ids[0]=g->f2290->ids[0];
     p->ids[1]=g->f2290->ids[1];
     p->progress=g->f2290->progress;
    } else {
     p->ids[1]=g->f2290->ids[1];
     p->progress=g->f2290->progress;
    }
   }
   { int l;
   for(l=0;l<2;l++) {
    if(p->modes[l]<23) { p->flags[l]=1; p->buffers[l]=&D_L00_001B3400[i*2+l]; }
    else { p->flags[l]=0; p->buffers[l]=&D_L00_001B5C00[i*2+l]; }
   }
   }
   func_L00_00310D30(g->f2290,p);
  }
 }
}
