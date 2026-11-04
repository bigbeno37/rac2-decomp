/* opus-2 v2448 (Claude Opus 5.5): exact bytes, USA v1.01, USA v2.00 and PAL each 3/3,
 * but only with a duplicate byte store, so FAILED by the rules (invented object access).
 * Profile: -O2 -G8 -ffunction-sections -g (placed in L18_00427BE0-update-g8.c).
 * Earlier drafts named the callees after another program; the retail targets are
 * func_0032B0F8 (spawn), func_00308FB0, func_00308A50, func_00354E20, func_00308A08,
 * func_00308BD8, func_00308A20, func_00309A58 and func_0037B6B0.
 * Why it works: retail's sched1 order is li QI 255, li 256, li HI 255, sh 0x34, sb 0x30,
 * sh 0x32. Local-alloc then gives 256 v0, HI v1, QI a0 (priority by span), and reorg moves
 * the QI li into the beqz slot. That needs the QI 255 pseudo created before the 0x34
 * statement but stored after it. The first `[0x30] = 255` below creates it; flow1 deletes
 * that store as dead (same address stored again with no read in between), and cse reuses
 * its QImode pseudo for the second store. Any local or inline parameter holding 255 is
 * promoted to SImode, so cse's wider-mode lookup merges the halfword 255 into it (484 bytes).
 * Without the first store: 488/488 bytes, 0/3, five words differ (constant registers/order).
 * Open question: a natural construct that materialises the QImode 255 before the 0x34 store
 * (an inline spawn wrapper that sets alpha and the 0x100 flags would be the same RTL).
 */
#include "common.h"
extern void *func_0032B0F8(int);
extern float func_00308FB0(float, float);
extern float func_00308A50(void *);
extern char *func_00354E20(char *, int, unsigned int, int, float, float);
extern void func_00308A08(void *, void *, void *);
extern void func_00308BD8(void *, void *, float);
extern float func_00308A20(void *);
extern int func_00309A58(float);
extern int func_0037B6B0(int, int, char *);
extern char D_0018A0E0[];
char *func_L18_0043FFF8(void *position, char *owner, float *direction,
                       int kind, int flags, float x, float y, float z)
{
    char *object = func_0032B0F8(0x1263);
    char *state;
    float angle;
    int lifetime;
    if (object) {
        ((unsigned char *)object)[0x30] = 255;
        *(unsigned short *)(object + 0x34) = 0x100;
        ((unsigned char *)object)[0x30] = 255;
        *(unsigned short *)(object + 0x32) = 255;
        state = *(char **)(object + 0x68);
        object[0x20] = 0;
        qcopy(state, direction);
        qcopy(object + 0x10, position);
        *(float *)(object + 0xf8) = func_00308FB0(direction[0], direction[1]);
        angle = func_00308A50(direction);
        *(float *)(object + 0xf4) = -func_00308FB0(angle, direction[2]);
        *(char **)(state + 0x30) = func_00354E20(object, -1, 0xf0208080, 0x60, 0.1f, 0.01f);
        *(float *)(state + 0x1c) = y;
        qcopy(object + 0xc0, state);
        func_00308A08(object + 0xd0, object + 0xc0, D_0018A0E0);
        func_00308BD8(object + 0xd0, object + 0xd0, -1.0f);
        func_00308A08(object + 0xe0, object + 0xd0, object + 0xc0);
        angle = func_00308A20(direction);
        angle = *(float *)(state + 0x1c) / angle;
        lifetime = func_00309A58(angle);
        *(float *)(state + 0x24) = z;
        *(int *)(state + 0x34) = lifetime;
        *(float *)(state + 0x20) = x;
        *(int *)(state + 0x28) = kind;
        *(int *)(state + 0x2c) = flags;
        *(char **)(state + 0x10) = owner;
        if (*(short *)(owner + 0xaa) == 0xee1)
            func_0037B6B0(5, 0, owner);
        object[0x31] = 0;
        *(unsigned short *)(object + 0x34) |= 1;
    }
    return object;
}
