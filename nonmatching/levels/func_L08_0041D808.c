/* Claude Opus 5.5 (xhigh effort), task v2570, USA v1.01, 5 November 2026: third and last Opus attempt.
 * Best check: EXACT BYTES. check.py prints `08_tabora: OK` and `func_L08_0041D808: 1 of 1 places
 * match` for this file as a dedicated unit, but it is NOT landable as written, so the result is
 * FAILED and the marker stays: the tests below are vanishing tests on placeholder conditions and
 * their only effect is on scheduling and allocation. A maintainer has to rule on them.
 * Not in the PAL or v2.00 catalogue. No linked build was run.
 *
 * To reproduce: copy everything from the #include down to src/levels/08_tabora/L08_0041D808-g8.c,
 * add `src/levels/08_tabora/L08_0041D808-g8.c<TAB>-O2 -G8 -ffunction-sections -g` to
 * config/cflags.txt and take the marker out of L08_003D8348.c. -g is required: without it the same
 * C gives 3300 bytes.
 *
 * Not landable, all kept here because they are what makes the bytes exact:
 * - HudCoord (sprites A and B) and HudCoord2 (the size conversion, and the last sprite written
 *   out): a real `mode == 1` test with a dead float arm, then a second test on a live register
 *   whose arm only feeds a third test. The game-side meaning of `tex == 1`, `live != 0`,
 *   `x != 0` and of the `+ 4.0f` arms is invented. None of it emits a word.
 * - `player` as an int with a second set, in the last sprite's dead test: see 2 below.
 *
 * What the compiler needs, measured here against gcc/flow.c, jump.c, haifa-sched.c and alias.c:
 * 1. The vanished boundary has three parts. At cse the test must be one-armed with a one-use label
 *    and no label in the arm, or s1=1, f22=0.5f, f21=1.0f, f20=0.0f and s2=0x8710 stop being shared
 *    (`&&` gives the label two uses). At allocation a block-local load of mode must sit at the end
 *    of the first block: that is the pseudo in a0 that pushes tex to a2 and y to a3. At sched2 a
 *    block boundary must still exist. A test whose arm flow1 emptied is deleted at flow2 and its
 *    load dies with it, so it cannot supply that boundary; if it stayed, its load would stay too.
 *    Hence a second test on a live register, whose arm is non-empty when flow2 builds the CFG and
 *    empties during flow2: that jump survives sched2 and jump2 removes it with nothing left behind.
 *    The arm only empties if its sole consumer is a third test that flow2 deletes. A single arm
 *    insn is turned into a conditional move at jump1, so it needs two insns or a float.
 * 2. The gradient stores: retail's order needs an anti-dependence from `lwc1 0x1840(player)` to the
 *    two stack stores, which needs player to have no known alias base, and that needs a second set
 *    of its pseudo that survives to sched1. Using it for the buffer pointer takes s0 from the
 *    block-local pointer; the dead test after player's last use changes nothing else.
 * 3. 0x41E3E4: with no sched2 boundary at the last sprite, `daddu s2,v0` wins the depend_count tie
 *    against the lwc1. Its use in the next block must stop counting.
 *
 * Real wordings found on the way, valid whatever is ruled on the tests:
 * - func_L08_0033CD18 is `void (s32, s32, ...)` (L08_002F2D10.c), not int: that fixes 0x41DF40.
 * - `h=D_001A7344-h; h+=hz;` as two statements gives h the a3 preference.
 * - gradient b is stored before a.
 * - `text` is one local across the three text arms: splitting it loses 8 bytes.
 *
 * Still unknown: the real source of the live-register test. Its registers are tex/x at the sites
 * tried here, and upstream's `do { } while (0)` after `(int)(S * v + 0.5f)` in 002D53F0.c and
 * 002CD3C0.c probably stands in for the same construct. Lead: the matched `menu_vector_offset`
 * in 002CEBD0-g8.c has the same `if (mode == 1) value += offset` shape.
 */
#include "common.h"

extern u8 D_00189E20[];
extern int D_L08_001B2508_view[] __asm__("D_L08_001B2508") __attribute__((sda));
#define D_L08_001B2508 (D_L08_001B2508_view[0])
extern u8 * D_L08_001A904C_view[] __asm__("D_L08_001A904C") __attribute__((sda));
#define D_L08_001A904C (D_L08_001A904C_view[0])
extern float D_L08_001B3244_view[] __asm__("D_L08_001B3244") __attribute__((sda));
#define D_L08_001B3244 (D_L08_001B3244_view[0])
extern int D_L08_001B3240_view[] __asm__("D_L08_001B3240") __attribute__((sda));
#define D_L08_001B3240 (D_L08_001B3240_view[0])
extern int D_001A7340_view[] __asm__("D_001A7340") __attribute__((sda));
#define D_001A7340 (D_001A7340_view[0])
extern int D_001A7344_view[] __asm__("D_001A7344") __attribute__((sda));
#define D_001A7344 (D_001A7344_view[0])
extern int D_001A79F0_view[] __asm__("D_001A79F0") __attribute__((sda));
#define D_001A79F0 (D_001A79F0_view[0])
extern int D_00138320 NOT_SDA;
extern float D_L08_001B031C __attribute__((sda));
extern int D_L08_001B2508_gp SDATA(D_L08_001B2508);
extern int D_L08_001B3240_gp SDATA(D_L08_001B3240);
extern u8 D_L08_001B1A34 __attribute__((sda));
extern u8 D_L08_001B1A35 __attribute__((sda));
extern u8 D_L08_001B1A36 __attribute__((sda));
extern u8 D_L08_001B1A37 __attribute__((sda));
extern u8 D_L08_001B1A38 __attribute__((sda));
extern u8 D_L08_001B1A39 __attribute__((sda));
extern u8 D_L08_001B1A3A __attribute__((sda));
extern u8 D_L08_001B1A3B __attribute__((sda));
extern u8 D_L08_001B1A3C __attribute__((sda));
extern u8 D_L08_001B1A3D __attribute__((sda));
extern u8 D_L08_001B1A3E __attribute__((sda));
extern u8 D_L08_001B1A3F __attribute__((sda));
extern u8 D_L08_001B1A40 __attribute__((sda));
extern int D_L08_001B1A74 __attribute__((sda));
extern int D_L08_001B1A78 __attribute__((sda));
extern int D_L08_001B1A90 __attribute__((sda));
extern int D_L08_001B1A94 __attribute__((sda));
extern int D_L08_001B1A98 __attribute__((sda));
extern int D_L08_001B1AD4 __attribute__((sda));
extern int D_L08_001B1AD8 __attribute__((sda));
extern float D_L08_001B199C __attribute__((sda));
extern float D_L08_001B1A80 __attribute__((sda));
extern float D_L08_001B1A84 __attribute__((sda));
extern float D_L08_001B1A88 __attribute__((sda));
extern float D_L08_001B1A8C __attribute__((sda));
extern float D_L08_001B1AAC __attribute__((sda));
extern float D_L08_001B1AB0 __attribute__((sda));
extern float D_L08_001B1AB4 __attribute__((sda));
extern float D_L08_001B1AB8 __attribute__((sda));
extern float D_L08_001B1ABC __attribute__((sda));
extern float D_L08_001B1AC0 __attribute__((sda));
extern float D_L08_001B1AC4 __attribute__((sda));
extern float D_L08_001B1AC8 __attribute__((sda));
extern float D_L08_001B1ACC __attribute__((sda));
extern float D_L08_001B1AD0 __attribute__((sda));

extern void func_L08_00389B68(int, int, u64, int, u64);
extern int func_00282978(void *);
extern int func_L08_00300090(void), func_L08_003000C0(void);
extern void func_L08_00300100(void);
extern int func_00336D50(void *, int);
extern void func_00300A00(int, int, float, float, float, float, float);
extern float func_00282C48(float);
extern int func_00283D38(int, int, float);
extern void func_0027EAE8(int, int, int, int, int, void *);
extern int func_002E5DA0(int, int, void *, int);
extern float func_00283CE0(int), func_002A7798(float), func_L08_003277C0(int, float);
extern int func_00283CF0(float);
extern u8 *func_00288FB8(int);
extern void func_L08_002F5CB8(int, int, u64, u8 *, int, float);
extern void func_L08_0033CD18(u8 *, u8 *, ...);
extern int func_001157AC(void *);
extern void func_0027F5B8(int, int, u64, u8 *, int);
extern float func_002ABB18(float), func_00283198(float), func_002A7790(float);
extern int func_0028E588(int, int);
extern int func_0028EA58(int, int, int, int, int, void *);
extern void func_L08_004207D0(void *);
#define FL(p,o) (*(float *)((u8 *)(p)+(o)))
#define IN(p,o) (*(int *)((u8 *)(p)+(o)))
#define SH(p,o) (*(short *)((u8 *)(p)+(o)))
#define BY(p,o) (*((u8 *)(p)+(o)))
#define G(off) D_L08_##off

static __inline__ u64 PackHudFlag(u64 flags, u8 value, int shift)
{
    return flags | ((u64)value << shift);
}

static __inline__ int HudY(float value, float adjustment)
{
    float y = value * D_L08_001B3244 + 0.5f;
    if (D_L08_001B3240 == 1) return (int)(y + adjustment);
    return (int)y;
}

static __inline__ int HudCoord(float value, int tex)
{
    float y = value * D_L08_001B3244 + 0.5f;
    int r = (int)y;
    int m = r + D_L08_001B3240;
    if (tex == 1) m = D_001A79F0 + 8;
    if (m) y += 4.0f;
    return r;
}

static __inline__ int HudCoord2(float value, int live)
{
    float y = value * D_L08_001B3244 + 0.5f;
    int r = (int)y;
    float m = 0.0f;
    if (D_L08_001B3240 == 1) y += 4.0f;
    if (live != 0) m = D_L08_001B3244 * 2.0f;
    if (m != 0.0f) y += 4.0f;
    return r;
}

int func_L08_0041D808(void)
{
    int player = (int)D_00189E20;
    u8 *moby = *(u8 **)(player + 0x1830);
    u8 *state = *(u8 **)(moby + 0x68);
    int mask = -1;
    int colors[4];
    struct HudGradient { int a, b; } gradientBase;
    u8 buffer[64];

    if (FL(player,0x183C) < FL(player,0x1840)*0.25f) {
        if ((D_L08_001B2508 / 90) & 1) mask = 0xFF0F0FFF;
    }
    {
        u64 test, alpha;
        test = G(001B1A39);
        test = PackHudFlag(test, G(001B1A3A), 1);
        test = PackHudFlag(test, G(001B1A3B), 4);
        test = PackHudFlag(test, G(001B1A3C), 12);
        test = PackHudFlag(test, G(001B1A3D), 14);
        test = PackHudFlag(test, G(001B1A3E), 15);
        alpha = G(001B1A34);
        alpha = PackHudFlag(alpha, G(001B1A35), 2);
        test = PackHudFlag(test, G(001B1A3F), 16);
        alpha = PackHudFlag(alpha, G(001B1A36), 4);
        alpha = PackHudFlag(alpha, G(001B1A37), 6);
        func_L08_00389B68(2,0x47,test|((u64)G(001B1A40)<<17),0x42,
                         alpha|((u64)G(001B1A38)<<32));
    }
    func_00282978(state+0x8E);
    if (D_L08_001A904C) {
        int x, y, w, h, firstColor, secondColor, tex;
        float delta, fraction, scale, sx;
        int hz;
        func_L08_00300090();
        func_L08_00300100();
        tex = func_00336D50(D_L08_001A904C+0x8710,0xA8);
        sx = D_001A7340/2;
        y = HudCoord(G(001B199C), tex);
        func_00300A00(tex,mask&0x60442D00,sx,(D_001A7344/2)+y,1.0f,1.0f,0.0f);
        tex = func_00336D50(D_L08_001A904C+0x8710,0xA9);
        sx = D_001A7340/2;
        y = HudCoord(G(001B199C), tex);
        func_00300A00(tex,mask&0x55F0C070,sx,(D_001A7344/2)+y,1.0f,1.0f,0.0f);
        delta=FL(player,0x183C)-FL(state,0xA4);
        delta*=0.2f;
        scale=func_00282C48(delta);
        if (scale>1.0f) delta/=scale;
        secondColor=G(001B1AD4); firstColor=G(001B1AD8);
        FL(state,0xA4)+=delta;
        fraction=FL(state,0xA4)/FL(player,0x1840);
        gradientBase.a=secondColor; gradientBase.b=firstColor;
        *(struct HudGradient *)colors=gradientBase;
        colors[1]=func_00283D38(secondColor,firstColor,fraction);
        x=(int)G(001B1AC4);
        y=D_001A7344-HudY(G(001B1AC8),-4.0f);
        w=(int)(G(001B1AC4)+G(001B1ACC)*fraction);
        h=HudY(G(001B1AC8),-4.0f);
        hz=HudCoord2(G(001B1AD0),x);
        h=D_001A7344-h;
        h+=hz;
        func_0027EAE8(x,y,w,h,0xFFFFF1,colors);
        func_L08_003000C0();
    }
    if (D_001A79F0==8 || D_001A79F0==19) {
        if (SH(state,0x8E)) {
            int alpha;
            float fraction, scale, fade;
            u8 *text;
            if (SH(state,0x8E)==1) func_002E5DA0(3,0,moby,0x85C);
            fraction=func_00283CE0(180-SH(state,0x8E));
            fraction/=func_00283CE0(180);
            scale=G(001B1ABC)+(G(001B1AC0)-G(001B1AAC))*func_002A7798(fraction);
            fade=func_L08_003277C0(3, 1.0f-fraction)*127.0f;
            alpha=func_00283CF0(fade*0.5f)<<24;
            text=func_00288FB8(0x2F59);
            func_L08_002F5CB8(D_001A7340-G(001B1A94),D_001A7344-HudY(G(001B1A98),-6.0f),
                (alpha|0x80)&mask,text,0x11,scale*0.96f);
            alpha=func_00283CF0(fade*0.75f)<<24;
            func_L08_002F5CB8(D_001A7340-G(001B1A94),D_001A7344-HudY(G(001B1A98),-6.0f),
                (alpha|0x800000)&mask,text,0x11,scale*0.98f);
            alpha=func_00283CF0(fade)<<24;
            func_L08_002F5CB8(D_001A7340-G(001B1A94),D_001A7344-HudY(G(001B1A98),-6.0f),
                (alpha|0x808080)&mask,text,0x11,scale);
        } else if (IN(state,0x6C)) {
            int x, y;
            float fraction, scale;
            u8 *text;
            if (FL(state,0x68) > D_L08_001B031C) {
                text=func_00288FB8(0x31E0);
                func_L08_0033CD18(buffer,text,func_00283CF0(FL(state,0x68)));
                x=D_001A7340-G(001B1A90);
                y=D_001A7344-HudY(G(001B1A98),-6.0f);
                func_0027F5B8(x,y,(u32)(mask&0x80FFDE8D),buffer,func_001157AC(buffer));
            } else if (D_00138320&0x80) {
                text=func_00288FB8(0x2F57);
                func_L08_002F5CB8(D_001A7340-G(001B1A94),D_001A7344-HudY(G(001B1A98),-6.0f),
                    (u32)(mask&0x80F0F0F0),text,0xD,G(001B1AB0));
            } else {
                fraction=func_00283CE0(D_L08_001B2508_gp);
                fraction=func_002ABB18(fraction*0.1f);
                fraction=func_00283198(fraction);
                scale=G(001B1AB4)+G(001B1AB8)*fraction;
                text=func_00288FB8(0x2F58);
                func_L08_002F5CB8(D_001A7340-G(001B1A94),D_001A7344-HudY(G(001B1A98),-6.0f),
                    (u32)(mask&0x80F0F0F0),text,0x11,scale);
            }
        }
        if (BY(state,0x8A)) {
            int alpha, x, y, w, h, sprite;
            float fraction, width, height, dx, dy, rise;
            { float num=func_00283CE0(SH(state,0x88));
            fraction=num/func_00283CE0(G(001B1A78)); }
            width=func_002A7790(fraction)*32.0f;
            height=func_002A7790(fraction)*24.0f;
            dx=G(001B1A80)*func_002A7798(fraction);
            dy=G(001B1A84)*func_002A7798(fraction);
            alpha=func_00283CF0(fraction*128.0f)<<24;
            rise=16.0f-width*0.5f;
            if (D_L08_001A904C) {
                int a;
                func_L08_00300090(); func_L08_00300100();
                a=func_00283CF0(func_00283CE0(96)*fraction)<<24;
                func_00300A00(func_00336D50(D_L08_001A904C+0x8710,0xAA),(a|0x442D00)&mask,
                    (float)(D_001A7340/2+32)-dx,D_001A7344/2+G(001B1A74),1,1,0);
                a=func_00283CF0(func_00283CE0(85)*fraction)<<24;
                func_00300A00(func_00336D50(D_L08_001A904C+0x8710,0xAB),(a|0xF0C070)&mask,
                    (float)(D_001A7340/2+32)-dx,D_001A7344/2+G(001B1A74),1,1,0);
                func_L08_003000C0();
            }
            if (BY(state,0x8A)<3) {
                { int c=alpha|0xF0F0B0; colors[0]=c; colors[1]=c; colors[2]=c; colors[3]=c; }
                sprite=func_0028E588(0xEAA2,3);
                x=func_00283CF0((float)D_001A7340-G(001B1A8C)-dy);
                { float v = (float)func_00283CF0(G(001B1A88)+(float)G(001B1A74)+(float)SH(state,0x8C)+rise)*D_L08_001B3244+0.5f;
                  y = (int)v; player = D_L08_001B3240; if (player == 1) v += 4.0f;
                  { float m = 0.0f; if (x != 0) m = G(001B1A84) * 2.0f; if (m != 0.0f) v += 4.0f; } }
                w=func_00283CF0(height); h=func_00283CF0(height);
                func_0028EA58(sprite,x,y,w,h,colors);
            }
        }
        func_L08_004207D0(moby);
    }
    func_L08_00389B68(2,0x47,0x5340B,0x42,0x8000000044ULL);
    IN(state,0x6C)=0; FL(state,0x68)=1000000.0f;
    return 1;
}
