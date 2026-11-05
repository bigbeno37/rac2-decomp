/* func_PAL_L09_0040FD68, the PAL copy of func_L09_0040AC48.
   Tier: opus-1 (task v2773). Unit flags: -O2 -G8 -ffunction-sections (a -g8.c name gives -G8).
   Best check.py result: 984/984 bytes, 0/1 places, first difference at +0x1e4; 8 real instructions differ.
   Same as USA except: 0.000277777785f becomes 0.00040000002f (0x39D1B718, which is the USA constant * 1.44f),
   240 becomes 200, and (a+b)*60.0f becomes ((a+b)*300.0f + 2.0f) * 0.16666667f.

   What still differs: all in case 3, two swapped register pairs, instruction order identical.
     retail: ticks in $v0, ticks+1 in $v1, 2.0f in $f4, (float)ticks in $f3.
     this C: ticks in $v1, ticks+1 in $v0, 2.0f in $f3, (float)ticks in $f4.
   local-alloc, from x.i.lreg with this C (sched1 order lhu, 300.0, addiu, 2.0, sh, float, 1/6, ...):
     ticks: 3 refs, 6 insns, priority 1/2; ticks+1: 2 refs, 3 insns, priority 2/3, so ticks+1 picks first.
     (float)ticks and 2.0f tie at 2 refs, 9 insns, and the earlier-born 2.0f picks first.
   Retail needs ticks over ticks+1 AND (float)ticks over 2.0f. No single move in the sched1 order gives both,
   so a source difference outside this statement (one more ref on ticks and on its float, or a different block
   shape) looks likely. That is an inference, not measured.

   Tried, all the same 8 instructions: `X < (float)ticks`, limit or float in named locals, 0.16666667f written
   first, ticks+1 in a pointer local, ticks = X + 1 then ticks - 1, float set before the store,
   -ffast-math with (a+b)*60.0f*5 + 2) / 6 (this folds to the same RTL, so 300.0f and the 1/6 product come from
   either form), -g, -frac2-keep-block-order. Worse: `(*p)++` (988 bytes), a subtraction test (992 bytes),
   ticks+1 in a named local, limit computed before the load, or before the store.
*/
#include "common.h"
extern void func_002A7750(void *, int, int);
extern int func_002B3868(void *);
extern void func_L09_0040B008(char *);
extern void func_L09_0040B0C8(char *);
extern int func_002E5CF8(int, int, void *);
extern void func_002E59F8(int);
extern void func_002AA548(float *, float *, float, float, float, float);
extern float D_L09_001B15A8 __attribute__((sda));
extern float D_L09_001B15AC __attribute__((sda));
extern char D_00188660[];
typedef struct { float value[2]; float rate[2]; } DobboDoorMotion;
void func_PAL_L09_0040FD68(char *obj) {
    char *data = *(char **)(obj+0x68);
    int next, next4, sound, ticks;
    float speed, accel, speed4, accel4;
    char *entry;
    switch (*(unsigned char *)(obj+0x20)) {
    case 0:
        *(int *)(obj+0x98) = 0;
        *(unsigned short *)(obj+0x34) = (*(unsigned short *)(obj+0x34) | 0x2141) & 0xEFFF;
        func_002A7750(obj,1,-1);
        break;
    case 1:
        if (*(unsigned char *)(obj+0xBC)) func_002A7750(obj,2,-1);
        else {
            *(int *)(obj+0x98) = 0;
            *(unsigned short *)(obj+0x34) = (*(unsigned short *)(obj+0x34) | 0x41) & 0xEFFF;
        }
        break;
    case 2:
        if (func_002B3868(obj)) {
            *(unsigned short *)(obj+0x34) = (*(unsigned short *)(obj+0x34) & 0xFFBE) | 0x1000;
            *(int *)(obj+0x98) = *(int *)(*(char **)(obj+0x24)+0x10);
            func_L09_0040B008(obj);
            *(int *)(data+0x1C) = func_002E5CF8(0,4,obj);
        }
        next = (*(int *)(data+0x14)+1)&1;
        speed = D_L09_001B15A8 * 0.00040000002f;
        accel = D_L09_001B15AC * 0.00040000002f;
        if (((DobboDoorMotion *)data)->value[*(int *)(data+0x14)] < 1.0f)
            func_002AA548(&((DobboDoorMotion *)data)->value[*(int *)(data+0x14)],(float *)(data+8)+*(int *)(data+0x14),1.0f,speed,accel,1.0f);
        if (((DobboDoorMotion *)data)->value[*(int *)(data+0x14)] > 0.6f)
            func_002AA548(&((DobboDoorMotion *)data)->value[next],(float *)(data+8)+next,1.0f,speed,accel,1.0f);
        func_L09_0040B0C8(obj);
        if (((DobboDoorMotion *)data)->value[next] >= 1.0f) func_002A7750(obj,3,-1);
        break;
    case 3:
        if (func_002B3868(obj)) *(int *)(data+0x18) = 200;
        ticks = *(unsigned short *)(obj+0x96);
        *(unsigned short *)(obj+0x96) = ticks + 1;
        if ((float)ticks > ((*(float *)(data+0x20)+*(float *)(data+0x24))*300.0f + 2.0f) * 0.16666667f)
            func_002A7750(obj,4,-1);
        func_L09_0040B0C8(obj);
        break;
    case 4:
        next4 = (*(int *)(data+0x14)+1)&1;
        speed4 = D_L09_001B15A8 * 0.00040000002f;
        accel4 = D_L09_001B15AC * 0.00040000002f;
        if (((DobboDoorMotion *)data)->value[next4] > 0.2f)
            func_002AA548(&((DobboDoorMotion *)data)->value[next4],(float *)(data+8)+*(int *)(data+0x14),0.2f,speed4,accel4,1.0f);
        if (((DobboDoorMotion *)data)->value[next4] < 0.6f)
            func_002AA548(&((DobboDoorMotion *)data)->value[*(int *)(data+0x14)],(float *)(data+8)+next4,0.2f,speed4,accel4,1.0f);
        func_L09_0040B0C8(obj);
        if (((DobboDoorMotion *)data)->value[*(int *)(data+0x14)] <= 0.2f) {
            obj[0xBC] = 0;
            *(int *)(obj+0x98) = 0;
            *(unsigned short *)(obj+0x34) = (*(unsigned short *)(obj+0x34) | 0x41) & 0xEFFF;
            func_002A7750(obj,1,-1);
            sound = *(int *)(data+0x1C);
            if (sound != -1) {
                entry = D_00188660 + sound*0x70;
                if (*(char **)(entry+0x88) == obj && *(unsigned char *)(entry+0x74)) func_002E59F8(sound);
            }
            *(int *)(data+0x1C) = -1;
        }
        break;
    }
}
