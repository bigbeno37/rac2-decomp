/* opus-2 (v2563), USA v1.01, -O2 -G8 -ffunction-sections (integrate in a -g8.c unit;
 * tested as src/levels/shared/L04_00402D90-hud-g8.c).
 * Best check.py: 1408/1360 bytes, 0/2 places, 266 of 352 rows differ; an LCS alignment shows
 * about 40 differing hunks, nearly all of them register names or the dead mode test below.
 * The opus-1 draft was 1340/1360 with 302 of 340 rows differing.
 *
 * What this draft reproduces, all from retail evidence:
 * - Saved-but-unused $s5 is the hoisted constant 1 of a per-draw `mode == 1` test
 *   (D_L04_001B2D40, boot D_001B23A0; D_L04_001B2D44 = boot D_001B23A4). The helper is the
 *   game's menu_screen_y (src/game/002CEBD0-g8.c). With its if/return arms cse1 stops at the
 *   join, so 0x8710, 120.0 and 0.5 are per-block single-use constants: loop.c hoists and
 *   merges them into $s6/$f24/$f23 and the constant 4 stops being hoisted. Frame 0x130,
 *   state pointer spilled to sp+0xa0, object in $fp, preheader after draw 3 and
 *   (float)right before draw 3's jal all follow from that.
 * - Retail's x pair: tx=(float)left after lookup 58 (it crosses calls, so sched1 lifts it
 *   above the jal; left has a stale call crossing, hence $s0), draws 1-2 evaluate (float)left
 *   after the test (PRE reaching copy, `mov.s $f20,$f22`), draw 3 reads tx.
 * - left doubles as the i*30 temporary, giving `mult $s0,...; daddu $v1,$s0,$zero`.
 * - view pointer set after the division gives `lui; addiu; swc1 0xb0()`; the two angle/sine
 *   pairs must be separate variables (retail swaps $f20/$f21 between them).
 * - `else if (frame>=b && frame<c && b!=c)`: retail's b==c path falls to the next test.
 *
 * Still differs:
 * 1. The mode test must vanish by flow2 (retail keeps no load and no constant 1). Here the
 *    identical arms merge only at jump2, leaving `lui; lw mode` and `addiu $s5,$zero,1`.
 *    flow2 removes the branch only if the join has a single predecessor (arm emptied by
 *    flow1's dead-store pass). But an if without else is AROUND-transparent to cse, so the
 *    constants are shared across blocks, are not movable, and nothing is hoisted.
 *    If/else or if/return keeps cse opaque but leaves two predecessors. Measured:
 *    `if (mode==1) y=scaled+offset; y=scaled;` inside a whole-draw do{}while(0) removes the
 *    test with no leftovers, keeps frame 0x130 and hoists 120/0.5 (LOOP_END stops cse1), but
 *    the x pair collapses to one pseudo, also with the do/while ending after the test (there
 *    the frame fell to 0x120). Inferred cause, not traced: cse2 ignores loop notes. Both the
 *    do/while and the dead store are forbidden forms, and neither reached exact bytes.
 *    Wanted: a real construct that is a cse boundary for cse1 and cse2 yet has one
 *    predecessor at flow2.
 * 2. y in $v1 (retail) vs $v0: in retail the sprite pseudo stays in $v0 past y's mfc1.
 * 3. i/colour pointer are $s3/$s4 here, $s4/$s3 in retail; retail has `lw $a0,0x68($fp);
 *    lbu $a2,0xbc($fp); sw $a0,0xa0($sp)` and reloads through $t1. Both may follow from 1.
 * All declarations are below.
 */
#include "common.h"
typedef struct { int v[5]; } B1202Five;
extern B1202Five D_L04_001B09B0, D_L04_001B09C8, D_L04_001B09E0, D_L04_001B09F8;
extern int D_L04_001A8FE8 __attribute__((sda));
extern unsigned char *D_L04_001A904C;
extern unsigned char D_L04_001BDB80[];
extern float D_L04_001B2D44;
extern int D_001A7B90;
extern int func_00283D38(int,int,float);
extern float func_00283198(float),func_00283180(float);
extern void func_0027AB40(void);
extern void *func_00336D50(void *,int);
extern void func_00300A00(void *,unsigned int,float,float,float,float,float);
extern void func_002FC560(int,unsigned long long);
extern int D_L04_001B2D40;
static inline int ScreenY(int mode, float value, int adjust)
{
    float scaled = value * D_L04_001B2D44 + 0.5f;
    if (mode == 1)
        return (int)scaled + adjust;
    return (int)scaled;
}
void func_L04_0040C880(unsigned char *m)
{
    unsigned char *p = *(unsigned char **)(m+0x68);
    int state = m[0xbc];
    B1202Five x = D_L04_001B09B0;
    B1202Five y = D_L04_001B09C8;
    B1202Five start = D_L04_001B09E0;
    B1202Five end = D_L04_001B09F8;
    int colors[5];
    int frame,i,side,left,right,a,b,c,d;
    void *sprite;
    float sign, fy, tx, ty;
    unsigned char low;
    low=state;
    if ((unsigned)(state-3)<2 || low==1 || low==2) frame=150-*(int *)(p+0x394);
    else frame=D_L04_001A8FE8+150;
    if (!D_L04_001A904C) return;
    for(i=0;i<5;i++) {
        if(i==4) { a=120;b=149;c=156;d=171; }
        else { left=i*30;a=left;b=a+30;c=b;d=a+45; }
        colors[i]=0;
        if(frame>=a && frame<b) colors[i]=func_00283D38(0,start.v[i],(float)(frame-a)*0.033333335f);
        else if(frame>=b && frame<c && b!=c) colors[i]=start.v[i];
        else if(frame>=c && frame<d) colors[i]=func_00283D38(start.v[i],end.v[i],(float)(frame-c)*0.06666667f);
        else if(frame>=d) colors[i]=end.v[i];
    }
    {
        float angle = 0.56217057f;
        float sine = func_00283198(angle);
        unsigned char *view;
        sine = sine / func_00283180(angle);
        view = D_L04_001BDB80;
        *(float *)(view + 0xb0) = sine;
        func_0027AB40();
    }
    for(side=0;side<2;side++) {
        if(side==0) {left=10;right=31;sign=1.0f;} else {left=498;right=478;sign=-1.0f;}
        sprite=func_00336D50(D_L04_001A904C+0x8710,58); tx=(float)left; ty=(float)ScreenY(D_L04_001B2D40,120.0f,0); func_00300A00(sprite,0x60442d00,(float)left,ty,sign,1.0f,0.0f);

        sprite=func_00336D50(D_L04_001A904C+0x8710,59); ty=(float)ScreenY(D_L04_001B2D40,120.0f,0); func_00300A00(sprite,0x55f0c070,(float)left,ty,sign,1.0f,0.0f);

        sprite=func_00336D50(D_L04_001A904C+0x8710,60); ty=(float)ScreenY(D_L04_001B2D40,120.0f,0); func_00300A00(sprite,0x55f0c070,tx,ty,sign,1.0f,0.0f);

        fy=(float)right;
        for(i=0;i<5;i++) {
            sprite=func_00336D50(D_L04_001A904C+0x8710,61);
            func_00300A00(sprite,colors[i],fy,(float)(D_001A7B90?y.v[i]:x.v[i]),0.8f,0.8f,0.0f);
        }
        func_002FC560(71,0x5360b);
    }
    if(m[0x20]!=3) {
        float angle = *(float *)(p+0x28c)*0.008726647f;
        float sine = func_00283198(angle);
        unsigned char *view;
        sine = sine / func_00283180(angle);
        view = D_L04_001BDB80;
        *(float *)(view + 0xb0) = sine;
        func_0027AB40();
    }
}
