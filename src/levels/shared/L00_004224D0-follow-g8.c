#include "common.h"

extern char D_L00_001B8E20[] NOT_SDA;
extern char D_00189F00[];
extern int D_0018A160 NOT_SDA;
extern float D_L00_001B14C4 __attribute__((sda));

extern void func_00282978(void *timer);
extern void func_00282CF0(void *out, void *a, void *b);
extern void func_00282D08(void *out, void *a, void *b, float t);
extern void func_00282D30(void *out, void *v, float k);
extern float func_00282DB0(void *a, void *b);
extern void func_00282DD8(void *out, void *a, void *b);
extern float func_00282DF0(void *v);
extern void func_00282F20(void *out, void *v, float length);
extern void func_002ACEC0(void *out, void *v, void *axis, float angle);
extern float func_002702C8(void *state, float current, float goal, float a, float b, float c);
extern float func_002703B0(void *state, float current, float goal, float a, float b, float c);
extern float func_002831B0(float x);
extern float func_00283248(float y, float x);
extern float func_00283BE0(float a, float b);
extern float func_00283CE0(int n);

#define F(p, n) (*(float *)((char *)(p) + (n)))

/* Final placement of follow camera CAM's basis for the frame (RAC1: follow_camera_build_view).
   In target mode 0xB it looks along the hero; otherwise it builds the forward row from the
   target-to-eye direction with the view axis at +0x20 projected off, eases the orbit and eye
   heights, pitches by the smoothed look angle clamped to +-70 degrees, blends out the start-up
   timer and re-orthogonalises the right and up rows. */
