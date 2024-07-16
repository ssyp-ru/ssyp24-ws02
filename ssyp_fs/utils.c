#include "utils.h"

void int_to_bytes(int32_t data, char *bytes) {
    bytes[0] = (char)data;
    bytes[1] = (char)((data >> 8) & 0xFF);
    bytes[2] = (char)((data >> 16) & 0xFF);
    bytes[3] = (char)((data >> 24) & 0xFF);
}

int32_t bytes_to_int(char *bytes) {
    return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
}
