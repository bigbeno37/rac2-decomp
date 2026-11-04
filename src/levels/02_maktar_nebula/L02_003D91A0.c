#include "common.h"

extern int D_L02_001B359C[] __attribute__((sda));
/* Read-only view: these loads may pass the state stores, as in retail. */
extern const int D_L02_001B359C_1[] __attribute__((sda));
extern int D_L02_001FF6B8[];
extern int func_00283CF0(float);
extern void func_002A04B8(char *, void *, void *, void *);
extern void func_00282A60(void *, int, int);
extern void func_L02_003DB6D8(char *);
extern void *func_002A9640(char *, int, int);
extern float func_002A96A0(char *, void *);
extern void func_002AB800(int, char *);
extern void func_002A7750(char *, int, int);
extern void func_002AF1D0(char *, char *, int, int);
extern int func_002B3BA8(char *, int, int, int, float, float);
extern void func_L02_003D4990(char *);
extern void func_L02_003FC020(void *, void *);
extern void func_L02_003D9728(char *);
extern void func_L02_003D9E68(char *);

/* Field-at-use prefix only; the full state allocation is not inferred. */
struct InitPrefix {
    u8 reserved00[0x28]; u8 mode, ready; u8 reserved2A[6];
    float radius; u8 reserved34[0x14]; u8 flag; u8 reserved49; u8 limit;
    u8 reserved4B[5]; int enabled; float gain, weight, scale, turn;
    u8 reserved64[0x18]; float rate, maximum, epsilon; int phase;
    u8 reserved8C[0x98]; float blend; u8 reserved128[0x11]; u8 active;
};
