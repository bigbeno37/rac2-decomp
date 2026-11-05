/* func_0027BA68 - 772 bytes, 28 places. Claude Opus 5.5 (xhigh), task v2614, USA v1.01.
 * Best check: 772/772 bytes, 0/28 places, check.py first mismatch +0x78, 4 rows really differ
 * (cdiff prints 47 of 193, the rest being masked relocations). Own unit needed, for example
 * src/game/0027BA68-qcopy.c with -O2 -G16 -Wa,-G16 -ffunction-sections -frac2-qcopy
 * -frac2-qcopy-barrier. Declarations are below. func_0027BA48 and func_0027B9C0 stay extern there.
 * func_002A0380 keeps its defined int return even though this caller ignores it.
 *
 * What differs: only the words before the first func_00282B50.
 *     here:   lhu, a0, a1, a2, andi, jal [sh]
 *     retail: lhu, a0, andi, a1, sh, jal [a2]
 * Every register is right. The one-pass group around view_position is what gives view_matrix its
 * eighth ref and keeps the addiu after the call (without it, 12 rows differ; see git history of
 * this file for the older `first` wording and the priority working).
 *
 * Why it stops here. sched2 trace, -fsched-verbose-5: after the call lhu and a0 issue at t; a1 and
 * a2 are both ready at t+1, so they take that cycle; andi waits for the load until t+2. Retail
 * needs a1 and a2 to miss t+1: {lhu,a0} {} {andi,a1} {sh,a2} {jal}. a2 is a constant into a hard
 * reg, so only anti/output links can reach it, and those cost 1 (ADJUST_COST). alu has two units
 * and no blockage. With -fno-schedule-insns the same a1/a2 rows differ, so sched1's order is not
 * the cause. Reasoned from the source and the trace, not proved: something in the block is missing.
 * Measured, none closer (on the C below):
 * - inline wrapper around the copy, or around the flag restore: same 4 rows.
 * - int return on func_002A0300 (lhu moves to v1, 5 rows; a false prototype anyway), void * return
 *   on func_00282B50, -g, -fno-sched-interblock, -fno-gcse: same rows or worse.
 * - dst local + `if (f) dead = 1;`, or a dead float store in the arm: same 4 rows, no branch left
 *   (the dumps were not read to see which pass removed it).
 * - The restore and call written in both arms of a test on two live registers: the arms stay, 812
 *   bytes, as expected if a call arm cannot cross-jump with a fall-through arm.
 * - A forced label between lhu and andi: 140 rows, saved registers scrambled, with or without a
 *   dst local. This argues that retail's body is one block through register allocation, so a
 *   vanished test here looks unlikely. One wording only, and forced labels are forbidden anyway.
 * Not tried, by reasoning only: loop notes. A note makes the next insn a full barrier, and a0 follows
 * the restore in RTL order, so a0 could not rise above andi and sh. This agrees with the older
 * do-while measurements on the restore and the call (4 to 18 rows).
 * Retail evidence: a scan of the boot image (levels not scanned) for load, a-reg, op, a-reg, store,
 * jal finds this site only, also counting the two-argument form. func_00282B50 decodes to a
 * handwritten lq/addi/sq loop, so this is a plain call, not a block-move libcall.
 * Next idea, untested: two no-code insns ranked above a1/a2 at t+1, or one they depend on. No C
 * source for such an insn found; a same-register extendsidi2 follows the move, so it comes too late.
 */
#include "common.h"

typedef struct { float x, y, z, w; } QuadPoint;
extern void func_0027B9C0(int, int, float);
extern void func_0027BA48(void);
extern void func_002FC560(unsigned int, unsigned long);
/* Snapshot the view, render the selected object into a texture, then draw
   its four-corner world-space quad after restoring the view. */
typedef struct {
    QuadPoint vertices[4];
    unsigned int colors[4];
    float uv[4][2];
    unsigned long state[4];
} CaptureQuad;
typedef struct { char pad[0x34]; unsigned short flags; } CaptureMoby;
typedef struct { QuadPoint position; float angles[4]; QuadPoint vertices[4]; float width, height; CaptureMoby *moby; } CaptureObject;
typedef struct { char pad[0x50]; CaptureObject *capture; } CaptureState;
extern CaptureState D_001B8840;
extern char D_001B5570[];
typedef struct { char pad[0xB0]; float scale; } CaptureView;
extern CaptureView D_001B9040;
extern char D_001C2AC0[];
extern QuadPoint D_001A8820 __attribute__((sda));
extern QuadPoint D_001A8830 __attribute__((sda));
extern QuadPoint D_001A8840 __attribute__((sda));
extern QuadPoint D_001A8850 __attribute__((sda));
extern float D_001A87F8[4] __attribute__((sda));
extern float D_001A8808[4] __attribute__((sda));
extern unsigned long D_001A7470[] __attribute__((sda));
extern int D_001B1B58[] __attribute__((sda));
extern void func_002A0250(void), func_002A01C8(void), func_002A01F0(void);
extern void func_002A0300(void *, int);
extern void func_00282B50(void *, void *, int);
extern void func_00129178(void *);
extern void func_001292C0(void *, void *, float);
extern void func_00129368(void *, void *, float);
extern void func_00129218(void *, void *, float);
extern void func_00279D18(void);
extern void func_002A0468(void *, int, int, int, int);
extern void func_0027AB40(void), func_002FC748(void), func_002A0220(void), func_002A03C0(void);
extern int func_002A0380(void);
extern void func_00280B90(void *, int, int);

void func_0027BA68(void)
{
    CaptureObject *object = D_001B8840.capture;
    QuadPoint saved_matrix[4], saved_position, matrix[4];
    CaptureQuad quad;
    if (object) {
        CaptureMoby *moby = object->moby;
        QuadPoint *position = &object->position;
        char *view_matrix;
        char *view_position;
        float saved_scale;
        void *scene;
        int i;
        func_002A0250();
        func_002A01C8();
        view_matrix = D_001B5570;
        func_002A01F0();
        scene = (void *)D_001B1B58[1];
        moby->flags |= 1;
        func_002A0300(scene, -1);
        moby->flags &= ~1;
        func_00282B50(saved_matrix, view_matrix, 0x40);
        do { view_position = view_matrix - 0x230; } while (0);
        qcopy(&saved_position, view_position);
        saved_scale = D_001B9040.scale;
        func_0027B9C0(8, 8, 1.0f);
        func_00129178(matrix);
        func_001292C0(matrix, matrix, object->angles[0]);
        func_00129368(matrix, matrix, object->angles[1]);
        func_00129218(matrix, matrix, object->angles[2]);
        func_00282B50(view_matrix, matrix, 0x40);
        qcopy(view_position, position);
        func_00279D18();
        func_002A0468(moby, 0x404040, 0xE, 0, 0);
        qcopy(D_001C2AC0, &D_001A8820);
        qcopy(D_001C2AC0 + 0x20, &D_001A8830);
        qcopy(D_001C2AC0 + 0x10, &D_001A8840);
        qcopy(D_001C2AC0 + 0x30, &D_001A8850);
        func_002A0300(moby, 1); func_0027BA48();
        D_001B9040.scale = saved_scale;
        func_0027AB40();
        func_00282B50(view_matrix, saved_matrix, 0x40);
        qcopy(view_position, &saved_position);
        func_00279D18();
        func_002FC560(0x42, 0x8000000044UL); func_002FC560(8, 5);
        func_002FC748(); func_002A0220(); func_002A0380();
        quad.state[1] = D_001A7470[0];
        quad.state[2] = 0xFF9000000260UL;
        quad.state[0] = 5;
        quad.state[3] = 0x8000000044UL;
        for (i = 0; i < 4; i++) {
            quad.colors[i] = 0x80808080;
            quad.uv[i][0] = object->width * D_001A87F8[i] + 0.5f;
            quad.uv[i][1] = object->height * D_001A8808[i] + 0.5f;
            qcopy(&quad.vertices[i], &object->vertices[i]);
        }
        func_00280B90(&quad, 0, 0);
    } else func_002A03C0();
}
