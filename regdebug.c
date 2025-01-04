#include "ani_lang.h"

int debug_label = 0;

void debug_value(int value) {
    fprintf(stderr, "label %d: value: %d\n", debug_label, value);
    debug_label++;
}