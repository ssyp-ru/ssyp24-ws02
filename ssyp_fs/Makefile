build: ssyp_fs

CFLAGS=-Werror -Wall -pedantic `pkg-config fuse3 --cflags`
LFLAGS=`pkg-config fuse3 --libs`

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

ssyp_fs: $(OBJ)
	gcc $(OBJ) -o ssyp_fs $(LFLAGS)

%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

run: ssyp_fs
	./ssyp_fs -f mount

test: utils.c test/main_test.c
	gcc utils.c test/main_test.c -o test/test

clean:
	rm -f *.o
	rm -f ssyp_fs

