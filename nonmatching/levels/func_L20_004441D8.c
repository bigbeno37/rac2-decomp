/* func_L20_004441D8, 1744 bytes. Tier opus-2. Build in its own -G8 unit with
 * -O2 -G8 -ffunction-sections -fstrict-aliasing -frac2-zero-ti-por (an L20_004441D8-g8.c file plus a
 * cflags line). check.py: 1740/1744 bytes, 0 of 1 places; the code up to 0x444740 matches.
 * What fixed what:
 * - s is loaded before the first call (the prologue matches).
 * - current++ block: jump.c's duplicate_loop_exit_test copies the entry test with copy_rtx, which
 *   drops MEM_SCALAR_P from the D_2FA0 load, so without strict aliasing that load stays behind the
 *   s->current store. -fstrict-aliasing with D_2FA0 declared `char *` frees it.
 * - pos as a union { Vec; TImode } (a union access has alias set 0): under strict aliasing the
 *   pos[1] store again orders the s->f230 load after it (0x444634, 0x4447CC).
 * - group loops written as the usual group walk `for` with the doubled p test (as in
 *   func_L20_00442E68): the index copy is $a0 as in retail.
 * - D_L20_001A8F20 as an incomplete `sda` array: an unsplit la gives retail's argument order.
 * Left:
 * - t2 (fade) for func_002BAA28 is set first here, after t1/sw in retail; nested calls change nothing.
 * - the D_220C reload at 0x444768 is $v0 here, $v1 in retail.
 * - j++ fills the func_002A7878(-0.5, 0.5) slot at 0x444804 (retail: nop) instead of the slot of the
 *   b[2] call; this accounts for the missing word. Only a banned volatile D_2250 read fixed it (opus-1).
 */
#include "common.h"

typedef float Vec[4] __attribute__((aligned(16)));
typedef struct { char pad[0x18]; float z; } Moby;
extern s16 **D_L20_001A9014;
extern s16 *D_L20_001FBB20[];
extern char *D_L20_001B3614, *D_L20_001B2F9C, *D_L20_001B308C;
extern char *D_L20_001B2FA0;
extern s32 D_L20_001B2FB8 __attribute__((sda)), D_L20_001B220C __attribute__((sda)), D_L20_001B2248 __attribute__((sda));
extern s32 D_L20_001B222C __attribute__((sda));
extern s32 D_L20_001B2210[] __attribute__((sda)), D_L20_001B2230[] __attribute__((sda));
extern float D_L20_001B2240[] __attribute__((sda)), D_L20_001B2250[] __attribute__((sda));
extern s32 D_L20_001B2210_gp SDATA(D_L20_001B2210), D_L20_001B2230_gp SDATA(D_L20_001B2230);
extern float D_L20_001B2240_gp SDATA(D_L20_001B2240), D_L20_001B2250_gp SDATA(D_L20_001B2250);
extern float D_L20_001B2BE8 __attribute__((sda));
extern float D_L20_001B2220[2] __attribute__((sda));
extern s32 D_0018C0B0[];
extern char D_L20_001A8F20[] __attribute__((sda));
extern void func_002E5CF8(int, int, void *);
extern float func_00282C48(float);
extern void func_0029F338(void *);
extern char *func_002AB1F0(void *);
extern void func_002A8F70(void *, int, int, float, void *, void *);
extern float func_002A89A8(void *, int, float);
extern void func_00282C90(void *), func_00282C88(void *);
extern float func_002A7878(float, float), func_002A7940(void);
extern void func_002830C0(void *, void *, void *);
extern int func_00283D38(int, int, float);
extern int func_002A77E0(int), func_002A7820(int, int);
extern void func_002BAA28(void *, void *, void *, int, int, int, int, int, int);
extern char *func_L20_00448A18(char *, char *, char *, char *);

typedef struct { char pad[0x21C]; int g1; int pad220; int groups[3]; int f230; int pad234[2]; int g2; char pad240[0x1858-0x240]; int current; } State;
#define I(p, o) (*(s32 *)((char *)(p) + (o)))
#define U(p, o) (*(u16 *)((char *)(p) + (o)))
#define B(p, o) (*(u8 *)((char *)(p) + (o)))

void func_L20_004441D8(char *obj)
{
    union { Vec f; long q __attribute__((mode(TI))); } pos;
    Vec a, b;
    State *s;
    float base;
    int i, j;

    s = *(State **)(obj + 0x68);
    func_002E5CF8(0x11, 0, obj);
    {
        int idx = s->groups[s->current];
        s16 *p = D_L20_001A9014[idx];

        if (p != 0) {
            do {
                char *e = D_L20_001B3614 + ((*p & 0x7FFF) << 5);

                if (e == 0)
                    break;
                p++;
                B(e, 0x1E) |= 4;
                if (p == 0)
                    break;
            } while (p[-1] >= 0 || p == D_L20_001A9014[s->groups[s->current]]);
        }
    }
    s->current++;
    {
    char *m;
    for (m = D_L20_001B2F9C; m <= D_L20_001B2FA0 && B(m, 0x20) != 0xFF; m += 0x100) {
        u16 id = U(m, 0xAA);

        if (id == 0x1233 || id == 0x1234 || (s16)id == 0x1231 || (s16)id == 0x1232 || (s16)id == 0x1357) {
            if (func_00282C48(((Moby *)obj)->z - ((Moby *)m)->z) < 10.0f)
                func_0029F338(m);
        }
    }
    }
    {
        s16 *p;
        float size = 0.0f;

        for (p = D_L20_001FBB20[s->g1];
             (s->g1 & 0xFF) != 0xFF && s->g1 <= D_L20_001B2FB8 && p != 0 &&
             ((p != 0 && p[-1] >= 0) || p == D_L20_001FBB20[s->g1]);
             p++) {
            char *m = D_L20_001B2F9C + ((*p & 0x7FFF) << 8);

            if (m == 0)
                break;
            if ((U(m, 0x34) ^ 1) & 1) {
                if (size == 0.0f) {
                    char *c = func_002AB1F0(m);

                    if (c == 0)
                        continue;
                    size = *(s16 *)(c + 4) * 0.25f;
                }
                func_002A8F70(m, D_0018C0B0[0], 0x10000, size, m + 0x10, D_L20_001A8F20);
            }
        }
    }
    {
        s16 *p;
        float size = 0.0f;

        for (p = D_L20_001FBB20[s->g2];
             (s->g2 & 0xFF) != 0xFF && s->g2 <= D_L20_001B2FB8 && p != 0 &&
             ((p != 0 && p[-1] >= 0) || p == D_L20_001FBB20[s->g2]);
             p++) {
            char *m = D_L20_001B2F9C + ((*p & 0x7FFF) << 8);

            if (m == 0)
                break;
            if ((U(m, 0x34) ^ 1) & 1) {
                if (size == 0.0f) {
                    char *c = func_002AB1F0(m);

                    if (c == 0)
                        continue;
                    size = *(s16 *)(c + 4) * 0.25f;
                }
                func_002A8F70(m, D_0018C0B0[0], 0x10000, size, m + 0x10, D_L20_001A8F20);
            }
        }
    }
    D_L20_001B2BE8 = func_002A89A8(obj + 0x10, 0, 0.5f) - 2.0f;
    base = ((Moby *)obj)->z - 0.5f;
    for (i = 0; i < D_L20_001B220C; i++) {
        int c1, c2, life, fade;
        float w;

        func_00282C90(pos.f);
        pos.f[0] = func_002A7878(-1.0f, 1.0f);
        pos.f[1] = func_002A7878(-1.0f, 1.0f);
        func_002830C0(pos.f, pos.f, D_L20_001B308C + (s->f230 << 7));
        pos.f[2] = base + func_002A7878(-0.5f, 0.5f);
        c1 = func_00283D38(D_L20_001B2210_gp, D_L20_001B2210[1], func_002A7878(0.0f, 1.0f));
        c2 = func_00283D38(D_L20_001B2210[2], D_L20_001B2210[3], func_002A7878(0.0f, 1.0f));
        w = D_L20_001B2220[func_002A77E0(2)];
        func_00282C88(a);
        a[3] = w;
        func_00282C88(b);
        b[3] = w;
        b[2] = func_002A7878(D_L20_001B2240_gp, D_L20_001B2240[1]) * 0.016666668f;
        life = func_002A7820(1, D_L20_001B2230_gp);
        fade = func_002A7820(D_L20_001B2230[1], D_L20_001B2230[2]);
        func_002BAA28(pos.f, a, b, c1, c2, life, fade, 60, D_L20_001B222C);
    }
    for (j = 0; j < D_L20_001B2248; j++) {
        func_00282C90(pos.f);
        pos.f[0] = func_002A7878(-1.0f, 1.0f);
        pos.f[1] = func_002A7878(-1.0f, 1.0f);
        func_002830C0(pos.f, pos.f, D_L20_001B308C + (s->f230 << 7));
        pos.f[2] = base + func_002A7878(-0.5f, 0.5f);
        qzero_nc(a);
        a[2] = func_002A7940();
        qzero_nc(b);
        b[2] = func_002A7878(D_L20_001B2250_gp, D_L20_001B2250[1]) * 0.016666668f;
        func_L20_00448A18(obj, (char *)pos.f, (char *)a, (char *)b);
    }
}
