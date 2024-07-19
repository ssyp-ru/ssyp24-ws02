#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "block.h"
#include "fs.h"

block_section allocate_blocks(fs_t *fs, int num);

#endif
