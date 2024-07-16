#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "block.h"
#include "fs.h"

block_sect allocate_blocks(fs *fs, int num);

#endif
