#include "common.h"

typedef int ParticleWord __attribute__((mode(TI)));
typedef union { ParticleWord word; float f[4]; } ParticleVector;
extern float ref_00302470_03 SDATA(ref_00302470_03);
extern float ref_00302470_04 SDATA(ref_00302470_04);
extern float ref_00302470_05 SDATA(ref_00302470_05);
extern float ref_00302470_06 SDATA(ref_00302470_06);
extern float ref_00302470_07 SDATA(ref_00302470_07);
extern float ref_00302470_08 SDATA(ref_00302470_08);
extern float ref_00302470_09 SDATA(ref_00302470_09);
extern int ref_00302470_01 SDATA(ref_00302470_01);
extern ParticleVector ref_00302470_02 __attribute__((nosda));
extern int D_001A7A10[] MACRO_ADDR;
extern char *D_0018C0B0 __attribute__((nosda));
extern float func_00282E48(void *, void *);
extern char *func_0029EFD0(int);
extern void func_002833F0(void *, void *);
extern void func_002AEC18(void *, int);
extern void func_002A04B8(void *, u32 *, u32 *, u32 *);
extern void func_002A0468(unsigned long long *, unsigned long long,
    unsigned long long, unsigned long long, unsigned long long);
extern void func_002A0488(void *, u32, u32, u32);
extern int func_002A77E0(int);
extern float func_002A7940(void);
extern float func_002A7878(float, float);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282DD8(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
extern void func_002AB668(void *, int, float);
extern int func_003022A0(char *, char *, float, float);
extern void func_002A0F48(void *);

typedef struct BoltVars {
    u8 pad00[0x20];
    ParticleVector velocity, spin;
    u8 kind;
    u8 pad41[0xF];
    float angle;
    u8 pad54;
    u8 flags;
    u8 pad56[2];
    float phase;
    u8 pad5c[0xC];
    float startZ;
} BoltVars;
typedef struct BoltClass { u8 pad00[0x24]; float scale; } BoltClass;
typedef struct BoltSpawnObject {
    u8 pad00[0x10];
    ParticleVector position;
    u8 state, pad21[3];
    BoltClass *objectClass;
    u8 pad28[4];
    float scale;
    u8 updateDistance, visible;
    short drawDistance;
    unsigned short flags;
    u8 pad36[0x2d];
    u8 life;
    u8 pad64[4];
    BoltVars *vars;
    u8 pad6c[0x4c];
    struct BoltSpawnObject *parent;
    u8 padbc[4];
    ParticleVector orientation[3];
    ParticleVector rotation;
} BoltSpawnObject;
/* Allocate the denomination near the camera and initialize its pickup motion. */
