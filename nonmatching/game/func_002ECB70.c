/* Claude Opus 5.5, opus-2, v2496, USA v1.01.
 * Best: 1252/1252 bytes, 0/28 places (95 of 313 cdiff rows; 34 differ beyond
 * relocations, was 42 for opus-1 v2269 and 111 for v0785) with
 * -O2 -G8 -ffunction-sections -g -fno-gcse (as src/game/002ECB70-g8.c).
 * New over v2269: `active = 0;` after the colour ternary fixes the prologue
 * order (retail sets s0 = 2 before s2 = 0).
 * Kept from v2269: `transform = matrix;` after func_0029FC20(object, 0, matrix)
 * reuses the a2 argument pseudo (retail addiu a2,sp,0x90; daddu s5,a2,zero);
 * a block-local `float *t = transform;` in the active branch spills only the copy
 * to 0x100(sp).
 * Remaining: active-loop increment order (retail uv s4 before old s6; swapping
 * the uv/old declarations only swaps their registers), was-active lui order
 * (retail: bne slot lui for the inactive arm, then separate luis v0=E38,
 * a0=CE0, v1=A30), and the quad loop.
 * Quad loop, measured in v2496: retail's `addiu s0,s3,1 ... daddu s3,s0` is gcse
 * PRE of `i + 1` (expression (plus i 1) inserted in the quad preheader; gcse on
 * reproduces it exactly), so the original was most likely built with gcse on.
 * Retail also rebuilds la D_00262350 at the top of the outer loop (lui stolen
 * into the bnez slot, alignment nop before the new label). Neither PRE (it
 * hoists HIGH(D_00262350)) nor loop.c (moves it out of both loops) leaves it
 * there for this source, with split addresses or MACRO_ADDR. loop.c refuses to
 * move only regs created by an inner loop's strength reduction, so the base
 * probably enters as part of an inner-loop giv's add value. With gcse on (no
 * -fno-gcse) the rest diverges (245 rows): PRE inserts (plus $at 80/84) and the
 * HIGHs of D_00214A30/CE0/D_00262350 at the ends of all three branches, since
 * $at-based frame addresses are killed by calls; retail computes them in the
 * quad preheader.
 * Matching-behaviour reference: RAC1 space.c render_environment_mapped_object
 * (adapted there from Lombyte, GPL-3.0). Required declarations follow.
 */
#include "common.h"
extern float D_002625B0[][4], D_002620A0[][4], D_00214A30[][4];
extern float D_00214CE0[][2], D_00214E38[][2];
extern short D_00262350[][8];
extern int D_001A8BB0;
extern float D_001B5200[];
extern int D_001ACBB0 __attribute__((sda));
extern int D_001ABF00 __attribute__((sda));
extern int D_001ABF04 __attribute__((sda));
extern int D_001ABF08 __attribute__((sda));
extern short D_00152C20 NOT_SDA;
extern unsigned long long func_0027C878(int);
extern void func_0029FC20(void *, int, void *);
extern float func_00282C48(float);
extern void func_00282948(int *);
extern float func_00283CE0(int);
extern void func_002830C0(void *, void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
extern float func_00282DB0(void *, void *);
extern void func_00282D30(void *, void *, float);
extern float func_00282C10(float);
extern void func_00280B90(void *, int, int);
void func_002ECB70(void *object)
{
 struct {
  float vertex[4][4];
  unsigned int color[4];
  float uv[4][2];
  unsigned long long packet[4];
 } quad;
 float matrix[16], normal[4], reflected[4], ray[4];
 float *transform;
 int active;
 unsigned int color;
 float fraction;
 int i, j;
 char *o = object;
 float (*points)[4] = D_002620A0;
 float (*normals)[4] = D_002625B0;
 quad.packet[1] = func_0027C878(21);
 quad.packet[0] = 0;
 color = D_001A8BB0 == 2 ? 0x28a5a281 : 0x3ca5a281;
 active = 0;
 quad.packet[2] = 0xff9000000260ULL;
 quad.packet[3] = 0x8000000044ULL;
 quad.color[0] = quad.color[1] = quad.color[2] = quad.color[3] = color;
 func_0029FC20(object, 0, matrix);
 transform = matrix;
 if (D_001A8BB0 == 0 || D_001A8BB0 == 2) {
  if (func_00282C48(D_001B5200[0x50] - *(float *)(o + 0x10)) < 16.0f &&
      func_00282C48(D_001B5200[0x51] - *(float *)(o + 0x14)) < 16.0f) active = 1;
 } else active = 1;
 if (D_001A8BB0 == 6 && D_00152C20 == 2) active = 0;
 if (active || D_001ABF00 == 1) {
  D_001ABF04 = 1;
  func_00282948(&D_001ABF08);
  fraction = func_00283CE0(D_001ABF08) / func_00283CE0(60);
  {
  float *t = transform;
  float (*n)[4] = normals;
  float (*p)[4] = points;
  float (*dest)[4] = D_00214A30;
  float (*uv)[2] = D_00214CE0;
  float (*old)[2] = D_00214E38;
  for (i = 0; i < 43; i++) {
   float divisor, x, y;
   func_002830C0(dest[i], p[i], t);
   func_00282CF0(ray, dest[i], &D_001B5200[0x50]);
   func_00282F20(ray, ray, 1.0f);
   func_002830C0(normal, n[i], t);
   func_00282F20(normal, normal, 0.1f);
   func_00282D30(reflected, normal, 2.0f * func_00282DB0(normal, ray));
   func_00282CF0(reflected, ray, reflected);
   func_00282F20(reflected, reflected, 1.0f);
   reflected[2] += 1.0f;
   divisor = func_00282C10(reflected[2] * 2.0f) * 2.0f;
   if (D_001ABF00 == 1 || D_001ABF08 == 0) {
    uv[i][0] = reflected[0] / divisor + 0.5f;
    uv[i][1] = reflected[1] / divisor + 0.5f;
   } else {
    x = reflected[0] / divisor + 0.5f;
    uv[i][0] = x + (old[i][0] - x) * fraction;
    x = reflected[1] / divisor + 0.5f;
    uv[i][1] = x + (old[i][1] - x) * fraction;
   }
  }
  }
  if (D_001ABF00 == 1) D_001ABF00 = 2;
 } else if (D_001ABF04 == 1) {
  D_001ABF04 = 0;
  {
   float (*dest)[4] = D_00214A30;
   float (*uv)[2] = D_00214CE0;
   float (*old)[2] = D_00214E38;
   float (*p)[4] = points;
   for (i = 0; i < 43; i++) {
    float x = uv[i][0];
    float y = uv[i][1];
    old[i][0] = x;
    old[i][1] = y;
    func_002830C0(dest[i], p[i], transform);
   }
  }
  D_001ABF08 = 60;
 } else {
  float (*dest)[4] = D_00214A30;
  float (*p)[4] = points;
  for (i = 0; i < 43; i++) func_002830C0(dest[i], p[i], transform);
  D_001ABF08 = 60;
 }
 {
 float *quad_u = &quad.uv[0][0];
 float *quad_v = &quad.uv[0][1];
 for (i = 0; i < 38; i++) {
  for (j = 0; j < 4; j++) {
   int index = D_00262350[i][j * 2];
   qcopy_nc(quad.vertex[j], D_00214A30[index]);
   {
    float x = D_00214CE0[index][0];
    float y = D_00214CE0[index][1];
    quad_u[j*2] = x;
    quad_v[j*2] = y;
   }
  }
  func_00280B90(&quad, 0, 0);
 }
}
}
