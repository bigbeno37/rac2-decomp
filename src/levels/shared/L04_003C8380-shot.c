#include "common.h"
typedef int QuadWord __attribute__((mode(TI)));
extern void *func_0029EFD0(int);
extern float func_002A7878(float, float);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282F20(char *, char *, float);
typedef struct { float v[4]; void *owner; int flags; unsigned char mode, enabled; unsigned short kind; float radius; int active; } SweepProbe;
extern float func_00282E20(void *);
extern float func_00283248(float, float);
extern void func_00282F70(void *, void *, float);
extern int func_002761F8(void *, void *, int, void *, void *);
extern char D_L04_001BFAE0[];
extern char D_L04_001BFB00[];
extern char D_L04_001A8F20[] __attribute__((sda));
extern void func_00283028(void *, void *, void *);
extern float func_00282DF0(void *);
extern int func_002A7820(int, int);
extern void func_L04_00358AE8(void *, void *, unsigned int, int, float);
extern void func_L04_00358868(void *, unsigned int, int, int, float);
extern unsigned char *func_L04_0035F3E0(void *, void *, int, float);
extern void func_L04_003F9C60(void *, int);
extern void func_0029F338(void *);
extern float func_002A7AA8(float, float, float, float, float);

/* Spawns a shot moby at pos heading along velocity. If the sweep from the owner to pos is
 * blocked, it bursts into sparks and is deleted at once; otherwise it gets 17 trail particles.
 * The owner is kept as an int: a pointer parameter would let the start-vector load
 * be scheduled above the probe stores. */
