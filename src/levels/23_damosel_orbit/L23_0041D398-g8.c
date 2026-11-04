/* Level 23 encounter damage phases. The function reads $gp data (hence -G8)
 * and lives in a unit of its own, like its neighbours.
 * The effect phases keep their index, table base, slot and direction in
 * block locals: sched1 then keeps the slot address after the random call,
 * and local-alloc gives direction, slot and index $s0, $s1 and $s2.
 * The local holding 10 puts its load ahead of the 1 while the stores keep
 * their order, as in retail.
 */
#include "common.h"
typedef int Quad __attribute__((mode(TI)));
/* Two five-part windows share this ten-pointer table. */
typedef struct { u8 prefix[0x358]; char *parts[10]; } PartState;
extern char *D_L23_001B1484 __attribute__((sda));
extern int D_L23_001B1468 __attribute__((sda));
extern int D_L23_001B1434 __attribute__((sda));
extern char *D_L23_001B13D0 __attribute__((sda));
extern float D_L23_001B16D0 __attribute__((sda));
extern int D_L23_001B3720;
/* Retail reads this byte again at 0041D43C on the entry-cleanup path. */
extern volatile unsigned char D_001A7AFD;
extern int func_002F5D10(void);
extern void func_002A6C58(char *, int, int);
extern void func_L23_0041D9B0(void);
extern void func_L00_003294F0(unsigned char *, int);
extern void func_0029F338(void *);
extern void *func_0032C700(void *, int, int, int, int, int, int, int, float, float, int, int, int, void *);
extern int func_002E5CF8(int, int, char *);
extern void func_0029FC20(char *, int, char *);
extern void func_00282D30(char *, char *, float);
extern float func_002A7878(float, float);
extern void func_00282CC0(char *, char *, char *);
extern char *func_00273730(char *, int, int, int, char *, char *, float *, int, float, float);
extern void func_00310C88(u8 *);
extern int func_002A77E0(int);

/* Move through the encounter's damage phases, detaching pieces and spawning debris. */
