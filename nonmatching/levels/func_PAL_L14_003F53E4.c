/*
 * func_PAL_L14_003F53E4, the PAL copy of func_L14_003F0D50: the same C, where both scroll
 * speeds are scaled by 1.2f for 50 Hz. Tier: Opus 5.5 (task v2907).
 *
 * Best check.py: 296 bytes, expected 300, 0 of 1 places. This is a catalogue defect, not a C
 * difference. The row starts on an alignment nop at 0x3F53E4: func_L00_003E39B8 ends there with
 * its completed return (jr $ra; addiu $sp,0x10), and the real entry is the 8-aligned 0x3F53E8.
 * build.Placement.derive() on this object at 0x3F53E8 returns OK, for all 296 bytes.
 *
 * What unblocks it: add "func_PAL_L14_003F53E4": (300, 4) to FUNCTION_ENTRY_OFFSETS in
 * tools/levels.py and rerun the PAL catalogue, as was done for func_PAL_L01_0039A174. Then this C
 * goes in place of its marker in src/pal/levels/14_aranos_prison/L14_003E96D8.c, with the default
 * flags (the template unit's). The declarations it needs are below.
 */
extern void func_L00_002E54E0(float);
extern int func_002B38B8(char *);
extern void func_L00_002E6EF8(void (*f)(char *), char *arg);
extern float func_002A7878(float, float);
extern void func_L14_003F0E68(char *);

void func_PAL_L14_003F53E4(char *arg0) {
    char *s2 = arg0;
    char *s1 = *(char **)(arg0 + 0x68);

    if (*(unsigned char *)(arg0 + 0x20) == 0) {
        *(int *)(s1 + 0x10) = 1;
        func_L00_002E54E0(2048.0f);
        *(unsigned char *)(arg0 + 0x20) = 1;
    }
    *(float *)(s1 + 0x0) = *(float *)(s1 + 0x0) + *(float *)(s1 + 0x4) * 1.2f;
    *(float *)(s1 + 0x8) = *(float *)(s1 + 0x8) + *(float *)(s1 + 0xC) * 1.2f;
    if (*(float *)(s1 + 0x0) >= 8.0f) {
        *(float *)(s1 + 0x0) = *(float *)(s1 + 0x0) - 8.0f;
    }
    if (*(float *)(s1 + 0x8) >= 8.0f) {
        *(float *)(s1 + 0x8) = *(float *)(s1 + 0x8) - 8.0f;
    }
    if (func_002B38B8(s2) != 0) {
        *(int *)(s1 + 0x10) = 0;
    }
    if (*(int *)(s1 + 0x10) != 0) {
        func_L00_002E6EF8(func_L14_003F0E68, s2);
    }
    *(float *)(s1 + 0x4) = func_002A7878(0.003f, 0.005f);
    *(float *)(s1 + 0xC) = func_002A7878(0.001f, 0.0025f);
}
