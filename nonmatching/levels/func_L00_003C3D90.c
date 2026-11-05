/*
 * func_L00_003C3D90: left in src/ by the opus-2 worker but not matching: func_L00_003C3D90: 0 of 27 places match (src/levels/shared/L00_003C3D90-g8.c)
 */
void func_L00_003C3D90(Moby *self)
{
    float offset[4],origin[4],position[4];
    EffectContext *ctx=self->ctx;
    float value,ratio,sign;
    int i,j,k,slot,col1,col2;
    if(D_L00_001A8F00) return;
    qcopy(&self->x,D_00189EA0);
    func_L00_0037E640();
    if(!D_L00_001A8F00) {
        if(!func_002E59B0(self,ctx->sound))
            ctx->sound=func_002E5CF8(3,4,self);
    } else if(func_002E59B0(self,ctx->sound)) clearSound(self,ctx);
    if(D_L00_00262570[D_00139568[0x2d]].active && self->oclass==0xa5c) {
        int saved=D_L00_001AFF98;
        D_L00_001AFF98=0;
        if(func_L00_003C3A70(1)) {
            clearSound(self,ctx);
            func_002E5CF8(2,0,self);
            D_L00_001AFF98=0;
            func_0029F338(self);
            return;
        }
        D_L00_001AFF98=saved;
    }
    value=func_L00_0037E668();
    if(!func_L00_003C5B18(value) || D_L00_001AFF44==40) {
        clearSound(self,ctx);
        D_L00_001AFF44=0;
        func_L00_0037E698();
        func_002E5CF8(2,0,self);
        D_L00_001AFF98=0;
        func_0029F338(self);
        return;
    }
    if(value!=ctx->value) {
        D_00139868.changed+=func_00283CF0(func_00282C48(ctx->value-value));
        ctx->value=value;
        ctx->timer=D_L00_001AFF90;
    }
    ratio=func_00283CE0(ctx->timer)/func_00283CE0(D_L00_001AFF90);
    if(self->oclass==0xa5c) {
        col1=D_L00_001AFF10;
        col2=D_L00_001AFF14;
    } else {
        col1=D_L00_001AFF18;
        col2=D_L00_001AFF1C;
    }
    ctx->colour=func_00283D38(ratio,col1,col2);
    func_00282978_1(&ctx->timer);
    func_00282D30(offset,D_00189E20_13.moby+0xe0,0.75f);
    func_00282CC0(&self->x,D_00189E20_13.position,offset);
    func_0029FCC0(D_00189E20_13.moby,10,origin);
    for(i=0;i<8;i++) {
        if(!D_L00_001B2950[i]) {
            Particle *effect;
            int id=func_002A7820(D_L00_001AFF64,D_L00_001AFF68);
            value=func_002A7940();
            ratio=func_002A7940();
            func_002AF000(position,D_L00_001AFF4C,value,ratio);
            func_00282CC0(position,position,origin);
            effect=(Particle *)(D_L00_001B2950[i]=(int)func_002C36D0(position,id,2,1));
            if(effect) {
                ParticleOwner *sub=&effect->owner;
                sub->dx=effect->x-self->x;
                sub->dy=effect->y-self->y;
                sub->dz=effect->z-self->z;
                sub->moby=self;
                effect->colour=D_L00_001AFF70|ctx->colour;
                effect->size=D_L00_001AFF6C*210000.0f;
                effect->sprite=D_L00_001FE940.f8[1];
                D_L00_001B2970[i]=id;
            }
            effect=(Particle *)func_002C36D0(position,id,2,1);
            if(effect) {
                ParticleOwner *sub=&effect->owner;
                sub->dx=effect->x-self->x;
                sub->dy=effect->y-self->y;
                sub->dz=effect->z-self->z;
                sub->moby=self;
                effect->colour=0x307f7f7f;
                effect->size=D_L00_001AFF6C*63000.00390625f;
                effect->sprite=D_L00_001FE940.f8[1];
            }
        } else if(func_00282948(D_L00_001B2970+i)) D_L00_001B2950[i]=0;
    }
    slot=0;
    func_00282948(&ctx->lock);
    {
    int t;
    for(t=0;t<4;t++) if(func_00282978(&ctx->timers[t])) {
        ctx->targets[t]=0;
        slot=t;
    }
    }
    if(func_002829D8(&ctx->cooldown)) {
        int count=func_00277F48(&self->x,1,D_00189E20_13.moby,0,D_L00_001AFF2C*0.3f);
        if(count) {
            HitPacket packet;
            float scaled[4],output[4];
            int p;
            WeaponEntry *weapon=&D_L00_00262570[D_00139568[0x2d]];
            packet.flags=65536;
            packet.one=1;
            packet.damage=5627.9248046875f;
            packet.kind=3;
            packet.strength=weapon->strength;
            packet.mode=2;
            packet.oclass=self->oclass;
            packet.source=self;
            for(p=0;p<count;p++) {
                Moby *object=D_L00_001C3280[p];
                float *health;
                if(!object->enabled || !object || object->state==0xfe || object->state==0xfd || !object->info || object->info->category!=5) continue;
                health=func_002AB1F0(object);
                if(health) {
                    if(health[0]<=0) continue;
                    func_00282D30(scaled,self->forward,health[4]);
                    func_00282CC0(position,&object->x,scaled);
                } else qcopy(position,&object->x);
                func_00282CF0(&packet,position,&self->x);
                func_00282F20(&packet,&packet,1.0f);
                func_002A8E20(object,&packet);
                func_00282D30(scaled,&packet,D_L00_001AFF2C*0.3f);
                func_00282CC0(output,&self->x,scaled);
                func_L00_003C58E8(output,ctx->colour);
                ctx->cooldown=0.5f;
            }
        }
    }
    if(!D_L00_001A8F00 && slot && self->oclass==0x1192 && !ctx->lock && !D_00189E20_13.busy && D_00189E20_13.upgrades>=23) {
        int hits=func_00277F48(&self->x,1,D_00189E20_13.moby,0,D_L00_001AFF40);
        if(hits) for(j=0;j<hits;j++) {
            float *health;
            int freeSlot;
            if(!D_L00_001C3280[j]->enabled || !D_L00_001C3280[j] || D_L00_001C3280[j]->state==0xfe || D_L00_001C3280[j]->state==0xfd || !D_L00_001C3280[j]->info || D_L00_001C3280[j]->info->category!=5) continue;
            health=func_002AB1F0(D_L00_001C3280[j]);
            if(health && health[0]<=0) continue;
            freeSlot=-1;
            for(k=0;k<4 && D_L00_001C3280[j]!=TARGET(ctx,k);k++) {
                if(!TARGET(ctx,k)) freeSlot=k;
            }
            if(k==4) {
                Particle *effect;
                Moby *object;
                if(freeSlot<0) break;
                object=D_L00_001C3280[j];
                TARGET(ctx,freeSlot)=object;
                effect=(Particle *)func_L00_00412FF8(object,D_L00_001AFF24);
                if(effect) {
                    effect->source=self;
                    D_L00_001AFF44++;
                    func_002E5CF8(5,0,self);
                    ctx->timers[slot]=func_002A7820(20,60);
                }
                break;
            }
        }
    }
    qcopy(position,origin);
    {
    int r;
    for(r=0;r<5;r++) {
        if(func_00282978(&D_L00_001B2990[0][r])) D_L00_001B2990[0][r]=0;
        if(!D_L00_001B2990[0][r]) {
            float velocity[4],scaled[4];
            int id=func_002A7820(3,45);
            float spread=D_L00_001AFF7C;
            int fade=64;
            D_L00_001B2990[2][r]=id;
            D_L00_001B2990[1][r]=fade;
            D_L00_001B2990[0][r]=id;
            func_002A7988(velocity,spread,spread);
            qcopy(D_L00_00240080[r],position);
            D_L00_00240080[r]->v[3]=0.04f;
            sign=1.0f;
            {
            int n;
            EffectVector *row=D_L00_00240080[r];
            for(n=1;n<10;n++) {
                func_00282CF0(scaled,D_L00_00240080[r][n-1].v,D_L00_001B8DC0);
                func_002ACEC0(velocity,velocity,scaled,func_002A7878(0.0872664749622345f,1.3089970350265503f)*sign);
                sign=-sign;
                func_00282CC0(D_L00_00240080[r][n].v,D_L00_00240080[r][n-1].v,velocity);
                row[n].v[3]=0.04f;
            }
            }
        }
    }
    }
    func_L00_002E6CF0(func_L00_003C4850,self);
}
