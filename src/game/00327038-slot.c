/* func_0032D758: start an attached effect on an object, in the first free slot of the level's
   effect table. Its own unit because the slot array has its real type here, where 00327038.c
   only has an opaque search view of it. D_001A7A80 is reached by an unsplit lui/addiu pair. */
#include "common.h"

/* Declarations */
extern s32 func_0032D700(void *object);
/* func_0032D758's effect type rows, kind flags and per-bank effect parameters. */
typedef struct { int active; unsigned char type; char pad[0x47]; short kind; char tail[0x92]; } EffectType;
extern EffectType D_00239BA0[];
extern unsigned char D_001A7A80[] MACRO_ADDR;
extern unsigned char D_00139568[];
typedef struct { float scale; int ticks; char pad[8]; } EffectParams;
extern EffectParams D_00236510[2][119];
extern float *func_002AB1F0(void *);
/* The matched slot consumer reads +0x0C as phase; creation clears its bits. */
typedef struct {
    int remaining, color, flags;
    union { float value; unsigned int bits; } phase;
    float rate;
    char *object;
    int tag, active;
} EffectSlot;
extern EffectSlot D_L00_002533B0[];
extern void func_002A04B8(void *, int *, int *, int *);

static __inline__ int EffectAlternate(int kind)
{
    EffectType *type = &D_00239BA0[D_00139568[kind]];
    if (type->active) {
        if (type->type == 2) return 1;
        if (type->type == 1) {
            int selected = 0;
            switch (kind) {
            case 12: case 14: case 17: case 18: case 53: selected = 1; break;
            }
            if (selected) return 1;
        }
    }
    return 0;
}

static __inline__ int EffectThird(int kind)
{
    EffectType *type = &D_00239BA0[D_00139568[kind]];
    int result = 0;
    if (type->active) result = type->type == 3;
    return result;
}
