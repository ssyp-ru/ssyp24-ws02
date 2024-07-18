#include "utils.h"
#include <assert.h>
#include <stdio.h>

void int_to_bytes(int32_t data, char *bytes) {
	bytes[0] = (char)data;
	bytes[1] = (char)(data >> 8);
	bytes[2] = (char)(data >> 16);
	bytes[3] = (char)(data >> 24);
}

void int_to_bytes64 (int64_t data, char* bytes) {
	bytes[0] = (char)data;
    bytes[1] = (char)(data >> 8);
    bytes[2] = (char)(data >> 16);
    bytes[3] = (char)(data >> 24);
	bytes[4] = (char)(data >> 32);
	bytes[5] = (char)(data >> 40);
	bytes[6] = (char)(data >> 48);
	bytes[7] = (char)(data >> 56);
}
                                                       
int32_t bytes_to_int(char *bytes) {
    return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
}

int64_t bytes_to_int64(char *bytes) {
    return ((int64_t)bytes[7] << 56) | ((int64_t)bytes[6] << 48) | ((int64_t)bytes[5] << 40) | ((int64_t)bytes[4] << 32) |
    		(bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
}

void int_to_bytes_test() {
	char bytes[8];
	int32_t value32 = 0x12345678;
	int64_t value64 = 0x123456789abcdef;

	int_to_bytes(value32, bytes);
	int32_t res32 = bytes_to_int(bytes);

	printf("%d %d\n", value32, res32);
	assert(value32 == res32); 
}
