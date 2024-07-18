#ifndef BLOCK_H_
#define BLOCK_H_

#include <stdint.h>

#include "inode.h"

#define SUPER_BLOCK_MAGIC 0x53535950
#define CURRENT_VERSION 1

typedef struct {
    uint16_t start;
    uint16_t len;
} block_section;


typedef struct {
    uint32_t magic;
    uint8_t version;

    uint32_t block_size;
    uint64_t total_blocks;
    uint64_t used_blocks;

    inode root_dir;
    uint64_t first_journal_block;
    uint64_t total_journal_blocks;

    uint64_t first_bitmap_blocks;
    uint32_t total_bitmap_blocks;

    // TODO: padding
} super_block;

#endif
