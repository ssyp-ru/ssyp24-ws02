#include "block_device.h"
#include "super_block.h"
#include "inode.h"

#define FS_BLOCK_SIZE = 4096

typedef struct {
    super_block_t *super_block;
    block_device_t *dev;
    inode_t *inode;
} fs_t;

void create_new_fs(char* path);
void initialize_fs(char* path);
fs_t* get_fs();
