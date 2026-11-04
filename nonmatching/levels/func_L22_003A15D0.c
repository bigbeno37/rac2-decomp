/*
 * func_L22_003A15D0: strict build rejected it: D_L22_001AF314 needs two values
 */
void func_L22_003A15D0(Obj_15D0 *obj) {
    Vars_15D0 *p = obj->f68;
    Obj_15D0 *found = 0;
    Obj_15D0 *it;
    int i;
    char *base;
    typedef int Quad __attribute__((mode(TI)));
    union { Quad q; float f[4]; } vec;
    func_L22_003A6230(obj);
    if (obj->f20 == 0) {
        float factor, health, damageScale, rangeScale;
        short x380;
        int eight, alpha;
        int difficulty=D_L22_001AF2E8;
        if (D_L22_001AF2F0 < difficulty) factor = 0.6f;
        else if (D_L22_001AF2EC < difficulty) factor = 0.9f;
        else goto full_health;
        D_L22_001B2AA8 = (int)(p->f326*factor);
        goto health_done;
full_health:
        D_L22_001B2AA8 = p->f326;
health_done:
        func_L00_0042A500(p->f46C);
        D_L22_001AF304 = 1;
        func_001163A0(*(volatile unsigned int *)0x10000800);
        p->f4EC=1;
        D_L22_001B2A8C=0; D_L22_001B2AB4=D_L22_001B1910;
        D_L22_001B2A90=0;
        p->f460=0; p->f41E=0;
        D_L22_001AF394=1; D_L22_001B2A88=0;
        p->f46A=0; p->f468=0;
        p->f464=0; p->f344=0;
        D_L22_001B2AD4=5;
        { int *d=D_L22_001AF3B0; int *s=(int *)D_L22_001AF5D0; int n; for(n=5;n!=0;n--) *d++=*s++; }
        D_L22_001B2AC8=-1; D_L22_001AF3C4=0; D_L22_001B2AD0=0;
        p->f34E=0; p->f34C=0;
        i=0; base=D_L22_001B211C;
        if(base<D_L22_001B2120) do {
            int id;
            Obj_15D0 *it;
            it=(Obj_15D0 *)(base+i*256);
            id=it->fAA;
            if(id==0x27D) found=it;
            if(id==0xE0B) D_L22_001AF314=it;
            if(id==0x133A) it->f34|=0x41;
            if(it->fAA==0xF6E) {
                Sub_15D0 *sub;
                D_L22_001AF2E0=it; it->f30=255;
                sub=(Sub_15D0 *)D_L22_001AF2E0->f68;
                sub->f8=0; sub->fA=0; sub->fC=-1;
            }
            i++;
            base=D_L22_001B211C;
        } while(base+i*256<D_L22_001B2120);
        vec.q=0; vec.f[0]=0.00161f; vec.f[1]=0.29459f; vec.f[2]=1.57105f;
        qcopy(&D_00189E20.rotation,&vec); qcopy(&D_00189E20.position,(char *)found+0x10);
        D_00189E20.health=p->f324;
        func_00282CF0(&vec,(char *)obj+0x10,(char *)&D_00189E20.rotation+0x20);
        D_L22_001B2AA4=func_00282DF0(&vec);
        obj->f34|=0x100;
        x380=func_002A77E0(30);
        eight=8;
        p->fD7=1; p->f70=37; p->f71=4;
        p->f77=eight; p->f72=eight; p->f76=eight;
        p->fCC=9; p->fCD=18; p->fCE=9; p->fCF=18; p->fD2=-1; p->fD3=-1;
        health=(float)D_L22_001B2AA8;
        rangeScale=900.0f/health; damageScale=9.0f/health;
        p->f7C=0.5833333730697632f; p->f80=1.0f/15;
        p->f84=1.0f/24; p->f88=1.0f/12;
        p->f8C=1.0f/60; p->f90=3; p->f94=4;
        p->f9C=0.4f; p->fA8=eight;
        p->fBC=rangeScale; p->fB8=damageScale;
        p->f130=0.0044444446f; p->f134=0.0044444446f;
        p->f138=0.20000002f; p->f110=2.5f; p->f114=0.75f;
        p->f120=2; p->f118=2.75f; p->f144=0; p->f380=x380;
        p->f11C=2.75f; p->f1F8=12; p->f1F9=11;
        { float health=(float)D_L22_001B2AA8;
        p->f148|=1; p->f140=0.7853982448577881f;
        p->f28=3; p->f26=60;
        p->f20=health;
        p->f24=(int)health; }
        alpha=255; obj->f32=alpha; obj->f30=alpha; obj->f48=1.0f;
        p->f374=-1; p->f382=-1; p->f35C=-1; p->f35A=-1; p->f358=-1;
        p->f3EC=-1; p->f4E4=300;
        p->f3E4=p->f3E8=0.0f; p->f388=0;
        p->f378=0; p->f37C=0; p->f37A=0;
        p->f35E=0; p->f350=0;
        obj->f34|=0x4000;
        obj->f2C=obj->f24->f24*D_L22_001AF39C;
        func_L22_003A3368(obj); func_L22_003A36A0((char *)obj);
        p->f440=func_0029EFD0(0xE0A); func_00283360((char *)p->f440+0xC0);
        p->f444=func_0029EFD0(0xE0C); p->f448=func_0029EFD0(0xF8F);
        p->f384=30; p->f43A=0;
    }
    func_0029B758(func_L22_003A4FF8,func_L22_003A5018);
    if(p->f440) {
        p->f440->f32=obj->f32; p->f440->f30=obj->f30;
        p->f440->f38=obj->f38;
        p->f440->f78=obj->f78;
        p->f440->f31=1;
        it=p->f440; it->f2C=it->f24->f24*D_L22_001AF39C;
        it=p->f440;
        if(it->f31 && func_00282E48((char *)it+0x10,D_L22_001B9C80)<24.0f) {
            func_002A93B0(p->f440); p->f440->f6F=22;
        }
    }
    if(p->f444) {
        p->f444->f32=80; p->f444->f30=obj->f30;
        p->f444->f38=obj->f38;
        p->f444->f78=obj->f78;
        p->f444->f31=1;
        it=p->f444; it->f2C=it->f24->f24*D_L22_001AF39C;
    }
    if(p->f448) {
        p->f448->f32=80; p->f448->f30=obj->f30;
        p->f448->f38=obj->f38;
        p->f448->f78=obj->f78;
        p->f448->f31=1;
        it=p->f448; it->f2C=it->f24->f24*D_L22_001AF330;
        func_002A0F48(p->f448);
    }
    func_002AFFD8((char *)obj+0x10,(char *)p+0x200,0); func_L00_00325600((char *)obj+0xC0,(char *)p+0x200,0); func_002B0ED8(obj,(char *)p+0x60,0);
    obj->f2C=obj->f24->f24*D_L22_001AF39C;
    if(D_L22_001AF304) {
        if(++D_L22_001AF304>32000) D_L22_001AF304=1;
        D_00189E20.mode=10;
        if(obj->f95!=1) {
            p->f350=0; p->f3E4=p->f3E8=0.0f; func_002A7750(obj,1,1);
        }
        if(D_00138180.buttons&0x50) {
            D_L22_001AF304=0; func_002A7750(obj,5,0); D_00138180.buttons=0; D_00189E20.mode=20;
        } else if(D_L22_001AF304<30) {
            D_00189E20.field2294=0; func_L00_002AF7D0();
            func_L00_00310C88(D_00189E20.field2290,&D_00189E20.field1870);
            D_00189E20.field187C=0; func_L22_002C9898(0,1);
        }
    } else if(D_00189E20.health<=0 && obj->f95!=1) {
        p->f350=0; p->f3E4=p->f3E8=0.0f;
        func_002A7750(obj,2,1); p->f384=30;
    }
    if(obj->f31 && func_00282E48((char *)obj+0x10,D_L22_001B9C80)<24.0f) {
        func_002A93B0(obj); obj->f6F=22;
    }
}
