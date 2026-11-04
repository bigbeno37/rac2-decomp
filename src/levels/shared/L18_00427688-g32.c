#include "include_asm.h"
#include "common.h"
typedef int Vector128 __attribute__((mode(TI)));

typedef struct { float f0,f4,f8,fC,f10,f14,f18; } MotionDefaults;
extern MotionDefaults D_L18_001B1218 __attribute__((sda));
extern int D_001A79F0 __attribute__((sda));
extern unsigned char D_0019B2E8[];
extern unsigned char D_L18_0021E7B0[][0x464];
extern float func_002A7878(float,float);
extern void func_002B60B0(unsigned char *,float,float,float,float);
extern int func_002A7820(int,int);
extern float func_L09_0033E7A0(int);
extern int func_L18_00427130(unsigned char *,int,int);
extern void func_L18_00426F78(unsigned char *);
extern void func_L18_00427010(unsigned char *);
extern int func_001163B0(void);
extern void func_L18_00426F28(unsigned char *,int,int,float);
extern void func_002A7750(unsigned char *,int,int);
