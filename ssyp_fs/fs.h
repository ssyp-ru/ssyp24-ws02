#ifndef FS_H_
#define FS_H_

#include "block_device.h"
#include "super_block.h"
#include "inode.h"

#define FS_BLOCK_SIZE 512

typedef struct {
    super_block_t *super_block;
    block_device_t *dev;
    inode_t *root_inode;
} fs_t;

void create_new_fs(block_device_t *dev);
void initialize_fs(block_device_t *dev);
fs_t* get_fs();

inode_t* search_inode_by_path(char *path, fs_t *fs);

// tests

void search_inode_by_path_test();

#endif
