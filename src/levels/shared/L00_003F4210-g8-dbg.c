/* Updates a bouncing explosive projectile: flight with a particle trail, bounces that lose speed,
   a fading slide after the fourth bounce, then an explosion that damages everything in range.
   Needs -g (pre-div.s nop pairs) and hazard labels (two branch targets). The length is stored
   to the member before it is scaled: that store is deleted after allocation, but until then it
   keeps the call result alive past the multiplication, which ties the product to the scale. */
#include "common.h"

typedef unsigned int QuadWord __attribute__((mode(TI)));
typedef union { QuadWord word; float v[4]; } Vector;
typedef struct {
    Vector impulse;
    int owner, flags;
    unsigned char enabled, kind;
    unsigned short object_class;
    float damage;
    int active;
} DamageRequest;

typedef struct { float r, g, b; } Color;
typedef struct {
    unsigned char pad[0x10];
    Vector velocity;
    int timer;
    char *owner;
    short particle_timer, bounces;
    float fade_scale;
} ProjectileState;
/* The hit pointer is outside the GP-relative displacement range. */
extern char *D_L00_001BF218 NOT_SDA;
typedef struct {
    char pad[0x18];
    char *owner;
    int flags;
    Vector position, pad30, normal;
} Collision;
extern Collision D_L00_001BF200;
extern int D_L00_001A8FF0;
extern Vector D_L00_001A8F20 __attribute__((sda)) NOT_SDA;
extern char *D_L00_001C3280[];
extern unsigned char D_00139587 NOT_SDA;
typedef struct {
    unsigned char pad[0x74];
    float damage;
    unsigned char rest[0x68];
} DamageTableRow;
extern DamageTableRow D_L00_00262570[];
extern Color D_L00_001B0920;
extern float D_L00_001B0900 __attribute__((sda));
extern int D_L00_001B0904 __attribute__((sda));
extern float D_L00_001B0908 __attribute__((sda));
extern float D_L00_001B090C __attribute__((sda));
extern float D_L00_001B0918 __attribute__((sda));
extern float D_L00_001B091C __attribute__((sda));
extern int func_00277258(Vector *, int, char *, float);
extern int func_00282948(int *);
extern int func_00282978(short *);
extern void func_00282CC0(Vector *, const Vector *, const Vector *);
extern void func_00282CF0(Vector *, const Vector *, const Vector *);
extern void func_L00_003252A8(void *, void *, int);
extern void func_00282D30(Vector *, const Vector *, float);
extern float func_00282DB0(const Vector *, const Vector *);
extern void func_L00_0031D670(Vector *, const Vector *, const Vector *, float);
extern int func_002A77E0(int);
extern float func_00283CE0(int);
extern char *func_002BDCE8(Vector *, float, float, float, int, const Vector *, float, int, int);
extern int func_002761F8(Vector *, Vector *, int, char *, int);
extern void func_00282F20(Vector *, const Vector *, float);
extern void func_00283028(Vector *, const Vector *, const Vector *);
extern int func_002E5CF8(int, int, char *);
extern void func_00282F70(Vector *, const Vector *, float);
extern float func_00282E20(const Vector *);
extern void func_003001E0(Vector *, const Vector *, const Vector *);
extern int func_00283CF0(float);
extern void func_00300190(void *, void *);
extern int func_002E5DA0(int, int, char *, int);
extern void func_002ABBB8(void *, float, void *, float, void *, int, int, int,
                         float, float, float, float, int, float, int, int, int, int, int);
extern void func_002A8BD0(char *, int, int, float);
extern int func_00277F48(Vector *, int, char *, DamageRequest *, float);
extern int func_00334160(char *, int, int);
extern void func_0029F338(void *);
extern float func_00283B98(float, float);
