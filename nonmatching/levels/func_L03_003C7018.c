/* v2574, opus-2, Claude Opus 5.5, USA v1.01, 2026-11-05. FAILED: not a match, third and last
   Opus attempt. Unit of its own with default flags (-O2 -G0 -ffunction-sections); the body below
   is allowed C only: 656/656 bytes, 0/1 places, 72 of 164 rows differ with relocations masked
   (77 for the previous draft on the same count). No exact-bytes form was found, allowed or not.

   Declarations: D_L03_001B0164 is the y column read at stride 8 and is MACRO_ADDR: it gives
   retail's adjacent `lui $s6; addiu $s6,$s6` and frees $v1/$a0/$a2 for the keys copy. The linked
   build has not been run on this name.

   What differs: all three (float)(int) sites (both draws and the loop's y) have a scheduling
   boundary right after cvt.s.w, and $f21/$f20 and $s4/$s3 are swapped (half/zero, i/buffer).
   Before each draw retail has only the row computation and `move a0,v0`; x, colour, zero, w and
   h all come after it, and the row's int temporary is in $v1. cc1 instead fills the
   mul/add/cvt stalls with those argument loads and with hoisted loop constants.
   Everything but the boundary now agrees: with it the loop body, frame, s0-s7 and fp fall in place.

   Measured (forbidden forms are probes, none landed):
   - do { real call; } while (0) around each draw, after `row = ...` (forbidden by WORKER2.md,
     although docs/COMPILER-NOTES.md:54 allows a nonempty group): 53/164 rows, default flags.
     A LOOP note is a sched1/sched2 barrier that cse and gcse do not see. It gives $v1, zero after
     the barrier and the loop registers, but not exact: combine still folds text and row across
     the note, so a0=v0 and the cvt.s.w land after it, and the note after the jal stops `i=0`
     reaching the jal delay slot. Wrapping only the row computation instead: 67/164.
   - if (y0 != 0 && xx != 0) row = 1.0f; with xx unset (forbidden, v2558's two-predecessor split):
     real blocks, so text/row stay pseudos and half gets $f20, but `text` then takes $v0 by copy
     preference (retail $a0) and the label costs cse and cprop: per-use 0x8710 and mtc1 zero,$f16
     (644 bytes). An offset local restores $s0.
   - if (text > 0) draw; else draw; (forbidden): 724 bytes, the call arms never cross-jump.
   - -fno-expensive-optimizations: no effect on this body (72 either way).
   - -fno-gcse: keeps the zero pseudo with a split, but loses retail's PRE of sp+0x10.
   - `int i = 0;` before the keys, a buffer-pointer local, or a struct copy of the keys: no gain.
     i=0 is the one post-call insn sched2 pulls over the jal, so a plain for after the keys fits.

   Rule for the next reader: retail shares zero and 0x8710 across the boundaries, as if gcse saw no
   block there, yet text and row survive combine, as if a real block ended there. Notes give only
   the first, a two-predecessor test only the second. The missing construct must do both. The
   same split shows in func_L05_0037A5F8 (see its draft). */
#include "common.h"

struct MenuCoordinate { float x, y; };
extern unsigned char *D_L03_001A904C __attribute__((sda));
extern float D_L03_001B014C SDATA(D_L03_001B014C);
extern float D_L03_001B0150 SDATA(D_L03_001B0150);
extern float D_L03_001B0154 SDATA(D_L03_001B0154);
extern float D_L03_001B0158 SDATA(D_L03_001B0158);
extern struct MenuCoordinate D_L03_001B0164[] MACRO_ADDR;
extern struct MenuCoordinate D_L03_001B0160_gp[] SDATA(D_L03_001B0160);
extern int D_L03_001B0140 SDATA(D_L03_001B0140);
extern int D_L03_001B3280 __attribute__((sda));
extern float D_L03_001B3284 __attribute__((sda));
extern char D_L03_001B0190[], D_L03_001B0198[];
extern unsigned char D_001A7A7C[] __attribute__((sda));
extern int func_00336D50(unsigned char *, int);
extern void func_00300A00(char *, int, float, float, float, float, float);
extern unsigned char *func_00288FB8(int);
extern int func_00115DA8(char *, char *, ...);
extern int func_00283CF0(float);
extern int func_0027FB40(int, int, unsigned long, unsigned char *, int);
extern int func_L00_00443DF8(char *);
extern int func_L00_00443E18(char *);

int func_L03_003C7018(void)
{
    if (D_L03_001A904C) {
        int text;
        float half = 0.5f, row, zero;
        text = func_00336D50(D_L03_001A904C + 0x8710, 0xc6);
        row = (float)(int)(D_L03_001B0150 * D_L03_001B3284 + half);
        zero = 0.0f;
        func_00300A00((char *)text, 0x60442d00, D_L03_001B014C, row,
            D_L03_001B0154, D_L03_001B0158, zero);
        text = func_00336D50(D_L03_001A904C + 0x8710, 0xc7);
        row = (float)(int)(D_L03_001B0150 * D_L03_001B3284 + half);
        func_00300A00((char *)text, 0x55f0c070, D_L03_001B014C, row,
            D_L03_001B0154, D_L03_001B0158, zero);
        {
            int keys[4] = {11599,12116,12769,12770};
            unsigned char buffer[128];
            int i;
            for (i = 0; i < 4; ++i) {
                int shift = 5, x, y;
                unsigned char *label = func_00288FB8(keys[i]);
                if (i == 1) {
                    func_00115DA8((char *)buffer, D_L03_001B0190, label, D_L03_001B0140+1);
                    shift = -4;
                } else func_00115DA8((char *)buffer, D_L03_001B0198, label);
                x = func_00283CF0(D_L03_001B0160_gp[i].x);
                {
                    row = D_L03_001B0164[i].x * D_L03_001B3284 + half;
                    {
                        float adjustment = (float)shift;
                        if (D_L03_001B3280 == 1) row += adjustment;
                    }
                    y = func_00283CF0((float)(int)row);
                }
                func_0027FB40(x, y, 0x80808080UL, buffer, 0x80);
            }
        }
        if (D_001A7A7C[0]) {
            func_L00_00443DF8((char *)D_L03_001A904C + 0x36f28);
            func_L00_00443E18((char *)D_L03_001A904C + 0x36f28);
        }
    }
    return 1;
}
