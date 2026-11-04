/* opus xhigh, task v2499: retained C attempt, USA v1.01; not linked.
 * Profile: -O2 -G8 -ffunction-sections (put it in its own unit, e.g. src/game/002F9BC0-ammo-g8.c,
 * and take the INCLUDE_ASM out of 002F9BC0.c).
 * Best: 1552/1552 bytes, 0 of 28 places; 16 real (non-relocation) rows differ, all in the block
 * before the first func_0028EA58 call (was 21 in v2072's draft).
 * Fixed in v2499:
 *   - First call's x: Cygnus reduce_expression_tree_depth (fold-const.c) flattens
 *     (y+(C4+b))+D8 into (y+D8)+(C4+b) when all four operands share one type. Declaring
 *     D_001AD3D8 unsigned stops the flattening (n_ops 3) and gives retail's grouping. An inline
 *     row_x(y,b) helper with D8 signed gives the same 16 rows.
 *   - 0x2FA524 addu operand order: read the icon through IconInfo *info = &D_00239BA0[...];
 *     the direct form splits base+0x3C off before combine.
 *   - Retail shifts alpha0 in place (sll s1,s1,24; or t2,s1,s5): "alpha1 <<= 24; alpha0 <<= 24;"
 *     before the colors gives retail's registers (t1 = D_001A7B90, t2 = alpha0|color, v1 = y int).
 * Still differs (sched2 order only): retail loads D_001AD3D0 before D_001B2280 (we pick the scale
 * first: equal priority 56, more dependents), and retail issues C4, D8, D_001A7B90 and the x chain
 * later (after both movz). Not tried: other argument shapes for the first call's h/w.
 * Earlier: clamp order swapped, color store orders (4 forms), inline scale_h(): no gain.
 */
#include "common.h"
typedef struct { int id, kind, spare[3], locked; } ItemRow;
typedef struct { int frame, id, spare[2]; } AmmoRow;
typedef struct {
    int state, tick;
    char pad08[0x64];
    int scroll;
    char pad70[0x10];
    int selected;
    char pad84[0xbc];
    union { ItemRow item[64]; AmmoRow ammo[96]; } rows;
} ListState;
typedef struct { char pad00[4]; unsigned char color; char pad05[0x37]; unsigned short icon; char pad3e[0xa2]; } IconInfo;
extern ListState D_0021F240;
extern unsigned char D_00139568[];
extern IconInfo D_00239BA0[];
extern float D_001B2280[] MACRO_ADDR;
extern int D_001A7B90 MACRO_ADDR;
extern int D_001A7340 MACRO_ADDR, D_001A7344 MACRO_ADDR;
extern void func_002FC620(int,int,int,int);
extern int func_00282C30(int);
extern void func_00285548(int,int,int,int,int,int,unsigned int);
extern int func_0028E588(int,int);
extern unsigned int func_00336EF8(int);
extern void func_0028EA58(int,int,int,int,int,unsigned int *);
extern int func_00288FB8(int);
extern int func_0027F288(char *,int,float);
extern void func_002F97A0(float,float,unsigned long,float,char *,int);
static __inline__ int screen_y(int y) { int result = (int)((float)y * D_001B2280[0] + 0.5f); if (D_001A7B90) result += 4; return result; }
extern int D_001AD39C MACRO_ADDR;
extern int D_001AD394 MACRO_ADDR;
extern int D_001AD3A4 MACRO_ADDR;
extern int D_001AD3AC MACRO_ADDR;
extern int D_001AD3A8 MACRO_ADDR;
extern int D_001AD3CC MACRO_ADDR;
extern int D_001AD398 __attribute__((sda));
extern int D_001AD390 __attribute__((sda));
extern int D_001AD3A0 __attribute__((sda));
extern int D_001AD3B0 __attribute__((sda));
extern int D_001AD3C0 __attribute__((sda));
extern int D_001AD3C4 __attribute__((sda));
extern int D_001AD3C8 __attribute__((sda));
extern int D_001AD3D0 __attribute__((sda));
extern unsigned int D_001AD3D8 __attribute__((sda));
extern int D_001AD3BC __attribute__((sda));
void func_002FA270(void)
{
    int pulse;
    int left, b;
    int i, index, y, icon, overlay, kind, special, alpha0, alpha1;
    unsigned int color, colors[4];
    char *text;
    double distance;
    IconInfo *info;
    func_002FC620(D_001AD390,D_001AD394,(int)((float)D_001AD398 * D_001B2280[0]+0.5f),screen_y(D_001AD39C));
    if (D_0021F240.scroll < 0) D_0021F240.scroll += 4;
    else if (D_0021F240.scroll > 0) D_0021F240.scroll -= 4;
    i = -5;
    left = 233;
    b = 4;
    pulse = ((func_00282C30(((D_0021F240.tick*4)&63)-32)+64)*0x10202)|0x80000000;
    func_00285548(left-(D_001AD3A0+b),screen_y(325-D_001AD3A4),D_001AD3A8+285,screen_y(D_001AD3AC+381),D_001A7340,D_001A7344,
        pulse);
    b = 2;
    func_00285548(left-(D_001AD3A0+b),screen_y(327-D_001AD3A4),D_001AD3A8+283,screen_y(D_001AD3AC+379),D_001A7340,D_001A7344,0x80442d00);
    y = D_0021F240.scroll - 47;
    do {
        index = i + D_0021F240.selected;
        if (index >= 0 && index < *(int *)((char *)&D_0021F240+0x740)) {
            info = &D_00239BA0[D_00139568[D_0021F240.rows.ammo[index].id]];
            icon = func_0028E588(info->icon,0);
            overlay = func_0028E588(0xeaa3,D_0021F240.rows.ammo[index].frame);
            special = 0;
            kind = D_00239BA0[D_00139568[D_0021F240.rows.ammo[index].id]].color;
            switch (D_0021F240.rows.ammo[index].id) {
            case 12: case 14: case 17: case 18: case 53: special = 1; break;
            }
            color = func_00336EF8(special && kind == 1 ? 2 : kind) & 0xffffff;
            distance = (double)(256.0f - (float)(y+2));
            if (distance < 0.0) distance = -distance;
            alpha0 = 128 - (int)(distance * 0.75);
            distance = (double)(256.0f - (float)(y+40));
            if (distance < 0.0) distance = -distance;
            alpha1 = 128 - (int)(distance * 0.75);
            if (alpha0 < 0) alpha0 = 0;
            if (alpha1 < 0) alpha1 = 0;
            alpha0 <<= 24;
            alpha1 <<= 24;
            colors[3] = colors[1] = alpha1|color;
            colors[2] = colors[0] = alpha0|color;
            func_0028EA58(icon,y+(D_001AD3C4+b)+D_001AD3D8,screen_y(D_001AD3C0+D_001AD3CC),D_001AD3D0,(int)((float)D_001AD3D0 * D_001B2280[0]+0.5f),colors);
            func_0028EA58(overlay,y+(D_001AD3C4+b),screen_y(D_001AD3C0-D_001AD3C8),D_001AD3C8,(int)((float)D_001AD3C8 * D_001B2280[0]+0.5f),colors);
            icon = func_0028E588(0xeaa3,3);
            func_0028EA58(icon,y+(D_001AD3C4+b),screen_y((D_001AD3C0-D_001AD3C8)+D_001AD3BC),D_001AD3C8,(int)((float)D_001AD3C8 * D_001B2280[0]+0.5f),colors);
        }
        i++;
        y += 56;
    } while (i < 7);
}
