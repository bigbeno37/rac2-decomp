/* func_PAL_L10_00428C8C (10:00428C8C, 15:00435814), the PAL copy of func_L10_004235A0.
   Tier: Opus 5.5. Best check.py: 0 of 2 places, "200 bytes, expected 204". The C is exact; the row is wrong.
   The same C, where the callees func_L05_0037D6E8 and func_L05_00445A98 become func_PAL_L05_0037FEB0 and
   func_PAL_L05_0044BEB8, and the two closing tests that free D_L10_001B1400 and D_L10_001B1404 are gone.
   Blocker: the catalogue row starts on the padding nop after func_L10_00423500 (level 10) and func_L15_0042FD40
   (level 15), whose return's delay slot is the word before it. Retail's prologue (addiu $sp,$sp,-0x20) is at +4,
   10:00428C90 and 15:00435818, the 8-byte aligned addresses, so the row says 204 bytes for a 200-byte body. No C
   emits that leading word.
   Measured: build.Placement.derive(), the comparison check.py runs, on this file compiled as the -g8 unit below
   returns no difference at row+4 in both levels, and "instruction at +0x0 differs" at +0 and +8.
   The linked build has not been run: it cannot place this C until the row is fixed.
   Repair, in tools/levels.py (not done here, workers may not edit it): add
       "func_PAL_L10_00428C8C": (204, 4),
   to FUNCTION_ENTRY_OFFSETS, then run python3 tools/carry.py --version pal --entries. That should give 200 bytes
   at 10:00428C90, 15:00435818 (not run).
   If it lands: unit src/pal/levels/shared/L10_00428C8C-g8.c, flags -O2 -G8 -ffunction-sections (the -g8 name gives
   them, no cflags.txt line is needed), and delete the INCLUDE_ASM line from L10_00428C8C.c, keeping the marker of
   func_PAL_L10_00428D58. Names resolve per program (level 10/level 15): D_L10_00221CD0 0x221D00/0x221E80,
   D_L10_002A3C70 0x2A39F0/0x29FB80, D_L10_001B13A4 0x1B13B4/0x1B1544, D_L10_001B13F0 0x1B1400/0x1B1590,
   D_L10_001B1440 0x1B1450/0x1B15DC. */
typedef struct GornConfig { int words[10]; } GornConfig;
extern char D_L10_00221CD0[];
extern GornConfig D_L10_002A3C70[];
extern int D_L10_001B13A4 __attribute__((sda));
extern char *D_L10_001B13F0 __attribute__((sda));
extern int D_L10_001B1440 __attribute__((sda));
extern void func_L05_0037B210(void);
extern void func_L05_00379F18(int);
extern int func_002ED8A0(int);
extern void func_L05_00379F28(int);
extern int func_L05_00442860(void);
extern void func_PAL_L05_0037FEB0(void);
extern void func_L05_00379F68(void);
extern void func_PAL_L05_0044BEB8(void);

/* Resets the mission's shared counters. */
void func_PAL_L10_00428C8C(void) {
    char *state;
    int i;
    func_L05_0037B210();
    func_L05_00379F18(1);
    state = D_L10_00221CD0;
    *(int *)(state + 0x28) = 100;
    *(int *)(state + 0x10) = 0;
    *(int *)(state + 0xC) = 0;
    D_L10_001B13A4 = 0;
    *(int *)(state + 0x14) = 0;
    *(int *)(state + 0x2C) = 1;
    i = func_002ED8A0(6);
    D_L10_001B1440 = 0;
    *(float *)(state + 0x30) = 8.0f;
    D_L10_002A3C70[i].words[5] = D_L10_002A3C70[i].words[4];
    func_L05_00379F28(0);
    if (func_L05_00442860()) {
        *(int *)(state + 0x2C) = 0;
        D_L10_001B13F0[0x20] = 1;
    }
    func_PAL_L05_0037FEB0();
    func_L05_00379F68();
    func_PAL_L05_0044BEB8();
}
