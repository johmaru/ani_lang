#include "ani_lang.h"

int foo() {
    printf("OK\n");
    return 0;
}

int hello() {
    printf("やあ\n");
    return 0;
}

int hello_args(int a, int b) {
    printf("やあ %d %d\n", a, b);
    return 0;
}

int test(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j) {
    printf("%d %d %d %d %d %d %d %d %d %d\n", a, b, c, d, e, f, g, h, i, j);
    return 0;
}