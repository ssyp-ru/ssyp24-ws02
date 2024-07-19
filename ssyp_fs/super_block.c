#include "super_block.h"
#include "utils.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void super_block_to_bytes(super_block_t *super_block, char *bytes) {
    int_to_bytes(super_block->magic, bytes);
    int_to_bytes(super_block->block_size, bytes + 4);
    int_to_bytes64(super_block->total_blocks, bytes + 8);
    int_to_bytes64(super_block->used_blocks, bytes + 16);
    int_to_bytes64(super_block->first_journal_block, bytes + 24);
    int_to_bytes64(super_block->total_journal_blocks, bytes + 32);
    int_to_bytes64(super_block->first_bitmap_blocks, bytes + 48);
    int_to_bytes(super_block->total_bitmap_blocks, bytes + 56);
    bytes[60] = (char)(super_block->version);
    int_to_bytes64(super_block->root_dir_block_id, bytes + 61);
    // inode_to_bytes(super_block.root_dir, bytes + 61);
}

void bytes_to_super_block(char *bytes, super_block_t *super_block) {
    super_block->magic = bytes_to_int(bytes);
    super_block->block_size = bytes_to_int(bytes + 4);
    super_block->total_blocks = bytes_to_int64(bytes + 8);
    super_block->used_blocks = bytes_to_int64(bytes + 16);
    super_block->first_journal_block = bytes_to_int64(bytes + 24);
    super_block->total_journal_blocks = bytes_to_int64(bytes + 32);
    super_block->first_bitmap_blocks = bytes_to_int64(bytes + 48);
    super_block->total_bitmap_blocks = bytes_to_int64(bytes + 56);
    super_block->version = bytes[60];
    super_block->root_dir_block_id = bytes_to_int64(bytes + 61);
    // bytes_to_inode(bytes + 61, &(super_block->root_dir));
}

void super_block_to_bytes_test() {
    super_block_t* super_block = malloc(sizeof(super_block_t));
    super_block_t* super_block_res = malloc(sizeof(super_block_t));
    char bytes[sizeof(super_block_t)];

    super_block->magic = SUPER_BLOCK_MAGIC;
    super_block->version = CURRENT_VERSION;
    super_block->block_size = 4096;
    super_block->total_blocks = 262144;
    super_block->used_blocks = 5;
    super_block->root_dir_block_id = 1;
    super_block->first_journal_block = 10;
    super_block->total_journal_blocks = 8;
    super_block->first_bitmap_blocks = 90;
    super_block->total_bitmap_blocks = 12;
    printf("\n----\n");
    printf("%d, %d, %d, %ld, %ld, %ld, %ld, %ld, %ld, %d",
        super_block->magic, super_block->version, super_block->block_size,
        super_block->total_blocks, super_block->used_blocks, super_block->root_dir_block_id,
        super_block->first_journal_block, super_block->total_journal_blocks,
        super_block->first_bitmap_blocks, super_block->total_bitmap_blocks);

    super_block_to_bytes(super_block, bytes);
    printf("\nbytes: ");
    for (int i = 0; i < 69; i++) printf("%c", bytes[i]);
    bytes_to_super_block(bytes, super_block_res);
    printf("\n----\n");
    printf("%d, %d, %d, %ld, %ld, %ld, %ld, %ld, %ld, %d",
        super_block->magic, super_block->version, super_block->block_size,
        super_block->total_blocks, super_block->used_blocks, super_block->root_dir_block_id,
        super_block->first_journal_block, super_block->total_journal_blocks,
        super_block->first_bitmap_blocks, super_block->total_bitmap_blocks);

    assert(super_block->magic ==super_block_res->magic);
    assert(super_block->version == super_block_res->version);
    assert(super_block->block_size == super_block_res->block_size);
    assert(super_block->total_blocks == super_block_res->total_blocks);
    assert(super_block->used_blocks == super_block_res->used_blocks);
    assert(super_block->root_dir_block_id == super_block_res->root_dir_block_id);
    assert(super_block->first_journal_block == super_block_res->first_journal_block);
    assert(super_block->total_journal_blocks == super_block_res->total_journal_blocks);
    assert(super_block->first_bitmap_blocks == super_block_res->first_bitmap_blocks);
    assert(super_block->total_bitmap_blocks == super_block_res->total_bitmap_blocks);
}
