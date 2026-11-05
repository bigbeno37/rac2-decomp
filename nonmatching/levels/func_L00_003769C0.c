#include "common.h"
/* Claude Opus 5.5 (opus-2, task v2599, third and last Opus pass), 5 November 2026; USA v1.01, -O2 -G0
 * -ffunction-sections (default profile), built as its own unit (src/levels/shared/L00_003769C0.c; L00_00358D98.c
 * declares D_L00_001B25E0 and D_L00_0022F7C0 with other types).
 * Best check: 1216/1216 bytes, 0 of 27 places, 2 rows differ, one register: y's load temp is v1, retail v0
 * (lw $v0,0x6C($s0); addiu $s1,$v0,-0x2F). Every other instruction equals retail, relocations aside.
 *
 * What was solved: retail's search IS a loop with notes, and loop.c skips it as "phony". for(;;j++) with a stable
 * label in the rolled exit code keeps jump.c from duplicating the test (duplicate_loop_exit_test refuses a
 * CODE_LABEL), so the entry `goto start` stays right after NOTE_INSN_LOOP_BEG; gcse PRE then inserts lui %hi(table)
 * in front of that jump, i.e. after the note, the first real insn of the loop is no label and no jump, and
 * scan_loop prints "Loop from .. is phony". Sched1 hoists j++ out of the leftover block, jump2 deletes the entry
 * jump. Flow's loop depth then gives idx s2/colour s3 and j a3/base a1. The label needs real code after it that
 * survives jump1: `n=1` (a local holding the argument, gone at cse1). `if(key==0 || key==row){if(key)color=..;break;}`
 * gives the same loop but `bne key,row` (retail row,key); with row first cse2 loses key!=0 and the test stays.
 *
 * Left, y's temp: loop.c hoists D_L00_0022F900 into the outer preheader; its HIGH temp cannot cross the last call
 * (no calls crossed), so it sits beside y's load and local-alloc gives it v0 first.
 * One-step variant 1: make rows a member, typedef struct { int words[0x50]; MenuRow rows[55]; } Menu705;
 * (rows at +0x140; rows[54].words[6] is MW(0x740)), row accesses D_L00_0022F7C0[0].rows[idx]. Cse2 then derives the
 * hoists from the MW base, they float away and y gets v0, no row pointer needed, but colour's sched1 live length
 * drops 176->171: 4*20/171=4678 > idx 3*11/71=4647, so idx/colour swap back (14 rows, s2/s3 only). It needs colour
 * live >= 173 or idx <= 70; idx looks fixed by retail's bytes. Colour's reset is the likely lever: lui r88 sits at
 * t=11 in sched1, behind $7=0x28 and $9=sp, a tie at priority 2 lost on luid.
 * One-step variant 2: variant 1 with color=0xf0f0b0 before the func_0028EA58 call: idx/colour right, y right, 4 rows
 * differ, only ori $s3 too early (a set before the call gets a call dependency, prio 3 in sched2).
 * Same bytes, tried: label-block arguments in locals, colors statement order, locals for the text-draw arguments.
 * Not tried: why retail's outer hoists differ, a local that survives combine in the text block (+2 colour live).
 */
extern float D_L00_001B25E0[] __attribute__((sda));
extern int D_001A7B90[] __attribute__((sda));
extern int D_001A7340[] __attribute__((sda));
extern int D_001A7344[] __attribute__((sda));
typedef struct { int words[0x744/4]; } Menu705;
extern Menu705 D_L00_0022F7C0[];
typedef struct { int key; unsigned int color; } MenuColor;
extern MenuColor D_L00_0028E9F0[];
typedef struct { int words[7]; } MenuRow;
extern MenuRow D_L00_0022F900[];
extern void func_002FC620(int, int, int, int);
extern int func_00282C30(int);
extern void func_00285548(int,int,int,int,int,int,unsigned int);
extern char *func_0028E588(int,int);
extern int func_0028EA58(char *,int,int,int,int,unsigned int *);
#define MW(o) (D_L00_0022F7C0[0].words[(o)/4])
#define MY(y) ((int)(D_L00_001B25E0[0]*(y)+0.5f)+(D_001A7B90[0]?4:0))
void func_L00_003769C0(void)
{
    int y;
    int bottom, top;
    unsigned int pulse;
    unsigned int color;
    unsigned int colors[4];


    bottom=MY(382.0f);
    top=(int)(D_L00_001B25E0[0]*320.0f+0.5f);
    func_002FC620(58,456,top,bottom);
    if (MW(0x6c)<0) MW(0x6c)+=4;
    else if(MW(0x6c)>0) MW(0x6c)-=4;
    color=0xf0f0b0;
    pulse=(func_00282C30(((MW(4)<<2)&63)-32)+64)*0x10202;
    func_00285548(229,MY(325.0f),285,MY(381.0f),D_001A7340[0],D_001A7344[0],0x80000000|pulse);
    func_00285548(231,MY(327.0f),283,MY(379.0f),D_001A7340[0],D_001A7344[0],0x80442d00);
    y=MW(0x6c)-47;
    { int i;
    for(i=-5;i<7;i++,y+=56) {
        int idx, a,b;
        double d;
        char *text, *row;
        idx=i+MW(0x80);
        if(idx<0 || idx>=MW(0x740)) continue;
        row=(char *)&D_L00_0022F900[idx];
        text=func_0028E588(0xea97,*(int *)row);
        d=256.0f-(float)(y+2);
        if(d<0.0) d=0.0-d;
        a=128-(int)(d*0.75);
        d=256.0f-(float)(y+40);
        if(d<0.0) d=0.0-d;
        b=128-(int)(d*0.75);
        if(a<0) a=0;
        if(b<0) b=0;
        a<<=24; b<<=24;
        if(*(int *)row==0) {
            int j=0;
            char *label;
            int key;
            int n;
            for(;;j++) {
                key=D_L00_0028E9F0[j].key;
                if(key) {
                    if(D_L00_0022F900[idx].words[6]!=key) continue;
                    color=D_L00_0028E9F0[j].color;
                }
                n=1;
                break;
            }
            label=func_0028E588(0xea97,n);
            colors[2]=colors[0]=a|color;
            colors[3]=colors[1]=b|color;
            func_0028EA58(label,y+2,MY(332.0f),40,(int)(D_L00_001B25E0[0]*40.0f+0.5f),colors);
            color=0xf0f0b0;
        }
        colors[3]=colors[1]=b|color;
        colors[2]=colors[0]=a|color;
        func_0028EA58(text,y+2,MY(332.0f),40,(int)(D_L00_001B25E0[0]*40.0f+0.5f),colors);
    }
    }
}
#undef MW
#undef MY
