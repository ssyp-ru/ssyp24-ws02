build: mem_fs test

CFLAGS=-Werror -Wall -pedantic `pkg-config fuse3 --cflags`
LFLAGS=`pkg-config fuse3 --libs`

SRC=fs_command.c write_test.c
HDR=fs_command.h write_test.h
OBJ=$(SRC:.c=.o)

mem_fs: $(OBJ) $(HDR) main.o
	gcc $(OBJ) main.o -o mem_fs $(LFLAGS) 

test: $(OBJ) $(HDR) main_test.o
	gcc $(OBJ) main_test.o -o test

%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

run: mem_fs
	./mem_fs -f mount

run_test: test
	./test

clean:
	rm -f *.o
	rm -f mem_fs
	rm -f test

