/* opus-1 retained attempt (v2297), USA v1.01, pinned game compiler, -G8 file.
 * check.py: 1352 bytes versus 1352, frame 0x360; 0 of 5 places match;
 * 51 of 338 rows differ, all of them relocations or spill-slot offsets.
 * Code, allocation, scheduling and frame now match retail; only the order of
 * the eight 4-byte spill slots differs. Retail: 0x2C0 d, 0x2C4 j<2, 0x2C8 rot,
 * 0x2CC sp+0x40, 0x2D0 next, 0x2D4 sp+0x50, 0x2D8 sp+0x54, 0x2DC transform.
 * Ours: d, next, j<2, transform, sp+0x50, sp+0x54, rot, sp+0x40 (reload assigns
 * slots in pseudo order: the PRE base pseudos and next/j<2 come out in a
 * different order; see the PRE hash-order notes in the compiler docs).
 * Keys found: 16-byte aligned RingVec (blocks the expand_expr bitpos fold),
 * q[j] inside every switch case (one fixed-row IV at sp+0x1B0), indexed
 * for (j) first loop with g[2] before g[3] (separate end pointer, constant
 * order), index-form UV loads with the v table as MACRO_ADDR (unsplit la, so
 * loop.c reduces it into s7), g = D_00189F20 - 0x100 assigned after the call.
 * Declarations: all below (D_L01_001AF5F0/5F8 via SDATA, D_L01_001AF5FC as
 * MACRO_ADDR, D_L01_001AF5D8..5E8/618/620 sda). No linked-build claim.
 * Related RAC1 source: GPL-3.0 gold_bolts_and_teleporters.c,
 * draw_moby_radius_ring; RAC2 constructs four quads for a batched draw.
 */
extern char D_00189EA0[];
extern void func_00282CF0(void *, void *, void *);
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define B(p,o) (*(unsigned char *)((char *)(p)+(o)))
typedef int TI __attribute__((mode(TI)));
typedef float RingVec[4] __attribute__((aligned(16)));
typedef struct {RingVec v[4]; unsigned c[4]; float uv[4][2]; long g[4];} RingQuad;
extern int D_L01_001AF5D8 __attribute__((sda));
extern int D_L01_001AF5DC __attribute__((sda));
extern int D_L01_001AF5E0 __attribute__((sda));
extern int D_L01_001AF5E4 __attribute__((sda));
extern int D_L01_001AF5E8 __attribute__((sda));
extern int D_L01_001AF5F0[] SDATA(D_L01_001AF5F0);
extern float D_L01_001AF5F8[] SDATA(D_L01_001AF5F8);
extern float D_L01_001AF618 __attribute__((sda));
extern float D_L01_001AF620 __attribute__((sda));
extern float D_L01_001AF5FC[] MACRO_ADDR;
extern char D_00189F20[];
extern char D_00189E20[];
extern float D_L01_001B1FA4;
extern void func_00283658(void *,void *);
extern long func_0027C878(int);
extern void func_002AC9F8(void *,float);
extern unsigned func_00283D38(unsigned,unsigned,float);
extern void func_00282C90(void *);
extern void func_002833F0(void *,void *);
extern void func_L01_002E1510(void *,int,void *,int,void *);
void func_L01_00398B50(char *m)
{
    RingQuad q[4];
    float mat[12];
    float pos[4];
    float transform[12];
    float rot[4];
    char *d=*(char **)(m+0x68);
    float alpha,h;
    int j,k;
    func_00283658(mat,m+0xc0);
    qcopy(pos,m+0x10);
    for(j=0;j<4;j++) {
        q[j].g[1]=func_0027C878(D_L01_001AF5E8);
        q[j].g[2]=0xFF9000000260L;
        q[j].g[3]=(long)D_L01_001AF5D8;
        q[j].g[3]|=(long)D_L01_001AF5DC<<2;
        q[j].g[3]|=(long)D_L01_001AF5E0<<4;
        q[j].g[3]|=(long)D_L01_001AF5E4<<6;
        q[j].g[3]|=0x8000L<<24;
        q[j].g[0]=0;
    }
    h=0.1f;
    alpha=(1.95f-F(d,0x34))*2.0f;
    if(alpha>1) alpha=1; else if(alpha<0) alpha=0;
    if(F(d,0x34)<1.15f) {
        float t, shrink;
        char *g;
        func_00282CF0(D_00189F20,m+0x10,D_00189F20-0x80);
        g=D_00189F20-0x100;
        I(g,0x108)=0;
        func_002AC9F8(D_00189F20,0.01666666753590107f);
        shrink=F(d,0x34);
        t=1.15f-shrink; if(t<0)t=0;
        {float fade=(0.2f-shrink)*5; if(fade<0)fade=0; D_L01_001B1FA4=fade;}
        h=t+0.1f;
        if(B(m,0x20)!=7 && B(m,0x20)!=8) {
            if(shrink<0.65f) {
                float v=shrink*1.5384615659713745f;
                *(short *)(g+0x2500)=1;
                F(g,0x2024)=v;F(g,0x2020)=v;F(g,0x2028)=2-v;
            } else *(short *)(g+0x2500)=0;
        }
    }
    for(j=0;j<4;) {
        int next=j+1;
        for(k=0;k<4;k++) {
            unsigned c;
            q[j].uv[k][0]=D_L01_001AF5F8[k*2];
            q[j].uv[k][1]=D_L01_001AF5FC[k*2];
            c=D_L01_001AF5F0[j!=0];
            q[j].c[k]=c;
            q[j].c[k]=func_00283D38(c&0xffffff,c,alpha);
            func_00282C90(q[j].v[k]);
            {float radius=F(d,0x34); if(j)radius+=0.01f; q[j].v[k][0]=radius;}
            if(k&1) {
                switch(j) {
                case 0:q[j].v[k][2]+=D_L01_001AF618+h;break;
                case 1:q[j].v[k][2]+=D_L01_001AF618+D_L01_001AF620;break;
                case 2:q[j].v[k][2]+=(D_L01_001AF618-h*0.8f)+D_L01_001AF620;break;
                case 3:q[j].v[k][2]+=(D_L01_001AF618+h*0.8f)+D_L01_001AF620;break;
                }
            } else {
                switch(j) {
                case 0:q[j].v[k][2]+=D_L01_001AF618-h;break;
                case 1:q[j].v[k][2]+=D_L01_001AF618-D_L01_001AF620;break;
                case 2:q[j].v[k][2]+=(D_L01_001AF618-h*0.8f)-D_L01_001AF620;break;
                case 3:q[j].v[k][2]+=(D_L01_001AF618+h*0.8f)-D_L01_001AF620;break;
                }
            }
        }
        j=next;
    }
    *(TI *)rot=0;rot[2]=0.19634954631328583f;
    func_002833F0(transform,rot);
    func_L01_002E1510(q,4,transform,32,mat);
}
