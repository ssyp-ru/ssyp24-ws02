typedef struct {
    char name[16];
    int value;
} ptest_ctx;
void ptest_run(void (*f)(ptest_ctx *ctx));
void ptest_fail(ptest_ctx *ctx);
int ptest_int(ptest_ctx *ctx, char *name);
int ptest_int_range(ptest_ctx *ctx, int from, int to, char *name);


