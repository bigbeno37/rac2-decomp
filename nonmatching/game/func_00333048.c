/*
 * func_00333048: check.py passed (func_00333048: 28 of 28 places match (src/game/0032EB28-g8.c)), but it calls func_00333048_kind, defined in the same unit and not placed everywhere func_00333048 is: the strict build would reject it ('needs .text.<callee> at its place, which is not C'). Needs an arrangement where those calls are external.
 */
void func_00333048(char *object)
{
    float direction[4] __attribute__((aligned(16)));
    float spark[4] __attribute__((aligned(16)));
    HitInfo333048 hit __attribute__((aligned(16)));
    float distance;
    float yaw;
    float angle;
    int i;
    int j;
    int k;
    int count;
    char *camera;

    func_002E5CF8(3, 0, (int *)object);
    for (i = 0; i < 30; i++) {
        distance = func_002A7878(0.0f, 0.37f);
        yaw = func_002A7940();
        func_002AF000(spark, distance, yaw, func_002A7940());
        spark[2] += func_002A7878(0.35f, 0.6f);
        func_00282CC0(spark, spark, object + 0x10);
        distance = func_002A7878(0.0016666668f, 0.016666668f);
        yaw = func_002A7940();
        func_002AF000(direction, distance, yaw, func_002A7940());
        func_002BDBC0(spark, direction, 0x4F20507F, 0x102F, func_002A7820(30, 57), 150000.0f);
    }
    camera = D_001B5200;
    for (j = 0; j < 50; j++) {
        angle = func_00283248(*(float *)(camera + 0x140) - *(float *)(object + 0x10),
                              *(float *)(camera + 0x144) - *(float *)(object + 0x14));
        func_00283CF0(func_002A78D8(1.0f, 2.0f));
        direction[0] = *(float *)(object + 0x10) + func_002A78D8(0.0f, 0.15f);
        direction[1] = *(float *)(object + 0x14) + func_002A78D8(0.0f, 0.15f);
        direction[2] = *(float *)(object + 0x18) + func_002A78D8(0.0f, 0.15f);
        distance = D_001B0C30;
        angle = func_00283B98(angle, func_002A78D8(0.0f, -1.5707965f));
        spark[0] = func_00283180(angle) * 0.016666668f * distance;
        spark[1] = func_00283198(angle) * 0.016666668f * distance;
        distance = D_001B0C34;
        spark[2] = distance * 0.016666668f;
        func_002C7E28(direction, spark, 0x140);
    }
    func_002ACAD0(object, D_001B1CF0);
    func_0032C700(object + 0x10, 1.0f, 20.0f, 1, 25, 20, 20, 20, 45, 1, 1, 1, 0, D_001AD7EC);
    count = func_00277F48(object + 0x10, 0, object, 0, 3.0f);
    if (count > 0) {
        char **list = D_001BF000;
        k = count;
        do {
            char *moby = *list;
            if (*(short *)(*(char **)(moby + 0x24) + 0x46) == 5
                || (*(unsigned short *)(moby + 0x34) & 0x4000)) {
                float radius;
                func_00282CF0(direction, moby + 0x10, object + 0x10);
                func_00282F20(direction, direction, 1.0f);
                func_00282F20(hit.vector, hit.vector, 1.0f);
                radius = 10.0f;
                if (func_00333048_kind(0x10))
                    radius = 55.0f;
                func_002A8C00(hit.vector, object, 0x810000, direction, radius);
                *(TI128_333048 *)hit.vector = *(TI128_333048 *)direction;
                hit.vector[3] = 5627.9248f;
                hit.source = 3;
                hit.flag = 1;
                hit.type = 5;
                hit.oclass = *(short *)(moby + 0xAA);
                func_002A8E20(moby, &hit);
            }
            list++;
        } while (--k);
    }
    func_0029F338(object);
}
