/* Nonmatching, Claude Opus 5.5 xhigh, task v2453 (5 November 2026).
 * Best check: 1756/1756 bytes, 0/1 USA v1.01 places, first difference at +0x494.
 * 5 of 439 rows really differ (one displaced instruction); every other differing row is
 * relocation display only. All seven PRE spill slots (0xADC-0xAF4) now match retail.
 * Profile: -O2 -G8 -ffunction-sections (default for a -g8 name), dedicated unit
 * src/levels/02_maktar_nebula/L02_003E80F8-g8.c, INCLUDE_ASM line in L02_003DB310.c
 * replaced by a comment. Not checked in PAL or USA v2.00. Declarations are below.
 *
 * Changes from the v2023/faithful-review draft: the draw loop is the natural
 * `for (draw = 0; draw < count; draw++) if (visible[draw]) ...` (loop.c's dbra gives
 * retail's count-down form unchanged), and four declaration initializers (i, layer,
 * pass, uvIndex; within the five-initializer rule, all on used locals) raise the insns
 * entering gcse from 396 to 400. PRE then hashes into 201 buckets, which is retail's
 * slot order: bucket = (14030 + sp offset) % 201 for the sp+C reaching registers and
 * (14030 + n) % 201 for objectIndex+1 (objectIndex is pseudo 316, needs 304-357).
 * Measured counts (cc1 -ds, insns in the cse dump): pass++/layer++ with the natural draw
 * loop = 394; nextPass/nextLayer as below = 396. N=203 (404-407 insns) also fits
 * if objectIndex stays in 286-337.
 *
 * Remaining, at +0x484 to +0x494: retail issues `daddu s3,s4` (the loop.c-hoisted copy
 * of PRE's &offset register for the first vertex loop) before the two bound `la`s; here
 * it comes after them. sched1 (and sched2 after it) rank all three alike: each births
 * a live-out set-once pseudo, so adjust_priority gives them the same max_priority;
 * reg weight 1 each, no in-block dependents, so LUID decides. The `la`s are PRE inserts
 * at the end of the preheader block, and loop.c emits its hoisted copy after them.
 * In retail the bounds must be biv-elimination temps (emitted after the hoists), i.e.
 * the natural `for (vertex = 0; vertex < 4; vertex++)` with `if (vertex < 2)`: that form
 * gives retail's order here, but the bounds become `la D_L02_001B0AB0__gp+32/+64`.
 * This assembler decides gp-relative per symbol, whatever the offset (measured:
 * `.extern foo__gp,1` gives `la foo__gp+32` = addiu $gp; incomplete `big` gives lui/addiu),
 * while retail has absolute lui/addiu for sym+off and $gp for the plain base. So the
 * natural form is 2 instructions short. Needs either a tool ruling on `la sym+off`
 * or a source construct that keeps the bound loads out of PRE and emitted after the
 * loop.c hoist; none found. Tried for the displaced row (v2023 and v2453), none better:
 * explicit bound locals; bounds in reused locals to skip the birthing boost (s0/s3 swap
 * or worse); plain Quad copy; qcopy_nc; an explicit offset local; -frac2-gcse-unsplit-edges
 * (byte-identical). The do-while with the bound views below is the workaround for the
 * assembler rule; D_L02_001B0AF0 is also packet 1's own geometry block.
 */
#include "include_asm.h"
#include "common.h"
typedef int Quad __attribute__((mode(TI)));
typedef struct { float vertex[4][4]; unsigned int color[4]; float uv[4][2]; unsigned long long state[4]; } Packet __attribute__((aligned(16)));
extern int D_L02_001B30C8;
extern char *D_L02_001B359C;
extern unsigned short *D_L02_001FC060[];
extern int D_L02_001B0A68[] __attribute__((sda)), D_L02_001B0A98[] __attribute__((sda));
extern int D_L02_001B0A68_gp SDATA(D_L02_001B0A68), D_L02_001B0A98_gp SDATA(D_L02_001B0A98);
extern float D_L02_001B0A78[][2] SDATA(D_L02_001B0A78);
extern float D_L02_001B0AB0[][4] NOT_SDA;
extern float D_L02_001B0AB0_gp[][4] SDATA(D_L02_001B0AB0);
/* Measured address views: the sign boundary, geometry end, and strided UV V
 * pointer below do not establish separate owning allocations. */
extern float D_L02_001B0AD0[][4] MACRO_ADDR;
extern float D_L02_001B0AF0[][4] MACRO_ADDR;
extern float D_L02_001B0AF0_gp[][4] SDATA(D_L02_001B0AF0);
extern int D_L02_001B0B30 __attribute__((sda));
extern unsigned int D_L02_001B0B38[] SDATA(D_L02_001B0B38);
extern float D_L02_001B0B40 __attribute__((sda));
extern float D_L02_001B0B48[][2] NOT_SDA;
extern float D_L02_001B0B48_gp[][2] SDATA(D_L02_001B0B48);
extern float D_L02_001B0B4C[][2] MACRO_ADDR;
extern char D_L02_001BA500[];
extern float func_00283CE0(int);
extern void func_00283410(void *, void *), func_0029FC20(void *, int, void *), func_002830C0(void *, void *, void *);
extern int func_00283DB8(void *, float);
extern void func_00283788(void *, void *, void *);
extern void func_L02_002EF968(void *, int, int, void *, void *, int);
extern void func_00282C88(void *), func_002833F0(void *, void *), func_00283098(void *, void *, void *);
extern void func_00283738(void *, void *, void *), func_00280B90(void *, void *, int);
extern unsigned long long func_0027C878(int);
extern void func_00283388(void *), func_00282CF0(void *, void *, void *), func_00282DD8(void *, void *, void *);
extern void func_00282F20(void *, void *, float), func_00282CC0(void *, void *, void *);
extern float func_002A7878(float, float), func_002A7940(void);
extern unsigned int func_00283D38(unsigned int, unsigned int, float);
extern void func_00281EB8(void *, int, unsigned int, int, float, float, float);

void func_L02_003E80F8(char *actor) {
    float matrices[32][16] __attribute__((aligned(16)));
    float rotation[16] __attribute__((aligned(16)));
    unsigned int visible[32];
    float tilt[12] __attribute__((aligned(16)));
    float probe[4] __attribute__((aligned(16)));
    Packet packet[2];
    float angles[4] __attribute__((aligned(16))), point[4] __attribute__((aligned(16)));
    float doubled[12] __attribute__((aligned(16))), identity[16] __attribute__((aligned(16)));
    float normal[4] __attribute__((aligned(16))), offset[4] __attribute__((aligned(16)));
    unsigned short *entry;
    unsigned short id;
    float frame;
    int count = 0, i = 0, layer = 0, pass = 0, nextLayer, nextPass;
    int uvIndex = 0;
    int draw;
    unsigned int color;
    *(Quad *)probe = 0;
    probe[0] = 30.0f;
    probe[3] = 1.0f;
    *(Quad *)point = 0;
    frame = func_00283CE0(D_L02_001B30C8 % 600);
    point[0] = (frame * 0.0016666667f + frame * 0.0016666667f) * 3.1415927f - 3.1415927f;
    *(Quad *)angles = *(Quad *)point;
    func_00283410(rotation, angles);
    entry = D_L02_001FC060[*(unsigned char *)(actor + 0x21)];
    while (count < 32) {
        char *object;
        id = *entry;
        object = D_L02_001B359C + (id & 0x7FFF) * 0x100;
        visible[count] = (*(unsigned short *)(object + 0x7C) >> 15) ^ 1;
        func_0029FC20(object, 0, matrices[count]);
        func_002830C0(point, probe, matrices[count]);
        point[3] = 30.0f;
        if (func_00283DB8(point, 512.0f) != -1) {
            func_00283788(matrices[count], matrices[count], rotation);
            count++;
        }
        if ((short)*entry++ < 0) break;
    }
    func_L02_002EF968(packet, D_L02_001B0A68_gp, D_L02_001B0A98_gp, D_L02_001B0A78, D_L02_001B0AB0_gp, 1);
    func_L02_002EF968(packet + 1, D_L02_001B0A68[1], D_L02_001B0A98[2], D_L02_001B0A78, D_L02_001B0AF0_gp, 1);
    packet[0].color[1] = packet[0].color[3] = D_L02_001B0A98[1];
    packet[1].color[1] = packet[1].color[3] = D_L02_001B0A98[3];
    func_00282C88(angles);
    angles[0] = 0.3926991f;
    func_002833F0(tilt, angles);
    for (i = 0; i < 2; i++) {
        func_00283098(packet[i].vertex[2], packet[i].vertex[2], tilt);
        func_00283098(packet[i].vertex[3], packet[i].vertex[3], tilt);
    }
    func_00283738(doubled, tilt, tilt);
    for (pass = 0; pass < 16; pass = nextPass) {
        int pair = (pass & 1) ? 2 : 0;
        nextPass = pass + 1;
        for (layer = 0; layer < 1; layer = nextLayer) {
            nextLayer = layer + 1;
            func_00283098(packet[layer].vertex[pair], packet[layer].vertex[pair], doubled);
            func_00283098(packet[layer].vertex[pair + 1], packet[layer].vertex[pair + 1], doubled);
            for (draw = 0; draw < count; draw++) {
                if (visible[draw]) func_00280B90(packet + layer, matrices[draw], 1);
            }
        }
    }
    packet[0].state[1] = func_0027C878(D_L02_001B0B30);
    for (uvIndex = 0; uvIndex < 4; uvIndex++) {
        packet[0].uv[uvIndex][0] = D_L02_001B0B48_gp[uvIndex][0];
        packet[0].uv[uvIndex][1] = D_L02_001B0B4C[uvIndex][0];
        packet[0].color[uvIndex] = D_L02_001B0B38[uvIndex & 1];
    }
    func_00283388(identity);
    {
    int objectIndex;
    for (objectIndex = 0; objectIndex < count; objectIndex++) {
        int vertex;
        float *geometry;
        if (!visible[objectIndex]) continue;
        func_00282CF0(point, D_L02_001BA500, matrices[objectIndex] + 12);
        func_00282DD8(normal, matrices[objectIndex], point);
        normal[3] = 1.0f;
        qcopy(identity + 12, matrices[objectIndex] + 12);
        geometry = (float *)D_L02_001B0AB0_gp;
        {
            float *g = geometry;
            float (*v)[4] = packet[0].vertex;
            do {
                float extent = g[2];
                if ((int)g < (int)D_L02_001B0AD0) extent += D_L02_001B0B40;
                else extent -= D_L02_001B0B40;
                func_00282F20(*v, normal, extent);
                func_00282F20(offset, matrices[objectIndex], g[0]);
                g += 4;
                func_00282CC0(*v, *v, offset);
                v++;
            } while ((int)g < (int)D_L02_001B0AF0);
        }
        func_00280B90(packet, identity, 0);
        for (vertex = 0; vertex < 4; vertex++) {
            float extent = geometry[vertex * 4 + 2];
            if (vertex < 2) extent += D_L02_001B0B40;
            else extent -= D_L02_001B0B40;
            func_00282F20(offset, normal, extent * -2.0f);
            func_00282CC0(packet[0].vertex[vertex], packet[0].vertex[vertex], offset);
        }
        func_00280B90(packet, identity, 0);
        *(Quad *)offset = 0;
        offset[0] = 2.666f;
        offset[3] = 1.0f;
        func_002830C0(offset, offset, matrices[objectIndex]);
        frame = func_002A7878(0.0f, 1.0f);
        color = func_00283D38(D_L02_001B0A98_gp, D_L02_001B0A98[1], frame);
        frame = func_002A7940();
        func_00281EB8(offset, 11, color, 1, 2.0f, 0.0f, frame);
        frame = func_002A7878(0.0f, 1.0f);
        color = func_00283D38(D_L02_001B0A98[2], D_L02_001B0A98[3], frame);
        frame = func_002A7940();
        func_00281EB8(offset, 11, color, 1, 2.75f, 0.0f, frame);
        frame = func_002A7940();
        func_00281EB8(offset, 49, color, 1, 2.75f, 0.0f, frame);
    }
    }
}
