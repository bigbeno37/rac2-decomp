#include "common.h"
typedef unsigned int WeatherQuad __attribute__((mode(TI)));
typedef struct {
    char pad[0x140];
    float position[4];
    float rotation[4];
} WeatherHero;

extern int D_001A79F0[] __attribute__((sda));
extern int D_L01_001C9A40 NOT_SDA;
extern int D_L01_001A94C0 SDATA(D_L01_001A94C0);
extern float D_L01_001AF62C SDATA(D_L01_001AF62C);
extern float D_L01_001AF630 SDATA(D_L01_001AF630);
extern float D_L01_001AF634 SDATA(D_L01_001AF634);
extern float D_L01_001AF640 SDATA(D_L01_001AF640);
extern int D_L01_001AF644 SDATA(D_L01_001AF644);
extern float D_L01_001AF648 SDATA(D_L01_001AF648);
extern float D_L01_001AF64C SDATA(D_L01_001AF64C);
extern float D_L01_001AF650 SDATA(D_L01_001AF650);
extern float D_L01_001AF654 SDATA(D_L01_001AF654);
extern float D_L01_001B27B0 SDATA(D_L01_001B27B0);
extern float D_L01_001B2F00[] __attribute__((sda));
extern int D_L01_001B2F04;
extern float D_L01_001B2EF0[] __attribute__((sda));
extern float D_L01_001B2EF4[] __attribute__((sda));
extern float D_L01_001B208C[] __attribute__((sda));
extern float D_L01_001B2090[] __attribute__((sda));
extern char D_L01_001B94C0[];
typedef struct { WeatherQuad vec; } WeatherWind;
extern WeatherWind D_L01_001B27A0 __attribute__((sda));
extern int func_002ACD38(void *);
extern void func_L01_003993C8(int);
extern int func_002829D8(void *);
extern void func_00282CF0(void *, void *, void *);
extern float func_00282E20(void *);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern float func_002A7878(float, float);
extern float func_00283180(float);
extern float func_00283198(float);
extern void func_L00_00323F28(void *, void *, float, float, float);
extern float func_002B0520(void *);
extern void func_002B0668(void *);
extern void func_L01_00326DD8(void *, int, float, void *);
extern void func_L01_00331CA0(void *, int, void *, float);

#define hero ((WeatherHero *)(D_L01_001B94C0 - 0x140))
