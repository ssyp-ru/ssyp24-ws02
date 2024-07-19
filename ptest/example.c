#include <assert.h>

#include "ptest.h"

int add(int a, int b) {
    if (b > 5) {
        return b;
    }
    return a + b;
}

void ordinary_test() {
    int expected = 10;
    int res = add(3, 7);
    assert(res == expected);
}

void property_test(ptest_ctx *ctx) {
    int a = ptest_int_range(ctx, 0, 10, "a");
    int b = ptest_int_range(ctx, 0, 10, "b");
    int res = add(a, b);
    if (res != a + b) {
        ptest_fail(ctx);
    }
}

int main() {
    // ordinary_test();

    ptest_run(property_test);
}
