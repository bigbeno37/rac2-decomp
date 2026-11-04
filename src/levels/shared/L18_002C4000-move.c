/* func_L18_003F6CA8 from L18_002C4000.c: dispatches on the object's byte at +0x42.
 * Its own unit: the velocity snapshot needs -frac2-qcopy -frac2-qcopy-barrier, and the
 * selector switch's jump table is this unit's only .rodata.
 */
#include "common.h"

typedef int Vector128 __attribute__((mode(TI)));

extern float func_0029F530(char *);
extern int func_L07_00321D68(char *, void *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
extern float func_00283248(float, float);
extern float func_002AAA00(float *, float *, int, float, float, float, float);
extern void func_00282D30(void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern int func_L01_00321178(char *, char *);
extern void func_002B5278(unsigned char *, float);
extern void func_002B5258(char *);
extern int func_002B4FF0(char *, float *, float);
extern void func_002A7750(unsigned char *, int, int);
extern float func_L00_002EE290(void *);
extern void func_00283028(void *, void *, void *);
extern Vector128 D_L18_001BFC40;
