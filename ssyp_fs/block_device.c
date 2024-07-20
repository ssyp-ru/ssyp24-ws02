#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "block_device.h"

void open_block_device(char *file_path, block_device_t *dev, size_t size) {
    // TODO: allocate whole file?
    // TODO: direct io to bench?
    dev->fd = open(file_path, O_CREAT | O_RDWR, 0666);
    dev->in_mem_data = NULL;
    dev->device_size = size;
}

void open_in_memory_block_device(block_device_t *dev, size_t size) {
    dev->fd = 0;
    dev->in_mem_data = malloc(size);
    dev->device_size = size;
}

void close_block_device(block_device_t *dev) {
    close(dev->fd);
}

void get_blocks(block_device_t *dev, int from_block_id, int num, char *bytes) {
    size_t count = num * DEVICE_BLOCK_SIZE;
    size_t offset = from_block_id * DEVICE_BLOCK_SIZE;

    if (dev->in_mem_data != NULL) {
        assert(offset + count <= dev->device_size);
        memcpy(bytes, dev->in_mem_data + offset, count);
        return;
    }

    // TODO: check for max device size
    ssize_t size = pread(dev->fd, bytes, num * DEVICE_BLOCK_SIZE, from_block_id * DEVICE_BLOCK_SIZE);
    if (size < 0) {
        printf("error while pread: %s, size: %ld, offset: %ld\n", strerror(errno), count, offset);
    }
    assert(size == num * DEVICE_BLOCK_SIZE);
}

void set_blocks(block_device_t *dev, int from_block_id, int num, char *bytes) {
    size_t count = num * DEVICE_BLOCK_SIZE;
    size_t offset = from_block_id * DEVICE_BLOCK_SIZE;

    if (dev->in_mem_data != NULL) {
        assert(offset + count <= dev->device_size);
        memcpy(dev->in_mem_data + offset, bytes, count);
        return;
    }

    // TODO: check for max device size
    size_t size = pwrite(dev->fd, bytes, count, offset);
    if (size < 0) {
        printf("error while pwrite: %s, size: %ld, offset: %ld\n", strerror(errno), count, offset);
    }
    assert(size == num * DEVICE_BLOCK_SIZE);
}

int is_exist(char *path) {
    struct stat s;
    return stat(path, &s) == 0;
}
