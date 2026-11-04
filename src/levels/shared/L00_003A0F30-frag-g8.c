#include "common.h"
typedef int HeroQ __attribute__((mode(TI)));
typedef union { unsigned char b[0x2600]; signed char c[0x2600]; short h[0x1300]; unsigned short uh[0x1300]; int i[0x980]; unsigned int u[0x980]; float f[0x980]; char *ptr[0x980]; HeroQ q[0x260]; } Hero;
extern Hero D_00189E20;
typedef int VectorBits __attribute__((mode(TI)));
typedef float Vector[4] __attribute__((aligned(16)));

extern float D_L00_001AF62C __attribute__((sda));
extern float D_L00_001AF630 __attribute__((sda));
extern int D_L00_001AF634 __attribute__((sda));
extern int D_L00_001AF638 __attribute__((sda));
extern unsigned int D_L00_001AF63C __attribute__((sda));
extern float D_L00_001AF640 __attribute__((sda));
extern float D_L00_001AF644 __attribute__((sda));
extern float D_L00_001AF648 __attribute__((sda));
extern float D_L00_001AF64C __attribute__((sda));
extern float D_L00_001AF650 __attribute__((sda));
extern float D_L00_001AF654 __attribute__((sda));
extern void func_00283098(void *, void *, void *);
extern void func_00282CC0(void *, void *, void *);
extern void func_002ACEC0(void *, void *, void *, float);
extern float func_002A7878(float, float);
extern float func_00283B98(float, float);
extern void func_00282D30(void *, void *, float);
extern char *func_L00_00333F90(void *, void *);
extern float func_00282C48(float);
extern void func_002AB668(void *, void *, float);
extern void func_002ACDE8(void *, void *, void *);
extern float func_002A78D8(float, float);
extern int func_00283CF0(float);
extern void func_00282CF0(void *, void *, void *);
extern char *func_002C16C0(void *, void *, unsigned int, int, int, int, float, float, float, float, float);

/* Emit a rotating fan of fragments followed by smoke particles. */
