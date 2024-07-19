build: ssyp_fs test

CFLAGS=-Werror -Wall -pedantic `pkg-config fuse3 --cflags` -g
LFLAGS=`pkg-config fuse3 --libs`

SRC=allocator.c block_device.c fs_command.c utils.c fs.c inode.c super_block.c
HDR=allocator.h block_device.h fs_command.h utils.h fs.h inode.h super_block.h
OBJ=$(SRC:.c=.o)

ssyp_fs: $(OBJ) $(HDR) main.o
	gcc $(OBJ) main.o -o ssyp_fs $(LFLAGS)

test: $(OBJ) $(HDR) main_test.o
	gcc $(OBJ) main_test.o -o test

%.o : %.c $(HDR)
	gcc -c $(CFLAGS) $< -o $@

run: ssyp_fs
	./ssyp_fs -f mount

run_test: test
	./test

clean:
	rm -f *.o
	rm -f ssyp_fs
	rm -f test

