#include "common.h"
typedef int ParticleWord __attribute__((mode(TI)));
typedef union { ParticleWord word; float f[4]; } ParticleVector;
typedef struct { int a,b,c,d; } __attribute__((aligned(16))) Quad16;
extern void func_00282CC0(void *, void *, void *);
extern char ref_00305798_00[][0x30];

typedef struct {
    ParticleVector position, velocity;
    float timer;
    char *owner;
    u8 reserved28[8];
} ParticleTrail;
typedef struct {
    Quad16 normal;
    int owner, flags;
    u8 kind, source;
    unsigned short objectClass;
    float damage;
    int mode;
    float secondaryDamage;
} HitContactView;
typedef struct { u8 prefix[0x70]; float damage; u8 remainder[0xe0 - 0x74]; } ParticleDamageDescriptor;
typedef struct { u8 prefix[0x18]; char *hit; u8 pad1C[4]; ParticleVector position; } ParticleHit;
extern ParticleHit ref_003054E8_00;
extern ParticleDamageDescriptor ref_003054E8_02[];
extern u8 D_00139568[];
typedef struct { u8 prefix[0x2290]; char *moby; } ParticlePlayer;
extern ParticlePlayer D_00189E20;
extern int func_002829D8(float *);
extern int func_002761F8(void *, void *, int, void *, void *);
extern void func_00282F20(void *, void *, float);
extern void func_002A8C00(char *, int, int, float, Quad16 *);
extern void func_002A8E20(char *, void *);
extern int func_00334160(char *, int, int);
extern void func_00306680(void *, int);

/* Advance owned particle trails and turn a collision into a contact and impact. */
