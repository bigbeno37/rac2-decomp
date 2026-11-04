#include "common.h"
/* Claude Opus 5.5 (opus-2, task v2514), 5 November 2026; USA v1.01, -O2 -G0 -ffunction-sections (default profile),
 * built as its own unit (src/levels/shared/L00_003769C0.c; L00_00358D98.c declares D_L00_001B25E0 and
 * D_L00_0022F7C0 with other types).
 * Best check: 1216/1216 bytes, 0 of 27 places; 22 rows differ, all register choices (relocations aside).
 * New since opus-1: natural D_L00_0028E9F0[j].key / [j].color accesses with j++ on the back path
 * ({ j++; goto top; }) give retail's separate base-first colour address (addu v0,a1,a0; lw 4(v0)) and the
 * 1216-byte layout (sched1 interblock moves j++ into the beqz delay slot); the text block writing
 * colors[3]/[1] (b|color) before colors[2]/[0] fixes its or/store order.
 * Left: (1) idx s3 / colour s2, retail s2/s3: global priorities colour 4*19/176 = 0.432 > idx 3*10/71 = 0.423.
 * (2) inner loop j a1 / base a3, retail a3/a1 (j 3*8/24 = 1.0 ties rowkey 1.0, base 2*6/16 = 0.75).
 * (3) y's load temp v1, retail v0: loop.c hoists D_L00_0022F900 HIGH/LO_SUM (later rematerialized) into the
 * outer preheader, and its 2-insn temp takes v0 inside the temp's span.
 * (1) and (2) both follow if the search is a natural loop (flow weights refs by loop depth: idx 11 refs beats
 * colour, base/rowkey 1.5 beat j 1.375); a for loop with a top test gives exactly retail's a0/a1/a2/a3 and
 * colour-address forms but is rotated and its test duplicated (1268 B, loop.c ignores it: multiple entry
 * points). for(;;)/do-while with goto exits are valid loops, so loop.c hoists the rowkey address and walks a
 * pointer (103 rows). Next: a natural loop that loop.c rejects without a peeled test.
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
        top:
            key=D_L00_0028E9F0[j].key;
            if(key) {
                if(D_L00_0022F900[idx].words[6]!=key) { j++; goto top; }
                color=D_L00_0028E9F0[j].color;
            }
            label=func_0028E588(0xea97,1);
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
