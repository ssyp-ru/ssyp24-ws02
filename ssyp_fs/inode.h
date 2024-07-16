#ifndef INODE_H_
#define INODE_H_

#include <stdint.h>

typedef struct inode_directory_short_form_entry {
    uint8_t name_len;
    uint8_t file_type;
    uint64_t inode_address;
    char *name;
} inode_directory_short_form_entry_t ;

typedef struct inode_directory_short_form {
    uint8_t entries_num;
    //inode_t* parent;
    // TODO: entries...
} inode_directory_short_form_t;

enum INODE_TYPE {
    INODE_TYPE_DIRECTORY,
    INODE_TYPE_REGULAR,
};

typedef struct inode {
    
    uint32_t magic;
    uint8_t inode_type;
    int32_t uid;
    int32_t gid;
    uint32_t flags; // uid, gid permissions
    uint32_t atime; // TODO: add nanoseconds?
    uint32_t mtime;
    uint32_t ctime;
    uint64_t file_size;

    char object_name[256]; // TODO: variable name size

    // parent addr

    // data stream

    
} inode_t;


#endif
