#include "block_device.h"
#include "super_block.h"

#define FS_BLOCK_SIZE = 4096

typedef struct {
    super_block_t *super_block;
    block_device_t *dev;
} fs_t;

void create_new_fs(char* path);
