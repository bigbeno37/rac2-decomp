/* opus-2 (Claude Opus 5.5), task v2439, USA v1.01: check.py 648/648 bytes, 0/2 places,
 * 24 rows differ (plus the relocations). Closest form so far; RAC1-style separate locals
 * (draw_beam_and_arc_two_pass_quads in walloper_and_beam_effects.c) and an index loop
 * give the retail frame 0x150, every loop register and every giv.
 * Two differences remain:
 * 1. Prologue: retail copies a0 to $s2 after the camera qcopy (the qcopy is a scheduler
 *    barrier in this unit), so $s2's save and copy follow $s6/$s7. Reproduced exactly by
 *    the diagnostic form `void f(RibbonPoint *line, ...)`, `RibbonPoint *points;`, and after
 *    the qcopy `points = line; line = 0;`: the kill of the parameter stops gcse cprop from
 *    turning the loop uses back into the parameter (cse2 then drops the copy otherwise);
 *    combine then merges the a0 copy into the late copy. `line = 0` is a dead store, so
 *    not landable; a plain `points = line;` without the kill is copy-propagated away, and
 *    `prev = points++;` changes the allocation (102 rows). A natural kill is still missing.
 * 2. Spill slots 0xEC/0xF0 (the gcse PRE reaching regs of &q.vertex[2] and [3]) are
 *    swapped. Hashes are (14030 + offset) % S with S = (max_cuid/2)|1; retail order needs
 *    14062 % S < 14078 % S: S = 65 or 71 and above (67 and 69 fail). This form has S = 67,
 *    the diagnostic form S = 69. Declaration initializers (`int i = 0;`) are deleted before
 *    gcse here and do not move S. Retail needs 2-4 more insns at gcse than the diagnostic
 *    form, which matches the missing construct of (1) adding a few insns that die later.
 * Needs (already in L04_002CC748.c): D_L04_001B9680, func_00282CF0, func_00282CC0,
 * func_00282F20; declares the rest below.
 */
#include "common.h"

extern char D_L04_001B9680[];
extern void func_00282CF0(float *, float *, float *);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282F20(float *, float *, float);
typedef struct { float v[4]; } RibbonPoint;
typedef struct {
    float vertex[4][4]; unsigned int color[4]; float uv[8]; u64 state[4];
} RibbonPacket;
extern void func_00281DE8(void *, int, int, int);
extern void func_00280B90(void *, int, int);
extern void func_00282DD8(void *, void *, void *);

void func_L04_0030C948(RibbonPoint *points, int first_texture, int middle_texture,
                     int last_texture, int count, unsigned int color)
{
    RibbonPacket q;
    float camera[4];
    float cross[4];
    float side[4];
    float camera_delta[4];
    float segment[4];
    int i;
    int changed;

    changed = 0;
    qcopy(camera, D_L04_001B9680);
    func_00281DE8(&q, first_texture, 0, 0x80);
    q.color[3] = color;
    q.color[2] = color;
    q.color[1] = color;
    q.color[0] = color;
    q.uv[0] = 0.0f;
    q.uv[1] = 0.0f;
    q.uv[2] = 1.0f;
    q.uv[3] = 0.0f;
    q.uv[4] = 0.0f;
    q.uv[5] = 1.0f;
    q.uv[6] = 1.0f;
    q.uv[7] = 1.0f;
    func_00282CF0(camera_delta, points[0].v, camera);
    func_00282CF0(segment, points[1].v, points[0].v);
    func_00282DD8(cross, segment, camera_delta);
    func_00282F20(side, cross, points[0].v[3]);
    func_00282CC0(q.vertex[0], points[0].v, side);
    func_00282CF0(q.vertex[1], points[0].v, side);
    for (i = 1; i < count; i++) {
        func_00282CF0(camera_delta, points[i].v, camera);
        func_00282CF0(segment, points[i].v, points[i - 1].v);
        func_00282DD8(cross, segment, camera_delta);
        func_00282F20(side, cross, points[i].v[3]);
        func_00282CC0(q.vertex[2], points[i].v, side);
        func_00282CF0(q.vertex[3], points[i].v, side);
        func_00280B90(&q, 0, 0);
        if (!changed) {
            if (i == count - 2) func_00281DE8(&q, last_texture, 0, 0x80);
            else func_00281DE8(&q, middle_texture, 0, 0x80);
            changed = 1;
        } else if (i == count - 2) {
            func_00281DE8(&q, last_texture, 0, 0x80);
        }
        qcopy(q.vertex[0], q.vertex[2]);
        qcopy(q.vertex[1], q.vertex[3]);
    }
}
