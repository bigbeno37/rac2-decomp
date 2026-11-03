/* Opus 5.5 xhigh, task v2382; USA v1.01 game -O2 -G0 -ffunction-sections, 28 places.
 * Best: 448/448 bytes, 28 of 28 places on USA v1.01: this C is exact there. PAL and v2.00: 0 of 28.
 * What still differs: both releases have o + 0x880 for o + 0x870 and o + 0xA78 for o + 0xA18 (all other
 * rows equal; checked with check.py --diff). That is a moved field, not a wording: the toolchain has no
 * per-release macro, so one C definition cannot give both offsets.
 * What would unblock it: tools/carry.py --demote for pal and us2 after their builds, which gives this
 * function the release names, as func_00340498 already has (func_PAL_00342518, func_US2_003413A8). Then
 * this C goes back in src/game/0033F030.c in place of the marker, and each release's copy takes this
 * body with its two offsets in src/<version>/.
 * What fixed the old 20 differences: the sibling descriptor order (func_00344F98, func_003463C8), f[14]
 * right after f[9], and the post-call stores f[10], f[12], f[11], f[15], f[18], f[8], f[7], f[6], f[5].
 * That moves the q3 store later in sched1, so child ranks above the q3 address in local-alloc.
 */
typedef struct { unsigned int lo; unsigned int hi; } Word2_340498;
extern const Word2_340498 D_001AE090;
extern const Word2_340498 D_001AE098;
extern const Word2_340498 D_001AE0A0;
extern const Word2_340498 D_001AE0A8;
extern const Word2_340498 D_001AE0B0;
extern char D_00259CE0[];
extern char *D_001A8CFC __attribute__((sda));
extern void func_00336390(void *, void *, int);
extern void func_00337210(int *);

static __inline__ void initDescriptor_340498(int *f) { f[20] = 0; f[21] = 1; }

void func_00340638(char *o)
{
    Word2_340498 q0 = D_001AE098;
    Word2_340498 q1 = D_001AE0A0;
    Word2_340498 q2 = D_001AE098;
    Word2_340498 q3 = D_001AE0A8;
    Word2_340498 q4 = D_001AE090;
    Word2_340498 q5 = D_001AE0B0;
    int f[23];
    char *child;

    initDescriptor_340498(f);
    f[19] = *(int *)(o + 0x820);
    f[21] = *(int *)(o + 0x81C) == 1;
    f[3] = *(int *)(o + 0x814);
    f[4] = *(int *)(o + 0x818);
    f[16] = (int)(o + 0xA18);
    f[9] = (int)&q4;
    f[14] = 1;
    f[1] = 3;
    f[2] = 2;
    f[0] = (int)D_00259CE0;
    f[17] = (int)(o + 0x870);
    f[13] = 0;
    child = o + 0x17C;
    func_00336390(child, D_001A8CFC + 0x8710, 0x5D);
    f[10] = (int)child;
    f[12] = (int)(o + 0x214);
    f[11] = 0;
    f[15] = (int)(o + 0x7D0);
    f[18] = *(int *)(o + 0x80C);
    f[8] = (int)&q3;
    f[7] = (int)&q2;
    f[6] = (int)&q1;
    f[5] = (int)&q0;
    f[20] = (int)&q5;
    f[22] = 0;
    func_00337210(f);
}
