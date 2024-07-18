#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "block.h"
#include "inode.h"
#include "block_device.h"
#include "block.c"

void open_block_device(char *file_path, block_device *dev) {
    // TODO: allocate whole file?
    // TODO: direct io to bench?
    dev->fd = open(file_path, O_CREAT | O_RDWR);
}

void close_block_device(block_device *dev) {
    close(dev->fd);
}

void get_blocks(block_device *dev, int from_block_id, int num, char *bytes) {
    // TODO: check for max device size
    pread(dev->fd, bytes, num * DEVICE_BLOCK_SIZE, from_block_id * DEVICE_BLOCK_SIZE);
}

void set_blocks(block_device *dev, int from_block_id, int num, char *bytes) {
    // TODO: check for max device size
    pwrite(dev->fd, bytes, num * DEVICE_BLOCK_SIZE, from_block_id * DEVICE_BLOCK_SIZE);
}

void is_exist(char* path) {
	if (stat(path) == 0) return;
        
    FILE* a = fopen(path, "w");
    fclose(a);
	
    super_block* = malloc(sizeof(super_block));
    super_block->magic = SUPER_BLOCK_MAGIC;
    super_block->version = CURRENT_VERSION;
    super_block->block_size = 4096;
    super_block->total_blocks = 262144;
    super_block->used_blocks = 0;
    super_block->first_journal_block;
    super_block->total_journal_blocks;
    super_block->first_bitmap_blocks;
    super_block->total_bitmap_blocks;

    inode* = malloc(sizeof(inode));
    inode->magic = 0x123456;
    inode->type = INODE_TYPE_DIRECTORY;
    inode->inodeid = 2;
    inode->uid = 1;
    inode->gid = 1;
    inode->flags = 0;
    inode->atime = 0;
    inode->mtime = 0;
    inode->ctime = 0;
    inode->file_size = 0;
    inode->data_type = 0;
    inode->data_elems = 0;
    inode->file_node = NULL;
    
    inode_directory_short_form_t* b = malloc(sizeof(inode_directory_short_form_t));
    b->type = INODE_TYPE_DIRECTORY;
    b->inode_id = 2;
    b->block_id = 0;
    b->name_len = 4;
    b->name = "root";
    inode->dir_blocks = b;

    //super_block->inode = *inode;
    char* bbytes = malloc(sizeof(super_block));
    super_block_to_bytes(super_block, bbytes);
    char* ibytes = malloc(sizeof(inode));
    inode_to_bytes(inode, ibytes);

    block_device* block_device = malloc(sizeof(block_device));
    open_block_device(path, block_device);
    set_blocks(block_device, 0, 1, bbytes);
    set_blocks(block_device, 1, 1, ibytes);
    close_block_device(block_device);
}
