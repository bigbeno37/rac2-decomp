/*
 * func_L00_003B0DA0 - tier opus-1 (task v2307), NONMATCHING.
 * Best check.py result: 1236/1232 bytes, 0/27 places (us).
 * Build: needs $gp-relative sda globals, so it belongs in
 * src/levels/shared/L00_003A8E28-g8.c (-G8) after that file's existing
 * declarations (D_L00_0023BB78, D_L00_001AF810/811 sda, D_L00_001B19C8
 * MACRO_ADDR, HeroL00 D_00189E20, func_L00_003B0CE8, and the existing
 * prototype "extern char *func_L00_003B0DA0(void *);"), with the INCLUDE_ASM
 * line in L00_003A8E28.c removed. The declarations below go with it;
 * D_L00_0023BB78_1 is a _1 alias of the node array (value taken from the
 * retail bytes) so the nodes are a real struct array.
 *
 * What matches: frame 0xB0 with the count spilled at sp+0x30; per-field node
 * address forms (base+8, base+0x14, in-loop base+0xC); merge/already inner
 * loops strength-reduced to a pointer with a reversed counter; pair loops with
 * i+1 at sp+0x34 and the centroid loop shape.
 * What still differs:
 * - first block: jump2 gives "bc1t epilogue; v0=0" where retail has
 *   "bc1f; lbu a0" followed by "b epilogue; v0=0";
 * - link fill loop and cursor loop: register choices swapped
 *   (pointer/value v1/v0, s3/s4, a0/a1);
 * - already loop: D_810 is kept in a register and re-masked (andi) instead of
 *   being reloaded in each entry's preheader;
 * - found loop: retail is the rotated for-loop shape ("b body"; a++/p++; a<8
 *   exit; links test; found falls through), with D_810 hoisted into a1. A plain
 *   for+break gets its exit test duplicated by jump.c (LOOP_TEST_THRESHOLD 30),
 *   the copied found block jumps into the loop, and loop.c rejects it
 *   ("multiple entry points"). The do-while below is strength-reduced but
 *   body-first (Cygnus expand_end_loop keeps do-while unrotated) and leaves
 *   the found block outside the loop.
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
    int count = 0, i, j, k, n;
    float x, y, z, divisor;

    if (D_L00_001AF811 != D_L00_001AF810) {
        TrailNode *last = &NODES[D_L00_001AF810 - 1];
        previous[0] = last->xyz[0];
        previous[1] = last->xyz[1];
        previous[2] = last->xyz[2];
        if (func_00282E48(D_00189E20.position, previous) < D_L00_001AF81C) return 0;
    }
    node = &NODES[D_L00_001AF810];
    node->xyz[0] = D_00189E20.position[0];
    node->xyz[1] = D_00189E20.position[1];
    node->xyz[2] = D_00189E20.position[2];
    node->used = D_L00_001B19C8;
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
            already = 0;
            for (a = 0; a < 8; ++a) {
                entry = NODES[cursor].links[a];
                if (entry == cursor) continue;
                for (b = 0; b < 8; ++b)
                    if (NODES[entry].links[b] == D_L00_001AF810) already = 1;
            }
            if (!already) {
                a = 0;
                do {
                    if (NODES[cursor].links[a] == cursor) {
                        NODES[cursor].links[a] = D_L00_001AF810;
                        joined[count++] = cursor;
                        break;
                    }
                } while (++a < 8);
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
                x = y = z = 0.0f;
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
