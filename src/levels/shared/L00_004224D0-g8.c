#include "include_asm.h"
#include "common.h"
extern char D_L00_001B8E20[], D_L00_001BF220[], D_L00_001BF230[];
extern int D_001A79F0;
extern char *D_0018C0B0 NOT_SDA;
extern float D_L00_001B14E8 __attribute__((sda)), D_L00_001B14EC __attribute__((sda));
extern float D_L00_001B14F0 __attribute__((sda)), D_L00_001B14F4 __attribute__((sda));
extern void func_00282CC0(void *,void *,void *), func_00282CF0(void *,void *,void *);
extern void func_00282D30(void *,void *,float),func_00282DD8(void *,void *,void *);
extern void func_00282F20(void *,void *,float),func_002ACEC0(void *,void *,void *,float);
extern float func_00282DF0(void *),func_00282DB0(void *,void *),func_00282C48(float),func_002831B0(float);
extern float func_002702C8(void *,float,float,float,float,float),func_002703B0(void *,float,float,float,float,float);
extern int func_00277258(void *,int,int,float);
extern void func_L00_00425A98(char *,float *,float *);
#define F(p,n) (*(float *)((char *)(p)+(n)))
#define P(p,n) (*(char **)((char *)(p)+(n)))
