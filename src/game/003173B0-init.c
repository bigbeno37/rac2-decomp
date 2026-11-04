#include "include_asm.h"
#include "common.h"

/* In its own unit: level 00's copy calls func_L00_003BAAD0 where the boot calls
   func_003181E0, so that callee must stay an external here. */

#ifndef RAC2_T_STATE_001B5200
#define RAC2_T_STATE_001B5200
typedef struct {
    char pad_000[0x140];
    float pos[3];
} State_001b5200;
#endif

extern State_001b5200 D_001B5200;
extern short *D_001EE5A0[];
extern u8 *D_001B1B5C MACRO_ADDR;
extern int D_001B1B78 MACRO_ADDR;
extern int func_00283CF0(float);
extern float func_002A78D8(float, float);
extern void func_002B60B0(void *, float, float, float, float);
extern int func_002A77E0(int);
extern void func_002A7750(void *, int, int);
extern void func_002A7470(void *, int, int, int);
extern float func_00282E48(void *, void *);
extern void func_002A93B0(void *);
extern int func_002B0ED8(void *, void *, int);
extern int func_002A8508(int, int);
extern int func_002B3BA8(void *, int, void *, float, float, int);
extern void func_003181E0(void *);
extern void func_003182C0(void *);
