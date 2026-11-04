/* Tier: opus-2 (Claude Opus 5.5). FAILED: 256/260 bytes; us 0/27, pal 0/27, us2 0/27.
 * Profile: -O2 -G8 -ffunction-sections (put it in a sibling L00_0032C638-g8.c; the plain
 * L00_0032C638.c unit is not -G8 and gives 248 bytes with no $gp store).
 * Everything up to +0xC4 matches (the nops after the loop-entry andi/lb are .p2align 3 pads on
 * the rotated loop labels). One local `v` shared by the state byte (loop 1) and the callback
 * (loop 2) makes global alloc put the callback in $v1 as retail does: the shared pseudo inherits
 * the loop-1 conflict with $v0. A single-pointer `update` local gives $v0.
 * Still differs: retail `beqz v1; nop; daddu a0,s0; jalr v1; nop`; ours `beql v1,zero; lhu v0,0x34(s0);
 * jalr v1; daddu a0,s0`. A volatile read of the 0x34 flags in the test after the callback (forbidden
 * as a chosen access) empties the beqz slot, which leaves only the jalr slot: 29/65 rows differ,
 * 256/260 bytes. Reorg's backward scan always moves `move a0,s0` into the jalr slot (eligible:
 * length 1, dslot no); retail needs it blocked (a label before the call, or a volatile resource in
 * the call's needed set). Tried with the same bytes: unprototyped callback type.
 */
extern int D_L00_001FACE0 __attribute__((nosda));
extern char *D_L00_001B1EA8;
extern char *D_L00_001B1EA8_1 __attribute__((sda));
extern char *D_L00_001B1E9C;
extern void func_002A05A8(void *object);
extern void func_002A0F48(void *object);
void func_L00_0032CF60(void)
{
    char *scan, *object, *last = 0;
    unsigned int v;
    D_L00_001FACE0 = 0;
    D_L00_001B1EA8 = 0;
    for (scan = D_L00_001B1E9C; (v = *(unsigned char *)(scan + 0x20)) != 255; scan += 0x100) {
        if ((v & 0x80) == 0 &&
            (*(unsigned short *)(scan + 0x34) & 2) == 0) {
            if (last != 0)
                *(char **)(last + 0x28) = scan;
            else
                D_L00_001B1EA8_1 = scan;
            last = scan;
        }
    }
    if (last != 0)
        *(char **)(last + 0x28) = 0;
    object = D_L00_001B1EA8;
    while (object != 0) {
        if (*(signed char *)(object + 0x20) >= 0) {
            if ((*(unsigned short *)(object + 0x34) & 0x40) == 0)
                func_002A05A8(object);
            v = *(unsigned int *)(object + 0x64);
            if (v != 0)
                ((void (*)(void *))v)(object);
            if ((*(unsigned short *)(object + 0x34) & 4) == 0)
                func_002A0F48(object);
        }
        object = *(char **)(object + 0x28);
    }
}
