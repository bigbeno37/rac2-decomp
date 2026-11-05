/* func_PAL_00339BF8, the PAL copy of func_00337E98 (src/game/00336A78.c).
   Tier: opus-1, task v2667. Flags: -O2 -G0 -ffunction-sections (the file's defaults).
   Best check.py: 460/460 bytes, 0/28 places, 1 instruction differs, at +0x38:
     retail daddu $s2, $s3, $zero    this C daddu $s2, $zero, $zero
   So `i` starts as a copy of the register holding `length` (0), not as a literal.
   PAL's own changes, both exact here: the store of the second argument at +0x1C4, and the
   line terminator after the loop. The memset prototype must return void * (a void one swaps
   $v0/$v1 in the line-table store, as an ignored call_value changes allocation).
   Tried, all give the literal: i = length; i = length = 0; int i = length; for (i = 0; ...).
   Ruled out from the compiler source and dumps: cse1 folds the copy (cprop reports 0 const
   and 0 copy props); reload_cse keeps a const_int 0 (cheap CONST_INT guard in
   reload_cse_simplify_operands, rtx_cost < 2 in reload_cse_simplify_set); cse_set_around_loop
   needs COST (reg) < COST (const), and that cost is 0. No USA level 00 function has this
   idiom with a saved-register destination, so a matched example was not available.
   Lead: the copy must exist as RTL after cse2 or come from a value cse cannot see as 0.
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
    i = 0;
    if (box->text[0] != 0) {
        unsigned char *next = &box->text[1];

        for (; i < 0xFF && box->text[i] != 0; i++, next++) {
            if (box->text[i] == 1) {
                line[length] = 0;
                box->text[i] = 0;
                single = 0;
                length = 0;
                box->lines[box->lineCount] = next;
                box->lineCount++;
                width = func_0027F178(line, -1);
                box->maxWidth = (box->maxWidth < width) ? width : box->maxWidth;
            } else {
                line[length] = box->text[i];
                length++;
            }
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
