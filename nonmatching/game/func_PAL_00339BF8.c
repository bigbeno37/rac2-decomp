/* func_PAL_00339BF8, the PAL copy of func_00337E98 (src/game/00336A78.c).
   Tier: opus-2, task v2712 (after opus-1, v2667). Flags: -O2 -G0 -ffunction-sections (the file's defaults).
   Best check.py: 460/460 bytes, 0/28 places, 1 instruction differs, at +0x38:
     retail daddu $s2, $s3, $zero    this C daddu $s2, $zero, $zero
   So `i` starts as a copy of the register holding `length` (0), not as a literal.
   PAL's own changes, both exact here: the store of the second argument at +0x1C4, and the
   line terminator after the loop. The memset prototype must return void * (a void one swaps
   $v0/$v1 in the line-table store, as an ignored call_value changes allocation).

   New in v2712: the plain for loop below gives the same bytes as the older form, so PAL does not
   need the `if (box->text[0] != 0)` guard or the `next` pointer that the USA template has.
   These all give the literal, or are further away: for (i = length; ...); length = 0 moved after
   the calls; i = 0 then length = i (3 rows differ, order of the sets); single = length + 1; a
   while (1) with break (464 bytes). That is a tie, so a further rewording will not help.

   Measured with cc1 pass dumps: with `do { } while (0); i = length;` at the top of the body (a
   probe only, an empty group is not allowed, and the result is still the literal),
   (set i (reg length)) survives cse1, gcse and loop, and only cse2 folds it to const_int 0. A
   real label instead hands it to gcse's cprop, which folds it across blocks. So retail's copy
   has to stay hidden from cse2 while gcse sees no block boundary.
   Read in the compiler source, none of these can make the copy from `i = 0`: reload_cse
   (reload_cse_simplify_set returns early for rtx_cost < 2, reload_cse_simplify_operands has the
   cheap CONST_INT guard, CONST_COSTS is 0), cse_insn's cost order (src_folded wins ties),
   cse_set_around_loop (needs REG_LOOP_TEST_P and COST (reg) < COST (src)). No mips peephole was
   read in full: there are four define_peepholes at mips.md:11155 and they are unchecked.
   Idiom scan of USA boot and level code (saved register set to a small constant, then copied to
   another saved register with no branch or branch target between): 7 hits, none with value 0.
   The two read at C level, func_L15_00438518 (if (alpha > 90) alpha = 90) and func_L00_00335558
   (if (!r) r = 1), look like conditional-move copies; their disassembly was not checked. Retail
   has no movn/movz near +0x38 here.
   Lead: a construct that blinds cse2 only and leaves no instruction. Open category in
   docs/MATCHING-BLOCKERS.md, "A cse block boundary that leaves no instruction".
   Needs: only the declarations below. The extern for func_PAL_00339BF8 in the unit takes
   void *; drop it when this definition goes in, as the definition precedes its callers. */

extern int func_0027F178(unsigned char *text, int length); /* text width */
extern void *func_00115484(void *, int, int);

/* Text box laid out by func_PAL_00339B88: text at +0xA8, its line table at +0x1D8. */
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

void func_PAL_00339BF8(TextBox *box, int mode, int duration, float x, float y)
{
    unsigned char line[0x100];
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
    box->lines[box->lineCount - 1] = box->text;
    single = 1;
    for (i = 0; i < 0xFF && box->text[i] != 0; i++) {
        if (box->text[i] == 1) {
            line[length] = 0;
            box->text[i] = 0;
            single = 0;
            length = 0;
            box->lines[box->lineCount] = &box->text[i + 1];
            box->lineCount++;
            width = func_0027F178(line, -1);
            box->maxWidth = (box->maxWidth < width) ? width : box->maxWidth;
        } else {
            line[length] = box->text[i];
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
