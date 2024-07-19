#include "super_block.h"
#include "utils.h"

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
