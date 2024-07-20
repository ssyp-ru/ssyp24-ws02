#ifndef INODE_H_
#define INODE_H_

#include <stdint.h>
#include "block.h"
#define INODE_MAGIC 0x123456

enum inode_type{
    INODE_TYPE_DIRECTORY = 0,
    INODE_TYPE_REGULAR   = 1,
};

typedef struct {
    uint8_t type;
    uint64_t inode_id;
    uint64_t block_id;
    uint8_t name_len;
    char name[256];
} inode_directory_short_form_t ;

typedef struct {
    // header
    uint32_t magic;
    uint8_t type;
    uint64_t inode_id;
    int32_t uid;
    int32_t gid;
    uint32_t flags; // uid, gid permissions
    uint32_t atime; // TODO: add nanoseconds?
    uint32_t mtime;
    uint32_t ctime;
    uint64_t file_size;
    
    // data
    uint8_t data_type; // 0 - inode short form, 1 - file data blocks
    uint32_t data_elems;
    // One of them will be NULL (depending of inode type (dir/reg)):
    inode_directory_short_form_t dir_blocks[100];
    block_section* file_block;
    // and now we have 'data_elems' elements of types depending on 'data_type'
} inode_t;


void inode_to_bytes(inode_t *inode, char *bytes);
void bytes_to_inode(char *bytes, inode_t *inode);


#endif
