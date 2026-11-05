/* func_PAL_L00_004169B0, the PAL copy of func_L00_00411B30: the same C, where rename func_L00_00412928 becomes func_PAL_L00_004177A8. */
#include "common.h"

typedef int Vector128 __attribute__((mode(TI)));
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
#ifndef RAC2_T_NODE16_F23626AD8
#define RAC2_T_NODE16_F23626AD8
typedef struct Node16_F23626ad8 Node16_F23626ad8;
#endif

#ifndef RAC2_T_NODE104_F23626AD8
#define RAC2_T_NODE104_F23626AD8
typedef struct Node104_F23626ad8 Node104_F23626ad8;
#endif

#ifndef RAC2_T_NODE24_F23626AD8
#define RAC2_T_NODE24_F23626AD8
typedef struct Node24_F23626ad8 Node24_F23626ad8;
#endif

struct Node16_F23626ad8 {
    short h0;
    unsigned char pad2[2];
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
};

struct Node104_F23626ad8 {
    unsigned char pad0[16];
    Node16_F23626ad8 *p16;
};

struct Node24_F23626ad8 {
    unsigned char pad0[52];
    unsigned short h52;
    unsigned char pad54[50];
    Node104_F23626ad8 *p104;
};

#ifndef RAC2_T_OBJ_F23626AD8
#define RAC2_T_OBJ_F23626AD8
#endif

extern void func_002AFFD8(char *a0, void *a1, int a2);

extern void func_00282D30(void *a0, void *a1, float f12);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
#endif

/* func_L00_004096B0 is defined in L00_004096B0-sweep-g8.c. */

/* func_L00_00409E48 is defined in L00_004096B0-g8.c. */

/* func_L00_0040A4A8 is defined in L00_0040A4A8.c. */

/* func_L00_0040B098 is defined in L00_0040B098.c. */

/* func_L00_0040B2C0 is defined in L00_0040B2C0-flags-g8.c. */

/* func_L00_0040BA30 is defined in L00_0040BA30-g8.c. */

/* func_L00_0040C7D8 is defined in L00_0040C7D8-g8.c. */

/* func_L00_0040CCA0 is in L00_0040CCA0.c. */

/* func_L00_0040D058 is defined in L00_0040D058-ribbon.c. */

/* Tests the handle cached in the object and, while the handle reports itself already released, drops the next sub-handle, otherwise runs the object's destroy helper. */
/* func_L00_0040DF58 is in L00_0040DF58-g8.c. */

/* func_L00_0040F6A0 is in L00_0040F6A0-g16.c. */

/* func_L00_00410178 is defined in L00_00410178.c. */

/* func_L00_00410690 is defined in L00_00410690-g64.c. */

/* Reads the halfword at +0x1A of the object's node and grades it: 4 at or above 90, 3 above 60,
   2 above 30, else 1. The two lower bounds stay in variables, which is what keeps them in
   registers instead of an immediate compare. */
/* func_L00_00410888 is defined in L00_00410888-g8.c. */

/* func_L00_00410D20 is defined in L00_00410D20.c. */

/* Calls a helper with a referenced node and three identity bytes of the object, then copies those bytes into a linked record when a node flag bit is set and the record exists with a non-zero leading short. */
/* func_L00_004115E8 is defined in L00_004096B0-g8.c. */

/* Walks the thirty records of the level's block and, for every record whose +0x18 pointer is
   set, folds it into a sixteen-byte buffer: the record's own transform, then the scale of its
   +0x0C word, then its first three words added to the buffer. The block's address differs
   from level to level, so it takes a per-level name. */
extern char D_L00_00256BC0[];
extern char *func_002AB1F0(char *);
extern float func_00283B98(float, float);
extern float func_00283180(float), func_00283198(float), func_00283CE0(int);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282D08(void *, void *, void *, float);
extern int func_00282948(void *);
extern void func_002A8C00(void *, void *, int, void *, float);
extern void func_002A8E20(void *, void *);
extern void func_PAL_L00_004177A8(void *, int, int);
void func_PAL_L00_004169B0(char *object)
{
    Vector128 temp[1];
    char event[48];
    char *p, *q, *node, *first, *second;
    int i;
    float fraction, amount;
    for (i = 0; i < 48; i++) {
        p = D_L00_00256BC0 + i * 0xd0;
        if (*(char **)(p + 0xac) != object) continue;
        first = p + 0x20;
        qcopy(first, p + 0x40);
        if (*(char **)(p + 0xb4)) {
            node = func_002AB1F0(*(char **)(p + 0xb4));
            if (node) {
                func_002AFFD8(*(char **)(p + 0xb4) + 0x10, temp, 1);
                func_00282D30(temp, temp, *(float *)(node + 0x10));
                func_00282CC0(temp, temp, *(char **)(p + 0xb4) + 0x10);
                func_00282CC0(first, temp, first);
            } else qcopy(first, *(char **)(p + 0xb4) + 0x10);
        }
        if (*(int *)(p + 0xb8)) {
            *(float *)(p + 96) = func_00283B98(*(float *)(p + 96), *(float *)(p + 108) * 100.0f);
            *(float *)(p + 100) = func_00283B98(*(float *)(p + 100), *(float *)(p + 112) * 100.0f);
            *(float *)(p + 104) = func_00283B98(*(float *)(p + 104), *(float *)(p + 116) * 100.0f);
            *(float *)(p + 132) = func_00283B98(*(float *)(p + 132), *(float *)(p + 144) * 100.0f);
            *(float *)(p + 136) = func_00283B98(*(float *)(p + 136), *(float *)(p + 148) * 100.0f);
            *(float *)(p + 140) = func_00283B98(*(float *)(p + 140), *(float *)(p + 152) * 100.0f);
        } else {
            *(float *)(p + 96) = func_00283B98(*(float *)(p + 96), *(float *)(p + 108));
            *(float *)(p + 100) = func_00283B98(*(float *)(p + 100), *(float *)(p + 112));
            *(float *)(p + 104) = func_00283B98(*(float *)(p + 104), *(float *)(p + 116));
            *(float *)(p + 132) = func_00283B98(*(float *)(p + 132), *(float *)(p + 144));
            *(float *)(p + 136) = func_00283B98(*(float *)(p + 136), *(float *)(p + 148));
            *(float *)(p + 140) = func_00283B98(*(float *)(p + 140), *(float *)(p + 152));
        }
        *(float *)(p + 0) = func_00283180(*(float *)(p + 96)) * *(float *)(p + 120);
        *(float *)(p + 4) = func_00283198(*(float *)(p + 100)) * *(float *)(p + 124);
        *(float *)(p + 8) = -func_00283198(*(float *)(p + 104)) * *(float *)(p + 128);
        *(float *)(p + 16) = func_00283180(*(float *)(p + 132)) * *(float *)(p + 156);
        *(float *)(p + 20) = func_00283198(*(float *)(p + 136)) * *(float *)(p + 160);
        *(float *)(p + 24) = -func_00283198(*(float *)(p + 140)) * *(float *)(p + 164);
        qcopy(p + 0x30, p + 0x50);
        if (*(char **)(p + 0xb0)) {
            node = func_002AB1F0(*(char **)(p + 0xb0));
            if (node) {
                func_002AFFD8(*(char **)(p + 0xb0) + 0x10, temp, 1);
                func_00282D30(temp, temp, *(float *)(node + 0x10));
                func_00282CC0(temp, temp, *(char **)(p + 0xb0) + 0x10);
                func_00282CC0(p + 0x30, temp, p + 0x30);
            } else qcopy(p + 0x30, *(char **)(p + 0xb0) + 0x10);
        }
        first = p + 0x30;
        q = p;
        fraction = func_00283CE0(*(int *)(q + 0xb8));
        fraction /= func_00283CE0(*(int *)(q + 0xbc));
        second = q + 0x20;
        func_00282D08(first, second, first, 1.0f - fraction);
        if (!func_00282948(q + 0xb8)) continue;
        if (*(int *)(*(char **)(q + 0xb0) + 0x98) && !*(int *)(q + 0xc0) && *(int *)(q + 0xc4)) {
            func_00282CF0(temp, first, second);
            func_00282F20(temp, temp, 0.040000003f);
            amount = *(float *)(q + 0xc8);
            *(int *)(object + 0xb8) = *(int *)(*(char **)(q + 0xcc) + 0x20);
            func_002A8C00(event, object, 0x210000, temp, amount);
            event[0x18] = 5;
            event[0x19] = 3;
            *(short *)(event + 0x1a) = *(short *)(object + 0xaa);
            func_002A8E20(*(char **)(q + 0xb0), event);
            *(int *)(q + 0xc0) = 1;
            func_PAL_L00_004177A8(first, 10, 1);
        }
        if (func_00282948(q + 0xa8)) *(int *)(q + 0xac) = 0;
    }
}

/* resets a state record: zero words, plus/minus one float flags and two 64-bit fields in the retail store order */
/* func_L00_00412110 is defined in L00_00412110.c. */

/* func_L00_00412230 is defined in x-qcopy.c. */

/* Derives a 16-byte element at index-1 from its index-2 and index-3 neighbours through two bound helpers; original game role is unknown. */
/* func_L00_00412470 is defined in L00_00412470.c. */
