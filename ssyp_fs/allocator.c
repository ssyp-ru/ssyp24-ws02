#include "allocator.h"
#include "block.h"

block_section allocate_blocks(fs *fs, int num) {
    block_section b;
    b.start = fs->super_block->used_blocks;
    b.len = num;

    fs->super_block->used_blocks += num;

    // TODO: save superblock
    return b;
}
