/*
 * func_PAL_L00_003EED30: EXACT C, held out of src/ only by a catalogue defect. Tier: opus-1
 * (Claude Opus 5.5), task v2673. It is the PAL copy of func_L00_003EAA58: the same C, with
 * 0.25f -> 0.3f, 0.00305556f -> 0.0044f, and the 50 Hz counts 40 -> 33, 20 -> 17, 10 -> 8.
 *
 * Best check.py --version pal: 0 of 27 places, "560 bytes, expected 556". No instruction differs.
 * With the size test bypassed, build.Placement.derive(obj, section, address) accepts all 560
 * bytes at all 27 places and gives every symbol one value, with these default flags
 * (-O2 -G0 -ffunction-sections) and with the template's -g.
 *
 * The real function is 560 bytes. At all 27 places +552 is `jr $ra`, +556 is its delay slot
 * `addiu $sp, $sp, 0x70` (27BD0070), and the next row, func_PAL_L00_003EEF60, starts at +560.
 * The row keeps v1.01's 556: 0.25f takes one instruction and 0.3f takes two. v1.01's next
 * function is also 560 bytes on, after 4 bytes of padding, so the stretches are equally long
 * and tools/carry.py step 2 appears to have copied the cut. The linked build allows a size
 * difference only when the extra bytes are zero, so it rejects this one as well.
 *
 * To land: make the row 560 bytes, with its fingerprint, then replace the INCLUDE_ASM line in
 * src/pal/levels/shared/L00_003EED30.c with the C below. It needs no new unit or flags line.
 *
 * Corrected from the tool's draft: the callback is func_PAL_L00_003EEF60, not the draft's
 * func_L00_003EAC88, which has no PAL row. That function is defined later in the same file with
 * a typed owner pointer, hence the unprototyped declaration. It sits at +560 at all 27 places,
 * so the same-file reference is placed everywhere this function is.
 */
extern void func_PAL_L00_003EEF60();
/* Partial resident state layout measured from this handler's accesses. */
typedef struct {
    char p0[0xBF8];
    int field_BF8;
    char p1[0x1230 - 0xBFC];
    int field_1230;
    char p2[0x1240 - 0x1234];
    int field_1240;
    char p3[0x1450 - 0x1244];
    int field_1450;
    char p4[0x2294 - 0x1454];
    int field_2294;
    char p5[0x22BC - 0x2298];
    unsigned char field_22BC;
    char p6[0x2534 - 0x22BD];
    int field_2534;
} HeroState;
extern HeroState D_00189E20;

extern char D_00138180[];
extern void func_0029FCC0(char *, int, void *), func_002FDF40(void *, void *, float, float, float, float, int, int);
extern int func_PAL_002FF5F8(void *, void *, void *, void *, char *, int, int, float);
extern int func_00282948(void *), func_L00_002D3F30(int, int);
extern void func_PAL_L00_002AF458(void), func_002E5CF8(int, int, char *), func_PAL_L00_002AF6C0(void), ref_L00_003EAA58_00(void *, char *);
extern char *func_PAL_L00_0038ACC8(void *, void *);
void func_PAL_L00_003EED30(char *self)
{
    char a[16], b[16];
    char *bp;
    char *st = *(char **)(self + 0x68);
    int ready;
    float scale;
    char *shot;
    func_0029FCC0(self, 0, a);
    bp = b;
    scale  =  D_00189E20.field_2294 == 29 ? 11.0f : 4.5f;
    func_002FDF40(bp, a, 1.5f, scale, 0.3f, 0.0044f, 0, 0);
    *(int *)(st + 0x24) = func_PAL_002FF5F8(a, bp, st, st + 0x10, self, 0, 0, 0.0044f);
    ready = 1;
    if (D_00189E20.field_2294 == 29) {
        if (!func_00282948(&D_00189E20.field_2534)) ready = 0;
    } else D_00189E20.field_2534 = 33;
    switch (*(unsigned char *)(self + 0x20)) {
    case 0:
        self[0x20] = 1;
        *(int *)(st + 0x24) = 0;
        /* Fall through. */
    case 1:
        if ((*(int *)(D_00138180 + 0x1A4) & D_00189E20.field_1230) &&
            !D_00189E20.field_22BC && ready) {
            if (func_L00_002D3F30(-1, 1)) {
                func_PAL_L00_002AF458();
                self[0x20] = 2;
                *(int *)(st + 0x20) = 17;
            } else func_002E5CF8(2, 0, self);
        }
        break;
    case 2:
        if (func_00282948(st + 0x20)) {
            func_002E5CF8(0, 0, self);
            shot = func_PAL_L00_0038ACC8(a, bp);
            *(char **)(shot + 0xB8) = self;
            func_PAL_L00_002AF6C0();
            self[0x20] = 3;
            *(int *)(st + 0x20) = 8;
        }
        break;
    case 3:
        if (func_00282948(st + 0x20)) self[0x20] = 1;
        break;
    }
    ref_L00_003EAA58_00(func_PAL_L00_003EEF60, self);
}
