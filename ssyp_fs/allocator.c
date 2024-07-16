#include "allocator.h"

block_sect allocate_blocks(fs *fs, int num) {
    block_sect b;
    b.start = fs->super_block->used_blocks;
    b.len = num;

    fs->super_block->used_blocks += num;

    // TODO: save superblock
    return b;
}
