/* Tier opus-2, 5 November 2026: best legal C is still opus-1's body below, 1368/1376 bytes, 0 of 27
 * places (check.py, placed at the end of src/levels/shared/L00_002D3798-g8.c, which already declares
 * D_L00_001B19C8 as `int[] sda` and D_00189E20 as Hero5940: there use D_L00_001B19C8[0] and
 * (H *)&D_00189E20).
 *
 * Cause of the last-loop difference, confirmed with -dr/-dj/-ds dumps: retail's exit test (i<56, the
 * map/table `exists` test, i==10, i==9 and the AF8 break; expand_end_loop rotates everything up to the
 * last `break` found within LOOP_TEST_THRESHOLD = 30 insns) was NOT duplicated by jump1.  jump1 copies
 * it only if the exit code holds no CODE_LABEL, call, LOOP_CONT note, libcall or asm (jump.c
 * duplicate_loop_exit_test, threshold 30).  Left undivided, cse1 follows the bottom test into the body
 * (body label: one use, after a barrier), so the body's tbl[map[i]].state reuses the test's 0xE0 and
 * table-base pseudos; loop.c then makes the map address a giv (s1) and hoists 17/18/53; the rerun jump
 * before cse2 duplicates the test.  That gives s3=0xE0, s2=base, frame 0x60 with s7/fp.
 *
 * Proof (NOT landable, dead test): put `if(i>=56) D_L00_001A903C=0;` as the first statement of the last
 * loop body (the conditional memory store keeps a label through jump1; cse1 folds the test from i<56),
 * drop the a/b/c locals and write `if(i==17) continue; if(i==18) continue; if(i==53) continue;`.
 * The whole last loop then matches retail.  Only two scheduling slips remain: the prologue's
 * `lui s5`/`lui s4` order (retail lui s4 first) and the position of `daddu a1,zero,zero` before the
 * `sb ...0x7A($at)` at the end.  With that statement the region is exactly 30 insns at expand time
 * (the AF8 break is insn 30), so any legal blocker must cost at most 3 expand insns, or else the rest of
 * the region must get cheaper.  Blockers that vanish by the pre-cse2 jump: a label whose jump cse1 or
 * gcse cprop folds (i<56 is known at the region start; ok==1 is known to gcse there), sitting before
 * the map/table loads, since a label after them splits cse1's EBB.  Already tried, no effect (jump1
 * still duplicates): `i<56 && ok` / `ok && i<56` as the loop condition; `if(i>=56) ok=0;` (jump1
 * turns register assignments into scc/cmove, so only stores keep a label); `i==10||i==9` (fold turns it
 * into an sltiu range test, which retail does not have).
 * Opus-1 notes still apply to the rest: D_001A7A28, D_001A7398 and D_001A7AF8 are MACRO_ADDR incomplete
 * arrays; D_001A7A00 is an sda unsigned char array (count through *(int *)D_001A7A00, flag
 * D_001A7A00[0x7A]); the last loop indexes D_00139568[i]; ref_L00_002D5EB8_00 is the sda int at the
 * `>= 11` test.
 */
#include "common.h"
extern int D_L00_001B19C8;
extern int D_0018C350[];
extern unsigned char D_001A7A00[] __attribute__((sda));
extern int D_0018C2EC[];
extern unsigned char D_001A7A28[] MACRO_ADDR;
extern unsigned char D_00139568[];
extern int D_L00_001A903C;
extern unsigned short D_0018C958 __attribute__((section(".data")));
extern int D_00139868[];
extern int D_001A7398[] MACRO_ADDR;
extern unsigned char D_001A7AF8[] MACRO_ADDR;
typedef struct { char p[0x340]; int f340; char q[0x1248-0x344]; int f1248; } H;
extern char D_00189E20[];
typedef struct { int exists; unsigned char state; char pad[7]; int index; char pad2[58]; short value; char pad3[32]; int limit; char pad4[112]; } Entry1269;
extern Entry1269 D_L00_00262570[];
extern void func_L00_002D5E30(int*,int*,int*);
extern void func_0028A180(int,int), func_L00_003F1300(void), func_L00_0030C7F8(int), func_002B0A18(int,int), func_002E5E58(int,int,int);
extern int func_00288D58(void), func_00287F60(int);
extern void func_L00_0037E6A8(int,int), func_00287EB0(int), func_00288508(int);
extern int ref_L00_002D5EB8_00 __attribute__((sda));
static inline int special(int i) { int r=0; switch(i) {case 12:case 14:case 17:case 18:case 53: r=1; break;} return r; }
static inline int unlocked(Entry1269 *e,int i){ if(e->state>=2 || (e->state==1 && special(i))) return 1; return 0;}
static inline int owned(int i){ if(D_L00_00262570[D_00139568[i]].exists){ int n=func_00287F60(i); if(n>=2 || (n==1 && special(i))) return 1; } return 0; }
void func_L00_002D5EB8(void) {
 int count, i, j, ok;
 if ((D_L00_001B19C8&15)==1 && D_L00_001B19C8!=1) return;
 count=4;
 {int *p=D_0018C350; func_L00_002D5E30(p,p + -1,&count);}
 for(j=0;j<10;j++) if(D_001A7A28[j]) count++;
 if (*(int *)D_001A7A00!=count) {
  if (ref_L00_002D5EB8_00>=11 && *(int *)D_001A7A00<count) {
   if (!D_0018C958) func_0028A180(0x9a9,0);
   func_L00_003F1300(); func_L00_0030C7F8(0x168); func_002B0A18(0x1235,-1); func_002E5E58(6,0,0);
  }
  D_0018C2EC[0]=count; *(int *)D_001A7A00=count;
 }
 for(i=0;i<56;i++) {
  if (D_L00_00262570[D_00139568[i]].state) {
   if (!func_00288D58()) continue;
   if (func_00287F60(i)==1) continue;
  }
  {
   Entry1269 *e=&D_L00_00262570[D_00139568[i]];
   int sel = 0;
   if (e->exists) sel = e->state == 3;
   if (sel) continue;
  }
  if(func_00287F60(i)<=0) continue;
  {
   Entry1269 *e=&D_L00_00262570[D_00139568[i]];
   if((D_00139868[i]>>5)<e->limit || e->limit<=0) continue;
   if(i==31 || i==9 || i==45) { func_L00_0037E6A8(i,D_L00_00262570[D_00139568[i]].value); continue; }
   {
    H *h=(H*)D_00189E20;
    if(h->f1248==i) {
     if(!h->f340) continue;
     if(D_001A7398[e->index]!=i) continue;
     func_L00_0037E6A8(i,e->value); continue;
    }
   }
  }
  if(func_00288D58()) {
   Entry1269 *e=&D_L00_00262570[D_00139568[i]];
   if(!e->exists) continue;
   if(unlocked(e,i)) {
    if(owned(i)) { func_00287EB0(i); func_00288508(i); }
   }
  } else { func_00287EB0(i); func_00288508(i); }
 }
 if((D_L00_001B19C8&127) || D_001A7A00[0x7A]) return;
 {
 int a=17,b=18,c=53;
 ok=1;
 for(i=0;i<56;i++) {
  if(D_L00_00262570[D_00139568[i]].exists) {
   if(i==10) continue;
   if(i==9) continue;
   if(!D_001A7AF8[i]) {ok=0; break;}
   if(i==14) continue;
   if(i==12) continue;
   if(i==a) continue;
   if(i==b) continue;
   if(i==c) continue;
   if(func_00287F60(i)<=0) continue;
   if(!D_L00_00262570[D_00139568[i]].state) {ok=0; break;}
  }
 }
 }
 if(ok && !D_001A7A00[0x7A]) {
  D_001A7A00[0x7A]=1; func_002E5E58(1,0,0); func_002B0A18(0x1233,-1); D_L00_001A903C=1;
 }
}
