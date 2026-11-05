/* func_US2_00338D48 (464 bytes, 28 places), USA v2.00's copy of func_00337E98.
   Tier: Claude Opus 5.5, xhigh. Unit flags: -O2 -G0 -ffunction-sections (the default for its file).

   Best check.py: 464/464 bytes, 0 of 28 places, 1 instruction differs. Compare with
   cdiff.py <checkout> func_US2_00338D48 --version us2: it lists 6 rows, 5 of them only the
   unresolved jal targets of the local object.

   What still differs: +0x38 (00338D80). Retail initialises i with `daddu s3,s4,zero`, a copy of
   length's register; this C gives `daddu s3,zero,zero`. Everything else is exact, including the
   registers, the frame and the delay slots.

   What was tried for that row, all the same bytes (a tie): i = 0; i = length; i = length = 0 in
   the for init; length set just before the loop, with i = 0 or i = length. `int i = 0` declared
   first is worse (8 rows).

   Why it is hard, from the compiler source: cse and cse2 replace a register copy by its known
   constant, reload_cse never turns a cheap CONST_INT into a register (reload_cse_simplify_set,
   reload_cse_simplify_operands), and cse_set_around_loop needs COST(reg) < COST(src), which is
   false for a constant. A scan of the v2.00 boot program for `daddu sX,zero,zero` followed by
   `daddu sY,sX,zero` in one block finds this function and no other, out of 4,924 zero loads, so
   the cause is something unusual here. It is not yet known.

   What the v2.00 source changes, each one measured:
   - new first statement: box->mode = mode (+0x1C4), also stored by func_US2_00338CD8;
   - after the loop: if (i < 0xFF && box->text[i] == 0) line[length] = 0;
   - the loop walks a pointer. text[i] and text[i + 1] are reduced by loop.c, and their setup
     insns land after the hoisted constant 1 and box->lines, which is what shares s1 between
     box->lines and the walking pointer. That takes the local `text` with &box->text[i + 1].
     box->text[i] in the loop leaves box + 0xA8 in the loop test, cse_around_loop keeps it in
     the loop, and nothing is reduced. An explicit pointer or next biv gets the bytes but sets
     up before the hoists, so the registers are wrong.
   - the outer if (box->text[0] != 0) of func_00337E98 is not needed: the plain for gives it.

   Needs: the TextBox typedef below, func_0027F178 and func_00115484 as declared in
   src/us2/game/00338AB8.c. Callers must pass (TextBox *). */

extern void *func_00115484(void *, int, unsigned int);
extern int func_0027F178(unsigned char *text, int length); /* text width */

/* Text box laid out by func_US2_00338CD8: text at +0xA8, its line table at +0x1D8. */
typedef struct {
    float *position;
    float *scale;
    unsigned char pad08[0xA0];
    unsigned char text[0x100];
    int unused1A8;
    int unused1AC;
    int lineCount;
    int unused1B4;
    int duration;
    unsigned char pad1BC[8];
    int mode;
    unsigned char pad1C8[0xC];
    int maxWidth;
    unsigned char *lines[30];
} TextBox;

/* func_US2_00338D48, the v2.00 copy of func_00337E98. */
void func_US2_00338D48(TextBox *box, int mode, int duration, float x, float y)
{
    unsigned char line[0x100];
    unsigned char *text;
    int length = 0;
    int single;
    int i;
    int width;

    box->mode = mode;
    box->position[0] = x;
    box->position[1] = y;
    box->duration = duration;
    box->text[0xFF] = 0;
    box->maxWidth = 0;
    box->lineCount = 1;
    func_00115484(line, 0, sizeof(line));
    func_00115484(box->lines, 0, sizeof(box->lines));
    text = box->text;
    box->lines[box->lineCount - 1] = text;
    single = 1;
    for (i = 0; i < 0xFF && text[i] != 0; i++) {
        if (text[i] == 1) {
            line[length] = 0;
            text[i] = 0;
            single = 0;
            length = 0;
            box->lines[box->lineCount] = &box->text[i + 1];
            box->lineCount++;
            width = func_0027F178(line, -1);
            box->maxWidth = (box->maxWidth < width) ? width : box->maxWidth;
        } else {
            line[length] = text[i];
            length++;
        }
    }
    if (i < 0xFF && box->text[i] == 0) {
        line[length] = 0;
    }
    if (single) {
        box->maxWidth = func_0027F178(box->text, -1);
    } else {
        width = func_0027F178(line, -1);
        box->maxWidth = (box->maxWidth < width) ? width : box->maxWidth;
    }
    box->scale[0] = 1.0f;
}
