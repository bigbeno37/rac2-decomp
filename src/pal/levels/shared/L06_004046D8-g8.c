#include "common.h"

/* func_PAL_L06_004046D8, the PAL copy of func_L06_004009B8: the same C, where the
 * 60 Hz divisor D_L06_001AFF20 * 60.0f becomes (D_L06_001AFF20 * 300.0f + 2.0f) / 6. */

/* Draws the scrolling mesh grid as textured quads, fading each column by age.
 * Corresponds to RAC1 render_data_processor_linked_effect; the packet layout,
 * loop lifetimes and matrix copies follow the measured RAC2 body.
 */
#define RI(p,o) (*(s32 *)((char *)(p)+(o)))
#define RF(p,o) (*(float *)((char *)(p)+(o)))
typedef int QuadWord __attribute__((mode(TI)));
typedef struct { float x,y,z,w; } __attribute__((aligned(16))) RenderVec;
typedef struct { RenderVec points[4]; u32 colors[4]; float uv[4][2]; u64 r70,texture,flags,blend; } RenderQuad;
extern char *D_L06_001B281C;
extern char D_L06_001B9840[];
extern float D_L06_001AFE7C[] MACRO_ADDR;
extern s32 D_L06_001AFE50 __attribute__((sda));
extern s32 D_L06_001AFE54 __attribute__((sda));
extern s32 D_L06_001AFE58 __attribute__((sda));
extern s32 D_L06_001AFE5C __attribute__((sda));
extern u32 D_L06_001AFE70 __attribute__((sda));
extern u32 D_L06_001AFE74 __attribute__((sda));
extern float D_L06_001AFE78[8] __attribute__((sda));
extern RenderVec D_L06_001AFEA0[4] __attribute__((sda));
extern float D_L06_001AF71C;
extern s32 D_L06_001B31B4;
extern s32 D_L06_001B31B0;
extern float D_L06_001AFF20 __attribute__((sda));
extern float func_00282E48(void *,void *);
extern void func_002833B8(void *,float);
extern void func_00283410(void *,void *);
extern void func_00283788(void *,void *,void *);
extern float func_00283CE0(s32);
extern s32 func_00283CF0(float);
extern u64 func_0027C878(s32);
extern u32 func_00283D38(u32,u32,float);
extern void func_00282CC0(void *,void *,void *);
extern void func_00280B90(void *,void *,s32);
