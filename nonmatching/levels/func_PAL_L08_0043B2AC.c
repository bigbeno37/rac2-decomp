/* func_PAL_L08_0043B2AC, the PAL copy of func_L08_004356B8: the same C, where both
   `vars+0x18` steps become `D_L08_001B1F90 * 1.2f` and 0.00029088825f becomes 0.0003490659f.

   Tier: opus-1 (Claude Opus 5.5). Result: CATALOGUE, not a C problem.
   Best check.py: 668 bytes, expected 672, 0 of 3 places match.
   What differs: only the row's first word. The row starts on a `nop` that is alignment padding:
   08:0043B2AC, 19:0042F3CC and 30:00423DA4 each follow `jr ra; nop`, the last words of
   func_L08_004356A0 (20 bytes, ending exactly there), and the function's own
   `addiu sp,sp,-0x80` is at +4 on an 8-byte boundary. v1.01's row cuts the same word: the
   template starts at 004356B8, after that function's padding at 004356B4.
   build.Placement.derive of this object at row address + 4 returns no difference at all three
   places, every relocation bound, so this C is exact for a row of 668 bytes at 08:0043B2B0,
   19:0042F3D0 and 30:00423DA8. No C emits a leading nop.
   To land it: add "func_PAL_L08_0043B2AC": (672, 4) to the measured-entries table in
   tools/levels.py beside "func_PAL_L01_0039A174": (524, 4) and rerun `catalogue`; the row keeps
   its name. Then src/pal/levels/shared/L08_0043B2AC-g8.c with this text, marker removed from
   L08_0043B2AC.c. A -g8 name alone gives -O2 -G8 -ffunction-sections, the template unit's
   flags; no config/cflags.txt line is needed.
   Declarations: all in this file. Link not yet run, so `needs two values` is still unchecked
   for the D_L08_ names. */
#include "common.h"
extern float D_L08_001B1F94 __attribute__((sda)), D_L08_001B1F98 __attribute__((sda)), D_L08_001B1F78 __attribute__((sda)), D_L08_001B1F7C __attribute__((sda)), D_L08_001B1FA4 __attribute__((sda));
extern float D_L08_001B1F8C __attribute__((sda)), D_L08_001B1F80 __attribute__((sda)), D_L08_001B1F84 __attribute__((sda)), D_L08_001B1F88 __attribute__((sda));
extern float D_L08_001B1F90 __attribute__((sda)), D_L08_001B1F74 __attribute__((sda)), D_L08_001B1FA0 __attribute__((sda));
extern u8 D_L08_001B1F9C __attribute__((sda));
extern float D_L08_001B031C[] __attribute__((sda));
extern float D_L08_001B9D30[];
extern void func_0029F338(void *);
extern float func_00282E80(void *, void *);
extern int func_00283DB8(void *, float);
extern void func_00283098(void *, void *, void *);
extern void func_00282CC0(void *, void *, void *);
extern void func_L08_003D3928(void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282D30(void *, void *, float);
extern float func_00283248(float, float);
extern float func_00283B98(float, float);

/* Interpolates an object between two points and turns it toward its target. */
void func_PAL_L08_0043B2AC(u8 *self)
{
    float base[4] __attribute__((aligned(16)));
    float endpoint[4] __attribute__((aligned(16)));
    float delta[4] __attribute__((aligned(16)));
    u8 *vars = *(u8 **)(self+0x68);
    u8 *target;
    float distance;
    float limit, scale;
    int result;
    *(float *)(self+0x2C) = *(float *)(*(u8 **)(self+0x24)+0x24) *
        (D_L08_001B1F94 + (D_L08_001B1F98-D_L08_001B1F94)* *(float *)(vars+0x18));
    self[0x23] = D_L08_001B1F9C;
    target = *(u8 **)(vars+0x10);
    if (!target || target[0x20]==0xFE || target[0x20]==0xFD) {
        func_0029F338(self);
        return;
    }
    distance = func_00282E80(target+0x10, vars);
    limit = D_L08_001B1F7C;
    *(float *)(vars+0xC) = D_L08_001B1FA4;
    result = func_00283DB8(vars,limit);
    qcopy(base, vars);
    base[2] += D_L08_001B1F8C;
    delta[0] = D_L08_001B1F80;
    delta[1] = D_L08_001B1F84;
    delta[2] = D_L08_001B1F88;
    func_00283098(delta,delta,D_L08_001B9D30);
    func_00282CC0(endpoint,delta,D_L08_001B9D30-140);
    if (distance < D_L08_001B1F78) func_L08_003D3928(self,vars);
    if (distance < D_L08_001B1F7C && (result > 0 || distance < D_L08_001B031C[0]))
        *(float *)(vars+0x18) += D_L08_001B1F90 * 1.2f;
    else *(float *)(vars+0x18) -= D_L08_001B1F90 * 1.2f;
    if (*(float *)(vars+0x18)>1.0f) *(float *)(vars+0x18)=1.0f;
    else if (*(float *)(vars+0x18)<0.0f) *(float *)(vars+0x18)=0.0f;
    func_00282CF0(delta,base,endpoint);
    func_00282D30(delta,delta,*(float *)(vars+0x18));
    func_00282CC0(self+0x10,endpoint,delta);
    *(float *)(self+0xF8) = func_00283248(*(float *)vars-*(float *)(self+0x10),*(float *)(vars+4)-*(float *)(self+0x14));
    distance = func_00282E80(self+0x10,vars);
    distance = func_00283248(distance,*(float *)(vars+8)-*(float *)(self+0x18));
    scale = D_L08_001B1FA0*0.0003490659f;
    *(float *)(self+0xF4) = -distance;
    *(float *)(self+0xF0) = func_00283B98(*(float *)(self+0xF0),scale);
}
