#include "block_device.h"
#include "inode.h"
#include "super_block.h"
#include "fs.h"
#include <stdlib.h>

fs_t filesystem;

void create_new_fs(char *path) {
    super_block_t *super_block = malloc(sizeof(super_block_t));
    super_block->magic = SUPER_BLOCK_MAGIC;
    super_block->version = CURRENT_VERSION;
    super_block->block_size = 4096;
    super_block->total_blocks = 262144;
    super_block->used_blocks = 2;
    super_block->root_dir_block_id = 1;
    super_block->first_journal_block = 0;
    super_block->total_journal_blocks = 0;
    super_block->first_bitmap_blocks = 2;
    super_block->total_bitmap_blocks = super_block->total_blocks/8/4096;

    inode_t *inode = malloc(sizeof(inode_t));
    inode->magic = INODE_MAGIC;
    inode->type = INODE_TYPE_DIRECTORY;
    inode->inode_id = 0;
    inode->uid = 1;
    inode->gid = 1;
    inode->flags = 0;
    inode->atime = 0;
    inode->mtime = 0;
    inode->ctime = 0;
    inode->file_size = 0;
    inode->data_type = 0;
    inode->data_elems = 0;
    inode->file_block = NULL;
    
    // Create a serialize function for this and then save it to a block.
    inode_directory_short_form_t *b = malloc(sizeof(inode_directory_short_form_t));
    b->type = INODE_TYPE_DIRECTORY;
    b->inode_id = 0;
    b->block_id = 1;
    b->name_len = 4;
    b->name = "root";
    inode->dir_blocks = b;

    // super_block->inode = *inode;
    char *bbytes = malloc(sizeof(super_block_t));
    super_block_to_bytes(super_block, bbytes);
    char *ibytes = malloc(sizeof(inode_t));
    inode_to_bytes(inode, ibytes);

    block_device_t *block_device = malloc(sizeof(block_device_t));
    filesystem.dev = block_device;
    open_block_device(path, block_device);
    set_blocks(block_device, 0, 1, bbytes);
    set_blocks(block_device, 1, 1, ibytes);
    close_block_device(block_device);
    free(super_block);
    free(inode);
    free(b);
    free(bbytes);
    free(ibytes);
}

void initialize_fs(char* path) {
    filesystem.dev = malloc(sizeof(block_device_t));
    filesystem.super_block = malloc(sizeof(super_block_t));
    filesystem.inode = malloc(sizeof(inode_t));
    open_block_device(path, filesystem.dev);
    char* bbytes = malloc(512);
    char* ibytes = malloc(512);
    get_blocks(filesystem.dev, 0, 1, bbytes);
    get_blocks(filesystem.dev, 1, 1, ibytes);
    bytes_to_super_block(bbytes, filesystem.super_block);
    bytes_to_inode(ibytes, filesystem.inode);
}

fs_t* get_fs() {
    return &filesystem;
} 
