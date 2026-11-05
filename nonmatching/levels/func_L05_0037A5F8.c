/* v2558, opus-2, Claude Opus 5.5, USA v1.01, 2026-11-05. FAILED: not a match.
   Build it as -O2 -G8 -ffunction-sections -frac2-qcopy -frac2-qcopy-barrier
   -frac2-zero-ti-por. Body below (allowed C only): 1168/1168 bytes, 0/4 places,
   72 real instruction rows differ (79 before), relocations aside. New wordings:
   owner = D_225DD0[i] as its own statement; per-arm scale/size locals, because
   expand_binop swaps `y = distance*y` when target == op1; per-site int c,
   u64 color and int y. Each draw site then ties c to color in s0, and the
   `daddu s0,s0,zero` in the jal slot is a same-reg extendsidi2: empty unless it
   fills a delay slot.

   Closest form, NOT allowed (uninitialised test, dead-store arm), 4 real rows:
       #define SPLIT(v) if ((v) != 0 && xx != 0) y = 1.0f
   with `int xx;` never assigned, and on the body below
       entry: yi = (int)(...); SPLIT(yi); y = (float)yi;
       sites: sy = (int)(...); SPLIT(sy); fy = (float)sy;
   Left: mfc1/mtc1 at +0x3C/+0x70 use v0 (retail a1); por/sq at +0x18C/+0x190
   use v1 (retail a0). No exact-bytes form was found.
   This form gives the entry saves in retail order, s0=0 in the jal slot, the
   draw sites exact, D_FB220 rematerialized and fp = D_225DD0.

   Why (gcc 991111 flow.c and toplev.c): merge_blocks deletes a branch over an
   empty arm only when the join has one predecessor, so
   `if (x) dead;` is gone before sched2 and allocation changes but scheduling
   does not. `if (a && b) dead;` joins with two predecessors, the branches
   reach sched2 as a real block end, and jump2 removes them as jumps to .+1.
   They leave no word if they compare live registers or zero. The test must use
   the conversion result: testing other values moves `sd s6` before the cvt.w.s.
   xx is live from entry through the loop, crosses every call, takes s6 (saved
   and never referenced in retail) and pushes D_FB220 out of a register. Retail
   sets s6 nowhere, so its pseudo has no set: no real value can fill that role.

   Reopen with: a real construct that reads a variable with no set in the
   function; and for the rest, why entry y-int avoids v0 and v1. That looks like
   two more pseudos live at the fix insn (with v0/v1 busy reload's scratch would
   be a0, and por equalled the scratch in the two dumps read, which is a hint,
   not a rule). `if (yi != sy && nextList != 0)` on a shared site y gives 11
   rows (alpha and the site y move); texture for nextList gives 7, both 13.

   Ruled out: do { ... } while (0) around each draw gets the entry saves right
   but is 1176 bytes and leaves i=0 after the second call, so it is not the
   construct here (and the worker guide does not allow it). PAL and US2 not
   rechecked on this draft.
*/
/* v2316, opus-1, Claude Opus 5.5, USA v1.01, 2026-11-03. Body below unchanged:
   still 1168/1168 bytes, 0/4 places, 126/292 rows differ with the -G8 profile.
   Diagnostic only (forbidden, not kept): an empty asm barrier right after each
   float->int Y conversion (entry and both draw sites) gives 82 rows with the
   entry block exact (y-int in a1, saves in retail order, s0=0 in the jal slot);
   adding per-site u64 colour locals feeding the draw call gives 74 rows with
   both draw sites exact apart from relocations. So retail has a block boundary
   right after each mfc1 that survives sched1/sched2 and then vanishes, which
   fits a test of the live y-int (bnez/bltz/bgez) with arms that compile alike,
   e.g. an inlined screen-coordinate helper. No real construct found yet; reopen
   with one. Left over under the diagnostic: entry y-int v1 not a1 once colour
   locals exist; preheader hoists D_FB220 into s7 (retail spills and
   rematerializes it, fp = D_225DD0); i+1 in s6 not s7; clear por v0 not a0;
   D_4F8 into f0 not f20; mul.s f20,f20,f21 operand order. Retail saves s6 but
   never uses it, so a vanished call-crossing pseudo probably took s6 and forced
   the FB220 spill (see unused-save notes, b0430). PAL and US2 differ the same way.
*/
/* v0893, GPT-6.1 Sol high, USA v1.01, current pinned compiler, 2026-10-29.
   Dedicated -O2 -G8 -ffunction-sections -frac2-qcopy -frac2-qcopy-barrier
   -frac2-zero-ti-por: 1168/1168 bytes, 0/4 places, first mismatch +0x1C.
   The absolute macro views remove cached scale/object-table high halves.
   Camera view is +0x230 from its position, as retail computes it. Corrected
   scale association to distance*(radius/128), and the clamps to ordered >/<.
   Reusing distance for marker size and y for scale/screen Y matches saved FP
   allocation; calculating radius/128 before the clear matches constant order.
   Remaining: entry saves/conversion scheduling, list-table base hoisted into
   s7 (retail rematerializes it), loop-next index s6 versus s7, clear register
   and scale multiply scheduling, and color assembly versus Y conversion.
   A separate rounded-Y integer/color staging grows to 1180; a macro list-table
   view with split color staging shrinks to 1148. Debug labels do not close it.
   Reopen with an evidenced list-base/color lifetime; no handwritten claim.
   Required declarations are included below.
   1 November 2026: both draw sites share alpha (retail's a0, set to 0x80 in the
   slot of the second site's first test) and c. In retail every integer insn of
   those sites and of the entry comes after "cvt.w.s; mfc1", though sll is ready
   at the top of the block and an ALU slot is free: mtc1/cvt.s.w, lw 0x150, sll
   and the texture argument behave as if in a later block. The conversion
   result's register (a1 at the entry, v1 at both sites) also looks global. A
   static inline drawing helper taking y as an int was tried: worse (115 rows).
*/

#include "common.h"
typedef int Quad __attribute__((mode(TI)));
extern float D_L05_001AC4E0 __attribute__((sda));
extern float D_L05_001AC4E4 __attribute__((sda));
extern float D_L05_001AC4E8 __attribute__((sda));
extern float D_L05_001AC4EC __attribute__((sda));
extern int D_L05_001AC4F0 __attribute__((sda));
extern int D_L05_001AC4F4 __attribute__((sda));
extern float D_L05_001AC4F8 __attribute__((sda));
extern int D_L05_001AC4FC __attribute__((sda));
extern float D_L05_001B2D84[] MACRO_ADDR;
extern u8 *D_L05_00225DD0[] NOT_SDA;
extern u16 *D_L05_001FB220[];
extern u8 *D_L05_001B251C[] MACRO_ADDR;
typedef struct { float position[4]; u8 pad[0x220]; float view[12]; } L05HudCamera;
extern L05HudCamera D_L05_001B96C0;
extern u64 func_0027C878(int);
extern void func_L05_003008C0(float,float,float,float,int,int,int,int,u64,u64);
extern int func_L00_004272A0(void);
extern s32 func_002ED688(u8 *);
extern float func_00282E48(void *,void *);
extern void func_002833F0(void *,void *);
extern void func_00282CF0(void *,void *,void *);
extern void func_00282F20(void *,void *,float);
extern void func_002836E8(void *,void *);
extern void func_00283098(void *,void *,void *);
extern float func_002A7798(float);
extern int func_00283CF0(float);
void func_L05_0037A5F8(void)
{
    float offset[4],rotation[4],rotate[12],view[12];
    float distance,x,y;
    u64 texture;
    u8 *object,*state,*owner;
    u16 *list,*nextList;
    int i,alpha,yi;
    yi = (int)(D_L05_001AC4E4*D_L05_001B2D84[0]+0.5f);
    y = (float)yi;
    texture = func_0027C878(D_L05_001AC4FC);
    func_L05_003008C0(D_L05_001AC4E0,y,D_L05_001AC4E8,D_L05_001AC4EC,0,0,D_L05_001AC4F0,D_L05_001AC4F4,0x80808080ULL,texture);
    for (i=0;i<func_L00_004272A0();i++) {
        owner = D_L05_00225DD0[i];
        nextList = D_L05_001FB220[owner[0x21]];
        if (!nextList) continue;
        list = nextList;
        do {
            object = D_L05_001B251C[0]+((*list&0x7FFF)<<8);
            state = (u8 *)func_002ED688(object);
            if (*(int *)(state+0x110) && *(int *)(state+0x14C)) {
                distance = func_00282E48(D_L05_001B96C0.position,object+0x10);
                if (distance <= 128.0f) {
                    float scale = D_L05_001AC4F8;
                    float size;
                    *(Quad *)rotation = 0;
                    scale = scale*0.0078125f;
                    rotation[2] = 1.5707964f;
                    size = distance*scale;
                    func_002833F0(rotate,rotation);
                    func_00282CF0(offset,object+0x10,D_L05_001B96C0.position);
                    func_00282F20(offset,offset,size);
                    func_002836E8(view,D_L05_001B96C0.view);
                    func_00283098(offset,offset,view);
                    func_00283098(offset,offset,rotate);
                    alpha = func_00283CF0(func_002A7798(1.0f-distance*0.0078125f)*128.0f);
                    distance = offset[2]*0.125f+D_L05_001AC4E8*0.5f;
                    if (*(int *)(state+0x158)) { alpha=128; distance=D_L05_001AC4E8*0.5f; }
                    if (distance > D_L05_001AC4E8+D_L05_001AC4E8) distance=D_L05_001AC4E8+D_L05_001AC4E8;
                    else if (distance < 0.0f) distance=0.0f;
                    x = offset[0]+D_L05_001AC4E0;
                    {
                    int sy, c;
                    float fy;
                    u64 color;
                    sy = (int)((D_L05_001AC4E4-offset[1])*D_L05_001B2D84[0]+0.5f);
                    fy = (float)sy;
                    c = *(s32 *)(state+0x150);
                    c |= alpha<<24;
                    color = c;
                    texture = func_0027C878(D_L05_001AC4FC);
                    func_L05_003008C0(x,fy,distance,distance,0,0,D_L05_001AC4F0,D_L05_001AC4F4,color,texture);
                    }
                } else if (*(int *)(state+0x158)) {
                    float size;
                    *(Quad *)rotation = 0;
                    rotation[2] = 1.5707964f;
                    size = D_L05_001AC4F8;
                    func_002833F0(rotate,rotation);
                    func_00282CF0(offset,object+0x10,D_L05_001B96C0.position);
                    func_00282F20(offset,offset,size);
                    func_002836E8(view,D_L05_001B96C0.view);
                    func_00283098(offset,offset,view);
                    func_00283098(offset,offset,rotate);
                    alpha=128;
                    distance = D_L05_001AC4E8;
                    if (distance > D_L05_001AC4E8+D_L05_001AC4E8) distance=D_L05_001AC4E8+D_L05_001AC4E8;
                    else if (distance < 0.0f) distance=0.0f;
                    x = offset[0]+D_L05_001AC4E0;
                    {
                    int sy, c;
                    float fy;
                    u64 color;
                    sy = (int)((D_L05_001AC4E4-offset[1])*D_L05_001B2D84[0]+0.5f);
                    fy = (float)sy;
                    c = *(s32 *)(state+0x150);
                    c |= alpha<<24;
                    color = c;
                    texture = func_0027C878(D_L05_001AC4FC);
                    func_L05_003008C0(x,fy,distance,distance,0,0,D_L05_001AC4F0,D_L05_001AC4F4,color,texture);
                    }
                }
            }
        } while ((s16)*list++ >= 0);
    }
}
