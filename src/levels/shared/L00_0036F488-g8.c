#include "common.h"

/* -G8 unit with -frac2-gcse-unsplit-edges (config/cflags.txt): retail keeps one %hi of the hero
 * and one of the pad state in saved registers and forms each block's %lo base afresh; the pad's
 * %hi is hoisted above the first branch on the gate's result. The globals below that retail reads
 * both $gp-relative (in delay slots) and absolute are MACRO_ADDR. */

typedef struct HeroObject { char pad00[0xAA]; short id; } HeroObject;
typedef struct Hero {
    char pad0000[0x123C];
    unsigned char mode;          /* 0x123C */
    char pad123D[0x1248 - 0x123D];
    int submode;                 /* 0x1248 */
    char pad124C[0x1830 - 0x124C];
    HeroObject *object;          /* 0x1830 */
    char pad1834[0x2294 - 0x1834];
    int state;                   /* 0x2294 */
    char pad2298[0x229C - 0x2298];
    int f229C;                   /* 0x229C */
    char pad22A0[0x22B4 - 0x22A0];
    unsigned char f22B4;         /* 0x22B4 */
} Hero;
typedef struct PadState { char pad000[0x1A4]; unsigned int pressed; } PadState;
typedef struct { u16 count; u16 pad; u32 maximum; u32 levels; } Statistics;

extern char D_00189E20[];
extern char D_00138180[];
#define HERO ((Hero *)D_00189E20)
#define PAD ((PadState *)D_00138180)

extern int D_L00_001A8F00 MACRO_ADDR;
extern int D_L00_001A8FB8 MACRO_ADDR;
extern int D_L00_001A8FF0 MACRO_ADDR;
extern int D_001A7428 MACRO_ADDR;
extern int D_L00_001B19C4 MACRO_ADDR;
extern int D_001A79F0 MACRO_ADDR;
extern u32 D_001A7390 MACRO_ADDR;
extern int D_L00_001B1AE0 MACRO_ADDR;
extern Statistics D_0018C750[];
extern unsigned char D_001A7BB0[] __attribute__((sda));
extern int D_001A7424 __attribute__((sda));
extern int D_L00_001A8FB4 __attribute__((sda));
extern int D_L00_001A8FF4 __attribute__((sda));
extern float D_L00_001B18E0 __attribute__((sda));

extern int func_L00_002F43F8(void);
extern int func_002B49E0(int, int, int, int, int);
/* The neighbouring stub in most programs; another function in six of them. */
extern unsigned int ref_L00_0036F488_00(void);
extern int func_L00_002D6728(void);
extern int func_L00_002D6768(void);
extern int func_L00_002D67A8(int);
extern int func_L00_002D6880(void);
extern void func_L00_002FC058(void);
extern int func_L00_0032EF10(int, int, int *);
extern void func_00290730(void);
extern int func_00290748(void);
extern void func_L00_002CB218(void);
extern int func_L00_0032B270(void);
extern int func_L00_002A3B80(void);
extern void func_L00_0032B2A8(int);
extern int func_002E5E58(int, unsigned int, void *);
extern void func_L00_003266D8(void);
extern void func_L00_0030C7F8(int);
extern void func_L00_0030C710(int);
extern void func_002F5220(void);
extern void func_L00_0037E5C0(void);
extern void func_L00_00310048(void);
extern void func_002B6658(void);
extern void func_002E5F60(void);
extern void func_L00_002A6DD0(void);
extern void func_002B9390(void);
extern void func_L00_002ED808(void);
extern void func_00272368(void);
extern void func_002E4AC8(void);
extern void func_002914B8(void);
extern void func_0029FFB8(void);
extern void func_0028E138(int);
extern void func_0029BCD8(void);
extern void func_00289178(void);
extern void func_002F52B8(void);
