#include "block_device.h"
#include "fs.h"
#include "fs_command.h"
#include <stdio.h>

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
    if (is_exist(STD_BLOCK_DEV_NAME) == 0) {
        printf("Device not found, create %s? (Y/n) ", STD_BLOCK_DEV_NAME);
        char choice;
        scanf("%c", &choice);
        if (choice != 'y' && choice != 'Y' && choice != '\n') {
            printf("abort\n");
            return 0;
        }
        create_new_fs(STD_BLOCK_DEV_NAME);
    }
    return fuse_main(argc, argv, &operations, NULL);
}
