/* opus-1 near miss, USA v1.01, default -O2 -G0 -ffunction-sections.
 * Best check: 1588/1588 bytes, 0 of 4 places; first mismatch at +0x14.
 * The only real difference is the prologue: here sched2 issues `li $s2,1`
 * (the switch's case-1 constant) at t=4 beside `sd $s3`; retail issues it
 * one cycle later, beside `sd $s4`. All rows after the prologue match; cdiff
 * flags `sw ...,0x8($at)` vs `0x22A8($at)`, but those are only the
 * D_L01_001B22A0+8 relocation.
 * What fixed the sol-high draft's differences:
 *  - the lock flag is a struct member (D_L01_001B22A0.busy, +8) in a
 *    MACRO_ADDR struct, so the stores are 2-word macros that stay out of
 *    delay slots (beql with the target's load in the slot);
 *  - `m->active = 1;` before `m->state = 2;` in the second activation block
 *    (state first in the first block) fixes the store order and the
 *    save-table registers;
 *  - D_L01_0021E4B0.collected[] as a struct member fixes the +0x464 add order.
 * Sched2 analysis: 1078 (li s2) has only an anti dependence on `sd $s2`
 * (cost 0), so it is ready at t=4 and takes the free ALU slot. Retail needs
 * it not ready at t=4, or insn 4 (daddu s1,a0) held to t=4; maybe an extra
 * LOG_LINK carried over from sched1, created by a different form of the
 * switch or case-1 constant. Not found.
 * Needs (from the file): extern char D_L01_001B94C0[]; extern char
 * D_00189EA0[]; void func_L01_00394148(void); plus the declarations below.
 */
#include "common.h"
extern char D_L01_001B94C0[];
extern void func_L01_00394148(void);
typedef int CamQuad __attribute__((mode(TI)));
typedef struct {
    CamQuad position, rotation;
    int trigger, camera, source, timer, mode, parameter;
    u8 reset, requireEnabled, requireHero, lock;
} CamVars;
typedef struct {
    char pad0[0x20]; u8 state; char pad21[0xF]; u8 field30; char pad31; u16 field32;
    char pad34[0x34]; CamVars *vars; char pad6C[0x3E]; s16 id; u16 group;
    char padAE[2]; u8 index; char padB1; u16 saveIndex; char padB4[8]; u8 active;
} CamMoby;
extern u8 D_0019B2E8[];
extern int D_001A79F0[] __attribute__((sda));
extern u8 D_0019B4A8[];
extern struct { u8 header[0x464]; u8 collected[0]; } D_L01_0021E4B0;
extern char D_00189EA0[];
extern int D_0018A160 NOT_SDA;
extern int ref_L01_00394240_00 MACRO_ADDR;
extern char *D_L01_001B264C[] __attribute__((sda));
extern int D_L01_001B1FA8[] __attribute__((sda));
extern struct {
    s16 f0, f2, f4, pad6;
    s32 busy;
} D_L01_001B22A0 MACRO_ADDR;
extern int D_L01_001B9780 NOT_SDA;
extern char D_L01_002A4370[];
extern void func_0026FEB8(void *, int, int);
extern void func_0029F338(void *);
extern int func_002ACBC8(void *, int);
extern void func_L00_002B9D60(void *, void *, int, int);
extern void func_00335438(void *, void *, int, int, int);
extern int func_002B38B8(void *);
extern int func_00282948(void *);
extern void func_003355A8(int);
extern void ref_L01_00394240_01(int);
extern void func_00335370(void *);
extern void func_003353C8(void *);
extern void func_002A0430(int);
extern void func_002DF3F8(void *, void *);
void func_L01_00394240(CamMoby *m)
{
    CamVars *v = m->vars;
    CamQuad position, rotation;
    int saved;
    func_L01_00394148();
    switch (m->state) {
    case 0:
        if (v->requireEnabled && (D_0019B2E8[m->index + (ref_L01_00394240_00 << 4)] & 0x80)) {
            func_0029F338(m);
        } else if (v->camera == -1) {
            func_0026FEB8(D_L01_002A4370, m->id, m->group);
            func_0029F338(m);
        } else {
            m->field30 = m->field32 = 255;
            m->state = 1;
        }
        break;
    case 1:
        if (D_L01_0021E4B0.collected[(s16)m->saveIndex]) goto saved_state;
        saved = D_0019B4A8[((s16)m->saveIndex >> 1) + (D_001A79F0[0] << 10)];
        if (m->saveIndex & 1) {
            if ((saved >> 4) & 7) goto saved_state;
        } else {
            if (saved & 7) {
            saved_state:
                m->state = 1;
                break;
            }
        }
        if ((func_002ACBC8(D_00189EA0, v->trigger) || m->active == 1) &&
            (!v->requireHero || *(int *)(D_00189EA0 + 0x2C0))) {
            qcopy(&position, D_L01_001B94C0);
            qcopy(&rotation, D_L01_001B94C0 + 16);
            if (v->source == -1) func_L00_002B9D60(D_00189EA0, D_00189EA0 + 0x10, 0x6F, 1);
            else func_L00_002B9D60(D_L01_001B264C[0] + v->source * 128 + 0x30,
                                  D_L01_001B264C[0] + v->source * 128 + 0x70, 0x6F, 1);
            if (v->mode) func_00335438(&position, &rotation, 2, v->parameter, 0);
            else func_00335438(D_L01_001B264C[0] + v->camera * 128 + 0x30,
                              D_L01_001B264C[0] + v->camera * 128 + 0x70, 0, 0, 0);
            D_L01_001B1FA8[0] = 1;
            m->state = 2;
            m->active = 1;
            if (v->lock) D_L01_001B22A0.busy = 1;
            { int camera = v->camera; char *base = D_L01_001B264C[0];
            qcopy(&v->position, base + camera * 128 + 0x30);
            qcopy(&v->rotation, base + camera * 128 + 0x70); }
        }
        if (D_001A79F0[0] == 19 &&
            (func_002ACBC8(D_00189EA0, v->trigger) || m->active == 1 || func_002B38B8(m)) &&
            (!v->requireHero || D_0018A160)) {
            qcopy(&position, D_L01_001B94C0);
            qcopy(&rotation, D_L01_001B94C0 + 16);
            if (v->source == -1) func_L00_002B9D60(D_00189EA0, D_00189EA0 + 0x10, 0x6F, 1);
            else func_L00_002B9D60(D_L01_001B264C[0] + v->source * 128 + 0x30,
                                  D_L01_001B264C[0] + v->source * 128 + 0x70, 0x6F, 1);
            if (v->mode) func_00335438(&position, &rotation, 2, v->parameter, 0);
            else if (D_001A79F0[0] == 19) func_00335438(D_L01_001B264C[0] + v->camera * 128 + 0x30,
                                                 D_L01_001B264C[0] + v->camera * 128 + 0x70, 1, 0, 0);
            else func_00335438(D_L01_001B264C[0] + v->camera * 128 + 0x30,
                              D_L01_001B264C[0] + v->camera * 128 + 0x70, 0, 0, 0);
            { unsigned int saveIndex = m->saveIndex;
            u8 *p;
            u8 b;
            D_L01_001B1FA8[0] = 1;
            m->active = 1;
            m->state = 2;
            if (saveIndex & 1) {
                p = &D_0019B4A8[((s16)saveIndex >> 1) + (D_001A79F0[0] << 10)];
                b = *p;
                if (((b >> 4) & 7) != 7) *p = b + 16;
            } else {
                p = &D_0019B4A8[((s16)saveIndex >> 1) + (D_001A79F0[0] << 10)];
                b = *p;
                if ((b & 7) != 7) *p = b + 1;
            } }
            if (v->lock) D_L01_001B22A0.busy = 1;
            { int camera = v->camera; char *base = D_L01_001B264C[0];
            qcopy(&v->position, base + camera * 128 + 0x30);
            qcopy(&v->rotation, base + camera * 128 + 0x70); }
        }
        break;
    case 2:
        if (func_00282948(&v->timer) || m->active != 1) {
            if (v->mode) func_003355A8(2);
            else func_003355A8(0);
            ref_L01_00394240_01(0);
            if (v->lock) D_L01_001B22A0.busy = 0;
            D_L01_001B1FA8[0] = 0;
            D_L01_001B9780 = 0;
            m->state = 3;
            m->active = 0;
        } else {
            if (v->lock) D_L01_001B22A0.busy = 1;
            func_00335370(&v->position);
            func_003353C8(&v->rotation);
        }
        break;
    case 3:
        if (v->reset && !(D_0019B2E8[m->index + (ref_L01_00394240_00 << 4)] & 0x80)) {
            func_002A0430(m->index);
            func_002DF3F8((char *)m + 0x10, (char *)m + 0xF0);
        }
        if (m->active == 1) m->state = 1;
        break;
    }
}
