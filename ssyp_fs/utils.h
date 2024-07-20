#include <stdint.h>

void int_to_bytes(int32_t data, char *bytes);
void int_to_bytes64(int64_t data, char* bytes);
int32_t bytes_to_int(char *bytes);
int64_t bytes_to_int64(char* bytes);

void int_to_bytes_test();
