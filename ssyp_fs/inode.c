#include "inode.h"
#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

char *short_form_to_bytes(inode_directory_short_form_t *entry, char *bytes) {
    bytes[0] = entry->type;
    int_to_bytes64(entry->inode_id, bytes + 1);
    int_to_bytes64(entry->block_id, bytes + 5);
    bytes[9] = entry->name_len;
    memcpy(bytes + 10, entry->name, entry->name_len);
    return bytes + 10 + entry->name_len;
}

char *bytes_to_dir_short_form(char *bytes, inode_directory_short_form_t *entry) {
    entry->type = bytes[0];
    entry->inode_id = bytes_to_int64(bytes + 1);
    entry->block_id = bytes_to_int64(bytes + 5);
    entry->name_len = bytes[9];
    memcpy(entry->name, bytes + 10, entry->name_len);
    return bytes + 10 + entry->name_len;
}

void inode_to_bytes(inode_t *inode, char *bytes) {
    int_to_bytes(inode->magic, bytes);
    int_to_bytes64(inode->inode_id, bytes + 4);
    int_to_bytes(inode->uid, bytes + 12);
    int_to_bytes(inode->gid, bytes + 16);
    int_to_bytes(inode->flags, bytes + 20);
    int_to_bytes(inode->atime, bytes + 24);
    int_to_bytes(inode->mtime, bytes + 28);
    int_to_bytes(inode->ctime, bytes + 32);
    int_to_bytes64(inode->file_size, bytes + 36);
    bytes[44] = (char)(inode->type);
    bytes[45] = (char)(inode->data_type);
    int_to_bytes(inode->data_elems, bytes + 46);
    if (inode->data_type == INODE_TYPE_DIRECTORY) {
        // inode->dir_blocks = malloc(inode->data_elems * sizeof(inode_directory_short_form_t));
        bytes = bytes + 50;
        for (int i = 0; i < inode->data_elems; i++) {
            bytes = short_form_to_bytes(&inode->dir_blocks[i], bytes);
        }
    } else {
        assert(0 && "not supported yet");
    }
}

void bytes_to_inode(char *bytes, inode_t *inode) {
    inode->magic = bytes_to_int(bytes);
    inode->inode_id = bytes_to_int(bytes + 4);
    inode->uid = bytes_to_int(bytes + 12);
    inode->gid = bytes_to_int(bytes + 16);
    inode->flags = bytes_to_int(bytes + 20);
    inode->atime = bytes_to_int(bytes + 24);
    inode->mtime = bytes_to_int(bytes + 28);
    inode->ctime = bytes_to_int(bytes + 32);
    inode->file_size = bytes_to_int64(bytes + 36);
    inode->type = bytes[44];
    inode->data_type = bytes[45];
    inode->data_elems = bytes_to_int(bytes + 46);
    // inode->dir_blocks = NULL;
    inode->file_block = NULL;
    if (inode->data_type == INODE_TYPE_DIRECTORY) {
        // inode->dir_blocks = malloc(inode->data_elems * sizeof(inode_directory_short_form_t));
        bytes = bytes + 50;
        for (int i = 0; i < inode->data_elems; i++) {
            bytes = bytes_to_dir_short_form(bytes, &inode->dir_blocks[i]);
        }
    } else {
        assert(0 && "not supported yet");
    }
}
