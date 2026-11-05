/*
 * func_L00_00308700: exact bytes in all 27 places and in PAL and v2.00 (v2617, commit c5b470bba), taken back out by
 * the user's choice on 2026-11-05. Its two one-pass groups exist only to set flow's reference weights, which
 * is not a block boundary. Needs a form without them. Below is what matched.
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
/* Finds or loads the cache entry for key, evicting the oldest on a miss, and moves it to the front.
   flow weights register references by loop-note depth: the two one-pass groups set that weight
   for the scan body and for the load-and-swap, which decides the saved registers and the swap's. */
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
            do {
                if (entry[1] < times[victim * 4]) {
                    victim = i;
                }
                ++i;
                entry += 4;
            } while (0);
        } while (i < 8 && *entry != key);
    }
    do {
        if (i == 8) {
            func_00297550(D_L00_001C93A0.e[victim].dst, key, D_L00_001C93A0.src, D_L00_001C93A0.offs);
            D_L00_001C93A0.e[victim].key = key;
            i = victim;
            miss = 1;
        }
        if (i != 0) {
            base = (u128_04340 *)&D_L00_001C93A0.e[0];
            other = base + i;
            temp = *other;
            *other = *base;
            *base = temp;
        }
    } while (0);
    D_L00_001C93A0.e[0].stamp = D_001B18D8;
    return miss;
}
