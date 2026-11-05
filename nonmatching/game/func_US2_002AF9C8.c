/* func_US2_002AF9C8: opus-1, task v2819. Exact C blocked by a CATALOGUE defect, not by the code.

   The row's size is one word short. Retail's function is 0x002AF9C8..0x002AFA58, 144 bytes; the row
   (boot and all 21 levels) says 140, so it ends on `jr $ra` at +0x88 and leaves that return's delay
   slot, `addiu $sp,$sp,0x30` (0x27BD0030) at +0x8C, outside the row. The next row starts at 0x002AFA80.
   140 is also v1.01's size for the template func_002AEB60, and v2.00 has one more instruction: the
   0.5f constant is `lui`+`mtc1` where v1.01 loads the 0x4C field with one `lwc1`.

   Best check.py result: `144 bytes, expected 140` at 0 of 22 places (the size test stops it).
   Measured past that test: this C, with the row's size overridden to 144 in memory only (no file
   changed), gives check.py `22 of 22 places match`. The word after the row is 0x27BD0030 at all 22.
   No instruction differs. tools/build.py has not been run on this C.

   Unblock: correct the row to 144 bytes through tools/levels.py (never by hand), then move this C
   to src/us2/game/002AF9C8.c. The default flags fit, as for func_US2_L08_003326F0, which has the
   same instructions and already matched at 144 bytes.

   func_US2_002AF9C8, the USA v2.00 copy of func_002AEB60: the same C, where the 0x4C field becomes 0.5f. */
extern float func_0029F530(void *);
extern float func_002AEAE0(int, float);

int func_US2_002AF9C8(char *object, float threshold)
{
    float current = func_002AEAE0(4, func_0029F530(object));
    float difference = current - threshold;
    float step = func_002AEAE0(4, *(float *)(object + 0x48) * 0.5f);
    if (threshold <= current)
        return difference < step;
    return 0;
}
