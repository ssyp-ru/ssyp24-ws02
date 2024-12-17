#ifndef PTEST_H
#define PTEST_H

#include <stdio.h>
#include <aio.h>
typedef struct {
    char name[16];
    int value;
} ptest_elem;
typedef struct {
    ptest_elem elems[256];
    int elems_len;
    int tests;
} ptest_ctx;
void ptest_run(void (*f)(ptest_ctx *ctx));
void ptest_fail(ptest_ctx *ctx);
int ptest_int(ptest_ctx *ctx, char *name);
int ptest_int_range(ptest_ctx *ctx, int from, int to, char *name);

#endif
