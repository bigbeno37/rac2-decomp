/* func_002F97A0: near miss, 1052/1052 bytes, boot (USA v1.01, 1 November 2026; revised 3 November).

   Profile: its own `-g8` unit, `-O2 -G8 -ffunction-sections` (named 002F5220-fade-g8.c).
   The integer sibling func_0027F2A8 (0027F128-font-g8.c) gives the glyph walk and the grey
   shadow; the fade is double arithmetic, so it goes through the soft-float helpers
   func_001234F0, func_00123028, func_00122A98, func_00122B00 and func_00123130. The sprite call takes its four floats first, like the
   integer func_0027E018, and the shop-state pointer is a local so reload rematerialises the
   `la` pair at its one use. In src/game/002F5220.c the shop type is Shop_002F9198.

   Everything matches except one scheduling window after the two fades (0x2F999C..0x2F9A88):
   retail clamps left then right and truncates the colour (dsll32/dsra32) just before the
   first `or`, storing colors[3], [0], [1], [2]. Clamping right first and shifting left and
   right in place (this version) gets closer than the 1 November draft.
   Findings: retail's clamp block order needs right's `subu` in the same sched1 block as the
   clamps, so `right = full - fade;` after the if/else join with a per-arm `full = 0x80`
   gives retail's order and the per-arm $s3; but then arm 1's `jal dptoli` tail is not
   cross-jumped (flow puts a USE after arm 2's block-ending call, jump.c find_cross_jump
   stops there), and local-alloc gives right=v1/-1=v0 where retail has right=a0, -1=v1,
   the left slt and the colour in v0 (retail's sched1 placed the truncation after movz L
   and the lbu after slt R). Copying the clamp/pack/draw tail into both arms grows to 1324
   bytes. Earlier tries: the colour masked inside the expressions, a u32/int/explicit-cast
   temporary, chained assignments, ternary clamps and every order of the four stores. */

#include "common.h"

/* One font glyph: its texture cell, its top offset and its advance. */
typedef struct { u8 u, v; signed char top, adv; } Glyph_002F97A0;

extern Glyph_002F97A0 D_00263EF0[];
typedef struct { char pad[0x50]; int mode; } Shop_002F97A0;

extern Shop_002F97A0 D_0021F240;
extern u64 func_0027C878(int);
extern float func_00283CE0(int);
extern void func_0027E1A0(float, float, float, float, int, int, int, int, u32 *, u64);

/* Prints up to `limit` characters of `text` from (x, y) at `scale`, in the large font, fading
   each glyph's left and right edges out with their distance from the screen centre (215 on
   the second screen layout, 256 elsewhere, three quarters as fast). Characters 0x80..0xa7
   draw their entry 0x40 later; control characters below 0x20 draw a grey 24-pixel sprite. */
void func_002F97A0(u64 color, u8 *text, int limit, float x, float y, float scale)
{
    u32 colors[4];
    u32 gray[4];
    Glyph_002F97A0 *glyphs;
    Shop_002F97A0 *shop = &D_0021F240;
    Glyph_002F97A0 *g;
    u64 texture;
    int count = 0;
    float size;
    float top;
    float dx;
    float dy;
    double d;
    int left;
    int right;
    int shade;
    int tint;
    u8 character;
    u8 *cursor;
    u32 c;

    texture = func_0027C878(2);
    size = scale * 16.0f;
    glyphs = D_00263EF0;
    color &= 0xFFFFFF;
    if (limit == 0 || *text == 0)
        return;
    cursor = text;
    do {
        if (glyphs[*cursor].adv != 0) {
            top = func_00283CE0(glyphs[*cursor].top) * scale;
            if (shop->mode == 2) {
                d = 215.0f - x;
                if (d < 0)
                    d = -d;
                left = 0x80 - (int)d;
                d = 215.0f - (x + size);
                if (d < 0)
                    d = -d;
                right = 0x80 - (int)d;
            } else {
                d = 256.0f - x;
                if (d < 0)
                    d = -d;
                left = 0x80 - (int)(d * 0.75);
                d = 256.0f - (x + size);
                if (d < 0)
                    d = -d;
                right = 0x80 - (int)(d * 0.75);
            }
            if (right < 0)
                right = 0;
            if (left < 0)
                left = 0;
            c = color;
            left <<= 24;
            right <<= 24;
            colors[0] = left | c;
            colors[1] = right | c;
            colors[2] = left | c;
            colors[3] = right | c;
            if (*cursor < 0x20) {
                c = colors[0];
                shade = (int)((c & 0xFF) + ((c >> 8) & 0xFF) + ((c >> 16) & 0xFF)) / 3;
                tint = (int)(c & 0xFF000000);
                tint += shade << 16;
                tint += shade << 8;
                shade += tint;
                gray[0] = shade;
                gray[1] = shade;
                gray[2] = shade;
                gray[3] = shade;
                func_0027E1A0(x, y + top, scale * 24.0f, scale * 16.0f, glyphs[*cursor].u,
                              glyphs[*cursor].v, 24, 16, gray, texture);
            }
            if ((u8)(*cursor + 0x80) < 0x28) {
                g = (Glyph_002F97A0 *)((((u8)*cursor + 0x40) << 2) + (int)glyphs);
                dx = func_00283CE0(g->adv) * scale;
                dy = func_00283CE0(g->top) * scale;
                func_0027E1A0(x + dx, y + dy, size, size, g->u, g->v, 16, 16, colors, texture);
            } else if ((u8)*cursor > 0x20) {
                func_0027E1A0(x, y + top, size, size, glyphs[*cursor].u, glyphs[*cursor].v, 16, 16,
                              colors, texture);
            }
            x += func_00283CE0(glyphs[*cursor].adv) * scale;
        }
        count++;
        if (count == limit)
            break;
        cursor++;
    } while (*cursor != 0);
}
