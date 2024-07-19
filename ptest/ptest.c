#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ptest.h"

void ptest_run(void (*f)(ptest_ctx *ctx)) {
	srand(time(0));
	ptest_ctx ctx;
	for (int i = 0; i < 100; i++) {
        ctx.elems_len = 0;
		f(&ctx);
        ctx.tests ++;
	}
}

void ptest_fail(ptest_ctx *ctx) {
	// TODO: print generated data of failed runv ds
    for(int i = 0; i < ctx->elems_len; i ++) {
        printf("gen %s: %d\n", ctx->elems[i].name, ctx->elems[i].value);
    }
    printf("Tests: %d\n", ctx->tests + 1);
    assert(0);
}

int ptest_int(ptest_ctx *ctx, char* name) {
	int val = rand();
	strcpy(ctx->elems[ctx->elems_len].name, name);
	ctx->elems[ctx->elems_len].value = val;
    ctx->elems_len ++;
	return val;
}

int ptest_int_range(ptest_ctx *ctx, int from, int to, char *name) {
	int val = rand() % (to - from) + from;
	strcpy(ctx->elems[ctx->elems_len].name, name);
	ctx->elems[ctx->elems_len].value = val;
    ctx->elems_len ++;
	return val;
}
