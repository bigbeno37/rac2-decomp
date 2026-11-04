/* opus-2 (Claude Opus 5.5, xhigh), 2026-11-05, USA v1.01 check.py, -O2 -G8 unit.
 * Best: 1088/1088 bytes, 0/28 places (first mismatch +0x118), 10 instructions differ.
 * Place in src/game/0034CEA0-g8.c (it uses $gp data): the declarations below are complete,
 * but that unit already has `extern void func_0034DB98(void *);`, hence the void * parameter.
 * Fixed versus earlier drafts (all measured):
 *  - Indexed member-array loops with the vtable address written from &a[i]
 *    (`*(HudVEntry **)((char *)&object->a[i] + 0x30)`): every giv's add value is then
 *    object+C, so loop.c cannot eliminate the counter biv. The rerun pass reverses it to
 *    retail's 7..bgez, and the loop-2 bases become loop.c giv inits after gcse, so they
 *    are no longer hoisted. `object->a[i].vtable` lets PRE hoist object+0x388 into a
 *    pointer register and the counter is eliminated (slt against an end pointer).
 *  - A separate counter (j) for loop 2 gives retail's s5/s3 counters and loop-1 rotation.
 *  - The text loop is `step = 3.0f; if (step < (float)count) { start; do ... while (step < (float)count); }`
 *    with &object->text inside: PRE copy s4=s0, hoisted s1=s4, mov.s f25/f26 copies, and the
 *    (float)yoffset hoist (PRE needs a real preheader block) all match retail.
 *  - `long long style = t->style; func_0027C878((int)style)` gives retail's ld+dsll32+dsra32.
 *  - EE-ABI interleaved func_0027F640 prototype (float drawX before the pointer) fixes the
 *    f12/a3 argument-move order.
 *  - `unsigned int shadow = 0;` (declaration initializer, 1 of 5) makes shadow the canonical
 *    CSE register (cse.c make_regs_eqv: referenced outside its CSE block), restoring retail's
 *    `and v1` + `daddu s0, v1` copy and the 1088-byte size.
 * Still differs:
 *  - both vtable calls: lw v0,0xC(v0); jalr v0 versus retail v1. sched1 ranks the delta lh (5)
 *    above the fn lw (4), so the vtable pseudo dies at the fn load and local-alloc reuses v0
 *    (birth and death share slot 2n). Retail needs fn loaded while the vtable pointer is still live.
 *    An int-returning fn (call_value) did not change it.
 *  - D_001A7BB4: sda incomplete array is one insn to gcc, so reorg fills the n-loop pre-test's
 *    bnel slot (retail: bnez + nop); plain or nosda forms let PRE/loop hoist the %hi into s7.
 *  - shadow block: sched2 puts `and s1,s1,v0` before `lw a0` (retail the reverse).
 * D_001A7A32 stays volatile (retail reloads it for the sprintf argument); unproven.
 */
#include "include_asm.h"

extern void func_00336320(void *);
extern void func_00336828(void *);
extern void func_00336A78(void *);
extern float *func_00335E18(void *);
extern float *func_00335E10(void *);
extern unsigned int *func_00335E20(void *);
extern void func_0034AD10(void *);
extern void *func_0027C878(int);
extern void func_0027F640(unsigned long long, char *, int, float, void *, int, float, float);
extern void func_0027FBD8(int, int, unsigned int, char *, int);
extern int func_00115DA8(char *, const char *, ...);
extern int D_001A8C64[] __attribute__((sda));
extern int D_001A79F8[] __attribute__((sda));
extern unsigned char D_001A7BB4[] __attribute__((sda));
extern volatile unsigned char D_001A7A32[] __attribute__((sda));
extern char D_001AE6F8[], D_001AE700[], D_001AE708[];
extern int D_001ADAE8 __attribute__((sda));
extern int D_001ADAEC __attribute__((sda));
extern unsigned int D_001ADAF0 __attribute__((sda));

typedef struct HudVEntry {
    short delta;
    short index;
    void (*fn)(void *);
} HudVEntry;

typedef struct HudElem4C {
    char pad0[0x30];
    HudVEntry *vtable; /* 0x30 */
    char pad34[0x18];
} HudElem4C;

typedef struct HudElem3C {
    char pad0[0x30];
    HudVEntry *vtable; /* 0x30 */
    char pad34[0x8];
} HudElem3C;

typedef struct HudText {
    char pad0[0x34];
    int font;          /* 0x34 */
    long long style;   /* 0x38 */
} HudText;

typedef struct HudOwner {
    HudElem4C top[3];       /* 0x0 */
    char pad0E4[0x1C];
    char panel100[0x48];    /* 0x100 */
    char panel148[0x3C];    /* 0x148 */
    HudElem4C mid[2];       /* 0x184 */
    char pad21C[0x1C];
    HudText text;           /* 0x238 */
    char pad240[0x280 - 0x238 - sizeof(HudText)];
    char panel280[0x40];    /* 0x280 */
    HudElem4C side[2];      /* 0x2C0 */
    HudElem4C a[8];         /* 0x358 */
    HudElem4C b;            /* 0x5B8 */
    HudElem3C c[8];         /* 0x604 */
    char pad7E4[4];
    char final[4];          /* 0x7E8 */
} HudOwner;

void func_0034DB98(void *arg)
{
    HudOwner *object = arg;
    char text[32], countText[16];
    HudVEntry *vt;
    float *scale;
    float x, y, step;
    unsigned int color, foreground;
    unsigned int shadow = 0;
    int i, j, count, n, alternate, yoffset, xoffset;

    func_00336320(&object->top[0]);
    func_00336320(&object->top[1]);
    func_00336320(&object->top[2]);
    func_00336828(object->panel148);
    func_00336A78(object->panel100);
    if (!D_001A8C64[0]) {
        HudElem4C *left = &object->side[0];
        HudElem4C *right = &object->side[1];
        func_00336320(left);
        func_00336320(right);
        { float *value = func_00335E18(left); *value = -*value; }
        { float *value = func_00335E18(right); *value = -*value; }
        func_00336320(left);
        func_00336320(right);
        { float *value = func_00335E18(left); *value = -*value; }
        { float *value = func_00335E18(right); *value = -*value; }
        for (i = 0; i < 8; i++) {
            vt = *(HudVEntry **)((char *)&object->a[i] + 0x30);
            vt[1].fn((char *)&object->a[i] + vt[1].delta);
        }
        for (j = 0; j < 8; j++) {
            vt = *(HudVEntry **)((char *)&object->c[j] + 0x30);
            vt[1].fn((char *)&object->c[j] + vt[1].delta);
        }
        func_00336320(&object->b);
        *func_00335E20(&object->b) = 0x70FFFEED;
    }
    func_00336320(&object->mid[0]);
    func_00336320(&object->mid[1]);
    func_00336A78(&object->text);
    func_00336828(object->panel280);
    x = func_00335E10(&object->text)[0];
    y = func_00335E10(&object->text)[1];
    x += 32.0f;
    y += -10.0f;
    color = *func_00335E20(&object->text);
    count = 1;
    n = D_001A79F8[0];
    while (n >= 10) { n /= 10; count++; }
    alternate = D_001A7BB4[0] != 3 && D_001A7BB4[0] != 5;
    func_00115DA8(text, alternate ? D_001AE6F8 : D_001AE700);
    yoffset = alternate ? 21 : 10;
    xoffset = alternate ? -34 : -29;
    step = 3.0f;
    if (step < (float)count) {
        float start = x + (float)xoffset;
        do {
            HudText *t = &object->text;
            float drawX = start - (float)(int)(step * 11.6f);
            long long style;
            step += 3.0f;
            scale = func_00335E18(t);
            style = t->style;
            func_0027F640(color, text, -1, drawX, func_0027C878((int)style), t->font,
                          y + (float)yoffset, *scale);
        } while (step < (float)count);
    }
    if (D_001A7A32[0] >= 2) {
        func_00115DA8(countText, D_001AE708, D_001A7A32[0]);
        foreground = (D_001ADAF0 & 0xFFFFFF) | (color & 0xFF000000);
        shadow = color & 0xFF000000;
        func_0027FBD8(D_001ADAE8, D_001ADAEC + 2, shadow, countText, -1);
        func_0027FBD8(D_001ADAE8 + 2, D_001ADAEC, shadow, countText, -1);
        func_0027FBD8(D_001ADAE8 + 2, D_001ADAEC + 2, shadow, countText, -1);
        func_0027FBD8(D_001ADAE8, D_001ADAEC, foreground, countText, -1);
    }
    func_0034AD10(object->final);
}
