/* 5 November 2026: retained reconstruction, not linked or counted.
 * Retain func_002A0380 as int and func_002FC3C0 with its 64-bit mask.
 * The repaired source still fails all 28 USA places at +0x14.
 * Panel lifetime and clamp scheduling remain open; the indirect callback contract is not newly qualified.
 * Earlier measurements below describe earlier sources or toolchains.
 */
/*
 * Opus 5.5 xhigh worker (v2493), USA v1.01, -O2 -G8 -ffunction-sections
 * (separate unit src/game/002CBC58-g8.c; the 14-entry enabled array needs
 * its SDATA declaration and the struct types clash with 002C7748-g8.c).
 * Best check.py result: 2108/2100 bytes, 0/28 places; 21 aligned rows
 * differ (the v2069 draft had 58). This draft is a PROBE, not landable:
 * it keeps a dead "panels = 0" in the empty D_0025A058 arm.
 *
 * Why the panels zero sinks (measured with -dj -ds -dG -dL -dt dumps):
 * jump after cse1 if-converts "if (page) panels = page->panels" into
 * panels = page ? page+0xEC : panels, then gcse cprop pass 1 replaces the
 * else operand with the entry constant 0 ("CONST-PROP: Replacing reg ...
 * with constant (const_int 0)"), giving movz and dropping the entry zero.
 * Retail kept the entry zero and movn, so in retail either cprop saw a
 * second reaching set of panels, or the if-conversion happened only in
 * the jump pass before cse2 (after gcse). Any real set of panels on one
 * path blocks cprop: with the arm's dead store here the prologue, the
 * movn, s5/s6/s7, the callback compare registers and the loop-alignment
 * nop at 0x2CBEA4 all match retail. But that arm then emits
 * "daddu s7,zero,zero" in the beql slot where retail has the volatile
 * D_001AB910 load (cprop turns its cmove into a plain zero store).
 *
 * Late if-conversion via ADDRESSOF also works for the selection:
 *   static inline void getPagePanels(PanelCBC58 ***out, PanelCBC58 **list)
 *   { if (D_001F2840.page) *out = list; }
 *   getPagePanels(&panels, D_001F2840.page->panels);
 * (the THEN arm must be one insn after purge_addressof; storing
 * "page->panels" inside the helper leaves t = page+0xEC; panels = t, which
 * no later jump pass converts). With &panels, cse1 reloads panels as a
 * MEM in every bounds-loop statement, so loop.c makes three panels givs
 * and tests a copy instead of s7 (50 rows differ, 529 insns). Needed: a
 * selection that converts late without making panels address-taken.
 *
 * Still different here besides that arm:
 * - Bounds loop: retail loads D_001F2840.page in the delay slot of the
 *   non-likely "beqz s7" (panels null test). The address is reg+0x14, so
 *   may_trap_p forbids filling it from the fall-through thread; the RTL
 *   must hold the load in the null-test block, i.e. the source reads page
 *   after the enabled test and before testing panels, while all four
 *   statements still reload page after each store. Not found.
 * - Crop clamp: the four slt and three movn/movz issue in another order.
 * The arm keeps retail's word count; the two extra words are the bounds
 * loop's separate page load (retail fills the beqz slot with it) and the
 * pass-loop alignment nop that this shift adds at 0x2CBFA0.
 * Declarations it needs follow. The crop arithmetic follows the renderer
 * in RAC1's src/game/pause.c; RAC2's layout, callbacks, capture gating
 * and fade are derived from retail.
 */
#include "common.h"

typedef struct PanelCBC58 PanelCBC58;
struct PanelCBC58 {
    int unused;
    int (*draw)(PanelCBC58 *);
    int pad8[2];
    int flags, pad14;
    int x, y, width, height;
};
typedef struct { int x, y, width, height; } RectCBC58;
typedef struct { RectCBC58 bounds[14]; char padE0[12]; PanelCBC58 *panels[14]; } PageCBC58;
typedef struct {
    int mode, active;
    char pad8[12];
    PageCBC58 *page;
    char pad18[8];
    void *buffer;
    char pad24[0x144];
    int field168, field16C;
    char pad170[0x50];
    float fade, step, target;
} StateCBC58;
extern StateCBC58 D_001F2840;
extern int D_001A7344, D_001A72DC;
extern int D_001AB91C __attribute__((sda));
extern volatile int D_001AB910 __attribute__((sda));
extern int D_001AA450[14] SDATA(D_001AA450);
extern int D_001AA43C __attribute__((sda));
extern int D_001AB92C __attribute__((sda));
extern int D_001AB930 __attribute__((sda));
extern long D_001A7470 __attribute__((sda));
extern char D_00259018[], D_0025A738[], D_0025A498[], D_0025A190[], D_0025A058[];
extern PanelCBC58 D_00259EE0;
extern char D_001AB920[], D_001AB928[];
typedef struct { char pad[0x1A0]; int flags; char pad1A4[0x20]; int pressed; } PadCBC58;
extern PadCBC58 D_00138180;
extern void (*D_00210AD0)(void) __attribute__((nosda));
extern void func_00284F40(int);
extern void func_002FC990(void *, int, int);
extern void func_0027B858(void);
extern void func_0027DDC0(int,int,int,int);
extern void func_0027C540(int), func_0027C660(void);
extern void func_0028E458(void), func_0029BF08(void), func_0029C2D0(void), func_0029C340(void);
extern void func_002FC560(int, long);
extern void func_002A0250(void), func_002A01C8(void), func_002A01F0(void);
extern void func_0026FEB8(void *);
extern int func_002DBAD0(PanelCBC58 *), func_002DA118(PanelCBC58 *);
extern int func_002D84B0(PanelCBC58 *), func_002DA9A8(PanelCBC58 *);
extern void func_00280510(int,int,int,float);
extern void func_0028FDD8(int,int,int,int,long,int,int);
extern void func_00280600(void);
extern void func_0027E018(int,int,int,int,int,int,int,int,long,long);
extern void func_002A0220(void),func_0029FEE0(void);
extern int func_002A0380(void);
extern void func_002FC3C0(unsigned long);
extern int func_00283CF0(float);

void func_002CBC58(int mode)
{
    PanelCBC58 **panels = 0;
    StateCBC58 *state = &D_001F2840;
    int pass, i, j, x, y, w, h, lw, lh, result;
    int left, top, right, bottom, tw, th;
    float delta;
    func_00284F40(0);
    func_002FC990(state->buffer, D_001A72DC, D_001A7344 << 11);
    if (!mode) {
    func_0027B858();
    func_0027DDC0(0,0,0,0x38);
    if (D_001F2840.mode == 4 && state->active) {
    if (D_001AB91C) {
        void *page = state->page;
        if (page == D_00259018) {
            func_0027C540(0); D_00210AD0(); func_0027C660(); return;
        }
        if (page == D_0025A738) {
            func_0027C540(0); func_0028E458(); func_0029BF08(); func_0027C660(); return;
        }
        if (page == D_0025A498 && !D_001AB910) {
            func_0027C540(0); func_0029C2D0(); func_0027C660();
        } else if (D_001F2840.page == (void *)D_0025A190 && !D_001AB910) {
            func_0027C540(0); func_0029C340(); func_0027C660(); return;
        } else if (D_001F2840.page == (void *)D_0025A058 && !D_001AB910) {
            panels = 0;
        }
    }
    func_002FC560(0x47,0x5360B);
    if (D_001F2840.field168) {
        func_002A0250(); func_002A01C8(); func_002A01F0();
    }
    if (D_001F2840.page) {
        panels = D_001F2840.page->panels;
    }
    if ((D_00138180.flags&15)==15 && (D_00138180.pressed&15))
        func_0026FEB8(D_001AB920);
    for (j=0;j<14;j++) {
        if (D_001AA450[j] && panels && panels[j]) {
            panels[j]->x = D_001F2840.page->bounds[j].x - D_001F2840.page->bounds[j].width/2;
            panels[j]->y = D_001F2840.page->bounds[j].y - D_001F2840.page->bounds[j].height/2;
            panels[j]->width = D_001F2840.page->bounds[j].width;
            panels[j]->height = D_001F2840.page->bounds[j].height;
            if (panels[j] == &D_00259EE0) panels[j]->height=256;
        }
    }
    if ((D_00138180.flags&15)==15 && (D_00138180.pressed&15))
        func_0026FEB8(D_001AB928);
    for (pass=0;pass<2;pass++) {
        for (i=0;i<14;i++) {
            PanelCBC58 *p;
            if (!panels || !(p=panels[i]) || (p->flags&4) || !D_001AA450[i] || !p->draw) continue;
            if (!pass && !(p->flags&2)) continue;
            if (pass==1 && (p->flags&2)) continue;
            if (p->flags&1) { p->draw(p); continue; }
            x=p->x; y=p->y; w=p->width; h=p->height;
            lw=7; while ((1<<lw)<w) lw++;
            lh=7; while ((1<<lh)<h) lh++;
            while (lw+lh>=18) lh--;
            if ((panels[i]->draw == func_002DBAD0 || panels[i]->draw == func_002DA118 ||
                 panels[i]->draw == func_002D84B0 || panels[i]->draw == func_002DA9A8) && !(panels[i]->flags&8)) {
                func_00280510(lw,lh,0,1.0f);
                func_0028FDD8(0,0,1<<lw,1<<lh,D_001AA43C,0,0);
                result=panels[i]->draw(panels[i]);
                func_00280600();
            } else result=panels[i]->draw(panels[i]);
            func_002FC560(0x42,0x8000000064L);
            func_002FC560(0x47,0x43);
            if (result&1) continue;
            tw=1<<lw; th=1<<lh; left=0; right=tw; top=0; bottom=th;
            if (result&2) { right=w; bottom=h; }
            else if (result&8) {
                left=(right-w)/2; top=(bottom-h)/2;
                right-=left;
                left=left > -1 ? left : 0;
                bottom-=top;
                top=top > -1 ? top : 0;
                if (right>tw) right=tw;
                if (bottom>th) bottom=th;
            } else if (result&4) {
                if (w<h) { left=right/2-w*right/(h*2); right-=left; }
                else { top=bottom/2-h*bottom/(w*2); bottom-=top; }
            } else if (!(result&16)) continue;
            if (D_001AB92C)
                func_0027E018(x,y,w,h,left,top,right-left,bottom-top,0x80808080L,D_001A7470);
        }
        if (D_001AB930 && !pass && D_001F2840.field168) {
            func_002A0220(); func_002A0380(); func_002FC3C0(16); func_0029FEE0();
            D_001F2840.field168=0; D_001F2840.field16C=0;
        }
    }
    func_0027B858();
    if (D_001F2840.fade != D_001F2840.target) {
        delta=D_001F2840.target-D_001F2840.fade;
        if (delta>D_001F2840.step) delta=D_001F2840.step;
        else if (delta < -D_001F2840.step) delta=-D_001F2840.step;
        D_001F2840.fade+=delta;
    }
    if (D_001F2840.fade != 0.0f)
        func_0027DDC0(0,0,0,func_00283CF0(D_001F2840.fade*128.0f));
    func_002FC560(0x42,0x8000000044L);
    func_002FC560(0x47,0x5360B);
    }
    }
}
