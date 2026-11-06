/* func_PAL_L08_003DC0D0, the PAL copy of func_L08_003D7FB8 (unit flags -O2 -G0 -ffunction-sections
 * -frac2-qcopy -frac2-qcopy-barrier). Tier: opus-1, task v3020.
 * Best check.py: 768/768 bytes, 0 of 1 places; 3 instructions differ, all at the last call:
 *   retail: addiu v1,zero,-1 / daddu a3,s1 / sw v1,0(sp) / daddu t0,s0 / daddu t3,v0
 *   this C: daddu t3,v0 / daddu a3,s1 / addiu v0,zero,-1 / daddu t0,s0 / sw v0,0(sp)
 * Everything else (constants, hoists, f28/f29, frame, order) is exact.
 *
 * Findings, measured from x.i.loop and -fsched-verbose-5:
 * - Retail hoists 5.0f, in the second loop pass after 0.065f (preheader order and f28/f29 show it),
 *   but not 2.0f or 1/6. threshold is 61 and drops by 3 per move, so that pass needs at most 58
 *   real insns in the inner loop; the plain template gives 59. Literal -1 is the only two-insn pair
 *   that can leave: `int none = -1` set outside the inner loop gives 58, the pseudo finds no saved
 *   GPR (all nine are used) and reload rematerialises it at the store.
 * - What is left is sched1: with a hoisted -1 the stack store has weight +1 (the source does not
 *   die), so it sorts after t3=v0 (weight 0), v0 is free at reload and find_reg takes it. Retail
 *   behaves as if v0 were live at the store (or the store's source died there, as in the template).
 * - Same bytes with `none` at function, outer-body or inner-body level, and with a named `life`
 *   local for the func_00283CF0 result.
 * Tried and rejected: a 5.0f local (hoists in pass 1, wrong order, and 2.0f follows into f30); an
 * inline scale helper (same RTL count); a one-pass group (loop.c: "phony", loses the shape).
 * A fix needs the store ahead of t3=v0 at sched1 with no extra RTL insn in the inner loop.
 */

/* func_PAL_L08_003DC0D0, the PAL copy of func_L08_003D7FB8: the same C at 50 Hz. */

typedef int TI128 __attribute__((mode(TI)));
typedef int s32;

extern void func_002ACB48(float *, float);
extern float func_002A7940(void);
extern float func_002A7878(float, float);
extern void func_00282D30(void *, void *, float);
extern float func_00283180(float);
extern float func_00283198(float);
extern int func_00283D38(int, int, float);
extern int func_00283CF0(float);
extern void func_002BAA28(void *, void *, void *, int, int, int, int, int, int);

void func_PAL_L08_003DC0D0(char *arg0, char *arg1) {
    float a[4];
    float b[4];
    float c[4];
    int n;
    int i;
    float angle;
    float r;
    float z;
    int col1;
    int col2;
    int none = -1;
    char *src = arg0 + 0x10;

    for (n = 0; n < 20; n++) {
        *(TI128 *)a = *(TI128 *)src;
        func_002ACB48(a, 0.25f);
        a[2] += 0.25f;
        angle = func_002A7940();
        r = func_002A7878(1.0f, 8.0f);
        z = func_002A7878(2.0f, 6.0f);
        func_00282D30(b, arg1, func_002A7878(0.0f, 0.0f) * 0.020000001f);
        b[0] += func_00283180(angle) * 0.020000001f * r;
        b[1] += func_00283198(angle) * 0.020000001f * r;
        b[2] += z * 0.020000001f;
        *(TI128 *)c = *(TI128 *)b;
        c[2] -= 0.2f;
        for (i = 0; i < 10; i++) {
            float k = func_002A7878(0.5f, 1.5f);

            b[3] = k * 0.125f;
            c[3] = k * 0.065f;
            func_002ACB48(a, 0.125f);
            func_002ACB48(c, 0.010000001f);
            col1 = func_00283D38(0x8000EEEE, 0x8000FF90, func_002A7878(0.0f, 1.0f));
            col2 = func_00283D38(0xFFEE, 0xFFEE, func_002A7878(0.0f, 1.0f));
            func_002BAA28(a, b, c, col1, col2, ((s32)(10) * 5 + 2) / 6, ((s32)(30) * 5 + 2) / 6, func_00283CF0((func_002A7878(15.0f, 75.0f) * 5.0f + 2.0f) * 0.16666667f), none);
        }
    }
}
