/* Claude Opus 5.5 (xhigh), 5 November 2026: FAILED, 1496/1496 bytes,
 * 0/1 USA v1.01 places (111 cdiff lines; previous draft 142). Standalone game
 * unit profile: -O2 -G2 -ffunction-sections -mastra-quad-saves
 * (src/core/001185E8.c). Change over the 3 November draft: the integer-part
 * loop counts with its own local k (`k = 0; while (++k < 163)`) instead of e.
 * That lowers e's ref count in that loop and puts e in $s4 and eps/precision
 * in $s5, as retail does (all the $s4/$s5 rows are gone).
 * Left: (1) retail has a once-through loop note before the dispatch: it is a
 * sched2 barrier (param spills and all saves come before the extraction) and
 * gives $L17 at 0x678 the .p2align 3 nop at 0x674. A diagnostic
 * do { u.d = value; ... switch ... } while (0); around the dispatch alone
 * reproduces both and leaves 47 lines, but it is forbidden. Ending that loop
 * later (after the sign test, the integer part, len, rounding, '.', or the
 * whole body) gives 83-360 lines. (2) Integer loop store `p[n] = digit + '0'`:
 * retail gives the address $v0 and the value $v1, stores, puts the litodp a0
 * move in the jal slot and n++ after the call. Here sched1 issues the address
 * (insn 245, ready one cycle before the value) with the digit copy, so the
 * value has the shorter life, local-alloc gives it $v0, and the a0 move goes
 * before the sb. A shared int temp (`c = digit + '0'; p[n] = c;`, c also used
 * by the reverse loop) gives the address $v0 but puts c in $a1. These make no
 * difference: n++ placement, p[n++], an int temp used only here, (char)digit.
 * The value must come first in sched1 (as in the fraction loop, where digit
 * dies at the value insn), and the source form that does that is unknown.
 * All declarations follow.
 */
#include "common.h"

extern char D_0013C080[];

extern char D_0013A490[];
extern char D_0013A498[];
extern char D_0013A4A0[];
extern char D_0013A4A8[];

extern int func_00118460(char *buf, int len);

extern char *func_00118548(int value, char *buf, unsigned int base);

extern double func_00117650(double);

extern double func_00117848(double, double);

char *func_001185E8(double value, int ndigits, char ch, int dot) {
    union {
        double d;
        int i[2];
    } u;
    char *p;
    char c;
    double f;
    double eps;
    int e;
    int n;
    int i;
    int j;
    int digit;
    int len;
    int precision;
    int k;

    u.d = value;
    n = u.i[0];
    n &= ~0xFFFF;
    switch (n) {
    case 0x7FF00000:
        return D_0013A490;
    case 0xFFF00000:
        return D_0013A498;
    case 0x7FF80000:
    case 0x7FFC0000:
    case 0xFFFC0000:
        return D_0013A4A0;
    case 0xFFF80000:
        return D_0013A4A8;
    }

    precision = ndigits;
    k = 0;
    n = 0;
    if (value < 0) {
        D_0013C080[0] = '-';
        p = D_0013C080 + 1;
        value = -value;
    } else {
        p = D_0013C080;
    }

    if (value >= 1.0) {
        f = func_00117650(value);
        value = value - f;
        while (++k < 163) {
            digit = (int)func_00117848(f, 10.0);
            p[n] = digit + '0';
            f = (f - (double)digit) * 0.1;
            n++;
            if (f < 1.0)
                break;
        }
        for (digit = 0, j = n - 1; digit < j; digit++, j--) {
            c = p[digit];
            p[digit] = p[j];
            p[j] = c;
        }
    }
    e = n;

    if (e == 0) {
        if (ch != 'f') {
            if (value != 0) {
                while ((value *= 10.0) < 1.0 && e >= -1020)
                    e--;
                e--;
            }
            if (value >= 1.0) {
                value = value * 0.1;
                precision = precision + 1;
            }
        } else {
            p[0] = '0';
            n = 1;
        }
    }

    if (e < 0) {
        len = precision;
    } else if (ch != 'f') {
        len = precision + 1;
    } else {
        len = precision + (e > 0 ? e : 1);
    }

    eps = 5.5511151231257827e-17;
    for (;;) {
        value *= 10.0;
        digit = (int)value;
        value -= (double)digit;
        eps *= 10.0;
        if (n >= len || value < eps || value > 1.0 - eps)
            goto round;
        p[n++] = digit + '0';
    }
round:
    if (value >= 0.5)
        digit = digit + 1;
    p[n] = digit + '0';
    n++;
    while (n <= len) {
        p[n] = '0';
        n++;
    }
    if (func_00118460(p, len + 1) == 0) {
        p[0] = '1';
        e = e + 1;
        len = len + 1;
    }
    if (ndigits != 0) {
        i = 1;
        if (ch == 'f' && e > 0)
            i = e;
        for (n = len; i < n; n--)
            p[n] = p[n - 1];
        p[i] = '.';
    } else {
        len = len - 1;
    }
    if (dot == 0)
        goto skip;
    while (len != 0 && p[len] == '0') len--;
    if (p[len] == '.') len--;
skip:
    if (ch != 'f') {
        p[++len] = ch;
        if (e >= 0) {
            if (e != 0) e--;
            p[++len] = '+';
            if (e < 10) p[++len] = '0';
        } else {
            p[++len] = '-';
            if (e > -10) p[++len] = '0';
            e = -e;
        }
        func_00118548(e, p + ++len, 10);
    } else {
        p[len + 1] = 0;
    }
    return D_0013C080;
}
