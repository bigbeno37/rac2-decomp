/*
 * func_00300AA8: matched only with a forbidden method (per-access volatile cast); AGENTS.md rules: not landed
 */
void func_00300AA8(char *object, float x, float y, int flags, float frame, float *transform)
{
    NativeDrawHeaderView *header=(NativeDrawHeaderView *)object;
    float matrix[16] __attribute__((aligned(16)));
    NativeCompactRow16 *texture;
    TextureUpload *upload;
    ScratchVertex *scratch;
    u8 *quad;
    volatile u32 *packet, *tagPacket;
    volatile NativeCompactU64 *payload;
    NativeCompactU64 *source;
    NativeCompactU64 command;
    int index, quadIndex, oldCursor, page, width, height, queued, vertex;
    unsigned int shift;
    if (!(flags >> 24)) return;
    for (index=0; index<header->textureCount; ++index) {
        NativeCompactU64 tex0_word;
        long width_bits, palette_bits;
        int one=1; /* set here, ahead of the clamp: both shifts use it */
        texture=&header->textures[index];
        width=(short)texture->compact.field0C;
        shift=width-6;
        oldCursor=D_001A72D0>>8;
        page=D_001A72D0+0x400;
        height=page>>8;
        queued=ref_00300AA8_00;
        if ((int)shift<0) shift=0;
        shift=one << shift;
        tex0_word=(NativeCompactU64)(int)shift<<14;
        tex0_word|=height;
        width_bits=((long)((NativeCompactU64)texture->compact.field0C<<48)>>22)|0x01300000;
        tex0_word|=width_bits;
        width+=(short)texture->compact.field0E;
        tex0_word|=(long)((NativeCompactU64)texture->compact.field0E<<48)>>18;
        palette_bits=(NativeCompactU64)oldCursor<<37;
        palette_bits|=(NativeCompactU64)1<<34;
        tex0_word|=palette_bits;
        tex0_word|=(NativeCompactU64)1<<63;
        /* width stays one register; the copy keeps page as the first addend. */
        width=one<<width;
        shift=width;
        width=page+shift;
        D_001A72D0=width;
        texture->compact.field00=tex0_word;
        if (queued<64) {
            upload=&ref_00300AA8_01[queued];
            upload->source=header->texels+(unsigned short)texture->compact.field0A*16;
            upload->oldPage=oldCursor;
            upload->reserved04=0;
            /* Written through the row offset, as the RAC1 upload queue does. */
            *(u8 **)((u8 *)ref_00300AA8_01+queued*16+8)=header->texels+(unsigned short)texture->compact.field08*16;
            upload->width=texture->compact.field0C;
            upload->height=texture->compact.field0E;
            upload->newPage=height;
            ref_00300AA8_00=queued+1;
        }
    }
    func_00300910();
    func_00282D30(transform+12,transform+12,166.66600036621094f);
    nativeProjection[0]=ref_00300AA8_03.scale[0]/ref_00300AA8_03.size[0];
    ref_00300AA8_02[9]=-(ref_00300AA8_03.scale[1]/ref_00300AA8_03.size[1]);
    if (D_001A7BB1) ref_00300AA8_02[9]*=0.7500000596046448f;
    func_00283788(matrix,nativeProjection,transform);
    matrix[12]+=(float)D_001A7350[0]+x*16.0f;
    matrix[13]+=(float)D_001A7350[1]+y*16.0f;
    /* PS2 scratchpad identity matrix and expanded skinning records. */
    func_00283388((float *)0x70001fc0);
    if (header->frameCount) func_00300748((NativeSpawnHeader *)header,(char *)0x70002000,frame);
    if (!header->packed) func_0029D258(header->vertices,matrix,header->vertexCount,flags);
    else func_0029D398(header->vertices,matrix,header->vertexCount,header->tint,flags);
    scratch=(ScratchVertex *)0x70000000;
    command=0x80000004cc007ffbULL;
    quad=header->quads;
    for (quadIndex=0; quadIndex<header->quadCount; ++quadIndex,quad+=4) {
        if (*(int *)quad>>8 == -1) {
            if ((short)quad[0]<header->textureCount) command=header->textures[quad[0]].compact.field00;
            else command=0x80000004cc007ffbULL;
            --quadIndex;
        } else {
            nativeQueue[0]=0x10000008;
            D_001B22A8[1]=0;
            D_001B22A8[2]=0;
            D_001B22A8[3]=0x50000008;
            tagPacket=D_001B22A8+4;
            D_001B22A8=tagPacket;
            qcopy((void *)tagPacket,&ref_00300AA8_06);
            packet=tagPacket+4;
            D_001B22A8=packet;
            payload=(volatile NativeCompactU64 *)packet;
            payload[0]=command;
            payload[1]=0x5c;
            vertex=quad[0];
            source=&scratch[vertex].color;
            payload[2]=*source++; payload[3]=source[0]; payload[4]=source[1];
            vertex=quad[1];
            source=&scratch[vertex].color;
            payload[5]=*source++; payload[6]=source[0]; payload[7]=source[1];
            vertex=quad[3];
            source=&scratch[vertex].color;
            payload[8]=*source++; payload[9]=source[0]; payload[10]=source[1];
            vertex=quad[2];
            source=&scratch[vertex].color;
            payload[11]=*source++; payload[12]=source[0]; payload[13]=source[1];
            D_001B22A8+=0x1c;
        }
    }
}
