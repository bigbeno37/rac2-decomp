/*
 * func_L00_00308700 (opus-2, v2629): FAILED under the rules, though the body below is exact: 288/288 bytes, 27 of 27
 * places in USA v1.01, PAL and v2.00 with no `while (0)` group. It is not landable: it enters a do/while by goto, the
 * loop test is always false, and `i = 0` exists only to end the loop, so it is the rejected one-pass group in another
 * spelling. It keeps flow's weights: the swap at depth 2, the load arm and the scan as in plain C.
 *
 * Best clean form: the same scan, `if (i == 8) { ... }`, then a plain `if (i != 0) { ...swap... }` with no loop: 288/288
 * bytes, 0/27 places, 8 instructions differ, all registers in the swap (ours: other v0, *base v1, temp a0; retail:
 * other a0, *base v0, temp v1; base is a1 in both). Everything before the swap is already exact, including all five
 * saved registers, so v2617's group around the scan body is not needed once the arm stays out of the loop.
 *
 * Why the clean form cannot match with these 8 RTL insns (lui, sll, addiu, addu, 2 lq, 2 sq in the swap block):
 * local-alloc priority is floor_log2(refs)*refs*size / (2*distance in insns). The two quads are at best 4/4 = 1.0,
 * the pointer (sll+addu tied, 5 refs) 10/(2*d), base (lui+addiu tied, 6 refs) 12/(2*d). Retail needs
 * quads > pointer > base, so d(pointer) >= 6 and d(base) >= 8, which takes 9 insns. Refs here come from
 * recompute_reg_usage just before local-alloc, so refs that combine removes earlier do not help.
 *
 * So retail has either a ninth insn that emits nothing or a higher loop depth. A ninth insn that ties to the pointer or
 * base raises their refs and makes it worse (computed, not compiled: 7 refs 14/12, 8 refs 24/16); it must sit on a third
 * pseudo, as RAC1's
 * `miss++; miss--;` does. Loop depth has a limit too: final aligns the first label after any NOTE_INSN_LOOP_BEG, and the
 * tail label at 0x3087EC is not 8-aligned, so a loop opening after the arm's join at 0x3087C8 adds a nop. Measured for
 * while, for, do/while and while+break around the swap. The loop must open before that join label, and a loop around
 * the arm too flips key/victim for $s2/$s3 and miss/times for $a3/$t0 (key 24/37 over victim 33/58, miss 8/66 over
 * times 3/26), which is why v2617 needed its second group. Only a goto keeps the arm out and the join label in.
 *
 * Ties: an inline swap helper, read/write helpers and a helper holding the whole block all give the clean form's
 * bytes. for(;;) with a break grows the function by three words with a goto, and adds saved registers with the arm
 * inside. The earlier two-group form is in commit c5b470bba. Needs the declarations below. Bytes depend on this cc1;
 * recompile before trusting them.
 */
typedef struct {
    int key;
    int stamp;
    unsigned char *dst;
    int pad;
} MenuCacheEntry_308700;
typedef struct {
    unsigned char *src;
    short *offs;
    int pad0[8];
    int active;
    int pad1;
    MenuCacheEntry_308700 e[8];
} MenuCache_308700;
extern MenuCache_308700 D_L00_001C93A0;
extern int D_001B18D8 MACRO_ADDR;
extern void func_00297550(unsigned char *dst, int row, unsigned char *src, short *offs);
/* rac1-decomp src/overlays/shared/map_menu_filters.c: ensure_menu_cache_entry */
/* Finds or loads the cache entry for key, evicting the oldest on a miss, and moves it to the front. */
int func_L00_00308700(int key) {
    int i;
    int victim;
    int miss;
    int *entry;
    int *times;
    u128_04340 *base;
    u128_04340 *other;
    u128_04340 temp;

    if (!D_L00_001C93A0.active) {
        return 0;
    }
    miss = 0;
    victim = 0;
    i = 0;
    if (D_L00_001C93A0.e[0].key != key) {
        times = &D_L00_001C93A0.e[0].stamp;
        entry = &D_L00_001C93A0.e[0].key;
        do {
            if (entry[1] < times[victim * 4]) {
                victim = i;
            }
            ++i;
            entry += 4;
        } while (i < 8 && *entry != key);
    }
    if (i != 8) {
        goto found;
    }
    func_00297550(D_L00_001C93A0.e[victim].dst, key, D_L00_001C93A0.src, D_L00_001C93A0.offs);
    D_L00_001C93A0.e[victim].key = key;
    i = victim;
    miss = 1;
    do {
found:
        if (i != 0) {
            base = (u128_04340 *)&D_L00_001C93A0.e[0];
            other = base + i;
            temp = *other;
            *other = *base;
            *base = temp;
            i = 0;
        }
    } while (i != 0);
    D_L00_001C93A0.e[0].stamp = D_001B18D8;
    return miss;
}
