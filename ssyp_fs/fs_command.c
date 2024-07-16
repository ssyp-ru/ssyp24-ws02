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
    struct stat stat;
} file;

file files[MAX_FILE_NUM];
int files_len = 0;

/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored. The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given. In that case it is passed to userspace,
 * but libfuse and the kernel will still assign a different
 * inode for internal use (called the "nodeid").
 *
 * `fi` will always be NULL if the file is not currently open, but
 * may also be NULL if the file is open.
 */
int do_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    printf("FUSE: do_getattr: %s\n", path);
    (void)fi;
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        //} else if (strcmp(path + 1, options.filename) == 0) {
    } else {
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = 20;
        stbuf->st_uid = 666;
        stbuf->st_gid = 666;
    }

    return res;
}

/** Read directory
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 */
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

/** Open a file
 *
 * Open flags are available in fi->flags. The following rules
 * apply.
 *
 *  - Creation (O_CREAT, O_EXCL, O_NOCTTY) flags will be
 *    filtered out / handled by the kernel.
 *
 *  - Access modes (O_RDONLY, O_WRONLY, O_RDWR, O_EXEC, O_SEARCH)
 *    should be used by the filesystem to check if the operation is
 *    permitted.  If the ``-o default_permissions`` mount option is
 *    given, this check is already done by the kernel before calling
 *    open() and may thus be omitted by the filesystem.
 *
 *  - When writeback caching is enabled, the kernel may send
 *    read requests even for files opened with O_WRONLY. The
 *    filesystem should be prepared to handle this.
 *
 *  - When writeback caching is disabled, the filesystem is
 *    expected to properly handle the O_APPEND flag and ensure
 *    that each write is appending to the end of the file.
 *
 *  - When writeback caching is enabled, the kernel will
 *    handle O_APPEND. However, unless all changes to the file
 *    come through the kernel this will not work reliably. The
 *    filesystem should thus either ignore the O_APPEND flag
 *    (and let the kernel handle it), or return an error
 *    (indicating that reliably O_APPEND is not available).
 *
 * Filesystem may store an arbitrary file handle (pointer,
 * index, etc) in fi->fh, and use this in other all other file
 * operations (read, write, flush, release, fsync).
 *
 * Filesystem may also implement stateless file I/O and not store
 * anything in fi->fh.
 *
 * There are also some flags (direct_io, keep_cache) which the
 * filesystem may set in fi, to change the way the file is opened.
 * See fuse_file_info structure in <fuse_common.h> for more details.
 *
 * If this request is answered with an error code of ENOSYS
 * and FUSE_CAP_NO_OPEN_SUPPORT is set in
 * `fuse_conn_info.capable`, this is treated as success and
 * future calls to open will also succeed without being send
 * to the filesystem process.
 *
 */
int do_open(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_open=%s\n", path);
    if (strcmp(path + 1, "hello1") != 0)
        return -ENOENT;

    if ((fi->flags & O_ACCMODE) != O_RDONLY)
        return 0;
    // return -EACCES;

    return 0;
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.	 An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 */
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

/** Create a directory
 *
 * Note that the mode argument may not have the type specification
 * bits set, i.e. S_ISDIR(mode) can be false.  To obtain the
 * correct directory type bits use  mode|S_IFDIR
 * */
int do_mkdir(const char *path, mode_t mode) {
    printf("FUSE: do_mkdir, path=%s\n", path);
    return 0;
}

/** Remove a directory */
int do_rmdir(const char *path) {
    printf("FUSE: do_rmdir, path=%s\n", path);
    return 0;
}

/** Rename a file
 *
 * *flags* may be `RENAME_EXCHANGE` or `RENAME_NOREPLACE`. If
 * RENAME_NOREPLACE is specified, the filesystem must not
 * overwrite *newname* if it exists and return an error
 * instead. If `RENAME_EXCHANGE` is specified, the filesystem
 * must atomically exchange the two files, i.e. both must
 * exist and neither may be deleted.
 */
int do_rename(const char *oldpath, const char *newpath, unsigned int flags) {
    printf("FUSE: do_rename, oldpath=%s newpath=%s\n", oldpath, newpath);
    return 0;
}

/** Change the permission bits of a file
 *
 * `fi` will always be NULL if the file is not currently open, but
 * may also be NULL if the file is open.
 */
int do_chmod(const char *path, mode_t mode, struct fuse_file_info *fi) {
    printf("FUSE: do_chmod, path=%s\n", path);
    return 0;
}

/** Change the owner and group of a file
 *
 * `fi` will always be NULL if the file is not currently open, but
 * may also be NULL if the file is open.
 *
 * Unless FUSE_CAP_HANDLE_KILLPRIV is disabled, this method is
 * expected to reset the setuid and setgid bits.
 */
int do_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi) {
    printf("FUSE: do_chown, path=%s\n", path);
    return 0;
}

/** Change the size of a file
 *
 * `fi` will always be NULL if the file is not currently open, but
 * may also be NULL if the file is open.
 *
 * Unless FUSE_CAP_HANDLE_KILLPRIV is disabled, this method is
 * expected to reset the setuid and setgid bits.
 */
int do_truncate(const char *path, off_t offset, struct fuse_file_info *fi) {
    printf("FUSE: do_truncate, path=%s\n", path);
    return 0;
}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.	 An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Unless FUSE_CAP_HANDLE_KILLPRIV is disabled, this method is
 * expected to reset the setuid and setgid bits.
 */
int do_write(const char *path, const char *buffer, size_t buffer_size, off_t offset, struct fuse_file_info *fi) {
    printf("FUSE: do_write, path=%s\n", path);
    return buffer_size;
}

/** Get file system statistics
 *
 * The 'f_favail', 'f_fsid' and 'f_flag' fields are ignored
 */
int do_statfs(const char *path, struct statvfs *stat) {
    printf("FUSE: do_statfs, path=%s\n", path);
    return 0;
}

/** Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync().  It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor, as opposed to
 * release which is called on the close of the last file descriptor for
 * a file.  Under Linux, errors returned by flush() will be passed to
 * userspace as errors from close(), so flush() is a good place to write
 * back any cached dirty data. However, many applications ignore errors
 * on close(), and on non-Linux systems, close() may succeed even if flush()
 * returns an error. For these reasons, filesystems should not assume
 * that errors returned by flush will ever be noticed or even
 * delivered.
 *
 * NOTE: The flush() method may be called more than once for each
 * open().  This happens if more than one file descriptor refers to an
 * open file handle, e.g. due to dup(), dup2() or fork() calls.  It is
 * not possible to determine if a flush is final, so each flush should
 * be treated equally.  Multiple write-flush sequences are relatively
 * rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will be called at any
 * particular point.  It may be called more times than expected, or not
 * at all.
 *
 * [close]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html
 */
int do_flush(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_flush, path=%s\n", path);
    return 0;
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file handle.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 */
int do_release(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_release, path=%s\n", path);
    return 0;
}

/** Synchronize file contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data.
 */
int do_fsync(const char *path, int wtf, struct fuse_file_info *fi) {
    printf("FUSE: do_fsync, path=%s\n", path);
    return 0;
}

/** Release directory
 */
int do_releasedir(const char *path, struct fuse_file_info *fi) {
    printf("FUSE: do_releasedir, path=%s\n", path);
    return 0;
}

/** Synchronize directory contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data
 */
int do_fsyncdir(const char *path, int wft, struct fuse_file_info *fi) {
    printf("FUSE: do_fsyncdir, path=%s\n", path);
    return 0;
}

/**
 * Initialize filesystem
 *
 * The return value will passed in the `private_data` field of
 * `struct fuse_context` to all file operations, and as a
 * parameter to the destroy() method. It overrides the initial
 * value provided to fuse_main() / fuse_new().
 */
void *do_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
    printf("FUSE: init\n");
    return NULL;
}

/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 */
void do_destroy(void *private_data) {
    printf("FUSE: do_destroy\n");
}

/**
 * Create and open a file
 *
 * If the file does not exist, first create it with the specified
 * mode, and then open it.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the mknod() and open() methods
 * will be called instead.
 */
int do_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    printf("FUSE: do_create, path=%s\n", path);
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
    // initialize files with "hello1"
    struct stat hello_stat;
    hello_stat.st_mode = S_IFREG | 0666;
    hello_stat.st_nlink = 1;
    hello_stat.st_size = 20;
    hello_stat.st_uid = 666;
    hello_stat.st_gid = 666;
    strcpy(files[files_len].path, "/hello1");
    files[files_len].stat = hello_stat;
    files[files_len].data_len = 0;
    char *hello_data = "hello from fuse\n";
    files[files_len].data_len = strlen(hello_data);
    memcpy(files[files_len].data, hello_data, files[files_len].data_len);
    files_len++;
}
