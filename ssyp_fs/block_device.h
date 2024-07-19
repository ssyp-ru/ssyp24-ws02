#ifndef BLOCK_DEVICE_H_
#define BLOCK_DEVICE_H_

#include <stdio.h>

#define DEVICE_BLOCK_SIZE 512

#define STD_BLOCK_DEV_NAME "db.fs"

typedef struct {
    int fd;
} block_device_t;


void open_block_device(char *file_path, block_device_t *dev);
void close_block_device(block_device_t *dev);

void get_blocks(block_device_t *dev, int from_block_id, int num, char* bytes);
void set_blocks(block_device_t *dev, int from_block_id, int num, char* bytes);

int is_exist(char* path);

#endif
