/* opus-2 worker v2524 (Claude Opus 5.5, xhigh), 2026-11-05: recompiled on
 * the c03 toolchain, same result (2340/2340 bytes, 0/27, 131 rows). Rule,
 * from resource.c: mark_referenced_resources assigns `volatil` at every MEM
 * it reads, so in the forward scan of fill_simple_delay_slots only a later
 * load clears the asm's flag in `needed` (a store's MEM only resets `set`).
 * Retail's own reloads `lw s7,0xBE0(sp)` / `lw s5,0xBE4(sp)` stand between
 * the copy and i = 0, and retail also leaves `lui s4` (the other candidate,
 * unsplit la) in place, so all trials after the copy are blocked. That fits
 * a reorg rule (sticky volatil, or a forward scan that stops at an asm, as
 * the backward scan's stop_search_p does) and no C wording: no label, jump,
 * USE or zero-length insn can sit there and survive both reorg passes.
 * A retail census (jal + nop after a qcopy with a forward candidate) would
 * back an upstream transformer like trap_delay_barrier.py.
 *
 * opus-1 worker v2398 (Claude Opus 5.5, xhigh), 2026-11-04.
 * Best check.py result: 2340 compiled bytes / 2340 retail bytes; 0/27 places;
 * 6 instructions differ (131 rows counting relocation-only ones).
 * Profile: default game -O2 -G8 -ffunction-sections in a standalone -g8.c
 * unit. All required declarations are below; common.h supplies qcopy.
 *
 * Only one difference remains: retail keeps a nop in the slot of
 * `jal func_00280328` at +0xB0 and has `daddu s2,zero,zero` (first loop's
 * i = 0) at +0x154. Here reorg's forward fill moves i = 0 into that slot, so
 * +0x154..+0x160 sit one row early and a loop-alignment nop lands at +0x164.
 * Frame, spill slots, schedule and registers agree everywhere else.
 *
 * Cause: the only candidates are i = 0 and `lui s4`. No branch targets that
 * range, so no label can stop the scan (jump2 deletes unused ones). The
 * last memory reference before i = 0 is the spill reload `lw s5,0xBE4(sp)`,
 * which clears the volatil flag that qcopy's asm set in resource.c. Retail
 * acts as if that flag stayed set: the removed -frac2-sticky-volatile-refs
 * behaviour, the same blocker as func_L00_003C5310 and func_L20_003E6728.
 * Tried with no change: -g, -frac2-qcopy -frac2-qcopy-barrier with a TI
 * assignment, -frac2-keep-block-order, -frac2-gcse-unsplit-edges. Worse:
 * -frac2-qcopy alone (56 rows), -mno-split-addresses (2244 bytes).
 *
 * What the wording fixes: one counter per loop (a shared i outranks the
 * givs in global.c); uv[i][0] before uv[i][1] for giv order; the struct
 * MeshPoint store through an inline helper (mem/s lets D_L00_001AF9DC's load
 * rise past the stores, and gives the addu/addiu pair); one i++ after the
 * colour arms (reorg doubles it); _1 sda aliases for the [1] and [7] reads.
 */
#include "common.h"
typedef unsigned int MeshQuad __attribute__((mode(TI)));
typedef union { MeshQuad q; float v[4]; } MeshVector;
typedef struct { float x, y, z; } MeshPoint;
extern float D_L00_001AF9DC __attribute__((sda)), D_L00_001AF9E0 __attribute__((sda)), D_L00_001AF9E4 __attribute__((sda));
extern int D_L00_001AF9D8 __attribute__((sda)), D_L00_001AF998 __attribute__((sda)), D_L00_001AF9D0 __attribute__((sda));
extern u32 D_L00_001AF9E8 __attribute__((sda));
extern int D_L00_001AF998_1[] __attribute__((sda)), D_L00_001AF9D0_1[] __attribute__((sda));
extern signed char D_L00_001AF9E8_1[] __attribute__((sda));
extern MeshVector D_L00_001B8DC0 NOT_SDA, D_0018A0E0 NOT_SDA;
extern float D_L00_00294CA0[][3], D_L00_00295390[][3];
extern float D_L00_00295660[][2], D_L00_00295B00[][2];
extern u32 D_L00_00295CE0[], D_L00_00295F30[];
extern u64 func_0027C878(int);
extern void func_002FC560(int,u64), func_00280328(void);
extern void func_00282CF0(void *,void *,void *), func_00282F20(void *,void *,float), func_00282DD8(void *,void *,void *), func_002830C0(void *,void *,void *);
extern float func_00283CE0(int);
extern int func_00283CF0(float);
extern void func_00281810(int,void *,int,void *,int);
static inline void MeshStorePoint(MeshPoint *point, MeshVector *value)
{
    point->x = value->v[0];
    point->y = value->v[1];
    point->z = value->v[2];
}
void func_L00_003B4560(unsigned char *object, float opacity)
{
    MeshVector transform[4];
    MeshPoint positions[148];
    float uv[148][2];
    MeshVector temp;
    int alpha;
    unsigned char *origin;
    D_L00_001AF9DC += D_L00_001AF9E0;
    if (D_L00_001AF9DC >= 7.0f) D_L00_001AF9DC -= 7.0f;
    { u64 texture = func_0027C878(D_L00_001AF9D8);
      origin = object + 0x10;
      func_002FC560(6,texture); }
    func_002FC560(0x42,0x8000000048ULL);
    func_002FC560(8,0);
    func_002FC560(0x14,0xFF9000000260ULL);
    func_00280328();
    func_00282CF0(&transform[0],&D_L00_001B8DC0,origin);
    func_00282F20(&transform[0],&transform[0],1.0f);
    func_00282DD8(&transform[1],&transform[0],&D_0018A0E0);
    func_00282F20(&transform[1],&transform[1],-1.0f);
    func_00282DD8(&transform[2],&transform[1],&transform[0]);
    qcopy(&transform[3], origin);
    {
    int i;
    for (i=0;i<148;++i) {
        temp.v[0] = 0.0f;
        temp.v[1] = D_L00_00294CA0[i][1] * D_L00_001AF9E4;
        temp.v[2] = D_L00_00294CA0[i][0] * D_L00_001AF9E4;
        temp.v[3] = 1.0f;
        func_002830C0(&temp,&temp,transform);
        MeshStorePoint(&positions[i], &temp);
        uv[i][0] = D_L00_00295660[i][1] + D_L00_001AF9DC;
        uv[i][1] = D_L00_00295660[i][0];
        if (D_L00_00295CE0[i] & 0xFF000000) {
            alpha=func_00283CF0(func_00283CE0(D_L00_001AF9E8_1[7])*opacity);
            if (alpha < 1) alpha=1;
            alpha <<= 24;
            alpha |= D_L00_001AF9E8;
            D_L00_00295CE0[i] = alpha;
        } else D_L00_00295CE0[i] = D_L00_001AF9E8;
    }
    func_00281810(D_L00_001AF998,positions,D_L00_001AF9D0,uv,1);
    }
    {
    int i;
    for (i=0;i<60;++i) {
        temp.v[0] = 0.0f;
        temp.v[1] = D_L00_00295390[i][1] * D_L00_001AF9E4;
        temp.v[2] = D_L00_00295390[i][0] * D_L00_001AF9E4;
        temp.v[3] = 1.0f;
        func_002830C0(&temp,&temp,transform);
        MeshStorePoint(&positions[i], &temp);
        uv[i][0] = D_L00_00295B00[i][1] + D_L00_001AF9DC;
        uv[i][1] = D_L00_00295B00[i][0];
        if (D_L00_00295F30[i] & 0xFF000000) {
            alpha=func_00283CF0(func_00283CE0(D_L00_001AF9E8_1[7])*opacity);
            if (alpha < 1) alpha=1;
            alpha <<= 24;
            alpha |= D_L00_001AF9E8;
            D_L00_00295F30[i] = alpha;
        } else D_L00_00295F30[i] = D_L00_001AF9E8;
    }
    func_00281810(D_L00_001AF998_1[1],positions,D_L00_001AF9D0_1[1],uv,1);
    }
    {
    int i;
    for (i=0;i<148;++i) {
        temp.v[0] = 0.0f;
        temp.v[1] = -D_L00_00294CA0[i][1] * D_L00_001AF9E4;
        temp.v[2] = D_L00_00294CA0[i][0] * D_L00_001AF9E4;
        temp.v[3] = 1.0f;
        func_002830C0(&temp,&temp,transform);
        MeshStorePoint(&positions[i], &temp);
        uv[i][0] = D_L00_00295660[i][1] + D_L00_001AF9DC;
        uv[i][1] = D_L00_00295660[i][0];
    }
    func_00281810(D_L00_001AF998,positions,D_L00_001AF9D0,uv,1);
    }
    {
    int i;
    for (i=0;i<60;++i) {
        temp.v[0] = 0.0f;
        temp.v[1] = -D_L00_00295390[i][1] * D_L00_001AF9E4;
        temp.v[2] = D_L00_00295390[i][0] * D_L00_001AF9E4;
        temp.v[3] = 1.0f;
        func_002830C0(&temp,&temp,transform);
        MeshStorePoint(&positions[i], &temp);
        uv[i][0] = D_L00_00295B00[i][1] + D_L00_001AF9DC;
        uv[i][1] = D_L00_00295B00[i][0];
    }
    func_00281810(D_L00_001AF998_1[1],positions,D_L00_001AF9D0_1[1],uv,1);
    }
    {
    int i;
    for (i=0;i<148;++i) {
        temp.v[0] = 0.0f;
        temp.v[1] = D_L00_00294CA0[i][1] * D_L00_001AF9E4;
        temp.v[2] = -D_L00_00294CA0[i][0] * D_L00_001AF9E4;
        temp.v[3] = 1.0f;
        func_002830C0(&temp,&temp,transform);
        MeshStorePoint(&positions[i], &temp);
        uv[i][0] = D_L00_00295660[i][1] + D_L00_001AF9DC;
        uv[i][1] = D_L00_00295660[i][0];
    }
    func_00281810(D_L00_001AF998,positions,D_L00_001AF9D0,uv,1);
    }
    {
    int i;
    for (i=0;i<60;++i) {
        temp.v[0] = 0.0f;
        temp.v[1] = D_L00_00295390[i][1] * D_L00_001AF9E4;
        temp.v[2] = -D_L00_00295390[i][0] * D_L00_001AF9E4;
        temp.v[3] = 1.0f;
        func_002830C0(&temp,&temp,transform);
        MeshStorePoint(&positions[i], &temp);
        uv[i][0] = D_L00_00295B00[i][1] + D_L00_001AF9DC;
        uv[i][1] = D_L00_00295B00[i][0];
    }
    func_00281810(D_L00_001AF998_1[1],positions,D_L00_001AF9D0_1[1],uv,1);
    }
    {
    int i;
    for (i=0;i<148;++i) {
        temp.v[0] = 0.0f;
        temp.v[1] = -D_L00_00294CA0[i][1] * D_L00_001AF9E4;
        temp.v[2] = -D_L00_00294CA0[i][0] * D_L00_001AF9E4;
        temp.v[3] = 1.0f;
        func_002830C0(&temp,&temp,transform);
        MeshStorePoint(&positions[i], &temp);
        uv[i][0] = D_L00_00295660[i][1] + D_L00_001AF9DC;
        uv[i][1] = D_L00_00295660[i][0];
    }
    func_00281810(D_L00_001AF998,positions,D_L00_001AF9D0,uv,1);
    }
    {
    int i;
    for (i=0;i<60;++i) {
        temp.v[0] = 0.0f;
        temp.v[1] = -D_L00_00295390[i][1] * D_L00_001AF9E4;
        temp.v[2] = -D_L00_00295390[i][0] * D_L00_001AF9E4;
        temp.v[3] = 1.0f;
        func_002830C0(&temp,&temp,transform);
        MeshStorePoint(&positions[i], &temp);
        uv[i][0] = D_L00_00295B00[i][1] + D_L00_001AF9DC;
        uv[i][1] = D_L00_00295B00[i][0];
    }
    func_00281810(D_L00_001AF998_1[1],positions,D_L00_001AF9D0_1[1],uv,1);
    }
}
