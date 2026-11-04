/*
 * func_L00_003B0DA0 - tier opus-2 (task v2526), NONMATCHING.
 * Best check.py result: 1236/1232 bytes, 0/27 places (us); about 27 of 308
 * instructions differ after alignment, nearly all register choices in the
 * cursor loop (opus-1's draft differed almost everywhere after the first block).
 * Build: needs $gp-relative sda globals, so it belongs at the end of
 * src/levels/shared/L00_003A8E28-g8.c (-G8), after that file's declarations
 * (D_L00_0023BB78, D_L00_001AF810/811 sda, D_L00_001B19C8 MACRO_ADDR, HeroL00
 * D_00189E20, func_L00_003B0CE8, and the prototype
 * "extern char *func_L00_003B0DA0(void *);"), with the INCLUDE_ASM line in
 * L00_003A8E28.c removed. D_L00_0023BB78_1 is a _1 alias of the node array.
 *
 * Solved here (keep these):
 * - found loop: a source pointer plus plain for+break. expand_end_loop rolls the
 *   exit test through the break, jump.c copies that test before the loop, cse1
 *   folds a<8, and jump2 cross-jumps the copy into "b body". loop.c rejects the
 *   loop (multiple entries), so p++ must be in the source, and its own counter
 *   (not a) and "head" must be in the source too.
 * - node block: storing "used" BEFORE xyz makes the whole block match and also
 *   gives the first block's bc1f/lbu/b/v0=0 layout. Observed, not traced: I never
 *   found why this store order changes the bc1f slot fill.
 * - z = x = y = 0.0f gives retail's centroid registers; the block-local "linked"
 *   gives retail's a-loop entry in v0.
 *
 * Ruled out by test: volatile count (sw leaves the beq slot); count as array,
 * stack scalar, inline out-param or *&count (second lw stays; the scalar forms
 * also lose the loop-top lw); int slot = count; slot = count++ (breaks the
 * cross-jump); store order in the found arm (tie); int stride local (own s7);
 * reusing n as row (copy still vanishes). PRE cannot place the first-block lbu:
 * ISOOUT(bb4)=1 and EARLYOUT(bb1)=1 in the block LCM equations.
 *
 * What still differs, each needing a new idea:
 * 1. a-loop: retail copies the cursor product with "daddu a0,a1,zero" before the
 *    loop, keeping it in a1 and giving a-loop locals a1/a2/a3/t1. NODES[cursor]
 *    gives a second mult; "row" (below) removes it, but gcse cprop folds row into
 *    the product ("COPY-PROP: Replacing reg"), so product and row share t1.
 *    Inferred, not shown: a copy made after cprop (by cse2) would need mult's 28
 *    in r157's pseudo, but loop.c hoists the a-loop's 28 first and pass 2 logs it
 *    "not desirable" (loop of 138 insns).
 * 2. found arm: retail is lw a0; ...; addiu a0,a0,1; sw a0. Here reload does not
 *    inherit count's a0 at count++ and emits a second lw (the one extra word).
 *    choose_reload_regs in reload1.c is the place to look; the cause is unproven.
 * 3. first block: retail lui a0/addiu a2 vs lui a2/addiu a3. The lreg dump has
 *    hero's %hi before the mult and the PRE copy to reg 397 last, so reg 108
 *    overlaps hero's %hi; probably why a0 is taken, not confirmed against retail.
 * 4. one nop after the first div.s; not in x.s, so gas adds it (cause unknown).
 *    The nop after lw v1 is .p2align 3 before the i loop and returns once 2 goes.
 */
typedef struct { float xyz[3]; int used; int particle; u8 links[8]; } TrailNode;
extern TrailNode D_L00_0023BB78_1[];
#define NODES D_L00_0023BB78_1
extern u8 D_L00_0023BA78[];
extern u8 D_L00_001AF818 __attribute__((sda));
extern float D_L00_001AF81C __attribute__((sda));
extern float D_L00_001AF820 __attribute__((sda));
extern int D_L00_001AF824 __attribute__((sda));
extern float D_L00_001AF828 __attribute__((sda));
extern float func_00282E48(void *, void *);
extern void func_L00_003B0AB0(int);
extern int func_L00_003B0B20(unsigned char, unsigned char, unsigned char *);
extern void func_L00_003B14D0(float, float, float);
char *func_L00_003B0DA0(void *unused)
{
    u8 joined[32];
    float previous[4];
    TrailNode *node;
    u8 cursor;
    int count = 0;
    int i, j, k, n;
    float x, y, z, divisor;

    if (D_L00_001AF811 != D_L00_001AF810) {
        TrailNode *last = &NODES[D_L00_001AF810 - 1];
        float gap;
        previous[0] = last->xyz[0];
        previous[1] = last->xyz[1];
        previous[2] = last->xyz[2];
        gap = func_00282E48(D_00189E20.position, previous);
        if (gap < D_L00_001AF81C) return 0;
    }
    node = &NODES[D_L00_001AF810];
    node->used = D_L00_001B19C8;
    node->xyz[0] = D_00189E20.position[0];
    node->xyz[1] = D_00189E20.position[1];
    node->xyz[2] = D_00189E20.position[2];
    {
        int l;
        u8 *link = &node->links[7];
        for (l = 7; l >= 0; --l) *link-- = D_L00_001AF810;
    }
    for (cursor = D_L00_001AF810 - 1; cursor != (u8)(D_L00_001AF811 - 1); --cursor) {
        float dx, dy, dz, distance;
        u8 entry;
        int a, b, already;

        if (!NODES[cursor].used) continue;
        dx = node->xyz[0] - NODES[cursor].xyz[0];
        dy = node->xyz[1] - NODES[cursor].xyz[1];
        dz = node->xyz[2] - NODES[cursor].xyz[2];
        distance = dx * dx + dy * dy + dz * dz;
        if (distance < D_L00_001AF828 * D_L00_001AF828) {
            for (entry = D_L00_001AF811; entry != D_L00_001AF810; ++entry) {
                if (entry == cursor) continue;
                for (b = 0; b < 8; ++b)
                    if (NODES[entry].links[b] == cursor)
                        NODES[entry].links[b] = D_L00_001AF810;
            }
            func_L00_003B0AB0(cursor);
            func_L00_003B0CE8(cursor);
        } else if (distance < D_L00_001AF820) {
            int row = cursor * sizeof(TrailNode);
            already = 0;
            for (a = 0; a < 8; ++a) {
                u8 linked = NODES[0].links[a + row];
                if (linked == cursor) continue;
                for (b = 0; b < 8; ++b)
                    if (NODES[linked].links[b] == D_L00_001AF810) already = 1;
            }
            if (!already) {
                u8 *p = NODES[cursor].links;
                u8 head;
                int free = 0;
                head = D_L00_001AF810;
                for (; free < 8; free++, p++) {
                    if (*p == cursor) {
                        *p = head;
                        joined[count++] = cursor;
                        break;
                    }
                }
            }
        }
    }
    if (++D_L00_001AF810 == D_L00_001AF811) func_L00_003B0CE8(D_L00_001AF811);
    for (i = 0; i < count; ++i) {
        for (j = i + 1; j < count; ++j) {
            n = func_L00_003B0B20(joined[j], joined[i], D_L00_0023BA78);
            D_L00_0023BA78[n++] = D_L00_001AF810 - 1;
            if (n > D_L00_001AF824) {
                D_L00_001AF818 = n;
                z = x = y = 0.0f;
                divisor = n;
                for (k = 0; k < n; ++k) {
                    func_L00_003B0AB0(D_L00_0023BA78[k]);
                    x += NODES[D_L00_0023BA78[k]].xyz[0];
                    y += NODES[D_L00_0023BA78[k]].xyz[1];
                    z += NODES[D_L00_0023BA78[k]].xyz[2];
                    func_L00_003B0CE8(D_L00_0023BA78[k]);
                }
                func_L00_003B14D0(x / divisor, y / divisor, z / divisor);
            }
        }
    }
    return (char *)&NODES[D_L00_001AF810 - 1];
}
