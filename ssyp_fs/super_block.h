#ifndef SUPER_BLOCK_H_
#define SUPER_BLOCK_H_

#include "inode.h"

#define SUPER_BLOCK_MAGIC 0x50595353
#define CURRENT_VERSION 1

typedef struct {
    uint32_t magic;
    uint8_t version;

    uint32_t block_size;
    uint64_t total_blocks;
    uint64_t used_blocks;

    // inode root_dir; (???????????????????)
    
    uint64_t root_dir_block_id;
    
    uint64_t first_journal_block;
    uint64_t total_journal_blocks;

    uint64_t first_bitmap_blocks;
    uint32_t total_bitmap_blocks;

    // TODO: padding
} super_block_t;

void super_block_to_bytes(super_block_t *super_block, char *bytes);
void bytes_to_super_block(char *bytes, super_block_t *super_block);

#endif
