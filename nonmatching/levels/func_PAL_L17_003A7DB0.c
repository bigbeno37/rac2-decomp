/* func_PAL_L17_003A7DB0, the PAL copy of func_L17_003A4740: the same C, where the 90-tick fade
   becomes 75.
   Tier: opus-1 (Claude Opus 5.5, xhigh), task v2648.
   Status: not a C problem. This body is byte-exact, but the catalogue row is 8 bytes too long.
   check.py: "520 bytes, expected 528", 0 of 1 places, every row shifted by two words.
   build.Placement.derive(obj, section, 0x003A7DB8) in level 17: no difference over all 520 bytes,
   giving D_L17_001AF52C = 0x001AF55C. That is an object comparison, not a linked build.
   What differs: only the row's first two words at 0x003A7DB0, `sw $a0,-0x3FEC($gp); nop`. They
   are not this function's. Retail's only reference builds 0x003A7DB8 (lui 0x003A8108, addiu
   0x003A8110) and nothing references 0x003A7DB0, as USA v1.01 builds 0x003A4748, not 0x003A4740.
   To land: give this row the correction func_L17_003A4740 and func_PAL_L01_0039A174 already
   have, (528, 8), then place this C as it stands. Needs only common.h and the externs below. */
#include "common.h"

extern int D_001A7340[] MACRO_ADDR;
extern int D_L17_001AF52C[];
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern unsigned long func_0027C878(int);
extern int func_002A77E0(int);
extern void func_0027E018(int, int, int, int, int, int, int, int, unsigned long long, unsigned long long);

void func_PAL_L17_003A7DB0(char *self) {
    char *data = *(char **)(self + 0x68);
    float fv[2];
    unsigned long packet;
    int nx, ny, i, j;
    float pv[2];
    float base, a, b;
    int color;
    int t0, t1;

    fv[0] = func_00283CE0(D_001A7340[2]);
    fv[1] = func_00283CE0(D_001A7340[3]);
    packet = func_0027C878(0x1E);
    nx = D_001A7340[0] / 32 + 1;
    ny = D_001A7340[1] / 32 + 1;

    for (i = 0; i < nx; i++) {
        int *p = D_L17_001AF52C;
        for (j = 0; j < ny; j++) {
            pv[0] = (float)(i * 32);
            pv[1] = (float)(j * 32);
            base = (float)*p;
            a = func_00283CE0(*(int *)(data + 0x140));
            b = func_00283CE0(0x4B);
            color = (func_00283CF0(a * 127.5f / b) << 24) + 0xFF7F7F;
            t0 = func_002A77E0(0x20);
            t1 = func_002A77E0(0x20);
            func_0027E018(func_00283CF0(pv[0] - base), func_00283CF0(pv[1] - base), 0x20, 0x20, t0, t1, 0x20, 0x20, color, packet);
        }
    }
}
