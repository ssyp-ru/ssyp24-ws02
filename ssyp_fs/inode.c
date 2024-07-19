#include "inode.h"
#include "utils.h"

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
}
