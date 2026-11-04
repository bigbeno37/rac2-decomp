/* Claude Opus 5.5 (opus-2, xhigh), v2411, 2026-11-04: 288/288 bytes, 0/27 places
 * (default -O2 -G0 -ffunction-sections, in place of the marker in
 * src/levels/shared/L00_002FC7A0.c). Counterpart: RAC1 ensure_menu_cache_entry
 * (rac1-decomp src/overlays/shared/map_menu_filters.c), same layout and code.
 *
 * EXACT-BYTES FORM (27/27 US places, measured here): RAC1's matched body verbatim,
 * i.e. the scan written as `i = 0; if (e[0].key != key) { times = &e[0].stamp;
 * entry = &e[0].key; do { do { if (entry[1] < times[victim * 4]) victim = i; ++i;
 * entry += 4; } while (i < 8 && *entry != key); } while (0); }` and
 * `miss++; miss--;` between `temp = *other;` and `*other = *base;`. Both tricks are
 * needed (either alone: 16 rows differ). They are not allowed here (do-while(0)
 * wrapper; a no-op pair), so this draft keeps the natural form below.
 *
 * Natural form below: prologue, scan loop, miss block, miss (t0) and times (a3)
 * all agree; only the swap block's registers differ (retail: other ptr a0,
 * *base v0, *other v1, base a1; here other v0, *base v1, *other a0).
 * Why (local-alloc, block of the swap): qtys hi/base 0.857, pointer (sll tied to
 * addu) 1.0, both TI values 1.0; the pointer wins the 1.0 tie by creation order
 * and takes v0. RAC1's leftover `(set miss miss)` sits between lo and addu and
 * stretches the pointer to 0.833 and base to 0.75, giving retail's order. Needs one
 * more RTL insn inside the pointer's life (not the TI values') that vanishes later,
 * without extra refs on miss (miss++/miss-- alone flips miss/times to a3/t0).
 * Tied (same bytes): struct pointers, a static inline swap helper, a
 * D_L00_001C93D0[] quad-array view; an aligned(16) struct copy goes via the stack.
 * Declarations below.
 */
typedef unsigned int MenuCacheQuad __attribute__((mode(TI), aligned(16)));
typedef struct {
    int key;
    int stamp;
    unsigned char *dst;
    int pad;
} CacheEnt_308700;
typedef struct {
    unsigned char *src;
    short *offs;
    int pad0[8];
    int active;
    int pad1;
    CacheEnt_308700 e[8];
} Cache_308700;
extern void func_00297550(unsigned char *dst, int row, unsigned char *src, short *offs);
extern Cache_308700 D_L00_001C93A0;
extern int D_001B18D8 __attribute__((section(".sdata")));
int func_L00_00308700(int key) {
    int i;
    int victim;
    int miss;
    MenuCacheQuad *base;
    MenuCacheQuad *other;
    MenuCacheQuad temp;

    if (!D_L00_001C93A0.active) {
        return 0;
    }
    miss = 0;
    victim = 0;
    for (i = 0; i < 8 && D_L00_001C93A0.e[i].key != key; i++) {
        if (D_L00_001C93A0.e[i].stamp < D_L00_001C93A0.e[victim].stamp) {
            victim = i;
        }
    }
    if (i == 8) {
        func_00297550(D_L00_001C93A0.e[victim].dst, key, D_L00_001C93A0.src, D_L00_001C93A0.offs);
        D_L00_001C93A0.e[victim].key = key;
        i = victim;
        miss = 1;
    }
    if (i != 0) {
        base = (MenuCacheQuad *)&D_L00_001C93A0.e[0];
        other = base + i;
        temp = *other;
        *other = *base;
        *base = temp;
    }
    D_L00_001C93A0.e[0].stamp = D_001B18D8;
    return miss;
}
