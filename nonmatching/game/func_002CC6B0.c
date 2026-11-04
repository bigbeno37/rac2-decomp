/* Claude Opus 5.5, xhigh effort, 5 November 2026 (task v2500).
 * USA v1.01, current pinned game compiler, -O2 -G8 -ffunction-sections
 * (dedicated unit src/game/002CC6B0-g8.c; take the INCLUDE_ASM marker out of
 * src/game/002C7748.c first). check.py: 2076/2076 bytes, 0/28 places
 * (first difference at +0x6a0). 45 rows differ once relocations are ignored,
 * in three places: +0x6A0..+0x6EC, +0x73C..+0x780 and +0x790..+0x7DC.
 *
 * New here:
 * - func_0029DB28 has its real prototype (it returns int, see
 *   src/game/0029DB28.c). The ignored call_value moves helpResult's reload to
 *   $v1, as in retail (+0x474).
 * - D_001AB984 is declared `extern const` and read as `*(int *)&D_001AB984`.
 *   The cast drops the scalar flag from the MEM, so sched1 keeps the tail's
 *   gap load below `so += k`, and flow2 keeps that intermediate store: the
 *   size is now right. sched2 keeps it there as well, though, while retail
 *   hoists the load to the top of the block (+0x790). So retail's load must
 *   have been pinned below the store at sched1 and free at sched2. The two
 *   schedulers use the same alias rules here, and a block boundary between
 *   them cannot be it: flow2's tidy_fallthru_edge keeps the label, and jump2
 *   runs only after sched2. A fixed-scalar load (plain extern int) is hoisted
 *   by sched1 and flow2 then deletes the store (2072 bytes).
 *
 * Still different:
 * (1) +0x6A0: in retail win.s[0] is reloaded into $v0 right next to its
 *     self-store and the y chain is in $a0. Local-alloc only gives that
 *     order if the s[0] load sits next to its store at allocation time,
 *     but sched1 ties it (prio 4) with the flags load and LUID order puts
 *     it first. The helper's argument order, direct statements (cse drops
 *     `win.s[0] = win.s[0]`) and a temporary all give the same bytes.
 * (2) +0x73C: retail has gap in $a0 and s[5] in $t2, so the $a1..$t1
 *     argument moves are set during s[5]'s life and only $a0's comes late.
 *     In ours sched1 puts `$a0 = &win` at t=1 (a free ALU slot). A probe
 *     test between the stores and the call (identical arms, not landable)
 *     frees the argument registers but gives gap $a1.
 * (3) +0x790: the gap load is below the intermediate store (see above).
 * Tried without success: one shared gap local (it gets a stack slot, and the
 * whole frame moves); a flow1-dead arm (jump1 removes it); an identical-arm
 * test around the `%=` (it keeps the store, but sched2 cannot hoist across
 * the arms; probe only).
 * No volatile, register pins or barriers.
 *
 * Earlier attempts (opus-1, sol-high, sol-low): RAC1 render_configured_text_label
 * counterpart (src/game/pause.c, GPL-3.0) adapted to RAC2; the item table at
 * +0x164 is indexed directly; the scroll tail follows RAC1 with a
 * struct-member field.
 */
#include "common.h"
typedef struct { short s[12]; } Win;
typedef struct { char pad[0x3C]; int so; } LabelSO;
extern char D_001AB988[], D_001AB990[], D_001AB998[], D_001AB9A0[];
extern unsigned char D_00263B50[], D_00264290[], D_00263EF0[], D_00139568[];
extern unsigned char D_001A7AF8[] MACRO_ADDR, D_001395B8[] MACRO_ADDR, D_001A7A60[] MACRO_ADDR;
extern int D_001C51D0 NOT_SDA;
extern int D_001A7B90[] MACRO_ADDR;
extern int D_001A79F0 __attribute__((sda));
extern int D_001AA440 __attribute__((sda));
extern const int D_001AB984 __attribute__((sda));
extern int D_001AB9A8 __attribute__((sda));
extern int D_001AB974 __attribute__((sda));
extern int D_001AB9AC __attribute__((sda));
extern int D_00138320[];
extern char *D_001F2854 NOT_SDA;
extern char D_0025A498[];
extern void func_002FC560(int, long);
extern int func_0029DB28(int *, int *, int *, int);
extern void func_00289FC8(int, int, void *);
typedef struct { unsigned char pad[4]; unsigned char available; unsigned char tail[0xDB]; } ItemConfig;
extern ItemConfig D_00239BA0[];
extern int func_00115DA8(char *, const char *, ...);
extern void *func_00115484(void *, int, int);
extern void func_0027C540(int);
extern long func_0027C878(int);
extern void func_0027C660(void), func_0027F120(void), func_0027F110(void);
extern char *func_00288FB8(int);
extern void func_0027FC70(void *, long, char *, long, long, unsigned char *);
extern void func_0027ECC8(int, int, int, int, int);
#define ST (*(char **)(D_001F2854 + 0xE8))
#define TEXT(id) func_00288FB8(id)
#define DRAW(c,t) func_0027FC70(&win,c,t,-1,tex,glyphs)
static inline int translated_edge(int extent, int origin) { return extent + origin; }
static inline void set_window_vertical(Win *p, unsigned short anchor, unsigned short top) { p->s[5] = anchor; p->s[0] = top; }
int func_002CC6B0(unsigned char *w) {
    char buf[0x40];
    int id;
    int helpResult;
    int originY, originX;
    Win win;
    Win tmp;
    char *text = D_001AB988;
    unsigned char *glyphs = D_00263B50;
    int font;
    int f0, flags, g, f, content, variant = 0, a21, a23, colour, t, wflags;
    /* func_0027C878's TEX0 is 64 bits, and FontPrintWindow reads all of them: on the console the
       register keeps them whatever the C says, on a PC an int keeps only the low half. */
    long tex;

    font = 1;
    f0 = *(int *)(w + 0x30);
    if (f0 & 8) {
        font = 3;
        glyphs = D_00264290;
    }
    if (f0 & 0x10) {
        font = 2;
        glyphs = D_00263EF0;
    }
    func_002FC560(0x42, 0x44);
    ((void (*)(int, long))func_002FC560)(0x47, 0x2004B);

    flags = *(int *)(w + 0x30);
    if (flags & 0x20) {
        content = D_001A79F0 - 1;
        if ((unsigned int)content >= 0x12) {
            content = -1;
        }
    } else if (flags & 0x40) {
        content = D_001C51D0 - 1;
    } else if (flags & 4) {
        t = D_001AA440;
        if (*(int *)(w + 0x44) < t) {
            *(int *)(w + 0x44) = D_001AA440;
        }
        content = 0;
        *(int *)(w + 0x48) = 0;
        *(int *)(w + 0x4C) = 0;
    } else if (flags & 0x80) {
        content = *(int *)(ST + 0x40);
        if (flags & 0x8000) {
            variant = D_001A7B90[0] != 0;
        }
    } else if (flags & 0x100) {
        char *cell;
        content = *(int *)(ST + 0x3C);
        cell = *(char **)(ST + 0x48) + content * 10;
        if (*(short *)(cell + 4) == 0) {
            id = D_001A7AF8[*(short *)(cell + 6)];
        } else {
            id = D_001395B8[*(short *)(cell + 6)];
        }
        if (id == 0) {
            content = -1;
            t = 0;
        }
    } else if (flags & 0x1000) {
        char *st = ST;
        content = *(int *)(st + 0x40);
        id = *(short *)(*(char **)(st + 0x34) + content * 12);
        *(int *)(w + 0x34) = 0xFFFF;
        func_00289FC8(id, 0, w + 0x34);
    } else {
        char *cell;
        int k;
        cell = (char *)(*(int *)(ST + 0x3C) * 10 + *(int *)(ST + 0x48));
        k = *(short *)(cell + 6);
        variant = 0;
        if (*(short *)(cell + 4) == 0)
            variant = ((ItemConfig *)(D_00139568[k] * 0xE0 + (int)D_00239BA0))->available != 0;
        content = k;
    }

    if (*(int *)(w + 0x44) == -1) {
        *(int *)(w + 0x44) = D_001AA440;
        *(int *)(w + 0x48) = content;
        *(int *)(w + 0x4C) = variant;
    }
    if (content != *(int *)(w + 0x48)) {
        int fade;
        t = D_001AA440;
        if (t < *(int *)(w + 0x44)) {
            *(int *)(w + 0x44) = D_001AA440;
        }
        fade = *(int *)(w + 0x44);
        { int was = fade < 1; fade--; if (was) fade = 0; }
        { int was = fade < 1; fade--; if (was) fade = 0; }
        { int was = fade < 1; fade--; if (was) fade = 0; }
        *(int *)(w + 0x44) = fade;
        if (fade != 0) {
            content = *(int *)(w + 0x48);
            variant = *(int *)(w + 0x4C);
        } else {
            *(int *)(w + 0x48) = content;
            *(int *)(w + 0x4C) = variant;
            *(int *)(w + 0x30) &= ~0x400;
            *(int *)(w + 0x3C) = 0;
        }
    } else {
        *(int *)(w + 0x44) += 3;
    }

    g = *(int *)(w + 0x30);
    if (g & 4) {
        if (*(int *)(w + 0x34) == 0) {
            return 1;
        }
        text = TEXT(*(int *)(w + 0x34));
    } else if (g & 0x1000) {
        if (*(int *)(w + 0x34) == 0xFFFF) {
            return 1;
        }
        if (*(int *)(w + 0x34) == 0) return 1;
        text = TEXT(*(int *)(w + 0x34));
    } else if ((g & 0x100) && content == -1) {
        text = D_001AB990;
    } else if (*(int *)(w + 0x34) != 0) {
        unsigned int at = (unsigned int)(content * *(int *)(w + 0x38)) >> 2 << 2;
        char *table = *(char **)(w + 0x34);
        text = TEXT(*(int *)(at + (unsigned int)(table + variant * 4)));
    }
    if ((*(int *)(w + 0x30) & 0x11E4) == 0 && ((unsigned char *)&D_001A7AF8)[content] == 0) {
        text = D_001AB990;
    }
    if (*(int *)(w + 0x30) & 0x200) {
        unsigned int at = (unsigned int)(content * *(int *)(w + 0x38)) >> 2 << 2;
        char *table = *(char **)(w + 0x34);
        int id = *(int *)(at + (unsigned int)table);
        if (id != 0xB4B && id != 0xB52 && id != 0xB56) {
            func_00115DA8(buf, D_001AB998, TEXT(0xB44), text);
            text = buf;
        }
    }
    f = *(int *)(w + 0x30);
    a23 = *(int *)(w + 0x18) + *(int *)(w + 0x20) / 2;
    a21 = *(int *)(w + 0x1C) + *(int *)(w + 0x24) / 2;
    if ((f & 0x4004) == 0x4004 && *(int *)(w + 0x34) == 0x112D) {
        func_0029DB28((int *)0x70000000, &helpResult, 0, 0);
        if (helpResult < 0) text = TEXT(0x112C);
        a21 += 8;
    }
    if (*(int *)(w + 0x30) & 0x800) if (D_001A7A60[content] == 0) {
        text = TEXT(0x2DC0);
        f |= 3;
    }
    if (text == 0) {
        text = D_001AB9A0;
    }
    wflags = 8;
    if (f & 1) {
        wflags = 9;
        a23 = *(int *)(w + 0x20) / 2;
    }
    if (f & 2) {
        wflags |= 2;
        a21 = *(int *)(w + 0x24) / 2;
    }

    func_0027C540(0);
    tex = func_0027C878(font);
    if (D_001F2854 == D_0025A498 || D_001AB9A8 != 0) { originY = a21; originX = a23 + 1; } else { originX = 1; originY = D_001AB974; }
    func_00115484(&tmp, 0, 0x18);
    tmp.s[0] = originY;
    tmp.s[1] = *(unsigned short *)(w + 0x24) + originY;
    tmp.s[2] = originX;
    tmp.s[3] = translated_edge(*(unsigned short *)(w + 0x20), originX - 4);
    tmp.s[4] = a23;
    tmp.s[5] = a21 - (*(int *)(w + 0x3C) >> 4);
    tmp.s[8] = *(int *)&D_001AB984;
    tmp.s[9] = wflags;
    tmp.s[11] = -(*(unsigned short *)(w + 0x3C) & 0xF);
    win = tmp;
    if (D_001AB9AC) func_0027ECC8(win.s[0], win.s[1], win.s[2], win.s[3], 0x40303030);
    if (*(int *)(w + 0x30) & 0x10000) {
        win.s[1] = *(unsigned short *)(w + 0x24) - 1;
    }
    colour = 0x80F0F0F0;
    win.s[9] |= 4;
    DRAW(colour, text);
    win.s[9] ^= 4;
    if (!(*(int *)(w + 0x30) & 0x2000) && !(win.s[7] + 4 < win.s[1] - win.s[0])) {
        if (!(*(int *)(w + 0x30) & 0x400)) {
            *(int *)(w + 0x30) |= 0x400;

        }
    } else {
        if (*(int *)(w + 0x30) & 0x400)
            *(int *)(w + 0x30) ^= 0x400;
        *(int *)(w + 0x3C) = 0;
    }

    {
        int y = a21 - (*(int *)(w + 0x3C) >> 4);
        set_window_vertical(&win, y, win.s[0]);
    }
    if (*(int *)(w + 0x30) & 0x20000) {
        func_0027F120();
    }
    DRAW(colour, text);
    if (*(int *)(w + 0x30) & 0x20000) {
        func_0027F110();
    }
    if (*(int *)(w + 0x30) & 0x400) {
        set_window_vertical(&win, win.s[5] + (win.s[7] + *(int *)&D_001AB984 * 3), win.s[0]);
        DRAW(colour, text);
        if (*(int *)(w + 0x30) & 0x400) {
            ((LabelSO *)w)->so += (D_00138320[0] & 0xF) == 0xF ? 10 : 3;
            ((LabelSO *)w)->so %= (win.s[7] + *(int *)&D_001AB984 * 3) << 4;
        }
    }
    func_0027C660();
    return 2;
}
