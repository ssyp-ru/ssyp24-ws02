#ifndef BLOCK_H_
#define BLOCK_H_

#include <stdint.h>

#include "inode.h"

#define SUPER_BLOCK_MAGIC 0x53535950

typedef struct {
    uint16_t start;
    uint16_t len;
} block_sect;


typedef struct {
    char name[256]; // TODO: constant, TODO: need this??
    uint32_t magic;
    uint8_t major_ver;
    uint8_t minor_ver;

    uint32_t block_size;
    uint64_t total_blocks;
    uint64_t used_blocks;

    inode_t root_dir;
    uint64_t first_journal_block;
    uint64_t total_journal_blocks;

    uint64_t first_bitmap_blocks;
    uint32_t total_bitmap_blocks;

    // TODO: padding
} super_block;

#endif
