#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ptest.h"

ptest_ctx test_data[256];
int test_data_len = 0;
void ptest_run(void (*f)(ptest_ctx *ctx)) {
	srand(time(0));
	ptest_ctx ctx;
	for (int i = 0; i < 100; i++) {
		f(&ctx);
        test_data_len = 0;
	}
}

void ptest_fail(ptest_ctx *ctx) {
	// TODO: print generated data of failed runv ds
    for(int i = 0; i < test_data_len; i ++) {
        printf("gen %s: %d\n", test_data[i].name, test_data[i].value);
    }
    assert(0);
}

int ptest_int(ptest_ctx *ctx, char* name) {
	int val = rand();
	strcpy(test_data[test_data_len].name, name);
	test_data[test_data_len].value = val;
    test_data_len ++;
	return val;
}

int ptest_int_range(ptest_ctx *ctx, int from, int to, char *name) {
	int val = rand() % (to - from) + from;
	strcpy(test_data[test_data_len].name, name);
	test_data[test_data_len].value = val;
    test_data_len ++;
	return val;
}
