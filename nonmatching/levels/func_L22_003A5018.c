/* Claude Opus 5.5 xhigh tier, task v2569, 5 November 2026.
 * Best probe, kept below: USA v1.01, 3008/3008 bytes, 0/2 places (levels 22 and 23), first
 * difference +0x558; 29 of 752 rows differ beyond relocated values (129 before this task).
 * NOT landable: `if (v != 0 && D == one) y = *(int *)ctl;` at five sites is a dead arm that
 * exists only to shape the schedule, and it needs value locals standing in for cse. No linked
 * build, no PAL/us2 check (no match). Compile in an isolated -g8.c unit with
 * -O2 -G8 -ffunction-sections -g. Without -g the pre-div.s nop pairs go.
 *
 * What retail has at the five (int)(... + 0.5f) sites (sprites 7, 8, both counters, box),
 * each point measured:
 * - A `D_L22_001B2980 == 1` test sits right after the mfc1. Its literal 1 is retail's saved
 *   constant: s3 and s4 are that register, reused by cse for the next real Y1 test. The box
 *   has no saved 1 because nothing reuses it and flow2 deletes it. The load of D into v0 at
 *   allocation is why the sprite pseudo lands in a0 (daddu a0,v0 mid-chain), as at sprite 9.
 *   Do not use the old `one`/`one2` idea as the explanation: with the test the 1 needs no local.
 * - Branch, load and constant all leave no word, so flow2 deleted them: tidy_fallthru_edge
 *   takes the branch once the arm is empty, then life_analysis the load. The arm must be
 *   emptied by flow1, not earlier: a one-insn reg arm becomes a cmove at jump1, and a
 *   trivially dead set goes in cse. Dead `y = v + 1` was the cmove case; a may-trap source
 *   (a load through a pointer, or FP arithmetic) stays a branch.
 * - cse crosses the site (0.5, 1.0, 1, format and &hud reach the next Y1), so the join is a
 *   single-use label over an arm without labels (AROUND path). An if/else join stops cse.
 * - Yet a boundary survives sched2 after all that. With a one-predecessor join this compiler
 *   merges the blocks at flow2 and sched2 interleaves the argument loads into the FP chain.
 *   can_delete_label_p has no LABEL_NAME test in this tree, so a C label does not hold it
 *   (`if (D != 1) goto L; ...; L: ;` gives the same bytes as the plain if).
 *
 * Forms measured, source shape only:
 * A. Kept below: first test on a live value, second on D. The D branch is tidied with its load,
 *    the value branch survives to jump2 (two-predecessor join) and leaves no word. Sprites 7
 *    and 8 and every saved register come out exact, but the label stops cse, hence locals.
 * B. `if (D_L22_001B2980 == 1) do { y = *(int *)ctl; } while (0);` from the plain draft, with no
 *    locals: 46 rows, 3016 bytes. cse's AROUND scan steps over NOTE_INSN_LOOP_END inside a
 *    skipped arm, flow keeps the notes, and they are a barrier in both schedulers. This gives
 *    s3, s4, f20-f22, format and &hud naturally. But LOOP_BEG makes final align the next label
 *    (.p2align 3, an extra nop at sprite 9), and retail's label at 0x3A567C is not aligned,
 *    so retail has no LOOP_BEG here. Also forbidden wording. A form that keeps B's notes
 *    effect without a LOOP_BEG, or a two-predecessor join that appears only after cse1, is
 *    what to look for next.
 * C. Identical arms cross-jumped at jump2 (boot's scaled_screen_size shape): schedule right,
 *    but the join stops cse and the sprite pseudo stays in v0.
 * D. `v = (int)s; if (m) v = (int)s + adj;`, hoping reorg's redundant_insn removes the arm:
 *    cse deletes the arm first (the fix is already available), so no boundary.
 *
 * Also solved here: icons 2 and 4 need the whole boot `scaled_screen_coord` shape in the
 * inline (see Coord): `float scaled = value * S + 0.5f; if (D == 1) ...`, with x computed
 * before the call. That shape is probably the real helper for every Y1/Y3 use.
 *
 * Remaining 29 rows, none tied to the boundary:
 * - Gauge 1 (0x3A5570): retail issues addiu t2 before sd a1 and addu a2 before sw t1. Moving
 *   ref_43++ or the colour statements changes nothing, so it is priority, not source order.
 * - Gauge 2 (0x3A55C8): retail issues the D_001A7340 load after mtc1/cvt/sd v0. Unsplit, plain
 *   sda and MACRO_ADDR declarations are identical; NOT_SDA and a split colour-word load are
 *   worse. Retail's instruction pairing shifts by one slot already at gauge 1, so look for
 *   something consuming one issue slot before 0x3A5570.
 * - Counter 1 (0x3A5768): retail has daddu a0,s2 ahead of lw s0 (colour), so colour takes the
 *   jal slot; counter 2 is the other way round.
 * - Box (0x3A5B04): retail gives 0.5 f1 and 320 f2, ours the reverse, plus sw v0 order. An
 *   inline, a local 320 or a local 0.5 did not flip it: 320's lifetime must be the longer one.
 *
 * Bounded investigation: about 35 compiles plus RTL and -fsched-verbose-5 dumps.
 * Earlier records: w02 restored real 32-bit address-word results for font and sprite lookups
 * and int message helpers. v1817 established: Y1/Y3 return int from an if/else with (int) in
 * both arms, jump2 cross-jumping sprite 9's tails; (float)Y3(sy) converts after the join;
 * func_0027FB40 takes u64 and returns int. Retail GP offsets of refs: 00=0324; 01=0414;
 * 02=0418; 03=0420; 04=0430; 05=3AE0; 06=3994; 07..30=0440..049C (four-byte steps);
 * 31=0314; 32..41=04A4..04C8 (four-byte steps); 42=04D0; 43=04E0; 44=04CC.
 */
#include "common.h"

extern u8 * D_L22_001A904C_base[] __asm__("D_L22_001A904C") __attribute__((sda));
#define D_L22_001A904C D_L22_001A904C_base[0]
extern int D_L22_001B2980_base[] __asm__("D_L22_001B2980") __attribute__((sda));
#define D_L22_001B2980 D_L22_001B2980_base[0]
extern float D_L22_001B2984_base[] __asm__("D_L22_001B2984") __attribute__((sda));
#define D_L22_001B2984 D_L22_001B2984_base[0]
extern int D_0018B694 NOT_SDA;
extern int D_L22_001B2AA8_base[] __asm__("D_L22_001B2AA8") __attribute__((sda));
#define D_L22_001B2AA8 D_L22_001B2AA8_base[0]
extern int D_001A7340 MACRO_ADDR;
typedef struct HudCounters {
    int unknown[2];
    int rightCount;
    int rightIcons;
    int unknown10[2];
    int leftCount;
    int leftIcons;
} HudCounters;
extern HudCounters D_L22_001B1910 NOT_SDA;
extern char D_L22_001AF4D8[] NOT_SDA;
extern int D_L22_001B2AC8 MACRO_ADDR;
extern int D_L22_001B2ACC_base[] __asm__("D_L22_001B2ACC") __attribute__((sda));
#define D_L22_001B2ACC D_L22_001B2ACC_base[0]
extern int D_L22_001B2AD0_base[] __asm__("D_L22_001B2AD0") __attribute__((sda));
#define D_L22_001B2AD0 D_L22_001B2AD0_base[0]
extern u64 D_L22_001AF418_base[] __asm__("D_L22_001AF418") __attribute__((sda));
#define D_L22_001AF418 D_L22_001AF418_base[0]
extern u64 D_L22_001AF428_base[] __asm__("D_L22_001AF428") __attribute__((sda));
#define D_L22_001AF428 D_L22_001AF428_base[0]
extern u8 *ref_L22_003A5018_00 __attribute__((sda));
extern float ref_L22_003A5018_01 __attribute__((sda)), ref_L22_003A5018_02 __attribute__((sda));
extern u64 ref_L22_003A5018_03 __attribute__((sda)), ref_L22_003A5018_04 __attribute__((sda));
extern int ref_L22_003A5018_05 __attribute__((sda));
extern float ref_L22_003A5018_06 __attribute__((sda));
extern int ref_L22_003A5018_07 __attribute__((sda));
extern int ref_L22_003A5018_08 __attribute__((sda));
extern int ref_L22_003A5018_09 __attribute__((sda));
extern int ref_L22_003A5018_10 __attribute__((sda));
extern int ref_L22_003A5018_11 __attribute__((sda));
extern int ref_L22_003A5018_12 __attribute__((sda));
extern int ref_L22_003A5018_13 __attribute__((sda));
extern int ref_L22_003A5018_14 __attribute__((sda));
extern int ref_L22_003A5018_15 __attribute__((sda));
extern int ref_L22_003A5018_16 __attribute__((sda));
extern int ref_L22_003A5018_17 __attribute__((sda));
extern int ref_L22_003A5018_18 __attribute__((sda));
extern int ref_L22_003A5018_19 __attribute__((sda));
extern int ref_L22_003A5018_20 __attribute__((sda));
extern int ref_L22_003A5018_21 __attribute__((sda));
extern int ref_L22_003A5018_22 __attribute__((sda));
extern int ref_L22_003A5018_23 __attribute__((sda));
extern int ref_L22_003A5018_24 __attribute__((sda));
extern int ref_L22_003A5018_25 __attribute__((sda));
extern int ref_L22_003A5018_26 __attribute__((sda));
extern int ref_L22_003A5018_27 __attribute__((sda));
extern int ref_L22_003A5018_28 __attribute__((sda));
extern int ref_L22_003A5018_29 __attribute__((sda));
extern int ref_L22_003A5018_30 __attribute__((sda));
extern int ref_L22_003A5018_31 __attribute__((sda));
extern int ref_L22_003A5018_32 __attribute__((sda));
extern int ref_L22_003A5018_33 __attribute__((sda));
extern int ref_L22_003A5018_34 __attribute__((sda));
extern int ref_L22_003A5018_35 __attribute__((sda));
extern int ref_L22_003A5018_36 __attribute__((sda));
extern int ref_L22_003A5018_37 __attribute__((sda));
extern int ref_L22_003A5018_38 __attribute__((sda));
extern int ref_L22_003A5018_39 __attribute__((sda));
extern int ref_L22_003A5018_40 __attribute__((sda));
extern int ref_L22_003A5018_41 __attribute__((sda));
extern u8 ref_L22_003A5018_42[] SDATA(ref_L22_003A5018_42);
extern int ref_L22_003A5018_43 __attribute__((sda));
extern int ref_L22_003A5018_44 __attribute__((sda));

extern int func_00336D50(void *, int);
extern void func_00300A00(void *, unsigned int, float, float, float, float, float);
extern int func_0028E588(int, int);
extern void func_0028EA58(void *, int, int, int, int, void *);
extern void func_0027EAE8(int, int, int, int, int, void *);
extern int func_00115DA8(char *, char *, ...);
extern int func_001157AC(char *);
extern int func_0027FB40(int, int, u64, char *, int);
extern char *func_00288FB8(int);
extern int func_L00_0030DD70(int, int);
extern void func_0029CC50(int, int, int);
extern int func_00282948(void *);
extern void func_002802E0(void *, int, int, int, int, int, int, int, int);
extern void func_0029B888(void *, unsigned int, char *, int, int, int, int, char *, int, int);
extern void func_L00_0030CF60(void);

/* Screen-space coordinates use the active video mode's vertical scale. */
static __inline__ int Y3(float y) {
    if (D_L22_001B2980 == 1)
        return (int)(y + 3.0f);
    return (int)y;
}
static __inline__ int Y1(float y) {
    if (D_L22_001B2980 == 1)
        return (int)(y + 1.0f);
    return (int)y;
}
static __inline__ int Y1x(float y, int one, float unit) {
    if (D_L22_001B2980 == one)
        return (int)(y + unit);
    return (int)y;
}
static __inline__ int Coord(float value, float offset) {
    float scaled = value * D_L22_001B2984 + 0.5f;
    if (D_L22_001B2980 == 1)
        return (int)(scaled + offset);
    return (int)scaled;
}
static __inline__ int Scale(float value, float half) {
    return (int)(value * D_L22_001B2984 + half);
}
#define Y0(y, h) Scale((float)(y), h)

int func_L22_003A5018(void) {
    char textBuffer[64];
    char *text;
    union { u64 d[2]; u32 w[4]; } colours;
    u8 box[32];
    u8 *obj = ref_L22_003A5018_00;
    u8 *ctl = *(u8 **)(obj + 0x68);
    int y;
    int one, one2;
    float half, half2, unit;
    char *format;
    HudCounters *hud;
    char *label, *hint;

    {
        void *sprite = (void *)(u32)func_00336D50(D_L22_001A904C + 0x8710, 0xC3);
        float sy = ref_L22_003A5018_02 * D_L22_001B2984 + 0.5f;
        sy = (float)Y3(sy);
        func_00300A00(sprite, 0x60442D00, ref_L22_003A5018_01, sy, 1.0f, 1.0f, 0.0f);
    }
    {
        void *sprite = (void *)(u32)func_00336D50(D_L22_001A904C + 0x8710, 0xC4);
        float sy = ref_L22_003A5018_02 * D_L22_001B2984 + 0.5f;
        sy = (float)Y3(sy);
        func_00300A00(sprite, 0x60241700, ref_L22_003A5018_01, sy, 1.0f, 1.0f, 0.0f);
    }
    {
        void *sprite = (void *)(u32)func_00336D50(D_L22_001A904C + 0x8710, 0xC5);
        float sy = ref_L22_003A5018_02 * D_L22_001B2984 + 0.5f;
        sy = (float)Y3(sy);
        func_00300A00(sprite, 0x55F0C070, ref_L22_003A5018_01, sy, 1.0f, 1.0f, 0.0f);
    }
    {
        void *sprite = (void *)(u32)func_00336D50(D_L22_001A904C + 0x8710, 0xC3);
        float sy = ref_L22_003A5018_02 * D_L22_001B2984 + 0.5f;
        sy = (float)Y3(sy);
        func_00300A00(sprite, 0x60442D00, ref_L22_003A5018_01, sy, -1.0f, 1.0f, 0.0f);
    }
    {
        void *sprite = (void *)(u32)func_00336D50(D_L22_001A904C + 0x8710, 0xC4);
        float sy = ref_L22_003A5018_02 * D_L22_001B2984 + 0.5f;
        sy = (float)Y3(sy);
        func_00300A00(sprite, 0x60241700, ref_L22_003A5018_01, sy, -1.0f, 1.0f, 0.0f);
    }
    {
        void *sprite = (void *)(u32)func_00336D50(D_L22_001A904C + 0x8710, 0xC5);
        float sy = ref_L22_003A5018_02 * D_L22_001B2984 + 0.5f;
        sy = (float)Y3(sy);
        unit = 1.0f;
        half = 0.5f;
        one = 1;
        func_00300A00(sprite, 0x55F0C070, ref_L22_003A5018_01, sy, -1.0f, unit, 0.0f);
    }
    {
        void *sprite = (void *)(u32)func_0028E588(0xEAA4, one);
        int v = Y0(ref_L22_003A5018_15, half);
        if (v != 0 && D_L22_001B2980 == one) y = *(int *)ctl;
        func_0028EA58(sprite, ref_L22_003A5018_14, v, ref_L22_003A5018_16, ref_L22_003A5018_17, ref_L22_003A5018_42);
    }
    {
        void *sprite = (void *)(u32)func_0028E588(0xEAA4, 0);
        int v = Y0(ref_L22_003A5018_19, half);
        if (v != 0 && D_L22_001B2980 == one) y = *(int *)ctl;
        func_0028EA58(sprite, ref_L22_003A5018_18, v, ref_L22_003A5018_20, ref_L22_003A5018_21, ref_L22_003A5018_42);
    }

    {
        float ratio = (float)D_0018B694 / (float)*(short *)(ctl + 0x324);
        int x, width;
        colours.d[0] = ref_L22_003A5018_03;
        width = ref_L22_003A5018_12;
        x = ref_L22_003A5018_09 + ref_L22_003A5018_07 + ref_L22_003A5018_13;
        y = ref_L22_003A5018_10 + ref_L22_003A5018_08 / 2 - ref_L22_003A5018_11 / 2;
        ref_L22_003A5018_43++;
        colours.w[1] = (colours.w[1] & 0xFFFF00FF) | ((int)(ratio * 255.0f) << 8);
        colours.d[1] = D_L22_001AF418;
        func_0027EAE8(x, y, x + (int)(ratio * (float)width), y + ref_L22_003A5018_11, 0xFFFFF1, &colours);
    }
    {
        float ratio = *(float *)(ctl + 0x20) / (float)D_L22_001B2AA8;
        int right, width, margin;
        margin = ref_L22_003A5018_09;
        colours.d[0] = ref_L22_003A5018_04;
        width = ref_L22_003A5018_12;
        colours.w[0] = (colours.w[1] & 0xFFFF00FF) | ((int)(ratio * 255.0f) << 8);
        colours.d[1] = D_L22_001AF428;
        right = D_001A7340 - margin - ref_L22_003A5018_07 - 4;
        func_0027EAE8(right - (int)(ratio * (float)width), y, right, y + ref_L22_003A5018_11, 0xFFFFF1, &colours);
    }

    {
        void *sprite = (void *)(u32)func_0028E588(0xEAA5, 0);
        float sy = (float)ref_L22_003A5018_23 * D_L22_001B2984 + half;
        func_0028EA58(sprite, ref_L22_003A5018_22, Y1x(sy, one, unit), ref_L22_003A5018_24, ref_L22_003A5018_25, ref_L22_003A5018_42);
    }
    {
        void *sprite = (void *)(u32)func_0028E588(0xEAA5, 0);
        float sy = (float)ref_L22_003A5018_27 * D_L22_001B2984 + 0.5f;
        func_0028EA58(sprite, ref_L22_003A5018_26, Y1(sy), ref_L22_003A5018_28, ref_L22_003A5018_29, ref_L22_003A5018_42);
    }
    one2 = 1;
    half2 = 0.5f;
    hud = &D_L22_001B1910;
    format = D_L22_001AF4D8;
    func_00115DA8(textBuffer, format, hud->leftCount);
    text = textBuffer;
    {
    int value = Y0(ref_L22_003A5018_33, half2);
    int colour;
    if (value != 0 && D_L22_001B2980 == one2) y = *(int *)ctl;
    colour = ref_L22_003A5018_30;
    func_0027FB40(ref_L22_003A5018_32, value, colour, text, func_001157AC(text));
    }
    func_00115DA8(text, format, hud->rightCount);
    {
    int value = Y0(ref_L22_003A5018_35, half2);
    int colour;
    if (value != 0 && D_L22_001B2980 == one2) y = *(int *)ctl;
    colour = ref_L22_003A5018_30;
    func_0027FB40(ref_L22_003A5018_34, value, colour, text, func_001157AC(text));
    }
    if (hud->leftIcons) {
        void *sprite = (void *)(u32)func_0028E588(0xEAA5, one2);
        float sy = (float)ref_L22_003A5018_37 * D_L22_001B2984 + half2;
        func_0028EA58(sprite, ref_L22_003A5018_36, Y1x(sy, one2, 1.0f), ref_L22_003A5018_40, ref_L22_003A5018_41, ref_L22_003A5018_42);
    }
    if (D_L22_001B1910.leftIcons >= 2) {
        void *sprite = (void *)(u32)func_0028E588(0xEAA5, 1);
        int x = ref_L22_003A5018_36 + ref_L22_003A5018_44;
        func_0028EA58(sprite, x, Coord((float)ref_L22_003A5018_37, 1.0f), ref_L22_003A5018_40, ref_L22_003A5018_41, ref_L22_003A5018_42);
    }
    if (D_L22_001B1910.rightIcons) {
        void *sprite = (void *)(u32)func_0028E588(0xEAA5, 1);
        float sy = (float)ref_L22_003A5018_39 * D_L22_001B2984 + 0.5f;
        func_0028EA58(sprite, ref_L22_003A5018_38, Y1(sy), ref_L22_003A5018_40, ref_L22_003A5018_41, ref_L22_003A5018_42);
    }
    if (D_L22_001B1910.rightIcons >= 2) {
        void *sprite = (void *)(u32)func_0028E588(0xEAA5, 1);
        int x = ref_L22_003A5018_38 - ref_L22_003A5018_44;
        func_0028EA58(sprite, x, Coord((float)ref_L22_003A5018_39, 1.0f), ref_L22_003A5018_40, ref_L22_003A5018_41, ref_L22_003A5018_42);
    }
    if (D_L22_001B2AC8 != -1 && obj[0x95] != 10) {
        switch (D_L22_001B2AC8) {
        case 0: func_L00_0030DD70((int)func_00288FB8(0x2FA3), -1); break;
        case 2: func_00288FB8(0x1241); func_0029CC50(0x1241, *(short *)(ctl + 0x32C), -1); break;
        case 1: func_00288FB8(0x1240); func_0029CC50(0x1240, D_L22_001B2ACC, -1); break;
        }
        ref_L22_003A5018_05 = 0;
        func_00282948(&D_L22_001B2AD0);
        if (D_L22_001B2AD0 <= 0) D_L22_001B2AC8 = -1;
    }
    if (ref_L22_003A5018_31) {
        int v = (int)(ref_L22_003A5018_06 * 320.0f + 0.5f);
        if (v != 0 && D_L22_001B2980 == 1) y = *(int *)ctl;
        func_002802E0(box, 240, 480, 44, 468, 256, v, 16, 3);
        label = func_00288FB8(0x9FC);
        hint = func_00288FB8(0x2F2C);
        func_0029B888(box, 0x80F0F0F0, label, 0, 0, -1, 0, hint, 0, 0);
        func_L00_0030CF60();
    }
    return 1;
}
