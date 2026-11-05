/* func_US2_00295D70: opus-1, task v2805. Exact C blocked by a CATALOGUE defect, not by the code.

   The row's size is one word short. Retail's function is 0x00295D70..0x00295F30, 448 bytes; the row
   (boot and all 27 levels) says 444, so it ends on `jr $ra` at +0x1B8 and leaves that return's delay
   slot, `addiu $sp,$sp,0x20` (0x27BD0020) at +0x1BC, outside the row. The next row starts at 0x00295F30.
   444 is also v1.01's size for the template func_002954C0, and v2.00 has one more store.

   Best check.py result: `448 bytes, expected 444` at 0 of 28 places (the size test stops it).
   Measured past that test: this C, compiled at 448 bytes, passes Placement.derive() over all 448 bytes
   at 28 of 28 places, and the word after the row is 0x27BD0020 at all 28. No instruction differs.
   tools/build.py has not been run on this C.

   To land: correct the row to 448 through tools/levels.py (never by hand), then put this body in place
   of the marker in src/us2/game/002957C0.c. The default flags fit, and it is the template's C with one
   edit: the added store `D_001C4FA0[0x2B8 / 4] = e;`. All seven callees keep their shared names. */
extern unsigned char D_001A7AFD __attribute__((section(".sdata")));
extern int D_001A79F0_3 __attribute__((sda));
extern int D_001C4FA0[];

extern void func_00298170(void);
extern int D_001F2840[];
extern void func_0029DEA8(int, int);
extern int func_002B7C80(void *, int, int);
extern short func_002B7CE0(int);
extern int func_002EEE50(int, int, int, int);
extern int func_002EEED0(int, int, int, int, int);
extern int func_002E5E58(int, unsigned int, void *);

/* resets the map's cache slots and reloads the current level's entry */
typedef struct { int sector, size; } MapFile;
typedef struct {
    char pad0[0x36C];
    int base;           /* 0x36C */
    char pad370[0x14F0 - 0x370];
    MapFile files0[28]; /* 0x14F0 */
    MapFile files1[28]; /* 0x15D0 */
} MapToc;
extern MapToc D_0014B540;

void func_US2_00295D70(void)
{
    int *arena;
    int a, b, c;
    int e;
    int *map;

    func_00298170();
    func_0029DEA8(D_001C4FA0[0x230 / 4], 1);
    arena = D_001F2840;
    a = arena[0x10C / 4];
    b = arena[0x110 / 4];
    D_001C4FA0[0x248 / 4] = a;
    c = a + 0x9A800;
    D_001C4FA0[0x288 / 4] = 0;
    D_001C4FA0[0x28C / 4] = c;
    arena[0x10C / 4] = c + 0x48000;
    D_001C4FA0[0x290 / 4] = b;
    arena[0x110 / 4] = b + 0x48000;
    D_001C4FA0[0x294 / 4] = 0;
    D_001C4FA0[0x298 / 4] = 0;
    D_001C4FA0[0x29C / 4] = -1;
    D_001C4FA0[0x2A0 / 4] = -1;
    D_001C4FA0[0x2A4 / 4] = -1;
    D_001C4FA0[0x2A8 / 4] = -1;
    D_001C4FA0[0x2AC / 4] = -1;
    D_001C4FA0[0x2B0 / 4] = -1;
    if (D_001C4FA0[0x238 / 4] == -1) goto empty;
    if (D_001C4FA0[0x23C / 4] == 0 && D_001A7AFD != 0) {
        MapToc *toc = &D_0014B540;
        int size = toc->files1[D_001C4FA0[0x230 / 4]].size;
        e = size << 7;
        func_002B7C80((void *)c, toc->files1[D_001C4FA0[0x230 / 4]].sector + toc->base, size);
        func_002B7CE0(1);
        D_001C4FA0[0x23C / 4] = 1;
        D_001C4FA0[0x240 / 4] = e;
        D_001C4FA0[0x2B8 / 4] = e;
        func_002EEE50(D_001C4FA0[0x28C / 4], D_001C4FA0[0x238 / 4], 0, e);
        D_001C4FA0[0x234 / 4] = -2;
        D_001C4FA0[0x24 / 4] = 1;
        D_001C4FA0[0x2A0 / 4] = D_001A79F0_3 + 0x100;
    } else {
        int size;
        map = D_001C4FA0;
        if (map[0x238 / 4] == -1) goto empty;
        size = map[0x240 / 4];
        map[0x2B8 / 4] = size;
        func_002EEED0(map[0x28C / 4], map[0x238 / 4], 0, size, 0);
        map[0x24 / 4] = 1;
        map[0x2A0 / 4] = D_001A79F0_3 + (D_001A7AFD ? 0x100 : 0);
        map[0x234 / 4] = -2;
    }
    goto done;
empty:
    D_001C4FA0[0x24 / 4] = 0;
done:
    func_002E5E58(0x11, 0, 0);
}
