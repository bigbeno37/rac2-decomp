/* Opus 5.5 xhigh (opus-2), 5 November 2026: EXACT BYTES, NOT LANDABLE YET.
 * tools/check.py: func_002859C8: 28 of 28 places match. linkcheck.py boot: "boot: OK: 2521763 loaded
 * bytes equal; 2599 C functions" (run on the identical-arms wording, with both generated tables at
 * their retail place; the adjust wording below then compiled to the same 28/28). PAL and v2.00:
 * "in neither catalogue" (they carry func_PAL_00286288 and func_US2_00286378, 3472 bytes; PAL's case 9
 * setup has the same instruction order).
 *
 * Why it is here and not in src/: `if (st != 0)` in cases 9, 10 and 11 is an always-true test on a known
 * address, which WORKER2 forbids until the maintainers rule (report FAILED, record the exact form). The
 * arms do differ in the source (adjust = 0) as func_002D3D88's do, and retail has no word for the test.
 * The condition is the only objection. Upstream's func_0033C028 uses this shape with a real value,
 * `if (w != 0)`; no real value is available here, see below.
 *
 * To build it: remove the marker from src/game/00285840.c and put this file at
 * src/game/00285840-panel-g8.c (the name gives it -G8; its D_001BAC80 type differs from 00285840-g8.c).
 *
 * What retail needs in cases 9 to 11: a real block boundary between the float loads and mul.s.
 *   9, 10: f1=100 v0=3 f0=D_001B23A4 f2=0.5 | mul.s sw &box a1 a0=&box a2 add.s a3 t0 t1 cvt jal
 *   11:    f1=100 f0=D_001B23A4 | f2=0.5 &box | mul.s s1=3 sw a0=&box a1 a2 add.s a3 ... jal
 * The test survives sched2, so each arm is scheduled from a fresh ready list, and jump2 cross-jumps the
 * arms and deletes the jump. It leaves no word only on a register already live: a load stays behind
 * (tried: `if (D_001B23A0 != 0)`, 3444 bytes).
 *
 * Why no real condition fits: at these points the only integer registers holding anything are $s1 (st),
 * $s0 (its %hi) and, in 9 and 10, $v0 = 3. The switch value is gone: retail's $v0 and $a0 are reused for
 * the table base and target (a `mode` local moves the switch head into $a1, 5 rows), and n = 3 folds in
 * cse. Retail's own test was on one of those registers, so a pointer test is the likely original.
 *
 * Why a one-pass group cannot replace the test in 9 and 10: final.c's shorten_branches aligns the first
 * CODE_LABEL after any NOTE_INSN_LOOP_BEG to 8, searching past LOOP_END. The next labels are 0x286474 and
 * 0x28652C, both 4 mod 8, hence the old 3444-byte results. Case 11's next label, 0x2865E8, is 0 mod 8,
 * so a group fits there. Keep its shape: the group holds the whole case body, because update_equiv_regs
 * sinks a single-use constant past the test only at loop depth 0, and k must get the same loop weight
 * as h or they swap $f1 and $f2. The case 11 arms are small because the 1 and 3 compares of the inner
 * switch must stay in cse's reach of the call block. In 9 and 10 the arms hold the call and end in the
 * load of `a`, since an arm ending in a call cannot cross-jump (flow's (use 0)).
 * A fresh block also explains sw before a0 in case 11: a barrier instead of a block boundary lets the
 * base pointer's birthing boost take the slot beside mul.s in sched1, swapping those two rows.
 *
 * Earlier findings that still hold:
 *   - every case keeps its own block-scoped variables, except case 11's `a`, shared by inner cases 1, 3;
 *   - memset (func_00115484) returns void *, strncpy (func_00115AC0) char *, sprintf (func_00115DA8) int;
 *   - the SPR base is a register variable (`spr`), not a folded constant;
 *   - `show = 1` precedes the f5C test in case 1;
 *   - inner cases 2 and 21 assign id1 after the text call, 17/18/20 before it;
 *   - D_001A8C8C is a plain small extern under -G8, D_001F29A0 must be non-small;
 *   - case 11's {0, 1, 2, 3} switch reproduces retail's decision tree; case 3's tail is func_00280190's
 *     body inlined.
 */
#include "include_asm.h"

typedef struct {
    short x;
    unsigned short f02;
    short f04;
    short f06;
    short f08;
    short f0A;
    short f0C;
    short h;
    short f10;
    unsigned short flags;
    short f14;
    short f16;
} MenuBox;

typedef struct {
    char pad0[0x40];
    int mode;
    int f44;
    char pad48[0x10];
    int f58;
    int f5C;
    int f60;
    char pad64[0x18];
    short f7C;
    short f7E;
    int f80;
} MenuState;

extern MenuState D_001BAC80;
extern MenuBox D_001A8A88;
extern MenuBox D_001A8AB8;
extern char D_001A8AA0[];
extern char D_001A8AA8[];
extern char D_001A8AD0[];
extern char D_00263B50[];
extern char *D_001A8A4C __attribute__((sda));
extern char *D_001A8A50 __attribute__((sda));
extern char *D_001A8A54 __attribute__((sda));
extern char *D_001A8A58 __attribute__((sda));
extern int D_001A8A70 __attribute__((sda));
extern int D_001A8A74 __attribute__((sda));
extern int D_001A8C88;
extern int D_001A8C8C;
extern int D_001F29A0[];
extern int D_001A7420;
extern unsigned short D_001A7344;
extern int D_001B1598;
extern float D_001B23A4;

extern void func_0027DDC0(int, int, int, int);
extern void func_0027C540(int);
extern void func_002FC560(unsigned int, unsigned long);
extern char *func_00288FB8(int);
extern char *func_00115AC0(unsigned char *, char *, int);
extern void func_00280190(MenuBox *, unsigned long, unsigned char *, int);
extern int func_001157AC(unsigned char *);
extern void func_0029B888(MenuBox *, int, void *, int, int, int, int, char *, char *, char *);
extern int func_0028E588(int, int);
extern void func_0028E868(int, int, int, int, int, int);
extern int func_0028E640(int);
extern void func_0028F410(float, float, float, float, float, int, int, long long);
extern void *func_00115484(void *, int, int);
extern int func_00288D80(void);
extern int func_00287E90(void);
extern int func_00115DA8(char *, const char *, ...);
extern void func_0027EB88(int, int, int, int, int, int);
extern int func_00283D38(int, int, float);
extern int func_0027FAA8(int, int, unsigned long, char *, int);
extern int func_0027C878(int);
extern void func_0027FC70(MenuBox *, unsigned long, char *, int, int, void *);
extern void func_002802E0(MenuBox *, int, int, int, int, int, int, int, int);
extern void func_0027C660(void);

#define SPR ((unsigned char *)0x70000000)

void func_002859C8(void)
{
    char buf[0x300];
    MenuBox box;
    MenuBox tmp;
    MenuBox tmp2;
    char buf2[0x100];
    MenuState *st = &D_001BAC80;

    if (D_001BAC80.f7E == 0)
        func_0027DDC0(0, 0, 0, 0x30);
    if (D_001BAC80.f7C != 0)
        return;
    func_0027C540(0);
    func_002FC560(0x42, 0x8000000044UL);
    func_002FC560(0x47, 0x5360B);
    switch (D_001BAC80.mode) {
    case 2: {
        unsigned char *spr;
        unsigned char *p;
        int h, len, i, x, y;

        box = D_001A8A88;
        spr = SPR;
        func_00115AC0(spr, func_00288FB8(0x9B4), 0x400);
        p = spr;
        while (*p >= 2)
            p++;
        while (*p == 1) {
            *p = 0;
            p++;
        }
        box.flags |= 4;
        func_00280190(&box, 0x80000000UL, spr, -1);
        box.flags ^= 4;
        h = box.h;
        len = func_001157AC(spr);
        for (i = 4; i >= 0; i--)
            spr[len + i] = 1;
        spr[len + 5] = 0;
        func_0029B888(&box, 0x80F0F0F0, spr, 1, 1, -1, 1, func_00288FB8(0x1137), 0, 0);
        x = box.x + h;
        func_002FC560(0x47, 0x3004B);
        y = x + 8;
        func_0028E868(func_0028E588(0x755D, 0), 0xE0, y, 0x40, 0x40, 0x80);
        func_0028F410(4096.0f, (x + 40) * 16, 272.0f, 272.0f,
                      (D_001B1598 % 55) * -0.11423973f, 0x40, 0x40,
                      func_0028E640(func_0028E588(0x755D, 1)));
        break;
    }
    case 4:
        if (D_001BAC80.f5C > 0) {
            unsigned char *p;
            int id;

            func_00115AC0(SPR, func_00288FB8(0x2DE5), 0x400);
            p = SPR;
            while (*p >= 2)
                p++;
            while (*p == 1) {
                *p = 0;
                p++;
            }
            id = D_001BAC80.f44 ? 0 : 0x1137;
            func_00115484(&tmp, 0, sizeof(tmp));
            tmp.f02 = D_001A7344;
            tmp.f04 = 0x60;
            tmp.f06 = 0x1A0;
            tmp.f08 = 0x100;
            tmp.f0A = 0x68;
            tmp.f10 = 0x10;
            tmp.flags = 1;
            box = tmp;
            func_0029B888(&box, 0x80F0F0F0, p, 1, 1, -1, 0, id ? func_00288FB8(id) : 0, 0, 0);
        }
        break;
    case 5:
        if (D_001BAC80.f5C > 0) {
            char *text;
            int id;

            text = func_00288FB8(D_001BAC80.f80 ? 0x2E04 : 0x2E03);
            id = D_001BAC80.f44 ? 0 : 0x1137;
            func_00115484(&tmp2, 0, sizeof(tmp2));
            tmp2.f02 = D_001A7344;
            tmp2.f04 = 0x60;
            tmp2.f06 = 0x1A0;
            tmp2.f08 = 0x100;
            tmp2.f0A = 0x68;
            tmp2.f10 = 0x10;
            tmp2.flags = 1;
            box = tmp2;
            func_0029B888(&box, 0x80F0F0F0, text, 1, 1, -1, 0, id ? func_00288FB8(id) : 0, 0, 0);
        }
        break;
    case 1: {
        int show, id1, id2;
        char *text;

        show = 1;
        if (D_001BAC80.f5C > 0) {
            text = D_001A8AA0;
            id1 = 0;
            id2 = 0;
            if (func_00288D80()) {
                id1 = 0x1137;
                text = func_00288FB8(0x2E04);
            } else {
                switch (D_001A7420) {
                case 6:
                    id1 = 0x113C;
                    id2 = 0x1138;
                    text = func_00288FB8(0x2DF4);
                    break;
                case 12:
                    if (D_001F29A0[0]) {
                        id1 = 0x2DE8;
                        text = func_00288FB8(0x2DE6);
                        break;
                    }
                case 13:
                    id1 = 0x113C;
                    id2 = 0x1138;
                    text = func_00288FB8(0x2DF0);
                    break;
                case 10:
                case 11:
                    text = func_00288FB8(0x2E00);
                    break;
                case 7:
                case 8:
                    text = func_00288FB8(0x2DFE);
                    break;
                case 14:
                case 15:
                    text = func_00288FB8(0x2DFF);
                    break;
                case 17:
                    id1 = 0x1137;
                    text = func_00288FB8(0x2E01);
                    break;
                case 18:
                    id1 = 0x1137;
                    text = func_00288FB8(0x2E03);
                    break;
                case 2:
                    text = func_00288FB8(0x2DE5);
                    id1 = D_001BAC80.f44 ? 0 : 0x1137;
                    break;
                case 19:
                    if (D_001F29A0[0]) {
                        id1 = 0x2DE8;
                        text = func_00288FB8(0x2DE6);
                    } else if (D_001A8C88) {
                        id1 = 0x113B;
                        id2 = 0x1138;
                        func_00115DA8(buf, D_001A8AA8, func_00288FB8(0x2DE9), 1, 1,
                                      func_00288FB8(0x2DEB));
                        text = buf;
                    } else {
                        id1 = 0x113B;
                        func_00115DA8(buf, D_001A8AA8, func_00288FB8(0x2DE9), 1, 1,
                                      func_00288FB8(0x2DEB));
                        text = buf;
                        if (D_001A8C8C)
                            id1 = 0x113C;
                    }
                    break;
                case 21:
                    text = func_00288FB8((!D_001A8C88 && func_00287E90()) ? 0x2E04 : 0x2E03);
                    id1 = 0x1137;
                    break;
                case 20:
                    id1 = 0x1137;
                    text = func_00288FB8(0x2E02);
                    break;
                case 24:
                    id1 = 0x113B;
                    id2 = 0x1138;
                    func_00115DA8(buf, D_001A8AA8, func_00288FB8(0x2DF2), 1, 1,
                                  func_00288FB8(0x2DEB));
                    text = buf;
                    break;
                case 23:
                    id1 = 0x113B;
                    id2 = 0x1138;
                    text = func_00288FB8(0x2DF6);
                    break;
                case 3:
                case 4:
                    if (D_001A8C88 && !D_001F29A0[0]) {
                        id1 = 0x113B;
                        id2 = 0x1138;
                        text = func_00288FB8(0x2DEC);
                    } else if (D_001A8C8C) {
                        id1 = 0x113B;
                        text = func_00288FB8(0x2DEC);
                    } else if (D_001F29A0[0]) {
                        id1 = 0x2DE8;
                        text = func_00288FB8(0x2DE6);
                    } else {
                        id1 = 0x2DE8;
                        text = func_00288FB8(0x2DEE);
                    }
                    break;
                case 5:
                    id1 = 0x2DE8;
                    text = func_00288FB8(0x2DE6);
                    break;
                default:
                    show = 0;
                    break;
                }
            }
            if (show) {
                func_00115484(&tmp, 0, sizeof(tmp));
                tmp.f02 = D_001A7344;
                tmp.f04 = 0x60;
                tmp.f06 = 0x1A0;
                tmp.f08 = 0x100;
                tmp.f0A = 0x68;
                tmp.f10 = 0x10;
                tmp.flags = 1;
                box = tmp;
                func_0029B888(&box, 0x80F0F0F0, text, 1, 1, -1, 0,
                              id1 ? func_00288FB8(id1) : 0, id2 ? func_00288FB8(id2) : 0, 0);
            }
        }
        break;
    }
    case 3: {
        float t;
        int c1, c2, id;

        t = 1.0f - D_001BAC80.f44 * 0.033333335f;
        box = D_001A8AB8;
        func_0027EB88(0x64, 0x12C, 0x60, 0x1A0, (int)(t * 80.0f), 0x40404);
        c1 = func_00283D38(D_001A8A70, D_001A8A74, t);
        c2 = func_00283D38(0x20FFFF, 0x8020FFFF, t);
        switch (D_001BAC80.f58) {
        case 0:
        case 1:
            func_0027FAA8(0xCA, 0x118, c2, func_00288FB8(0x113B), -1);
            func_0027FAA8(0x135, 0x118, c2, func_00288FB8(0x1138), -1);
            id = 0x2F28;
            break;
        case 2:
            func_0027FAA8(0xCA, 0x118, c2, func_00288FB8(0x113B), -1);
            func_0027FAA8(0x135, 0x118, c2, func_00288FB8(0x1138), -1);
            id = 0x2F29;
            break;
        case 3:
            func_0027FAA8(0x100, 0x118, c2, func_00288FB8(0x1137), -1);
            id = 0x2F2A;
            break;
        default:
            id = 0;
            break;
        }
        if (id)
            func_0027FC70(&box, c1, func_00288FB8(id), -1, func_0027C878(1), D_00263B50);
        break;
    }
    case 9: {
        char *a, *b, *c;
        float k = 100.0f;
        int n = 3;
        float sc = D_001B23A4;
        float h = 0.5f;
        float f;
        int adjust = 0;
        MenuBox *pb;

        if (st != 0) {
            pb = &box;
            f = sc * k + h;
            func_002802E0(pb, 0xF0, 0x1E0, 0x60, 0x1A0, 0x100, (int)f + adjust, 0x10, n);
            a = D_001A8A4C;
        } else {
            pb = &box;
            f = sc * k + h;
            func_002802E0(pb, 0xF0, 0x1E0, 0x60, 0x1A0, 0x100, (int)f, 0x10, n);
            a = D_001A8A4C;
        }
        if (!a)
            a = func_00288FB8(0x113B);
        b = D_001A8A50;
        if (!b)
            b = func_00288FB8(0x1138);
        c = D_001A8A54;
        if (!c)
            c = func_00288FB8(0x2DCD);
        func_0029B888(pb, 0x80F0F0F0, c, 1, 1, -1, 0, a, b, D_001A8A58);
        break;
    }
    case 10: {
        char *a, *c;
        float k = 100.0f;
        int n = 3;
        float sc = D_001B23A4;
        float h = 0.5f;
        float f;
        int adjust = 0;
        MenuBox *pb;

        if (st != 0) {
            pb = &box;
            f = sc * k + h;
            func_002802E0(pb, 0xF0, 0x1E0, 0x60, 0x1A0, 0x100, (int)f + adjust, 0x10, n);
            a = D_001A8A4C;
        } else {
            pb = &box;
            f = sc * k + h;
            func_002802E0(pb, 0xF0, 0x1E0, 0x60, 0x1A0, 0x100, (int)f, 0x10, n);
            a = D_001A8A4C;
        }
        if (!a)
            a = func_00288FB8(0x113B);
        c = D_001A8A54;
        if (!c)
            c = func_00288FB8(0x2DCD);
        func_0029B888(pb, 0x80F0F0F0, c, 1, 1, -1, 0, a, 0, D_001A8A58);
        break;
    }
    case 11: {
        int enabled;
        char *a;
        float k;
        float sc;
        float h;
        float f;
        int adjust = 0;
        MenuBox *pb;

        do {
            k = 100.0f;
            sc = D_001B23A4;
            if (st != 0) {
                h = 0.5f;
                pb = &box + adjust;
            } else {
                h = 0.5f;
                pb = &box;
            }
            f = sc * k + h;
            enabled = 1;
            func_002802E0(pb, 0xF0, 0x1E0, 0x60, 0x1A0, 0x100, (int)f, 0x10, 3);
            switch (D_001BAC80.f58) {
            case 0:
                break;
            case 1: {
                char *b;

                a = func_00288FB8(0x1137);
                b = func_00288FB8(0x2805);
                func_00115DA8(buf2, func_00288FB8(0x31E6), 15);
                func_0029B888(pb, 0x80F0F0F0, buf2, enabled, enabled, -1, 0, a, b, 0);
                break;
            }
            case 2:
                break;
            case 3:
                D_001BAC80.f60 = D_001BAC80.f5C / 60 + 1;
                func_00115DA8(buf2, D_001A8AD0, func_00288FB8(0x31ED), func_00288FB8(0x31EE),
                              D_001BAC80.f60, func_00288FB8(0x31EF));
                a = func_00288FB8(0x113B);
                func_0029B888(pb, 0x80F0F0F0, buf2, enabled, enabled, -1, 0, a,
                              func_00288FB8(0x1138), 0);
                break;
            }
        } while (0);
        break;
    }
    }
    func_0027C660();
}
