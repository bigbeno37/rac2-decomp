#include "common.h"
typedef int Quad __attribute__((mode(TI)));
typedef union { Quad quad; float f[4]; } Vector;
typedef struct { u8 pad00[0xC]; u8 rank; } MobyClass;
typedef struct { u8 pad00[0x24]; MobyClass *cls; u8 pad28[0x54]; s16 visible; } SlotMoby;
typedef struct { u8 pad00[0x34]; int dirty; } Panel;
/* One equipment slot of the hero (0x50 bytes). */
typedef struct {
    SlotMoby *moby, *holster;
    u8 pad08[0x11];
    u8 pose, swap, pad1B, state, restPose, pad1E[2];
    int anim, mode, item;
    u8 pad2C[0x24];
} Slot;
typedef struct {
    u8 pad0000[0x80];
    Vector position;
    u8 pad0090[0xE00];
    Panel *panel[2];
    u8 pad0E98[0x308];
    int quickTimer;
    u8 pad11A4[0x7C];
    Slot slots[4];
    u8 pad1360[0x137];
    u8 swimming;
    u8 pad1498[0x43C];
    int armed;
    u8 pad18D8[0x9BC];
    int move, pad2298, state;
    u8 pad22A0[0x16];
    u8 pending, locked, refresh, pad22B9[2], skin, blocked, pad22BD[0xB];
    int request[4], pad22D8[3], current[4], pad22F4[3], previous[4], pad2310[3], restore[4];
    int pad232C[3], queued;
    u8 pad233C[0x1B4];
    u8 diving;
} HeroState;
typedef struct { u8 pad00[4]; u8 fancy; u8 pad05[0xF]; int sound; u8 pad18[4]; int skin, pad20, flags; u8 pad28[0xB8]; } ItemClass;
extern u8 D_00189E20[];
extern u32 D_00138180[];
extern u8 D_00139568[];
extern ItemClass D_L00_00262570[];
extern int D_001A7398[] __attribute__((sda));
extern u8 D_001A7A80[] __attribute__((sda));
extern u8 D_001A7A00[] __attribute__((sda));
extern u8 D_001A7B08[] __attribute__((sda));
extern void func_L00_003ECC90(Vector *, int);
extern int func_L00_002D3F30(int, int);
extern void func_L00_003C3A70(int);
extern int func_L00_0032EF10(u32, int, int *);
extern int func_L00_002D6418(void);
extern int func_L00_002ADF80(void);
extern int func_002829A8(void *);
extern int func_00294630(int);
extern void func_00294420(int);
extern void func_L00_002AB3E0(void);
extern int func_002A7820(int, int);
extern void func_L00_002B1308(int);
extern void func_L00_002AF7D0(void);
extern void func_002A7470(void *, int, int, int);

#define ITEM_ENTRY(id) (*(ItemClass *)(D_00139568[(id)] * sizeof(ItemClass) + (u8 *)D_L00_00262570))

/* Applies the hero's pending item request for equipment slot SLOT. */
