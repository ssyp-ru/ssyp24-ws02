#ifndef BLOCK_DEVICE_H_
#define BLOCK_DEVICE_H_

#include <stdio.h>

#define DEVICE_BLOCK_SIZE 512

typedef struct {
    int fd;
} block_device;


void open_block_device(char *file_path, block_device *dev);
void close_block_device(block_device *dev);

void get_blocks(block_device *dev, int from_block_id, int num, char* bytes);
void set_blocks(block_device *dev, int from_block_id, int num, char* bytes);

#endif
