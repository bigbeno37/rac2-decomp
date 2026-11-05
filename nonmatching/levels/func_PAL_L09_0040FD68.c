/* func_PAL_L09_0040FD68, the PAL copy of func_L09_0040AC48.
   Tier: opus-2 (task v2782; opus-1 v2773 before it). Unit flags: -O2 -G8 -ffunction-sections (a -g8.c name gives -G8).
   Best check.py result: 984/984 bytes, 0/1 places, 7 real instructions differ, all in case 3, same order.
   Same as USA except: 0.000277777785f becomes 0.00040000002f, 240 becomes 200, and (a+b)*60.0f becomes
   ((a+b)*300.0f + 2.0f) * 0.16666667f.

   This body reuses `next` for ticks+1, which makes it a global pseudo so ticks gets $v0; ticks+1 lands in $s2 ($a1 if
   `sound` is reused; retail $v1), and 2.0f and (float)ticks stay swapped in f3/f4. Plain `*p = ticks + 1` is 8 rows.

   Cause, measured (cc1 -fsched-verbose-5 in the scratch dir). `ticks = (*p)++`, with `int ticks`, compiles to every
   register retail has: $v0, $v1, $f2, $f3, $f4, $f5. It is 988 bytes only because it keeps `andi $2,$2,0xffff`, which
   also pushes the mtc1/cvt.s.w after the 1/6 load. That andi is the zero_extend of the HImode temporary:
       r158:HI = lhu; r159 = r158#0 + 1; sh; r89 = zxn(r158); float(r89)
   So retail has one more ALU insn on the ticks path at sched1 (clock 4, beside the sh), tied to ticks, gone by sched2:
   it adds refs to ticks (priority over ticks+1), delays the float to clock 5 behind the 1/6 load, and so ranks
   1/6 (2/8) > float (2/8, later born) > 2.0f (2/10) > 300.0f (2/11), which gives f2, f3, f4, f5. sched2 without it
   then gives retail's order, as sched2's priorities settle every tie here. So the missing insn must be a no-code copy.

   The pass that makes that copy is regmove.c optimize_reg_copy_3: it folds a zero_extend of a dying HI pseudo into its
   load, leaving an SImode copy for local-alloc to tie. Here it backs out, because the increment between load and extend
   is (plus:SI (subreg:SI H 0) 1): replacing H with (subreg:HI H' 0) nests two subregs and recog rejects it. A HI
   add on this target always takes that paradoxical-subreg path (expand_binop, widen_operand), so `(*p)++` cannot pass.
   A use in a HI context (a store of H, a zero_extend) would pass, but retail has neither. What could unblock it: a
   wording where nothing uses H in a SUBREG between load and extend, or a compiler question for the maintainers: does
   retail's optimize_reg_copy_3 succeed here? Inferred from the bytes, not proved. Never tested on a changed compiler.

   Plain copies cannot do it: cse replaces x by T unless T is set again, and combine merges (set x T) into the float.
   Tried, all the 8-row result: (float)(unsigned short)ticks, (float)(ticks & 0xFFFF), unsigned short ticks,
   old = ticks++ on an unsigned short local, unsigned short old = ticks, `(float)count++` after the store; earlier:
   X < (float)ticks, named limit or float locals, 1/6 written first, ticks+1 in a pointer local, ticks = X+1 then
   ticks - 1, float before the store, -ffast-math (same RTL), -g, -frac2-keep-block-order.
   Worse: unsigned int ticks (1024, keeps the sign test), (float)(short)ticks (992), `(*p)++` in any place (988),
   int or unsigned c = x, old = c++ (988, a real move and an in-place addiu), `next` as the only tick local (s2/v0).
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
        next = ticks + 1;
        *(unsigned short *)(obj+0x96) = next;
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
