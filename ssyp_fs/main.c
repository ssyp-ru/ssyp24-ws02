#include "fs_command.h"

static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .open = do_open,
    .read = do_read,
    .mkdir = do_mkdir,
    .rmdir = do_rmdir,
    .rename = do_rename,
    .chmod = do_chmod,
    .chown = do_chown,
    .truncate = do_truncate,
    .write = do_write,
    .statfs = do_statfs,
    .flush = do_flush,
    .release = do_release,
    .fsync = do_fsync,
    .releasedir = do_releasedir,
    .fsyncdir = do_fsyncdir,
    .init = do_init,
    //.destroy = do_destroy,
    .create = do_create,
    .access = do_access,

};

int main(int argc, char *argv[]) {
    init_fs("fs.db");
    return fuse_main(argc, argv, &operations, NULL);
}
