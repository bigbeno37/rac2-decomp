#include "common.h"

typedef int Vector __attribute__((mode(TI)));
/* The trajectory table has at least the five observed entries; full extent unknown. */
extern float D_L02_001B0190[] SDATA(D_L02_001B0190);
extern float D_L02_001B01E4 __attribute__((sda));
extern float D_L02_001B01E8 __attribute__((sda));
extern float D_L02_001B01B8;
extern void func_0029FCC0(char *, int, char *);
extern void func_0029FC20(char *, int, char *);
extern void func_00283678(void *, void *);
extern void func_002AB028(void *);
extern float func_00283180(float);
extern void func_00282D30(void *, void *, float);
extern void func_00282F20(void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern void func_002AF1D0(char *, char *, int, int);
extern void func_002A10E8(char *);
extern void func_L02_003D5DF8(char *);
extern char *func_L02_003D5CF0(void *, void *, float);
extern int func_00283CF0(float);
/* NOTE REVIEW: typed int/FP interleaving (AGENTS.md): each bank's order, the stack arguments and the return
 * match 0032C700-explosion.c. Floats-first is the order the matched callers L16_0041EE90-g8.c,
 * shared/L00_003AB388-hit.c, shared/L00_003F5390-g8.c and shared/L04_00402D90.c use; it puts the
 * size move ahead of the integer arguments, as retail schedules it. */
extern char *func_0032C700(void *, float, float, int, int, int, int, int, int, int, int, int, int, char *);
extern int func_002A8C28(void *, void *, int, int, int, int, float, float, float);
