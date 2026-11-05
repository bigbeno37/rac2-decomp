/* opus xhigh, task v2595: retained C attempt, USA v1.01; not linked.
 * Profile: -O2 -G8 -ffunction-sections (put it in its own unit, e.g. src/game/002F9BC0-ammo-g8.c,
 * and take the INCLUDE_ASM out of 002F9BC0.c).
 * Best: 1552/1552 bytes, 0 of 28 places; 16 real (non-relocation) rows differ, all in the block
 * before the first func_0028EA58 call (same count as v2499; many wordings tie here).
 * Fixed in v2595:
 *   - Data names: _gp is 0x1AEFF0, so the older drafts' gp names were 0x10 too high (their
 *     C0/C4/C8/D0/D8 are really 3B0/3B4/3B8/3C0/3C8; the old D_001AD3D0 is a format string).
 *     Every address here was checked against retail. Values: 3B0 = {345, 8}, 3B8 = {30, 30},
 *     3C0 = {24, 0}, 3C8 = {3, 1}, read as y base, x base, sizes, and the icon's x/y offset.
 *   - Wording now follows the matched sibling func_002F9C80: screen_y macro, ?: clamps, its
 *     colour statements; a local x replaces the false `unsigned` on D_001AD3C8. Same bytes.
 * Root cause of the remaining rows (measured with cc1 -dS -dR -fsched-verbose-5 -dg):
 *   fix_truncsfsi2 has a GPR scratch that prints no code, but reload still assigns it, and
 *   sched2 sees it as a clobber. Reload takes the lowest free register (find_reg). Our second
 *   fix (h, into t0) gets a0 because sched1 issues `a1 = x + D_001AD3C8`, the death of the a0
 *   pseudo, one cycle before it. Clobbering a0 forces that addu ahead of the fix, adds 2 to the
 *   x chain's priority (C4 load 50 > movz 49) and lowers the h chain's (scratch a2 would add an
 *   output dep on `a2 = v1+4`: D0 load 57 > scale 56). A0 not free at that fix gives retail's
 *   order. Two throwaway asm probes (one extra no-code insn, not landable) moved the scratch to
 *   a2; the second also put the D0 load back ahead of the scale load, as retail has it.
 * What would fix it: exactly one more sched1 insn with priority >= 43 in the window after dptoli
 *   that leaves no code (a no-op copy reload deletes, a constant reload_cse deletes), with
 *   the x chain's own priorities unchanged or the extra insn inside that chain. sched1 fills every slot
 *   t=5..18, so one extra insn pushes the addu past the fix. The FP constant ($f20) would do it
 *   if anchored after dptoli (sched2 hoists it back), but its pseudo crosses calls.
 * Tried, all tied at 16 rows with scratch a0: x/h/w/colour locals, scale local, pointer colour
 *   stores, args evaluated before the colours, swapped shifts.
 *   Worse: inline clamp (119 rows), split HIGH/LO_SUM D_001AD3CC (40, wrong temp), pair arrays or
 *   structs for +0/+4 (+4 turns into a gp access; only the separate D_001AD3CC keeps its lui).
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
#define screen_y(y) ((int)((float)(y) * D_001B2280[0] + 0.5f) + (D_001A7B90 ? 4 : 0))
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
extern int D_001AD3B4 __attribute__((sda));
extern int D_001AD3B8 __attribute__((sda));
extern int D_001AD3C0 __attribute__((sda));
extern int D_001AD3C8 __attribute__((sda));
extern int D_001AD3BC __attribute__((sda));
void func_002FA270(void)
{
    int pulse;
    int left, b;
    int i, index, x, y, icon, overlay, kind, special, alpha0, alpha1;
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
            alpha0 = alpha0 < 0 ? 0 : alpha0;
            alpha1 = alpha1 < 0 ? 0 : alpha1;
            alpha0 <<= 24;
            alpha1 <<= 24;
            colors[2] = colors[0] = alpha0 | color;
            colors[3] = colors[1] = alpha1 | color;
            x = y+(D_001AD3B4+b);
            func_0028EA58(icon,x+D_001AD3C8,screen_y(D_001AD3B0+D_001AD3CC),D_001AD3C0,(int)((float)D_001AD3C0 * D_001B2280[0]+0.5f),colors);
            func_0028EA58(overlay,y+(D_001AD3B4+b),screen_y(D_001AD3B0-D_001AD3B8),D_001AD3B8,(int)((float)D_001AD3B8 * D_001B2280[0]+0.5f),colors);
            icon = func_0028E588(0xeaa3,3);
            func_0028EA58(icon,y+(D_001AD3B4+b),screen_y((D_001AD3B0-D_001AD3B8)+D_001AD3BC),D_001AD3B8,(int)((float)D_001AD3B8 * D_001B2280[0]+0.5f),colors);
        }
        i++;
        y += 56;
    } while (i < 7);
}
