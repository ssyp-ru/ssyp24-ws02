#include "block_device.h"
#include "fs.h"
#include "fs_command.h"
#include <stdio.h>
#include <stdlib.h>

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
    size_t max_fs_size = 1024 * 1024 * 1024;
    if (is_exist(STD_BLOCK_DEV_NAME) == 0) {
        printf("Device not found, create %s? (Y/n) ", STD_BLOCK_DEV_NAME);
        char choice;
        scanf("%c", &choice);
        if (choice != 'y' && choice != 'Y' && choice != '\n') {
            printf("abort\n");
            return 0;
        }
        block_device_t dev;
        open_block_device(STD_BLOCK_DEV_NAME, &dev, max_fs_size);
        create_new_fs(&dev);
        close_block_device(&dev);
    }
    block_device_t *dev = malloc(sizeof(block_device_t));
    open_block_device(STD_BLOCK_DEV_NAME, dev, max_fs_size);
    initialize_fs(dev);
    return fuse_main(argc, argv, &operations, NULL);
}
