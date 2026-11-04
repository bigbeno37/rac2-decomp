/* func_L04_004167B0 needs -G8 and -g (the line labels give the nop pair before its
 * div.s), so it has a unit of its own; see config/cflags.txt. */
#include "common.h"
typedef int Quad1006 __attribute__((mode(TI)));
typedef struct { char pad0[0x24]; float scale; } Model1006;
typedef struct {
 char pad0[0x10];
 Quad1006 pos;
 char pad20[0x4];
 Model1006 * model;
 char pad28[0x4];
 float scale;
 u8 state;
 u8 enabled;
 s16 opacity;
 u16 flags;
 char pad36[0xc];
 u8 animState;
 u8 anim;
 char pad44[0x4];
 float animSpeed;
 char pad4c[0x14];
 u8 flags60;
 char pad61[0x5f];
 Quad1006 axis0;
 Quad1006 axis1;
 Quad1006 axis2;
 Quad1006 rotation;
} Object1006;


#define F(p,o) (*(float *)((char *)(p)+(o)))
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define B(p,o) (*(unsigned char *)((char *)(p)+(o)))
#define H(p,o) (*(short *)((char *)(p)+(o)))

extern int D_L04_001B08F0 __attribute__((sda));
extern float func_00283248(float,float);
extern float func_00283B98(float,float);
extern void func_00282D30(void *,void *,float);
extern void func_00282CC0(void *,void *,void *);
extern float func_00282C48(float);
extern float func_00283BE0(float,float);
extern float func_002AA898(void *,float,void *,float,float,float,int);
extern char *D_L04_001B24DC;
extern float D_0018C2D8 NOT_SDA;
extern float D_00138288 NOT_SDA;
extern float D_0018A138 NOT_SDA;
/* Read as D_001A7A40[1]: an sda array at a nonzero offset is one unsplit lui+lbu pair. */
extern unsigned char D_001A7A40[] __attribute__((sda));
extern unsigned char D_00139605 NOT_SDA;
extern int D_L04_001A8FE8 __attribute__((sda));
extern Object1006 *D_L04_001B08F4 __attribute__((sda));
extern void func_002A05A8(void *);
extern void func_002A0F48(void *);
extern void func_002A7470(void *,int,int,int);
extern void func_00282948(void *);
extern void func_00282CF0(void *,void *,void *);
extern float func_L00_002EE290(void *);
extern void func_002AEC48(void *,void *,int,float,float);
extern void *func_0029EFD0(int);
extern void func_002B37E8(void *,void *);
extern void func_002AF1D0(void *,void *,int,int);
extern void func_0029FC20(void *,int,void *);
extern void func_002A91F0(void *,void *);
extern void func_L04_0040C368(void *);
extern void func_L04_00416390(char *,void *,void *,float);
