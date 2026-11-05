/* func_PAL_L01_002AE010, the PAL copy of func_L01_002AE128: the same C, where rename func_002C9280 becomes func_L00_002D3B00; rename func_002E3738 becomes func_00282F20; rename func_002E3470 becomes func_00282CC0; rename func_002D3C80 becomes func_002761F8; rename func_002D77E8 becomes func_00278E78; rename func_002C9628 becomes func_L00_002D3EA8; rename func_003148A0 becomes func_002AB1F0; rename func_00316D00 becomes func_002AD0B0; rename func_002E34A0 becomes func_00282CF0; rename func_002D4CE0 becomes func_00277258; rename func_002E3918 becomes func_00283098; rename func_002C92D8 becomes func_L00_002D3B58; .... */
#include "common.h"

extern void func_00282CF0(void *, void *, void *);
extern float func_00282DB0(void *, void *);
extern void func_00282F20(void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern int func_00283CF0(float);
extern unsigned char *D_L01_001FF164;
extern int func_00115AC0(char *, char *, int);
extern void func_L00_002AF7D0(void);
extern void func_L00_002B1F90(void);
extern void func_L00_002D2E68(void);
extern void func_L00_002A6708(void);
extern void func_L00_002A53B8(void);
extern void func_L00_002D24B8(void);
extern void func_002A677C(int);
extern void func_00283098(void *, void *, void *);
extern char *D_L01_001B255C MACRO_ADDR;
extern void func_00282F20(void *, void *, float);
typedef int MotionQuad __attribute__((mode(TI)));
typedef union { float v[4]; MotionQuad q; } MotionVec;

typedef struct {
    MotionVec motion;
    char pad10[0x70];
    MotionVec position;
    char pad90[0x50];
    MotionVec probeEnd;
    char padF0[0x4D0];
    char *held;
} MotionHero;
typedef struct { MotionVec forward[4]; MotionVec inverse[4]; } MotionPath;

extern MotionHero motionHero __asm__("D_00189E20");
typedef struct {
    char pad00[0x30];
    float hitX;
    float hitY;
} MotionCollision;
extern MotionCollision motionCollision __asm__("D_L01_001BF900");

extern char D_00188660[];
extern MotionPath *D_L01_001B264C[] __attribute__((sda));
extern int func_002ACBC8(void *, int);
extern void func_00282CC0(void *, void *, void *);

typedef struct { char *p[6]; } Sep6;

extern Sep6 D_L01_001AA550;

extern char D_L01_00189E20[];

extern char D_L01_00189E20[];

extern char *r1_D_L01_00160058 MACRO_ADDR;

extern int r1_D_L01_00160058_m __asm__("r1_D_L01_00160058") MACRO_ADDR;

typedef struct { char pad[0x18]; void *moby; int count; char gap[0x20]; float normal[4]; } ProbeResult;
extern ProbeResult D_L01_001BF900;

extern void func_L00_002D3B58(void *, void *, float);

extern int func_002761F8(void *, void *, int, void *, int);

extern int func_00278E78(void);

extern float func_L00_002D3EA8(void *);

extern void func_00282CF0(void *a0, void *a1, void *a2);

extern void func_00282CC0(void *a0, void *a1, void *a2);

extern void func_00283098(void *a0, void *a1, void *a2);

extern void func_00282CC0(void *a0, void *a1, void *a2);

extern float func_00283B98(float f12, float f13);

extern float func_002A7878(float f12, float f13);

extern float func_00283248(float f12, float f13);

extern char D_00188660[];

typedef int LedgeQuad __attribute__((mode(TI)));
typedef struct {
unsigned char pad0[464];
short cooldown;
unsigned char pad1d2[146];
float radius;
unsigned char pad268[180];
float groundHeight;
unsigned char pad320[382];
unsigned char pastApex;
unsigned char pad49f[1945];
float frame;
unsigned char padc3c[5716];
void * moby;
int state;
unsigned char pad2298[4];
int control;
unsigned char pad22a0[20];
unsigned char vehicle;
} LedgeProbeResident;
extern LedgeProbeResident ledgeHero __asm__("D_00189E20");
extern float func_L00_002D3B00(void *);
extern void func_L00_002D3B58(void *, void *, float);
extern float func_L00_002D3EA8(void *);
extern float func_00282DB0(void *, void *);
extern float func_00283180(float);
extern int func_002761F8(void *, void *, int, void *, int);
extern int func_00278E78(void);
extern char *func_002AB1F0(void *);
extern char *func_002AD0B0(void *);
extern int func_00277258(void *, int, void *, float);
typedef struct {
    unsigned char pad0[0x18];
    void *moby;
    unsigned char pad1c[4];
    float position[4];
    unsigned char pad30[16];
    float normal[4];
} LedgeProbeHit;
extern LedgeProbeHit ledgeHit __asm__("D_L01_001BF900");
#define FL(p,o) (*(float *)((char *)(p)+(o)))
#define IN(p,o) (*(int *)((char *)(p)+(o)))
/* The ledge probe of func_L00_002ACF88 without its vehicle == 1 branch. */
int func_PAL_L01_002AE010(void *position, void *unused, char *out)
{
    float v0[4], v1[4], v2[4], v3[4], v4[4], v5[4], v6[4], v7[4], v8[4], v9[4], v10[4], v11[4], v12[4], v13[4], v14[4], v15[4], v16[4], v17[4], v18[4], v19[4];
    char *a, *b;
    void *object;
    int go, found, i, kind, ok;
    int edge, j;
    float height;
    float step, max;
    float down, top, drop, d, length;
    LedgeProbeResident *hero;
    if (out) IN(out,0x28)=0;
    if (ledgeHero.cooldown) return 0;
    go=0;
    if (ledgeHero.vehicle==0) {
        if ((ledgeHero.control==4 && (ledgeHero.pastApex || func_L00_002D3B00((char *)&ledgeHero+0xf0)<0.07000001f)) || ledgeHero.control==2 || ledgeHero.state==8 || ledgeHero.state==25 || ledgeHero.state==26) go=1;
        if (ledgeHero.state==14 && ledgeHero.frame<37.0f) go=0;
    }
    if (out != (char *)&ledgeHero+0x550) go=1;
    if (!go) return 0;
    hero=&ledgeHero;
    down=-0.7f; top=1.15f; drop=-1.43f;
    found=0;
    qcopy(v0,position);
    func_00282F20(v4,hero,hero->radius);
    func_00282CC0(v0,v0,v4);
    func_00282F20(v4,(char *)&ledgeHero+0x20,1.65f);
    func_00282CC0(v1,v0,v4);
    func_00282F20(v4,(char *)&ledgeHero+0x20,-top);
    func_00282CC0(v2,v0,v4);
    func_00282F20(v3,hero,0.075f);
    for(i=0; i<4; i++) {
        func_00282CC0(v1,v1,v3);
        func_00282CC0(v2,v2,v3);
        if(func_002761F8(v1,v2,4,ledgeHero.moby,0)) {
            kind=func_00278E78();
            if(kind!=9 && kind!=12 && kind!=0 && func_L00_002D3EA8(ledgeHit.normal)<0.349065899848938f) found=1;
        }
        if(found) break;
    }
    object=0;
    if(!found) goto fail;
    if(ledgeHit.moby) {
        ok=0;
        a=func_002AB1F0(ledgeHit.moby);
        b=func_002AD0B0(ledgeHit.moby);
        if(a && (*(unsigned short *)(a+0x1e)&1)) ok=1;
        else if(b) ok=IN(b,0x3c)&1;
        if(!ok) return 0;
        object=ledgeHit.moby;
    }
    if(out) *(void **)(out+0x38)=ledgeHit.moby;
    qcopy(v6,ledgeHit.position);
    func_00282CF0(v5,v6,position);
    if(1.5f<func_L00_002D3B00(v5)) return 0;
    func_00282F20(v7,(char *)&ledgeHero+0x20,0.005f);
    func_00282CC0(v7,v7,v6);
    func_00282F20(v8,(char *)&ledgeHero,-1.0f);
    func_00282CC0(v8,v8,v7);
    if(func_002761F8(v8,v7,2,ledgeHero.moby,0)) {
        if(!ledgeHit.moby || ledgeHit.moby!=object) return 0;
    }
    if(out) {
        func_00282F20(v9,(char *)&ledgeHero+0x20,0.6f);
        func_00282CC0(v9,v9,v6);
        if(func_00277258(v9,4,ledgeHero.moby,0.45f)) IN(out,0x34)|=1;
        else IN(out,0x34)&=~1;
    }
    height=func_L00_002D3B00(v5);
    if(height+ledgeHero.groundHeight<1.8f || height<0.5f) return 0;
    *(LedgeQuad *)v11=0; FL(v11,0)=1.0f; FL(v11,8)=down;
    func_00283098(v11,v11,(char *)&ledgeHero);
    edge=0;
    func_00282CC0(v11,v11,v6);
    func_00282CF0(v9,position,v11);
    func_L00_002D3B58(v9,v9,0.0f);
    func_00282CC0(v9,v9,v11);
    func_00282F20(v10,(char *)&ledgeHero+0x20,-down*0.2f);
    for(j=0;j<5;j++) {
        if(func_002761F8(v9,v11,2,0,0)) {
            func_00282F20(v12,ledgeHit.normal,1.0f);
            edge=1;
            func_L00_002D3B58(v12,v12,0.0f);
            break;
        }
        func_00282CC0(v9,v9,v10);
        func_00282CC0(v11,v11,v10);
    }
    if(!edge) return 0;
    length=func_00282DB0((char *)&ledgeHero,v12);
    if(!(length<func_00283180(1.134464144706726f))) return 0;
    step=0.03f;
    max=2.4f;
    qcopy(v18,v6);
    func_00282F20(v17,(char *)&ledgeHero+0x2c0,-0.05f);
    func_00282F20(v16,v12,step);
    func_00282CC0(v15,v16,v17);
    func_00282CF0(v16,v16,v17);
    func_00282CC0(v13,v18,v16);
    func_00282CC0(v14,v18,v15);
    for(d=0.0f;d<max;d+=step) {
        if(!func_002761F8(v14,v13,4,ledgeHero.moby,0)) {
            if(out) {
                qcopy(out+0x10,v18);
                func_00282F20(v19,v12,0.5f);
                func_00282F20(out,(char *)&ledgeHero+0x2c0,-drop);
                func_00282CC0(out,v19,out);
                func_00282CC0(out,out+0x10,out);
                FL(out,0x24)=func_00283248(FL(v12,0),FL(v12,4));
                IN(out,0x28)=1;
            }
            return 1;
        }
        qcopy(v18,ledgeHit.position);
        func_00282CC0(v14,v18,v15);
        func_00282CC0(v13,v18,v16);
    }
fail:
    return 0;
}
#undef FL
#undef IN

extern char D_00189E20[];
extern void func_00283098(void *, void *, void *);
extern float func_00283B98(float, float);

/* A moby's model: the part of it the file uses. */
/* A moby (0x100 bytes), as far as the file uses it. */
/* One entry of the weapon table (0xE0 bytes). */
/* The hero block (D_00189E20), the fields func_L01_002B0318 uses. */
/* For the hero's two attached mobys: point them at their buffers, start the animation the
   current item's table entry names, fade them in or out (removing one that has faded out),
   then copy the hero moby's matrix to hero + 0x730 and ease hero + 0x778 to 0. */
extern void func_L00_002AF7D0(void);
extern void func_L00_002B1F90(void);
extern void func_L00_002D2E68(void);
extern void func_L00_002A6708(void);
extern void func_L00_002A53B8(void);
extern void func_L00_002D24B8(void);
extern void func_002A677C(int a0);
extern char D_00189E20[];

/* func_L01_002B3788 is defined in L01_002B3788-contact.c. */

/* func_L01_002B3D28 is defined in L01_002B3D28.c. */

/* Stops the hero against a wall: casts two rays along the movement direction (or the vector at
   the start of the hero's data when forced or nearly still), keeps the moby it hit and takes the
   velocity component into the wall's flattened normal away. */
/* func_L01_002B5EA0 is in L01_002A7248-collide-g8.c. */

/* func_L01_002B6948 is in L01_002B6948-velocity.c. */

/* func_L01_002BE0E0 is defined in L01_002A7248-timers-g8.c. */

/* func_L01_002C2CF0 is defined in L01_002C2CF0-keep-block-order.c. */

/* func_L01_002E1680 is defined in L01_002E1680.c. */

/* Draws a textured quad with the given corners and colour; rotate turns the texture
   coordinates by that many quarter turns. */
/* func_L01_002EABA8 is defined in L01_002A7248-pre.c. */

/* Defined in L01_002F14E8-g8.c. */

/* Defined in L01_00310BC0.c. */

/* rac1-decomp src/overlays/shared/mobyutil_0026E8E0.c: func_L01_0026F090 */
// Walks a list of moby indices and sets the state byte of each one that is non-negative.
/* Defined in L01_00311C18.c. */

/* func_L01_003120A8 is defined in L01_003120A8-helpers.c. */

/* chain of four probes on the same object; the first that reports a hit picks the code */
/* func_L01_00318638 is defined in L01_00318638-qcopy-barrier.c. */

/* Retail compares the vertical speed on landing, then stores the same value on both sides of the
 * test, so this call's arms merge at jump2 and only the comparison stays behind. */

/* One entity of the group: the kind word is at 0xAA. */
typedef struct L01Thing {
    u8 pad[0xAA];
    s16 kind;
} L01Thing;

/* The group: the count is at 0x44, the list of entities at 0x18C. */
typedef struct L01Group {
    u8 pad[0x44];
    s16 count;
    u8 pad46[0x18C - 0x46];
    L01Thing *items[1];     /* bounded by count */
} L01Group;

extern L01Group D_L01_001BD1C0;
/* shared native body of family d1b66aa7f0ecd20d */
extern int func_00115AC0(char *dst, char *src, int len);

/* Expands a template: copies src into the scratchpad, then writes it to out, replacing %d (next number) and %<n>d (n-th number) with the numbers listed in nums (every second int, up to ten, stopped by -1). */
/* func_L01_003245E0 is defined in L01_002A7248-g8.c (this unit has no $gp access for it). */

/* func_L01_003246D0 is defined in L01_003246D0.c. */

/* Scans the half-open span of the pair table for the caller's key. */
/* RAC1 counterpart: src/overlays/shared/span_particle.c, spawn_span_particle. */
/* Spawns a span particle of the given kind (0 to 2) at pos, moving along vec and carrying scale. */
typedef int Quad331CA0 __attribute__((mode(TI)));
typedef struct { unsigned char b0, b1, b2, b3; int color; unsigned char seed, alpha; unsigned short size; float life; union { Quad331CA0 q; float f[4]; } position; float payload[8]; } Particle331CA0;
extern Particle331CA0 *func_002B9030(int kind);
extern float func_002A7878(float low, float high);
extern int func_00283CF0(float value);
extern unsigned char *D_L01_001FF164;

/* Spawns the burst particle of the given kind at position, carrying the velocity and the size scale in its payload. */
/* func_L01_00331E08 is in L01_00331E08.c. */

/* Averages each 16-byte record of the table the argument points at against the record
   (i + 1) % n, storing the result in the record's own +0xC field and returning the
   running sum; a null argument reports and answers -1.0f. */
/* A closed path: the point count, then the points from offset 0x10. */
/* Moves distance further along the closed path from the given segment, walking forward or
   back by direction; writes the point reached to out and returns the segment it lies on. */
/* func_L01_003808C0 is in L01_003808C0-g8.c for its GP accesses. */

/* func_L01_0038EF40 is in L01_0038EF40.c: its unit needs -g. */

/* A model with kind 0x12 is exempt from the trigger's visibility switches. */
#define SPATIAL_TRIGGER_EXEMPT(p) \
    ((p) && (p)->model && (p)->model->kind == 0x12)

/* Applies a trigger's settings to the moby at index: each switch is positive to turn its
   effect on, negative to turn it off; inactive reverses them. */
#undef SPATIAL_TRIGGER_EXEMPT

extern int func_002ACBC8(void *, int);

