#include "block_device.h"
#include "block.h"

#define FS_BLOCK_SIZE = 4096

typedef struct {
    // TODO: current superblock
    super_block *super_block;

    block_device *dev;
} fs;
