/*
 * func_US2_00289840: strict build rejected it: D_001B1608 needs two values
 */
int func_US2_00289840(int key, int selection)
{
    SelectorState *state = &ref_L00_00288E18_00;
    if (state->active == 0 && state->key == -1 && D_001A63A8.active == 0 &&
        D_001A63A8.key == -1 && D_0018C958[selection].key != 65535 &&
        D_001A8BB0 != 6 && D_001B1608 >= 6) {
        state->key = key;
        state->selection = selection;
        state->value40 = 0;
        state->value44 = 0;
        return 1;
    }
    return 0;
}
