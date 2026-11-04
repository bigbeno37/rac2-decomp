#include "include_asm.h"
#include "common.h"

/* The steering input reader. The controller block's analog stick and button words are read
   through the plain symbol (lui/addiu), its small tunables through $gp, so the unit is -G8.
   -frac2-zero-ti-por gives the quadword zero of the axis vector (por + sq) and
   -frac2-hazard-labels keeps the deadzone division's join label on its mtc1 hazard nop. */

typedef unsigned int Quad __attribute__((mode(TI)));

typedef struct {
    char pad0[0x108];
    float stickX, stickY;
    char pad110[0x90];
    union {
        long buttons;
        struct { int held, pressed; } words;
    } input;
} ShipPad;

/* At the camera state's +0x50. */
typedef struct {
    float yawParams[3], pitchParams[3];
    char pad18[6];
    unsigned char mode, changed;
    char pad20[0x34];
    float pitchVelocity, yawVelocity;
    char pad5c[8];
    float pitch, yaw;
    char pad6c[8];
    float pitchLimit, yawLimit;
} InputMode;

typedef struct {
    char pad0[0x40];
    float value, velocity;
} SteeringRecord;

/* At the camera state's +0xD4. */
typedef struct {
    char pad0[4];
    s16 rollDirection, rollTimer;
    float rollInverse;
    s16 rollDelay, comboTimer, comboDelay;
    char pad12[2];
    float comboInverse;
} RollState;

extern ShipPad D_00138180;
extern int D_001A79F0;
extern unsigned char D_001A7A44;
extern float D_L05_001B1A68 __attribute__((sda));
extern float D_L05_001B1A40 __attribute__((sda));
extern float D_L05_001B1A44 __attribute__((sda));
extern float D_L05_001B1A48 __attribute__((sda));
extern float D_L05_001B1B74 __attribute__((sda));
extern float D_L05_001B1B78 __attribute__((sda));
extern float D_L05_001B1B7C __attribute__((sda));
extern short D_L05_001B1AE8 __attribute__((sda));
extern short D_L05_001B1AEC __attribute__((sda));
extern short D_L05_001B1AF0 __attribute__((sda));
extern short D_L05_001B1AF4 __attribute__((sda));
extern int D_L05_001B1B20 __attribute__((sda));
extern int D_L05_002A18E0 __attribute__((nosda));
extern void func_L05_00442880(char *);
extern float func_00282C48(float);
extern int func_L05_00379F38(void);
extern int func_L05_004459B0(char *);
extern float func_00282DB0(void *, char *);
extern int func_L05_00442F90(char *);
extern float func_002702C8(float *, float, float, float, float, float);
extern int func_00282978(short *);
extern float func_00283CE0(int);
