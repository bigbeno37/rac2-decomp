#include "common.h"

typedef int ParticleWord __attribute__((mode(TI)));
typedef union { ParticleWord word; float f[4]; } ParticleVector;
typedef struct { float x,y,z,w; } V4_F213135aa;
extern void func_00282D30(void *, void *, float);

typedef unsigned long long NativeCompactU64;
typedef union {
    s32 field00[4];
    struct {
        NativeCompactU64 field00;
        short field08;
        short field0A;
        unsigned short field0C;
        unsigned short field0E;
    } compact;
} NativeCompactRow16;
typedef struct {
    char pad00[4];
    short field04;
    char pad06[2];
    short field08;
    short field0A;
    char pad0C[0x14];
    u32 field20;
} NativeSpawnHeader;
extern void func_00300748(NativeSpawnHeader *, char *, float);
extern void func_00283388(float *);
/* Partial native draw header through the packed-vertex tint field. */
typedef struct {
    short vertexCount, quadCount, boneCount, textureCount, frameCount, boneStride;
    float scale;
    void *vertices;
    u8 *quads;
    NativeCompactRow16 *textures;
    u8 *texels;
    void *bones;
    int packed;
    u32 tint;
} NativeDrawHeaderView;
typedef struct {
    u8 *source;
    unsigned short reserved04, oldPage;
    u8 *secondSource;
    u8 width, height;
    unsigned short newPage;
} TextureUpload;
typedef struct { NativeCompactU64 color, coordinate, position, reserved18; } ScratchVertex;
extern int D_001A72D0;
extern int D_001A7350[2];
extern u8 D_001A7BB1;
extern int ref_00300AA8_00;
extern TextureUpload ref_00300AA8_01[64];
extern float nativeProjection[16] SDATA(ref_00300AA8_02);
extern float ref_00300AA8_02[16] __attribute__((sda));
typedef struct { char pad[0xB0]; float size[2]; char padB8[0x148]; float scale[2]; } NativeDrawView;
extern NativeDrawView ref_00300AA8_03;
extern ParticleWord ref_00300AA8_06 SDATA(ref_00300AA8_06);
/* The queue retains the qualifiers of its existing renderer declaration. */
extern volatile unsigned int * volatile D_001B22A8;
extern volatile unsigned int *nativeQueue SDATA(D_001B22A8);
extern void func_00300910(void);
extern void func_00282D30(void *, void *, float);
extern void func_00283788(void *, void *, void *);
extern void func_0029D258(void *, void *, int, int);
extern void func_0029D398(void *, void *, int, u32, int);

/* Upload referenced textures, transform the native vertices and submit each quad. */
