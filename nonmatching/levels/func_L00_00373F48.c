/* opus-1 (Claude Opus 5.5), task v2325, 4 November 2026. Best: 1024/1024 bytes, 0/27 places (check.py us), 7 rows.
 * Profile: -O2 -G64 -Wa,-G64 -ffunction-sections (build as a -g64.c unit with an explicit cflags line).
 * Fixed since the 1 November draft: state 3 registers (block-local g/c, so local-alloc ties them), both inventory
 * `or` operand orders (int temp t = bit | byte: an int IOR is not narrowed to QI, so combine cannot swap the
 * SUBREG byte first), the second test's reread and andi (002CF340-g8.c's (unsigned long) & 255UL form), and its
 * sllv-before-lbu order (mask in a temp m computed before the byte test).
 * Left: in the second update the shift amount (category*2) and the byte address swap a0/a1
 * (retail sll a1 / addu a0). Global alloc gives the address pseudo (refs 4, priority 2/3) a1, not a0, even
 * though it outranks the shift pseudo (1/3), so a preference or a0 conflict decides it, not priority.
 * A pointer variable for the address changed nothing.
 * Needs: D_001A7A80 MACRO_ADDR, D_L00_001AD4F0 SDATA, D_L00_0022F7C0 as the PurchaseModel union, Metadata 0xE0 rows.
 */
#include "common.h"
/*
 * GPT-6.1 Sol high tier, task v0932; pinned upstream compiler.
 * Retained check.py: 1028/1024 bytes, 0/27 places. No linked match.
 * Profile: -O2 -G64 -Wa,-G64 -ffunction-sections.
 * Measured 16-byte item and 0xE0-byte metadata layouts, per-category switch
 * lookups and direct model-state stores restore the retail 0x40 frame.
 * SDATA keeps the runtime parameter array incomplete and reproduces its GP address.
 * Retail rereads the inventory byte for the conditional update, so its complete
 * declaration is volatile. A nonvolatile diagnostic was 1020 bytes but lost that
 * reread. Residual: selected-item base sharing, shared state-4 stores, QI mask
 * extension, and scheduling. An inline byte-mask helper restores the two andi
 * instructions, but the combined explicit-store/helper trial was 1044 bytes;
 * an inline metadata helper was 1040. Neither matches any placement.
 * Next useful experiment needs evidence for the original selected-item accessor
 * and inline byte-mask interface, rather than another equivalent branch form.
 * Required declarations are included below.
 */

typedef struct { int category, group, cost, unused; } PurchaseItem;
typedef struct { int value; int pad[3]; } PurchaseRow;
typedef union {
    struct { char pad[0x80]; int index, state; } head;
    struct { char prefix[0x140]; PurchaseRow rows[0]; } category;
    struct { char prefix[0x144]; PurchaseRow rows[0]; } group;
    struct { char prefix[0x148]; PurchaseRow rows[0]; } cost;
} PurchaseModel;
extern PurchaseModel D_L00_0022F7C0;
extern char D_00138180[];
/* Retail rereads this byte between its bit test and update at 003742B0/003742C4. */
extern unsigned char D_001A7A80[] MACRO_ADDR;
extern unsigned char D_00139568[];
typedef struct { char pad[0x98]; int flag[3]; char tail[0x3C]; } Metadata;
extern char D_L00_00262570[];
/* Runtime menu parameters; the element count belongs to the selected menu. */
extern int D_L00_001AD4F0[] SDATA(D_L00_001AD4F0);
extern void func_L00_00373F40(void);
extern int func_002B0D70(void);
extern int func_002E5CF8(int, int, char *);
extern int func_00288FB8(int);
extern void func_002F88D8(void *);
extern int func_L00_00326950(int);
extern void func_L00_002F5890(int);
extern void func_002F73A8(void);
extern void func_002FB1A0(void);

static __inline__ unsigned char Bit(int n) { return 1 << n; }
void func_L00_00373F48(int *answer) {
    PurchaseModel *model = &D_L00_0022F7C0;
    char *state_model;
    int cost = model->cost.rows[model->head.index].value;
    int state = model->head.state;
    int category;
    int group;
    int enabled;
    char *metadata;
    if (state == 1) {
        if (*(int *)((char *)model + 0x740) != 0) {
            func_L00_00373F40();
            D_L00_0022F7C0.head.state = 2;
        } else D_L00_0022F7C0.head.state = 0;
    } else if (state == 2) {
        if (*(unsigned int *)(D_00138180 + 0x1C4) & 0x10) {
            *answer = 0;
            D_L00_0022F7C0.head.state = 4;
        } else if (*(unsigned int *)(D_00138180 + 0x1C4) & 0x40)
            D_L00_0022F7C0.head.state = 3;
    } else if (state == 3) {
        *answer = 0;
        if (func_002B0D70() < cost) {
            func_002E5CF8(2, 17, *(char **)((char *)model + 0x28));
            D_L00_0022F7C0.head.state = 4;
        } else {
            int g = D_L00_0022F7C0.group.rows[model->head.index].value;
            int c = D_L00_0022F7C0.category.rows[model->head.index].value;
            if ((unsigned long)D_001A7A80[g] & ((1U << (c * 2)) & 255UL)) *answer = 0;
            else *answer = 1;
            D_L00_0022F7C0.head.state = 4;
        }
    } else if (state == 4) {
        if (*answer != 0) {
        if (func_002B0D70() >= cost) {
            func_002F88D8((void *)func_00288FB8(0x2DCB));
            func_002E5CF8(7, 17, *(char **)((char *)model + 0x28));
            func_L00_00326950(cost);
            func_L00_002F5890(D_L00_001AD4F0[model->head.index]);
            category = D_L00_0022F7C0.category.rows[D_L00_0022F7C0.head.index].value;
            group = D_L00_0022F7C0.group.rows[D_L00_0022F7C0.head.index].value;
            switch (category) {
            case 0: enabled = ((Metadata *)(D_L00_00262570 + D_00139568[group] * 0xE0))->flag[0]; break;
            case 1: enabled = ((Metadata *)(D_L00_00262570 + D_00139568[group] * 0xE0))->flag[1]; break;
            case 2: enabled = ((Metadata *)(D_L00_00262570 + D_00139568[group] * 0xE0))->flag[2]; break;
            default: enabled = 0; break;
            }
            if (enabled) { int t = (1 << (category * 2)) | D_001A7A80[group]; D_001A7A80[group] = t; }
            category = D_L00_0022F7C0.category.rows[D_L00_0022F7C0.head.index].value;
            group = D_L00_0022F7C0.group.rows[D_L00_0022F7C0.head.index].value;
            switch (category) {
            case 0: enabled = ((Metadata *)(D_L00_00262570 + D_00139568[group] * 0xE0))->flag[0]; break;
            case 1: enabled = ((Metadata *)(D_L00_00262570 + D_00139568[group] * 0xE0))->flag[1]; break;
            case 2: enabled = ((Metadata *)(D_L00_00262570 + D_00139568[group] * 0xE0))->flag[2]; break;
            default: enabled = 0; break;
            }
            if (enabled) {
                unsigned long m = (1U << (category * 2)) & 255UL;
                if ((unsigned long)D_001A7A80[group] & m) {
                    int t = (1 << (category * 2 + 1)) | D_001A7A80[group];
                    D_001A7A80[group] = t;
                }
            }
            func_002F73A8();
        } else {
            func_002F88D8((void *)func_00288FB8(0x2DCC));
            func_002E5CF8(0, 17, *(char **)((char *)model + 0x28));
        }
        } else func_002E5CF8(0, 17, *(char **)((char *)model + 0x28));
        D_L00_0022F7C0.head.state = 0;
        func_002FB1A0();
    }
}
