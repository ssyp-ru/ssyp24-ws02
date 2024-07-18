#include "fs_command.h"

#include <assert.h>
#include <string.h>

void write_test() {
    const char *filename = "/test.txt";
    do_create(filename, 0, NULL);

    int write_size = do_write(filename, "hello", 5, 0, NULL);
    assert(write_size == 5);

    char buffer[256];
    int read_size = do_read(filename, buffer, 256, 0, NULL);

    assert(read_size == 5);
    assert(memcmp("hello", buffer, 5) == 0);
}
