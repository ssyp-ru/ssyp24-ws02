#include "fs_command.h"

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

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = 20;
        stbuf->st_uid = 666;
        stbuf->st_gid = 666;
    }

    return 0;
}

int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi,
               enum fuse_readdir_flags flags) {
    printf("FUSE: do_readdir: %s\n", path);

    filler(buffer, ".", NULL, 0, 0);  // Current Directory
    filler(buffer, "..", NULL, 0, 0); // Parent Directory

    if (strcmp(path, "/") == 0) // If the user is trying to show the files/directories of the root
                                // directory show the following
    {
        filler(buffer, "hello1", NULL, 0, 0);
        filler(buffer, "hello2", NULL, 0, 0);
    }

    return 0;
}

int do_open(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_open=%s\n", path);
    if (strcmp(path + 1, "hello1") != 0)
        return -ENOENT;

    if ((fi->flags & O_ACCMODE) != O_RDONLY)
        return 0;
    // return -EACCES;

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
    printf("FUSE: do_create, path=%s\n", path);
    return 0;
}

int do_access(const char *path, int wtf) {
    printf("FUSE: do_access, path=%s\n", path);
    return 0;
}
