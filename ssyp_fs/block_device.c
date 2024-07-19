#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#include "block_device.h"

void open_block_device(char *file_path, block_device_t *dev) {
    // TODO: allocate whole file?
    // TODO: direct io to bench?
    dev->fd = open(file_path, O_CREAT | O_RDWR, 0666);
}

void close_block_device(block_device_t *dev) {
    close(dev->fd);
}

void get_blocks(block_device_t *dev, int from_block_id, int num, char *bytes) {
    // TODO: check for max device size
    size_t size = pread(dev->fd, bytes, num * DEVICE_BLOCK_SIZE, from_block_id * DEVICE_BLOCK_SIZE);
    assert(size == num * DEVICE_BLOCK_SIZE);
}

void set_blocks(block_device_t *dev, int from_block_id, int num, char *bytes) {
    // TODO: check for max device size
    size_t size = pwrite(dev->fd, bytes, num * DEVICE_BLOCK_SIZE, from_block_id * DEVICE_BLOCK_SIZE);
    assert(size == num * DEVICE_BLOCK_SIZE);
}

int is_exist(char* path) {
    struct stat s;
    return stat(path, &s) == 0;
}
