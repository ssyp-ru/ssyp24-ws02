#include "fs.h"
#include "allocator.h"
#include "block_device.h"
#include "inode.h"
#include "super_block.h"

#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

fs_t filesystem;

void create_new_fs(block_device_t *dev) {
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
    super_block->total_bitmap_blocks = super_block->total_blocks / 8 / FS_BLOCK_SIZE;

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
    b->name_len = 1;
    b->name[0] = '/';
    inode->dir_blocks[0] = *b;

    // super_block->inode = *inode;
    char *bbytes = malloc(sizeof(super_block_t));
    super_block_to_bytes(super_block, bbytes);
    char *ibytes = malloc(sizeof(inode_t));
    inode_to_bytes(inode, ibytes);

    set_blocks(dev, 0, 1, bbytes);
    set_blocks(dev, 1, 1, ibytes);

    free(super_block);
    free(inode);
    free(b);
    free(bbytes);
    free(ibytes);
}

void initialize_fs(block_device_t *dev) {
    filesystem.dev = dev;
    filesystem.super_block = malloc(sizeof(super_block_t));
    filesystem.root_inode = malloc(sizeof(inode_t));

    char *bytes = malloc(FS_BLOCK_SIZE);
    get_blocks(filesystem.dev, 0, 1, bytes);
    bytes_to_super_block(bytes, filesystem.super_block);

    get_blocks(filesystem.dev, 1, 1, bytes);
    bytes_to_inode(bytes, filesystem.root_inode);

    free(bytes);
}

fs_t *get_fs() {
    return &filesystem;
}

const char *get_next_slash(const char *s) {
    while (*s != 0) {
        if (*s == '/') {
            break;
        }
        s++;
    }
    return s;
}

int add_inode_to_fs(inode_t *parent, int parent_block_id, inode_t *new, char *name, int name_len, fs_t *fs) {
    block_section block_id = allocate_blocks(fs, 1);
    char *bytes = malloc(FS_BLOCK_SIZE);
    inode_to_bytes(new, bytes);
    set_blocks(fs->dev, block_id.start, 1, bytes);

    // TODO: who allocate this memory?
    parent->dir_blocks[parent->data_elems].block_id = block_id.start;
    parent->dir_blocks[parent->data_elems].type = new->type;
    parent->dir_blocks[parent->data_elems].inode_id = new->inode_id;
    strncpy(parent->dir_blocks[parent->data_elems].name, name, name_len);
    parent->dir_blocks[parent->data_elems].name_len = name_len;
    parent->data_elems++;

    inode_to_bytes(parent, bytes);
    set_blocks(fs->dev, parent_block_id, 1, bytes);

    free(bytes);
    return block_id.start;
}

inode_t *search_inode_by_path(const char *path, fs_t *fs, unit64_t* block_id) {
    inode_t *inode = fs->root_inode;
    path++; // get rid of first '/'

    char *bytes = malloc(FS_BLOCK_SIZE);

    while (1) {
        const char *next = get_next_slash(path);

        if (inode->type == INODE_TYPE_REGULAR) {
            // TODO:
        }

        int name_len = next - path;
        int found = 0;
        for (int i = 0; i < inode->data_elems; i++) {
            if (name_len != inode->dir_blocks[i].name_len) {
                continue;
            }
            if (strncmp(path, inode->dir_blocks[i].name, name_len) != 0) {
                continue;
            }

            get_blocks(fs->dev, inode->dir_blocks[i].block_id, 1, bytes);

            inode_t *cur = malloc(sizeof(inode_t));

            bytes_to_inode(bytes, cur);
            inode = cur;
            block_id = inode->dir_blocks[i].block_id;
            found = 1;
            break;
        }
        if (!found) {
            return NULL;
        }
        if (*next == 0) {
            return inode;
        }
        path = next + 1;
    }
}

void search_inode_by_path_test() {
    block_device_t dev;
    open_in_memory_block_device(&dev, 100 * 1024);
    create_new_fs(&dev);

    initialize_fs(&dev);
    fs_t *fs = get_fs();

    inode_t *dir1 = malloc(sizeof(inode_t));
    dir1->inode_id = 3;
    dir1->magic = INODE_MAGIC;
    dir1->type = INODE_TYPE_DIRECTORY;
    dir1->uid = 1;
    dir1->gid = 1;
    dir1->flags = S_IFDIR | 0755;
    dir1->file_size = 0;
    dir1->data_type = 0;
    dir1->data_elems = 0;
    int block_id = add_inode_to_fs(fs->root_inode, fs->super_block->root_dir_block_id, dir1, "dir1", 4, fs);
    inode_t *dir2 = malloc(sizeof(inode_t));
    dir2->inode_id = 4;
    dir2->magic = INODE_MAGIC;
    dir2->type = INODE_TYPE_DIRECTORY;
    dir2->uid = 1;
    dir2->gid = 1;
    dir2->flags = S_IFDIR | 0755;
    dir2->file_size = 0;
    dir2->data_type = 0;
    dir2->data_elems = 0;
    block_id = add_inode_to_fs(dir1, block_id, dir2, "dir2", 4, fs);

    inode_t *dir3 = malloc(sizeof(inode_t));
    dir3->inode_id = 5;
    dir3->magic = INODE_MAGIC;
    dir3->type = INODE_TYPE_DIRECTORY;
    dir3->uid = 1;
    dir3->gid = 1;
    dir3->flags = S_IFDIR | 0755;
    dir3->file_size = 0;
    dir3->data_type = 0;
    dir3->data_elems = 0;
    add_inode_to_fs(dir2, block_id, dir3, "dir3", 4, fs);

    inode_t* file1 = malloc(sizeof(inode_t));
    file1->inode_id = 6;
    file1->magic = INODE_MAGIC;
    file1->type = INODE_TYPE_REGULAR;
    file1->uid = 1;
    file1->gid = 1;
    file1->flags = S_IFREG | 0666;
    file1->file_size = 0;
    file1->data_type = 0;
    file1->data_elems = 0;
    add_inode_to_fs(fs->root_inode, fs->super_block->root_dir_block_id, file1, "file1", 5, fs);

    inode_t *s1 = search_inode_by_path("/dir1", fs);
    printf("%ld\n", s1->inode_id);

    inode_t *s2 = search_inode_by_path("/dir1/dir2", fs);
    assert(s2 != NULL);
    printf("%ld\n", s2->inode_id);

    inode_t *s3 = search_inode_by_path("/dir1/dir2/dir3", fs);
    printf("%ld\n", s3->inode_id);

    inode_t *s4 = search_inode_by_path("/file1", fs);
    printf("%p\n", (void*)s4);
}
