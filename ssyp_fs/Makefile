build: ssyp_fs test

CFLAGS=-Werror -Wall -pedantic `pkg-config fuse3 --cflags`
LFLAGS=`pkg-config fuse3 --libs`

SRC=allocator.c block.c block_device.c fs_command.c utils.c
HDR=allocator.h block.h block_device.h fs_command.h utils.h
OBJ=$(SRC:.c=.o)

ssyp_fs: $(OBJ) $(HDR) main.o
	gcc $(OBJ) main.o -o ssyp_fs $(LFLAGS)

test: $(OBJ) $(HDR) main_test.o
	gcc $(OBJ) main_test.o -o test

%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

run: ssyp_fs
	./ssyp_fs -f mount

run_test: test
	./test

clean:
	rm -f *.o
	rm -f ssyp_fs
	rm -f test

