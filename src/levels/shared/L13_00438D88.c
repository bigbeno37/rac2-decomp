#include "common.h"

/* func_L13_00438D88 (-g: its line labels give the nop pair before both div.s).
   Samples the camera's projected paths for the settings entry's mode, smooths
   the position and builds the camera frame.  The types differ from those of
   L13_0041B670.c, hence a unit of its own. */

typedef int Quad __attribute__((mode(TI)));
typedef union { Quad quad; float f[4]; } Vector;

typedef struct { int count; int reserved[3]; Vector point[0]; } CameraPath;
typedef struct { int index; float distance; } PathCursor;
typedef struct { char pad[0x20]; float rate; char pad24[0x10]; float velocity; } TurnSpring;
typedef struct { Vector saved; char pad[0x3C]; float elevation; } CameraMotion;
typedef struct { PathCursor cursor; int pad8[2]; int marker; } CameraRun;
typedef struct {
    char prefix[0x80];
    Vector position;
    char before_path[0x5A0];
    CameraPath *path;
    PathCursor cursor;
} PathHero;
extern PathHero D_00189E20;
extern int D_001A79F0 __attribute__((sda));

extern float D_L13_001B9940[4];
extern char *D_L13_001B22C4[] __attribute__((sda));
typedef struct {
    char unknown00[0x10];
    CameraPath *path;
    char unknown14[0xC];
} CameraRouteRecord;
extern CameraRouteRecord *D_L13_001B2140 __attribute__((sda));
extern CameraPath *D_L13_001FE580[];
extern int func_L00_00341820(void *, void *, int *, float *, int, float);
extern int func_L00_00341FA8(void *, void *, void *, int *, float *, int, float, float, float);
extern void func_00282CF0(void *, void *, void *);
extern void func_00282CC0(void *, void *, void *);
extern void func_00282F20(void *, void *, float);
extern float func_002702C8(float *, float, float, float, float, float);
extern int func_00283CF0(float);
extern float func_00282C48(float);
extern float func_00282DF0(void *);
extern void func_00282D30(void *, void *, float);
extern float func_00282DB0(void *, void *);
extern float func_002831B0(float);

extern float func_002703B0(char *, float, float, float, float, float);
extern void func_00282DD8(void *, void *, void *);
extern void func_002ACEC0(void *, void *, void *, float);

typedef struct { char pad0[0x38]; float sample_distance; } CameraSettings;
