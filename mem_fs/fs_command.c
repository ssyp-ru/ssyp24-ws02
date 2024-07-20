#include "fs_command.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_FILE_NUM 10
#define MAX_FILE_SIZE 256

typedef struct {
    char path[256];
    char data[MAX_FILE_SIZE];
    int data_len;
    int is_directory;
    struct stat stat;
} inode;

inode files[MAX_FILE_NUM];
int files_len = 0;

int do_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    printf("FUSE: do_getattr: %s\n", path);
    printf("files_len: %d\n", files_len);

    memset(stbuf, 0, sizeof(struct stat));
    for (int i = 0; i < files_len; i++) {
        printf("%s %s\n", files[i].path, path);
        if (strcmp(files[i].path, path) == 0) {
            *stbuf = files[i].stat;
            return 0;
        }
    }

    return -ENOENT;
}

int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi,
               enum fuse_readdir_flags flags) {
    printf("FUSE: do_readdir: %s\n", path);

    filler(buffer, ".", NULL, 0, 0);  // Current Directory
    filler(buffer, "..", NULL, 0, 0); // Parent Directory
    printf("files_len: %d\n", files_len);

    if (strcmp(path, "/") == 0) { // If the user is trying to show the files/directories of the root
                                  // directory show the following
        for (int i = 1; i < files_len; i++) {
            filler(buffer, files[i].path + 1, NULL, 0, 0);
            printf("%s\n", files[i].path + 1);
        }
    }
    return 0;
}

int do_open(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_open=%s\n", path);
    for (int i = 0; i < files_len; i++) {
        if (strcmp(files[i].path, path) == 0) {
            return 0;
        }
    }
    if ((fi->flags & O_ACCMODE) != O_RDONLY) {
        return 0;
    }
    // return -EACCES;

    return -ENOENT;
}

int do_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    printf("FUSE: do_read: path=%s buf_size=%ld offset=%ld\n", path, size, offset);
    // size_t len;
    // (void)fi;
    for (int i = 0; i < files_len; i++) {
        if (strcmp(files[i].path, path) == 0) {
            if (offset < files[i].data_len) {
                if (offset + size > files[i].data_len)
                    size = files[i].data_len - offset;
                //  memcpy(buf, files[i].data + offset, size);
                memcpy(buf, files[i].data, size);
                return size;
            }
        }
    }
    return -ENOENT;
}

//         000         000
//       0  0  0     0  0  0
//       0000000     0000000
//       0  0  0     0  0  0
//         000         000

/** Create a directory
 *
 * Note that the mode argument may not have the type specification
 * bits set, i.e. S_ISDIR(mode) can be false.  To obtain the
 * correct directory type bits use  mode|S_IFDIR
 * */
int do_mkdir(const char *path, mode_t mode) {
    printf("FUSE: do_mkdir, path=%s\n", path);
    for (int i = 0; i < files_len; i++) {
        if (strcmp(files[i].path, path) == 0) {
            return -EEXIST;
        }
    }
    struct stat hello_stat;
    hello_stat.st_mode = S_IFDIR | 0755;
    hello_stat.st_nlink = 1;
    hello_stat.st_size = 0;
    hello_stat.st_uid = 666;
    hello_stat.st_gid = 666;
    strcpy(files[files_len].path, path);
    files[files_len].stat = hello_stat;
    files[files_len].data_len = 0;
    files_len++;
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
    printf("FUSE: do_write, path=%s, offset=%ld\n", path, offset);
    for (int i = 0; i < files_len; i++) {
        if (strcmp(files[i].path, path) == 0) {
            files[i].data_len += buffer_size;
            printf("-> %c %c %c %c\n", files[i].data[4], files[i].data[5], files[i].data[6], files[i].data[7]);
            memcpy(files[i].data + offset, buffer, buffer_size);
            printf("-> %c %c %c %c\n", files[i].data[4], files[i].data[5], files[i].data[6], files[i].data[7]);
            printf("%s\n---\n%s %ld %d\n", files[i].data, buffer, buffer_size, files[i].data_len);
            return buffer_size;
        }
    }
    return -ENOENT;
    //   printf("FUSE: do_write, path=%s\n", path);
    //  return buffer_size;
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
    struct stat hello_stat;
    hello_stat.st_mode = S_IFREG | 0666;
    hello_stat.st_nlink = 1;
    hello_stat.st_size = 0;
    hello_stat.st_uid = 666;
    hello_stat.st_gid = 666;
    strcpy(files[files_len].path, path);
    files[files_len].stat = hello_stat;
    files[files_len].data_len = 0;
    files_len++;
    return 0;
}
/**
 * Check file access permissions
 *
 * This will be called for the access() system call.  If the
 * 'default_permissions' mount option is given, this method is not
 * called.
 *
 * This method is not called under Linux kernel versions 2.4.x
 */
int do_access(const char *path, int wtf) {
    printf("FUSE: do_access, path=%s\n", path);
    return 0;
}

void init_fs() {
    printf("init_fs call\n");
    // initialize files with "hello1"
    struct stat hello_stat;
    memset(&hello_stat, 0, sizeof(struct stat));
    hello_stat.st_mode = S_IFDIR | 0755;
    hello_stat.st_nlink = 2;
    strcpy(files[files_len].path, "/");
    files[files_len].stat = hello_stat;
    files[files_len].is_directory = 1;
    files_len++;
}
