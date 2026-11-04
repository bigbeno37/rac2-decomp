#include "common.h"

typedef int Quad __attribute__((mode(TI)));
typedef struct { Quad v[3]; } __attribute__((aligned(16))) Axes4045E0;

extern char D_L04_001B9680[];
extern Quad D_L04_001BFAF0;
/* const: retail loads the three rows past the flag stores before them. */
extern const Axes4045E0 D_001B0630;

extern float func_00282E48(void *, void *);
extern void func_002A93B0(void *);
extern int func_00277258(void *, int, void *, float);
extern void func_L04_00402C88(u8 *);
extern void func_00282D30(void *, void *, float);
extern float func_002A7878(float, float);
extern void func_L04_00403530(u8 *, u8 *);
extern void func_L04_004027B0(u8 *, u8 *);
extern void func_L04_00400EE0(u8 *, u8 *);
extern void func_L04_004044B8(u8 *, u8 *);
extern void func_L04_00404100(u8 *, u8 *);
extern s32 func_L04_00400AB0(void *, void *, s32);
extern float func_00283CE0(int);
extern void func_002C96F8(float, float, void *, void *, void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern int func_00283DB8(void *, float);
extern float func_00282DB0(void *, void *);
extern void func_L04_00410518(u8 *, u8 *, void *, void *, void *, void *);

/* Update of the path follower: state 0 lands it, scales its three axes and draws six random
   phases; state 2 runs its behaviours while the leader is not waiting; state 3 walks to the
   nearest path node and, after 300 frames, leaves for the leader once it is 5 units away and
   out of sight; state 4 drops both targets. The leader's own state can hold it entirely. */
