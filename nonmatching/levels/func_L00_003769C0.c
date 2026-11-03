#include "common.h"
/* Claude Opus 5.5 (opus-1), 3 November 2026; USA v1.01, -O2 -G0 -ffunction-sections (default profile).
 * Best check: 1212/1216 bytes, 0 of 27 places match (built as its own unit, src/levels/shared/L00_003769C0.c).
 * Closer than the 1 November draft: D_L00_0022F900 and D_L00_0028E9F0 are plain externs (not sda), so the row
 * address is retail's split lui/addiu + mtlo/madd (no shared idx*28 mult/addu), the row/movz block matches, and
 * the table base is split like retail (lui hoisted, addiu in the goto loop).
 * Left: (1) retail recomputes the colour entry address base-first after the search (addu v0,a1,a0; lw s3,4(v0),
 * the 4 missing bytes); cse1 merges it with the key address (off+base) because the found block shares the
 * loop's extended basic block. A pointer variable set before the loop gives the base-first addu but adds a
 * register (1224 B), so retail's found block was probably a separate EBB (a label with uses) and gcse made the
 * base a1. (2) idx/colour take s2/s3 the other way: global priorities 30/70 (idx) vs 76/173 (colour) are nearly
 * tied. (3) i+1 in fp and y+56 on the stack in retail, swapped here; the increment order makes no difference.
 * (4) The text colours compute b|color before a|color.
 * Declarations: the externs below. L00_00358D98.c already declares D_L00_001B25E0 (scalar float) and
 * D_L00_0022F7C0 (ShopModel) with other types, so a merged version needs _1 aliases or its own unit (same flags).
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
            int off;
        top:
            off=j*8;
            key=((MenuColor *)((char *)D_L00_0028E9F0+off))->key;
            if(key) {
                j++;
                if(*(int *)((char *)D_L00_0022F7C0+0x158+idx*28)!=key) goto top;
                color=((MenuColor *)((char *)D_L00_0028E9F0+off))->color;
            }
            label=func_0028E588(0xea97,1);
            colors[2]=colors[0]=a|color;
            colors[3]=colors[1]=b|color;
            func_0028EA58(label,y+2,MY(332.0f),40,(int)(D_L00_001B25E0[0]*40.0f+0.5f),colors);
            color=0xf0f0b0;
        }
        colors[2]=colors[0]=a|color;
        colors[3]=colors[1]=b|color;
        func_0028EA58(text,y+2,MY(332.0f),40,(int)(D_L00_001B25E0[0]*40.0f+0.5f),colors);
    }
    }
}
#undef MW
#undef MY
