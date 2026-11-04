#include "common.h"

extern void *func_003100D8(int id);

extern int D_L00_001B1340 __attribute__((sda));

/* Allocates a record and tags it released-empty; returns it, or zero when the allocation failed. */
void *func_L00_004115A8(int unused)
{
    char *p = (char *)func_003100D8(0x1136);

    if (p != 0) {
        int v = 0xFF;

        D_L00_001B1340 = (int)p;
        *(unsigned char *)(p + 0x20) = 1;
        *(unsigned short *)(p + 0x32) = v;
        *(unsigned char *)(p + 0x30) = v;
    }
    return p;
}


typedef struct {
    char reserved00[0x18];
    int active;
    char reserved1C[0x14];
} SettingSlot;

typedef struct {
    float amount;
    float scale;
    int kind;
    float multiplier;
} SettingRow;

typedef struct { SettingRow rows[238]; } SettingsBank;

extern SettingSlot D_L00_00256620[30];
extern SettingsBank D_L00_002592C0;

void func_L00_004115E8(void) {
    int i;

    for (i = 29; i >= 0; i--)
        D_L00_00256620[i].active = 0;
    D_L00_001B1340 = 0;
    func_L00_004115A8(0);
    for (i = 119; i < 238; i++) {
        D_L00_002592C0.rows[i - 119].multiplier = 1.0f;
        D_L00_002592C0.rows[i].multiplier = 1.0f;
    }
    D_L00_002592C0.rows[30].amount = 2.0f;
    D_L00_002592C0.rows[30].scale = 0.67f;
    D_L00_002592C0.rows[30].kind = 4;
    D_L00_002592C0.rows[60].amount = 2.0f;
    D_L00_002592C0.rows[60].scale = 0.67f;
    D_L00_002592C0.rows[60].kind = 4;
    D_L00_002592C0.rows[23].amount = 10.0f;
    D_L00_002592C0.rows[23].scale = 5.00f;
    D_L00_002592C0.rows[23].kind = 2;
    D_L00_002592C0.rows[66].amount = 10.0f;
    D_L00_002592C0.rows[66].scale = 5.00f;
    D_L00_002592C0.rows[66].kind = 2;
    D_L00_002592C0.rows[41].amount = 1.0f;
    D_L00_002592C0.rows[41].scale = 2.00f;
    D_L00_002592C0.rows[41].kind = 3;
    D_L00_002592C0.rows[64].amount = 1.0f;
    D_L00_002592C0.rows[64].scale = 2.00f;
    D_L00_002592C0.rows[64].kind = 3;
    D_L00_002592C0.rows[24].amount = 2.0f;
    D_L00_002592C0.rows[24].scale = 2.00f;
    D_L00_002592C0.rows[24].kind = 2;
    D_L00_002592C0.rows[67].amount = 2.0f;
    D_L00_002592C0.rows[67].scale = 2.00f;
    D_L00_002592C0.rows[67].kind = 2;
    D_L00_002592C0.rows[31].amount = 1.0f;
    D_L00_002592C0.rows[31].scale = 1.00f;
    D_L00_002592C0.rows[31].kind = 1;
    D_L00_002592C0.rows[61].amount = 1.0f;
    D_L00_002592C0.rows[61].scale = 1.00f;
    D_L00_002592C0.rows[61].kind = 1;
    D_L00_002592C0.rows[149].amount = 10.0f;
    D_L00_002592C0.rows[149].scale = 4.00f;
    D_L00_002592C0.rows[149].kind = 6;
    D_L00_002592C0.rows[179].amount = 10.0f;
    D_L00_002592C0.rows[179].scale = 4.00f;
    D_L00_002592C0.rows[179].kind = 6;
    D_L00_002592C0.rows[142].amount = 10.0f;
    D_L00_002592C0.rows[142].scale = 8.00f;
    D_L00_002592C0.rows[142].kind = 3;
    D_L00_002592C0.rows[185].amount = 10.0f;
    D_L00_002592C0.rows[185].scale = 8.00f;
    D_L00_002592C0.rows[185].kind = 3;
    D_L00_002592C0.rows[160].amount = 2.0f;
    D_L00_002592C0.rows[160].scale = 4.00f;
    D_L00_002592C0.rows[160].kind = 3;
    D_L00_002592C0.rows[183].amount = 2.0f;
    D_L00_002592C0.rows[183].scale = 4.00f;
    D_L00_002592C0.rows[183].kind = 3;
    D_L00_002592C0.rows[143].amount = 6.0f;
    D_L00_002592C0.rows[143].scale = 2.00f;
    D_L00_002592C0.rows[143].kind = 3;
    D_L00_002592C0.rows[186].amount = 6.0f;
    D_L00_002592C0.rows[186].scale = 2.00f;
    D_L00_002592C0.rows[186].kind = 3;
    D_L00_002592C0.rows[150].amount = 1.0f;
    D_L00_002592C0.rows[150].scale = 1.00f;
    D_L00_002592C0.rows[150].kind = 1;
    D_L00_002592C0.rows[180].amount = 1.0f;
    D_L00_002592C0.rows[180].scale = 1.00f;
    D_L00_002592C0.rows[180].kind = 1;
}

typedef int Quadword __attribute__((mode(TI)));
typedef float Vector[4] __attribute__((aligned(16)));

extern char *func_0029EFD0(int);
extern void func_00282D30(void *, void *, float);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
extern void func_00282DD8(void *, void *, void *);
extern int func_002761F8(void *, void *, int, void *, int);
extern float func_002A7878(float, float);
extern float func_002A78D8(float, float);
extern float func_002A7940(void);
extern char *func_002AB1F0(void *);
extern void func_L00_0040B2C0(void *, void *);
extern void func_002A0F48(void *);
extern void func_002ACA50(void *, void *, int);
extern int func_002FED90(void *, int, float, float, float);

extern int D_L00_001B0FD8 __attribute__((sda));
extern unsigned char D_00139583 __attribute__((nosda));

typedef struct ScaleRecord {
    char pad00[0x50];
    float value;
    char pad54[0x8c];
} ScaleRecord;
extern ScaleRecord D_L00_00262570[];

typedef struct ProjectileState {
    /* 0x00 */ float last_target[3];
    /* 0x0C */ void *owner;
    /* 0x10 */ void *target;
    /* 0x14 */ float height;
    /* 0x18 */ float speed;
    /* 0x1C */ float yaw;
    /* 0x20 */ float spin_speed;
    /* 0x24 */ short life;
    /* 0x26 */ unsigned char homing;
    /* 0x27 */ unsigned char age;
    /* 0x28 */ float spin;
    /* 0x2C */ int index;
    /* 0x30 */ Vector forward;
    /* 0x40 */ Vector color;
    /* 0x50 */ int sound;
    /* 0x54 */ int handle;
} ProjectileState;

typedef struct ProjectileObject {
    /* 0x00 */ char pad00[0x10];
    /* 0x10 */ Vector position;
    /* 0x20 */ signed char status;
    /* 0x21 */ char pad21[0x0b];
    /* 0x2C */ float radius;
    /* 0x30 */ unsigned char alpha;
    /* 0x31 */ signed char enabled;
    /* 0x32 */ short color;
    /* 0x34 */ unsigned short flags;
    /* 0x36 */ char pad36[0x32];
    /* 0x68 */ ProjectileState *state;
    /* 0x6C */ char pad6c[0x50];
    /* 0xBC */ unsigned char hit;
    /* 0xBD */ char padbd[3];
    /* 0xC0 */ Vector forward;
    /* 0xD0 */ Vector right;
    /* 0xE0 */ Vector up;
} ProjectileObject;

typedef struct PlayerState {
    /* 0x0000 */ char pad0000[0x750];
    /* 0x0750 */ Vector up;
    /* 0x0760 */ Vector forward;
    /* 0x0770 */ char pad0770[0x1b20];
    /* 0x2290 */ char *object;
    /* 0x2294 */ int mode;
    /* 0x2298 */ char pad2298[0x254];
    /* 0x24EC */ short orientation;
} PlayerState;
extern PlayerState D_00189E20;

extern Vector D_L00_001B8FF0[3];   /* camera basis rows */
extern float D_L00_001B8DC0[4];    /* camera position */
extern float D_L00_001BF220[4];    /* collision hit point */
extern char D_L00_00255BA0[];
extern char D_L00_001B1FB0[] MACRO_ADDR;

/* Spawns a projectile (moby 0xED7) from owner at position, aimed at target or along the view. */
