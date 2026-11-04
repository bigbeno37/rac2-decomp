#include "include_asm.h"
#include "common.h"
extern void func_0028FC48(int, int, int, int, unsigned long, int);
extern int func_0028E588(int, int);
extern void func_0028E868(int, int, int, int, int, int);
extern int D_001AA43C __attribute__((sda));

void func_002D9CF8(int x, int y, int show)
{
    func_0028FC48(x - 5, y - 5, x + 5, y + 5, 0x80FFA888UL, 0);
    func_0028FC48(x - 4, y - 4, x + 4, y + 4, D_001AA43C, 0);
    if (show)
        func_0028E868(func_0028E588(0xE99D, 1), x - 13, y - 18, 30, 30, 128);
}

extern int ref_L00_002D68D8_00 __attribute__((sda));
extern int func_00283D38(int, int, float);
int func_002D68D8(int start, int first, int second)
{
    int count;
    float fraction;
    if (start < 0) start = 0;
    if (first == -1) first = (int)0x80FFA888u;
    if (second == -1) second = (int)0x8020FFFFu;
    count = ref_L00_002D68D8_00;
    if (count >= start)
        fraction = 1.0f - (float)(count - start) / (float)count;
    else
        fraction = 1.0f;
    return func_00283D38(first, second, fraction);
}

extern int func_002DE3A0(int);
extern int func_002DE550(int);
extern int func_002C9A60(void);
extern void func_00287E80(void);
extern void func_00339760(void *);
extern volatile int D_001A8C88;
extern char *D_001A8CFC __attribute__((sda));

int func_002DCAB8(char *object)
{
    func_002DE3A0(1);
    *(int *)(object + 0x48) = func_002DE550(0);
    *(int *)(object + 0x4C) = 0;
    func_002C9A60();
    func_00287E80();
    if (D_001A8C88 != 0 && D_001A8CFC != 0)
        func_00339760(D_001A8CFC + 0x3CAA8);
    return 0;
}

extern void func_0027C540(int);
extern void func_0027C660(void);
extern unsigned long long func_00294C40(int);
extern void func_0027E018(int, char *, int, int, int, int, int, int, unsigned long long, unsigned long long);
extern int D_001ABB48 __attribute__((sda));
extern char *D_001ABB4C;
extern int D_001ABB50;
extern int D_001ABB54;
extern int D_001ABB58 __attribute__((sda));
extern char *D_001ABB5C;
extern int D_001ABB60;
extern int D_001ABB64;

int func_002DAE90(char *object)
{
    unsigned long long color;
    unsigned long long packet;
    if (*(int *)(object + 0x44) < 4)
        return 0;
    color = 0x60A09080ULL;
    func_0027C540(0);
    packet = func_00294C40(*(int *)(object + 0x48));
    func_0027E018(D_001ABB48, D_001ABB4C, D_001ABB50, D_001ABB54, 0, 0, 0x100, 0x100, color, packet);
    packet = func_00294C40(*(int *)(object + 0x4C));
    func_0027E018(D_001ABB58, D_001ABB5C, D_001ABB60, D_001ABB64, 0, 0, 0x100, 0x100, color, packet);
    func_0027C660();
    return 8;
}

extern int D_001A7420;
extern char D_001F2840[];

/* One save-slot summary row of the save context at D_001393E0 (28 bytes). */
typedef struct {
    int state;              /* -1: empty slot */
    int score;
    int extra;
    int ticks;              /* play time, 60 per second */
    unsigned char pad[4];
    unsigned char date[4];
    int unknown;
} SaveRow;

typedef struct {
    int head[12];
    SaveRow records[4];     /* D_00139410 */
    unsigned char gap[0x15C - 0xA0];
    int mode;
    int unknown160;
    int selected;
} SaveData;

extern SaveRow D_00139410[];

/* GP-relative layout coefficients of the save-slot panel. */
extern int D_001ABBF0 __attribute__((sda)); /* line height */
extern int D_001ABBF4 __attribute__((sda)); /* slot gap */
extern int D_001ABBF8 __attribute__((sda));
extern int D_001ABBFC __attribute__((sda));
extern int D_001ABC00 __attribute__((sda));
extern int D_001ABC04 __attribute__((sda));
extern int D_001ABC08 __attribute__((sda));
extern int D_001ABC10 __attribute__((sda));
extern int D_001ABC14[] MACRO_ADDR;
extern int D_001ABC18 __attribute__((sda));
extern int D_001ABC1C[] MACRO_ADDR;
extern int D_001ABC20 __attribute__((sda));

/* Format strings. */
extern char D_001ABC28[] NOT_SDA;
extern char D_001ABAF0[] NOT_SDA;
extern char D_001ABB20[] NOT_SDA;
extern char D_001ABC38[] NOT_SDA;
extern char D_001ABC48[] NOT_SDA;

extern void func_0027F5B8(int, int, long, char *, int);
extern int func_0027FB40(int, int, u64, char *, int);
extern char *func_00288FB8(int);
extern int func_00115DA8(char *, char *, ...);
/* The game state at D_001F2840, as far as the page menu uses it. */
typedef struct {
    u8 pad0[8];
    int field8;             /* 0x08 */
    u8 padC[8];
    u8 *owner;              /* 0x14: active menu screen */
    int current;            /* 0x18 */
    u8 pad1C[0xC];
    int saved;              /* 0x28 */
    u8 pad2C[0xAF];
    u8 active;              /* 0xDB */
    u8 padDC[0x18];
    int page;               /* 0xF4 */
    u8 padF8[8];
    u8 *savedOwner;         /* 0x100 */
    int savedCurrent;       /* 0x104 */
    u8 pad108[0x2C];
    int lock;               /* 0x134 */
    u8 pad138[8];
    int savedMode;          /* 0x140 */
} PageRoot;

typedef struct {
    u8 pad[0x230];
    int page;               /* 0x230: current page, 0..27 */
} PageMap;

typedef struct {
    u8 pad[0x1C4];
    u32 buttons;            /* 0x1C4 */
} PagePad;

typedef struct {
    u8 pad[0x30];
    int selection[28];      /* 0x30: selected row per page */
    int count;              /* 0xA0: rows on the current page */
} PageMenu;

typedef struct {
    char pad[0x34];
    int item;               /* 0x34 */
    int pad38;
    int timer;              /* 0x3C */
} PagePreview;

extern char D_001C4FA0[];
extern char D_00138180[];
#define ROOT ((PageRoot *)D_001F2840)
#define MAP ((PageMap *)D_001C4FA0)
#define PAD ((PagePad *)D_00138180)

extern PagePreview D_0025A6E0;
extern u8 D_0025E680[];
extern u8 D_001A7BC8[] MACRO_ADDR;  /* page available, one byte per page */
extern int D_00139768[];            /* flag bits: bit (n & 31) of word n >> 2 */
extern int D_0025A620[];
extern int D_001A7478;
extern int D_001A79F0 __attribute__((sda));
extern int D_001A72D4 __attribute__((sda));
extern char D_001BAD40[];

extern int func_00295A10(int);
extern void func_00295C50(int);
extern int func_002E5E58(int, int, void *);
extern void func_0029D830(void);
extern int func_0029DB28(int *, int *, int *, int);
extern void func_002C9E60(int);
extern int func_00288AA0(char *, int);
extern int func_00288BE8(char *);
