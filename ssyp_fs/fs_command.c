#include "fs_command.h"
#include "inode.h"
#include "fs.h"
#include "allocator.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int do_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    printf("FUSE: do_getattr: %s\n", path);
    
    inode_t* current_inode = search_inode_by_path(path, get_fs());
    memset(stbuf, 0, sizeof(struct stat));
    if (!current_inode) return -ENOENT;
    if (current_inode->type == INODE_TYPE_DIRECTORY) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    } else {
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = 20;
        stbuf->st_uid = 666;
        stbuf->st_gid = 666;
        return 0;
    }
}

int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi,
               enum fuse_readdir_flags flags) {
    printf("FUSE: do_readdir: %s\n", path);

    //filler(buffer, ".", NULL, 0, 0);  // Current Directory
    //filler(buffer, "..", NULL, 0, 0); // Parent Directory
    
    inode_t* current_inode = search_inode_by_path(path, get_fs());
    if (!current_inode) return -ENOENT;
    for (int i = 0; i < current_inode->data_elems; i++) {
        filler(buffer, current_inode->dir_blocks[i].name, NULL, 0, 0);
    }

    return 0;
}

int do_open(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_open=%s\n", path);
    inode_t* current_inode = search_inode_by_path(path, get_fs());
    if (!current_inode) return -ENOENT;
    return 0;
}

int do_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    printf("FUSE: do_read: path=%s buf_size=%ld offset=%ld\n", path, size, offset);
    size_t len;
    (void)fi;
    if (strcmp(path + 1, "hello1") != 0)
        return -ENOENT;

    char *data = "hi from fuse\n";
    len = strlen(data);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, data + offset, size);
    } else
        size = 0;

    return size;
}

int do_mkdir(const char *path, mode_t mode) {
    printf("FUSE: do_mkdir, path=%s\n", path);
    return 0;
}

int do_rmdir(const char *path) {
    printf("FUSE: do_rmdir, path=%s\n", path);
    return 0;
}

int do_rename(const char *oldpath, const char *newpath, unsigned int flags) {
    printf("FUSE: do_rename, oldpath=%s newpath=%s\n", oldpath, newpath);
    return 0;
}

int do_chmod(const char *path, mode_t mode, struct fuse_file_info *fi) {
    printf("FUSE: do_chmod, path=%s\n", path);
    return 0;
}

int do_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi) {
    printf("FUSE: do_chown, path=%s\n", path);
    return 0;
}

int do_truncate(const char *path, off_t offset, struct fuse_file_info *fi) {
    printf("FUSE: do_truncate, path=%s\n", path);
    return 0;
}

int do_write(const char *path, const char *buffer, size_t buffer_size, off_t offset, struct fuse_file_info *fi) {
    printf("FUSE: do_write, path=%s\n", path);
    return buffer_size;
}

int do_statfs(const char *path, struct statvfs *stat) {
    printf("FUSE: do_statfs, path=%s\n", path);
    return 0;
}

int do_flush(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_flush, path=%s\n", path);
    return 0;
}

int do_release(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_release, path=%s\n", path);
    return 0;
}

int do_fsync(const char *path, int wtf, struct fuse_file_info *fi) {
    printf("FUSE: do_fsync, path=%s\n", path);
    return 0;
}

int do_releasedir(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_releasedir, path=%s\n", path);
    return 0;
}

int do_fsyncdir(const char *path, int wft, struct fuse_file_info *fi) {
    printf("FUSE: do_fsyncdir, path=%s\n", path);
    return 0;
}

void *do_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
    printf("FUSE: init\n");
    return NULL;
}

void do_destroy(void *private_data) {
    printf("FUSE: do_destroy\n");
}

int do_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    block_section section = allocate_blocks(get_fs(), 100);
    inode_t* file1 = malloc(sizeof(inode_t));
    file1->inode_id = get_fs()->super_block->used_blocks+1;
    file1->magic = INODE_MAGIC;
    file1->type = INODE_TYPE_REGULAR;
    file1->uid = 1;
    file1->gid = 1;
    file1->flags = S_IFREG | 0666;
    file1->file_size = 0;
    file1->data_type = 0;
    file1->data_elems = 0;

    char cur_c;
    int cur_i = strlen(path)-1;
    while (cur_c != '/') {
        cur_c = path[cur_i];
        cur_i--;
    }
    char* parent_path;
    strncpy(parent_path, path, cur_i+1);
    uint64_t* block_id;
    inode_t* parent = search_inode_by_path(parent_path, get_fs());
    add_inode_to_fs(parent, *block_id, file1, path+cur_i+1, strlen(path+cur_i+1), get_fs());

    printf("FUSE: do_create, path=%s\n", path);
    return 0;
}

int do_access(const char *path, int wtf) {
    printf("FUSE: do_access, path=%s\n", path);
    return 0;
}
