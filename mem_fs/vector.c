#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct vector{
    char *a;
    int len;
    int capacity;
};
char initialize_vector(struct vector *v){
    v->len = 0;
    v->capacity = 10;
    v->a = malloc(v->capacity);
}
void vector_write(struct vector *v, char *buff, int size, int offset, int capacity){
    for (int i = 0;i < size + 1; i++){
        v -> a[i] = buff[i];
        if (size > capacity){
            capacity = capacity*2;
        }
    }
}
void vector_test(){
    int size = 256;
    int offset = 256;
    int capacity=capacity;
    char buff[256];
    struct vector v;
    vector_write(&v, buff, size , offset,capacity);
    assert(v.a != NULL);
}
