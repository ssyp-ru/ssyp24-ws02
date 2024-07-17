build: hello_fs

CFLAGS=-Werror -Wall -pedantic `pkg-config fuse3 --cflags --libs`

hello_fs: main.c fs_command.o
	gcc main.c fs_command.o -o hello_fs $(CFLAGS) 

fs_command.o: fs_command.c fs_command.h
	gcc -c fs_command.c $(CFLAGS) 

run: hello_fs
	./hello_fs -f mount

clean:
	rm *.o
	rm hello_fs

format:
	clang-format -style=file -i main.c
