/* func_PAL_L01_0031C9C0, the PAL copy of func_L01_0031AB20: the same C. */
#include "common.h"

extern void func_00282CF0(void *, void *, void *);
extern float func_00282DF0(void *);
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
typedef struct {
    MotionVec velocity;
    char pad10[0x10];
    int flags;
    char pad24[0xC];
    int frame;
    float gain;
    float distance;
    int pad3C;
    int sound;
} MotionState;
typedef struct { MotionVec forward[4]; MotionVec inverse[4]; } MotionPath;

extern MotionHero motionHero __asm__("D_00189E20");
typedef struct {
    char pad00[0x30];
    float hitX;
    float hitY;
} MotionCollision;
extern MotionCollision motionCollision __asm__("D_L01_001BF900");

extern char D_00188660[];
extern int D_L01_001B2088[] __attribute__((sda));
extern MotionPath *D_L01_001B264C[] __attribute__((sda));
extern float D_L01_001BF928;
extern void func_00282D08(void *, void *, void *, float);
extern float func_00282E48(void *, void *);
extern float func_00282E80(void *, void *);
extern void func_00282F70(void *, void *, float);
extern void func_002833F0(void *, void *);
extern int func_002A8C28(void *, void *, int, int, int, int, float, float, float);
extern void func_002AC9F8(void *, float);
extern int func_002ACBC8(void *, int);
extern int func_002C9218(int, void *, void *, float);
extern void func_002E59F8(int);
extern int func_002E5CF8(int, int, void *);
extern int func_L00_002E0200(void *, float, float, int, void *);
extern int func_L01_0031B300(void *, void *, void *, void *);

extern void func_002E3470(void *, void *, void *);

typedef struct { char *p[6]; } Sep6;

extern Sep6 D_L01_001AA550;

extern char D_L01_00189E20[];

extern char D_L01_00189E20[];

extern char *r1_D_L01_00160058 MACRO_ADDR;

extern int r1_D_L01_00160058_m __asm__("r1_D_L01_00160058") MACRO_ADDR;

typedef struct { char pad[0x18]; void *moby; int count; char gap[0x20]; float normal[4]; } ProbeResult;
extern ProbeResult D_L01_001BF900;

extern float func_00282E20(void *);

extern void func_00282D30(void *, void *, float);

extern int func_002761F8(void *, void *, int, void *, int);

extern void func_002E3470(void *a0, void *a1, void *a2);

extern void func_002E3470(void *a0, void *a1, void *a2);

extern float func_00283B98(float f12, float f13);

extern float func_002A7878(float f12, float f13);

extern char D_00188660[];

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
#undef FL
#undef IN

extern char D_00189E20[];
extern void func_002AA2E8(void *, float, float);
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
static inline void MotionLandingSpeed(float *speed, float rising, float falling)
{
    if (0 < *speed) {
        *speed = rising;
    } else {
        *speed = falling;
    }
}

void func_PAL_L01_0031C9C0(char *m, MotionState *state, int path, float height, float radius, float extent)
{
    MotionVec old, start, delta, turn, target, change, samples[8], matrix[3], actual;
    float speed, gain, limit, accel, ground;
    int i, handle;
    int phase = D_L01_001B2088[0];

    if ((phase & 15) == (((int)m >> 8) & 15)) {
        float dist = func_00282E48(&motionHero.position, m + 0x10);

        if (*(unsigned short *)(m + 0x34) & 0x1000) {
            if (state->distance < dist) {
                *(unsigned short *)(m + 0x34) &= 0xEFFF;
            }
        } else if (dist < state->distance) {
            *(unsigned short *)(m + 0x34) |= 0x1000;
        }
    }
    qcopy(&old, m + 0x10);
    if (motionHero.held == m) {
        qcopy(&start, m + 0x10);
        start.v[2] += 0.5f;
        if (func_002761F8(&start, &motionHero.probeEnd, 2, m, 0)) {
            motionHero.held = 0;
        }
        func_00282CF0(&delta, m + 0x10, &motionHero.position);
        if (state->frame != -1 && func_002ACBC8(m + 0x10, state->frame)) {
            func_00283098(&delta, &delta, D_L01_001B264C[0][state->frame].inverse);
            delta.v[1] = 0;
            func_00283098(&delta, &delta, D_L01_001B264C[0][state->frame].forward);
        }
        gain = 4.0f;
        if (state->gain != 0) {
            gain = state->gain;
        }
        func_00282F70(&delta, &delta, gain);
        func_00282CC0(&delta, &delta, &motionHero.position);
        func_00282F20(&turn, &motionHero.motion, gain);
        func_00282CC0(&turn, &turn, &motionHero.position);
        turn.v[2] = delta.v[2];
        func_00282D08(&delta, &delta, &turn, 0.5f);
        func_00282CF0(&delta, &delta, &motionHero.position);
        func_00282F20(&delta, &delta, gain);
        func_00282CC0(&delta, &delta, &motionHero.position);
        if (state->flags & 4) {
            gain = func_00282E80(m + 0x10, &delta) * 0.64000005f;
            accel = 0.020000001f;
            limit = 0.32000002f;
        } else {
            gain = func_00282E80(m + 0x10, &delta) * 0.080000006f;
            accel = 0.008f;
            limit = 0.16000001f;
        }
        if (limit < gain) {
            gain = limit;
        }
        func_00282CF0(&target, &delta, m + 0x10);
        target.v[2] = 0;
        func_00282F20(&target, &target, gain);
        func_00282CF0(&change, &target, state);
        func_002AC9F8(&change, accel);
        state->velocity.v[0] += change.v[0];
        state->velocity.v[1] += change.v[1];
    } else {
        speed = func_00282DF0(state);
        func_002AA2E8(&speed, 0, 0.004f);
        func_00282F20(state, state, speed);
    }
    if (0.001f < func_00282DF0(state)) {
        if (!(state->flags & 2)) {
            qcopy(&start, m + 0x10);
            qcopy(&delta, m + 0x10);
            start.v[2] += 0.25f;
            delta.v[2] -= 1;
            ground = 0;
            if (func_002761F8(&start, &delta, 16, m, 0)) {
                ground = D_L01_001BF928;
            }
            if (*(float *)(m + 0x18) < ground) {
                *(float *)(m + 0x18) = ground;
                MotionLandingSpeed(&state->velocity.v[2], 0, 0);
            } else {
                state->velocity.v[2] -= 0.008f;
            }
        }
        if (!(state->flags & 4) && func_00282E80(m + 0x10, &motionHero.position) < radius + 2.5f) {
            float toward;

            func_00282CF0(&start, &motionHero.position, m + 0x10);
            start.v[2] = 0;
            func_00282F20(&start, &start, 1);
            toward = func_00282DB0(&start, state);
            if (0 < toward) {
                func_00282F20(&delta, &start, -toward);
                func_00282CC0(state, state, &delta);
            }
        }
        turn.q = 0;
        turn.v[2] = 0.7853982449f;
        func_00282CC0(m + 0x10, m + 0x10, state);
        qcopy(&delta, m + 0x10);
        delta.v[2] += 0.25f;
        func_002833F0(matrix, &turn);
        func_00282F70(&start, state, radius);
        for (i = 0; i < 8; i++) {
            func_00283098(&start, &start, matrix);
            func_00282CC0(&samples[i], m + 0x10, &start);
            samples[i].v[2] += 0.25f;
            func_L01_0031B300(m, state, &delta, &samples[i]);
        }
        func_00282D30(&target, state, 5);
        func_00282CC0(&target, &target, m + 0x10);
        target.v[2] += radius;
        func_002A8C28(m, &target, 0x4000000, 0, 1, 0, radius + 0.2f, 1, 1);
        qcopy(&change, m + 0x10);
        change.v[2] += height;
        if (func_L00_002E0200(&change, radius, extent, 0, m)) {
            *(float *)(m + 0x10) = motionCollision.hitX;
            *(float *)(m + 0x14) = motionCollision.hitY;
        }
        if (path != -1) {
            func_002C9218(path, m + 0x10, m + 0x10, radius);
        }
        func_00282CF0(&actual, m + 0x10, &old);
        gain = func_00282E20(&actual);
        if (gain < func_00282E20(state)) {
            state->velocity.v[0] = actual.v[0];
            state->velocity.v[1] = actual.v[1];
        }
        if (func_00282E20(state) != 0) {
            char *sound = D_00188660 + state->sound * 0x70;

            if (*(char **)(sound + 0x88) != m || !((unsigned char *)sound)[0x74]) {
                state->sound = func_002E5CF8(0, 4, m);
            }
        } else {
            handle = state->sound;
            if (handle != -1) {
                unsigned char *sound = (unsigned char *)D_00188660 + handle * 0x70;

                if (*(char **)(sound + 0x88) == m && sound[0x74]) {
                    func_002E59F8(handle);
                }
            }
            state->sound = -1;
        }
    } else {
        handle = state->sound;
        if (handle != -1) {
            char *sound = D_00188660 + handle * 0x70;

            if (*(char **)(sound + 0x88) == m && ((unsigned char *)sound)[0x74]) {
                func_002E59F8(handle);
            }
        }
        state->sound = -1;
    }
}

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

