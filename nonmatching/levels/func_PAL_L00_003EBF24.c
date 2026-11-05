/* func_PAL_L00_003EBF24 (opus-1), the PAL copy of func_L00_003E7C88: the same C, where the
 * per-frame step 1/60 becomes 1/50 (0x3CA3D70B, 0.020000001f), and the callee at +0x98 is
 * func_PAL_L00_003EC0E0.
 *
 * Best check.py: "436 bytes, expected 440", 0 of 4 places. The C is right; the row is not.
 * Its first word is 00000000: at all four places it follows a complete epilogue (jr $ra,
 * addiu $sp) and stands 4 mod 8 before an 8-aligned 27BDFFA0 (addiu $sp,$sp,-0x60), so it is
 * alignment padding. Shifted by that one word, all 109 words equal retail, relocated fields
 * masked. The USA row func_L00_003E7C88 is 436 bytes and leaves this padding word out.
 *
 * The same C named func_PAL_L01_003EE068 prints "23 of 23 places match" (build flags below):
 * that row is this body at the other 23 levels. No C can emit the nop, so this row needs a
 * catalogue fix (start +4, 436 bytes, and joined to func_PAL_L01_003EE068).
 *
 * Flags: -O2 -G64 -Wa,-G64 -ffunction-sections -fno-rerun-loop-opt
 */
#include "common.h"

typedef struct {
    char *object;
    unsigned char rgb[4];
    int unused;
    float fade;
    void (*callback)(char *);
    int rest[3];
} FadeEntry;

extern FadeEntry D_L00_0024EFC0[];
extern int D_L00_001B2B60[] __attribute__((sda));
extern float D_L00_001B065C __attribute__((sda));
extern int D_L00_001B0648[3] __attribute__((sda));
extern char *D_L00_001B0650[] __attribute__((sda));
extern void ref_L00_003E7C88_00(float *, float, float);
extern void func_PAL_L00_003EC0E0(char *, void *, float);
extern void func_L00_003E80A8(int, char *);
extern void func_002A0488(char *, int, int, int);
extern void func_0029F338(char *);

void func_PAL_L00_003EBF24(void) {
    int i;
    i = 0;
    if (i < D_L00_001B2B60[0]) {
        FadeEntry *base = D_L00_0024EFC0;
        FadeEntry *prev = (FadeEntry *)((char *)base - 32);
        float zero = 0.0f;
        do {
            float *fade = (float *)((char *)base + (i * 32 + 12));
            FadeEntry *p = &D_L00_0024EFC0[i];
            char *o = p->object;
            if (*fade != zero) {
                ref_L00_003E7C88_00(fade, 0.0f, D_L00_001B065C * 0.020000001f);
                func_PAL_L00_003EC0E0(o, D_L00_001B0648, *fade);
                func_L00_003E80A8(i, D_L00_001B0650[0]);
            } else {
                int callbacks = (int)&base->callback;
                void (**callback)(char *) = (void (**)(char *))(i * 32 + callbacks);
                if (*callback) {
                    func_002A0488(o, p->rgb[0], p->rgb[1], p->rgb[2]);
                    *(unsigned char *)(o + 35) = 128;
                    *(unsigned short *)(o + 52) &= ~2;
                    (*callback)(o);
                } else func_0029F338(o);
                {
                    FadeEntry *end = (FadeEntry *)(D_L00_001B2B60[0] * 32 + (int)prev);
                    D_L00_001B2B60[0]--;
                    D_L00_0024EFC0[i] = *end;
                }
            }
            i++;
        } while (i < D_L00_001B2B60[0]);
    }
}
