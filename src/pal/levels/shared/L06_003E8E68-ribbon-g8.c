/* Adapted from RAC1's render_data_processor_linked_effect in
   src/overlays/l10_orxon/platforms_doors_and_bombthrower.c (GPL-3.0). */
/* func_PAL_L06_003E8E68, the PAL copy of func_L06_003E57E8: the same C, where the
   fade duration D * 60 is scaled to 50 Hz as (D * 300 + 2) / 6. */
#include "common.h"
extern char *D_L06_001B281C;
extern char D_L06_001B9840[];
extern float func_00282E48(char *,char *);
extern void func_002833B8(void *,float);
extern void func_00283410(void *,void *);
extern void func_00283788(void *,void *,void *);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern int func_00283D38(int,int,float);
extern unsigned long func_0027C878(int);
extern void func_00282CC0(void *,void *,void *);
extern void func_00280B90(void *,void *,int);
extern int D_L06_001AF550 __attribute__((sda)),D_L06_001AF554 __attribute__((sda)),D_L06_001AF558 __attribute__((sda)),D_L06_001AF55C __attribute__((sda));
extern int D_L06_001AF570 __attribute__((sda)),D_L06_001AF574 __attribute__((sda));
extern float D_L06_001AF578[] SDATA(D_L06_001AF578);
extern float D_L06_001AF5A0[] SDATA(D_L06_001AF5A0);
extern float D_L06_001AF620 __attribute__((sda));
extern float D_L06_001AF57C[7] __attribute__((sda));
extern float D_L06_001AF71C;
extern int D_L06_001B31B0,D_L06_001B31B4;
typedef unsigned int Quad57 __attribute__((mode(TI)));
