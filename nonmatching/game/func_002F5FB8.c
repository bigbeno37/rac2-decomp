/* 3 November 2026 (opus-1, v2273), USA v1.01: 1172/1184 bytes, 0/28 places, frame 0xD0 (retail
 * 0xD0); opcode-sequence ratio 0.975 against retail versus 0.915 for the previous draft, which
 * now compiles to 1176 bytes with frame 0xC0. Game -O2 -G8. No complete match.
 * In-file use: drop the func_002EF010 line (the file declares it) and keep the rest.
 * What this form fixes:
 *  - textureIndex = 2 is set before the wait loop, so cse2 cannot fold the hoisted
 *    textureIndex << 4 in the main-loop preheader; with the post code inside for (;;)
 *    (if (done) {...; break;}) loop.c hoists the shift, reload rematerializes textureIndex
 *    as li 2; sll 4 and spills the product, as retail does at 0x6C(sp).
 *  - (first + 63) & -64 inside the loop: gcse PRE hoists first+63/second+63 and loop.c
 *    hoists -64; the frame becomes 0xD0.
 *  - TOC accesses through a struct alias D_0014B540_1 (header, base, files[]): loop.c hoists
 *    TOC and TOC+12.
 * Remaining differences:
 *  - Retail spills destination (0x68) and gives $fp to TOC+12; here destination takes $fp.
 *    Retail slot order is destination, 2<<4, first+63, second+63; here first+63, second+63,
 *    2<<4.
 *  - Retail's sector address is off + (TOC+8), with TOC+8 rematerialized (lui/addiu -0x4AB8)
 *    each iteration; here combine folds it into addu a0,s4,off; lw 8(a0). The size address
 *    off + fp(TOC+12) and base lw 4(s4) agree. Cause (4 November 2026, x.i.loop): loop.c
 *    hoists (plus TOC 12) only because it is the last use of the TOC lo_sum ("forces 580");
 *    (plus TOC 8) has life 1, savings 1, "not desirable" (threshold * 1 * 1 < 135 insns), so it
 *    stays in the loop. Retail hoisted both (TOC+8 then spilled to its REG_EQUIV, which frees
 *    fp for TOC+12 and pushes destination to 0x68). Size-first or base+sector orders make TOC+8
 *    the last use instead and leave TOC+12 in the loop (1164 bytes); MACRO_ADDR alias 1168.
 *  - Prologue registers differ: retail has displays=s2, display=s1, a 0x3F70 constant in s3,
 *    first in s7. The retail wait loop has an extra dead nop.
 * Retail addresses: ref_00 = D_001B220C, ref_01 = 0x1B2208, ref_10 (descriptor) = 0x1B2210,
 * ref_12 = 0x1B2218, ref_14 = gp 0x321C, ref_13 = gp 0x3224, ref_08 = 0x1B1604.
 */
#include "common.h"
extern int func_002EF010(int);
extern void func_002FC1D8(void);
typedef unsigned long long U64_5FB8;
typedef struct { U64_5FB8 packet; unsigned short tex0, tex1; short width, height; } Display_5FB8;
typedef struct { int frame; short *frames; char pad[0x2C]; int field34, active, mode; } Stream_5FB8;
extern int ref_L00_002F5FB8_00, ref_L00_002F5FB8_01;
extern char ref_L00_002F5FB8_04[];
extern int ref_L00_002F5FB8_05[];
extern int ref_L00_002F5FB8_06[];
extern Display_5FB8 ref_L00_002F5FB8_07[];
extern char *ref_L00_002F5FB8_08;
extern int *ref_L00_002F5FB8_10;
extern int ref_L00_002F5FB8_12;
extern int ref_L00_002F5FB8_13 __attribute__((sda));
extern int ref_L00_002F5FB8_14 __attribute__((sda));
extern int ref_L00_002F5FB8_16[];
extern int D_001393E0[];
typedef struct { int sector; int size; } TocFile_5FB8;
typedef struct { int header; int base; TocFile_5FB8 files[0]; } Toc_5FB8;
extern Toc_5FB8 D_0014B540_1[];
extern char D_001339B0[];
extern void func_0029CC98(int, int);
extern void func_00288BB8(void *);
extern int func_002B4B70(int,int);
extern void func_002FC3C0(unsigned long);
extern int func_00126288(void *,short,short,short,short,short,short,short);
extern void func_0011AEA0(int);
extern int func_001265B0(void *,void *);
extern int func_001244B8(int,int);
extern void func_002E5FE0(void);
extern void func_002B7170(void);
extern int func_00133580(int);
extern void func_00298F00(void);
extern int func_002B7C80(char *,int,int);
extern int func_0034EF58(void *,void *,char *,char *,int,int);
extern int func_00288A00(void *,int *,int *);
extern void func_00126DC0(void *);
extern void func_002F0FA0(void);
extern int func_002B49E0(int,int,int,int,unsigned char *);
void func_002F5FB8(void)
{
    long packet[12] __attribute__((aligned(16)));
    long low, dimensions, high;
    long base;
    long highFlags;
    int textureIndex;
    int frame, status, done, result;
    int first, second, nextFrame;
    Display_5FB8 *displays;
    char *destination;
    Display_5FB8 *display;
    Stream_5FB8 *stream;
    if (ref_L00_002F5FB8_00 == 0) {
        func_0029CC98((int)ref_L00_002F5FB8_04,-1);
        func_00288BB8(ref_L00_002F5FB8_05);
        func_002B4B70(0,0);
        return;
    }
    second = ref_L00_002F5FB8_06[5];
    first = ref_L00_002F5FB8_06[3];
    destination = (char *)(second + 0xE0000);
    textureIndex = 2;
    displays = ref_L00_002F5FB8_07;
    display = &displays[textureIndex];
    func_002FC3C0(1);
    base = 0x3F70;
    func_00126288(packet,(short)base,(short)((1 << display->width) >> 6),0x13,0,0,(short)(1 << display->width),(short)(1 << display->height));
    func_0011AEA0(0);
    func_001265B0(packet,ref_L00_002F5FB8_08 + display->tex0 * 16);
    func_001244B8(0,0);
    func_00126288(packet,0x3F6C,1,0,0,0,16,16);
    func_0011AEA0(0);
    func_001265B0(packet,ref_L00_002F5FB8_08 + display->tex1 * 16);
    func_001244B8(0,0);
    highFlags = 0x7ED8400000000L;
    low = ((long)((1 << display->width) >> 6) << 14) | (long)base;
    dimensions = ((long)display->width << 26) | 0x1300000L;
    low = low | dimensions;
    low = low | ((long)display->height << 30);
    high = low | highFlags;
    ref_L00_002F5FB8_07[2].packet = high | 0x8000000000000000ULL;
    func_0011AEA0(0);
    func_002E5FE0();
    func_002B7170();
    func_00133580(0);
    while (D_001393E0[0x15C/4] >= 3 || D_001393E0[0x164/4] >= 0) func_00298F00();
    frame = 0;
    status = -1;
    done = 0;

    for (;;) {
        int *descriptor;
        descriptor = ref_L00_002F5FB8_10;
        if (descriptor != 0 && descriptor[0] != 0) {
            func_002B7C80(destination,descriptor[0] + D_0014B540_1[0].base,(descriptor[1] + 2047)/2048);
            ref_L00_002F5FB8_16[0x34/4] = 0;
            ref_L00_002F5FB8_16[0x4C/4] = (int)destination;
        } else {
            ref_L00_002F5FB8_16[0x4C/4] = 0;
            ref_L00_002F5FB8_16[0x34/4] = 0;
        }
        stream = (Stream_5FB8 *)ref_L00_002F5FB8_05;
        result = func_0034EF58((void *)ref_L00_002F5FB8_01,(void *)ref_L00_002F5FB8_00,(char *)((first + 63) & -64),(char *)((second + 63) & -64),ref_L00_002F5FB8_13,ref_L00_002F5FB8_12 != 0);
        stream->mode = (result ^ 1) == 0 ? 1 : stream->mode;
        if (result != 0) { stream->mode = 1; func_00288BB8(stream); }
        if (ref_L00_002F5FB8_05[0] != 0) {
            ref_L00_002F5FB8_05[0]++;
            if (ref_L00_002F5FB8_05[0] >= 25) ref_L00_002F5FB8_05[0] = 1;
            nextFrame = stream->frames[ref_L00_002F5FB8_05[0]];
        } else {
            if (stream->active == 0) { done = 1; goto end_frame; }
            func_00288A00(stream,&frame,&status);
            if (status != 0) goto bad_status;
            nextFrame = frame;
        }
        {
            int index = func_002EF010(nextFrame);
            ref_L00_002F5FB8_01 = D_0014B540_1[0].files[index * 2 + 1].sector + D_0014B540_1[0].base;
            ref_L00_002F5FB8_14 = D_0014B540_1[0].files[index * 2 + 1].size;
        }
        goto end_frame;
bad_status:
        if (status == 1) done = 1;
end_frame:
        if (done) {
            func_00133580(0);
            func_00126DC0(D_001339B0);
            func_001244B8(0,0);
            display = &ref_L00_002F5FB8_07[textureIndex];
            func_001265B0(packet,ref_L00_002F5FB8_08 + display->tex1 * 16);
            func_001244B8(0,0);
            func_002F0FA0();
            display->packet = 0;
            func_002FC1D8();
            break;
        }
    }
    if (result == 0 && status == 1) func_002B49E0(2,2,frame,0,0);
    else func_002B4B70(0,0);
}
