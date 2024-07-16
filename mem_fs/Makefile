build: mem_fs

CFLAGS=-Werror -Wall -pedantic `pkg-config fuse3 --cflags --libs`

mem_fs: main.c fs_command.o
	gcc main.c fs_command.o -o mem_fs $(CFLAGS) 

fs_command.o: fs_command.c fs_command.h
	gcc -c fs_command.c $(CFLAGS) 

run: mem_fs
	./mem_fs -f mount

clean:
	rm -f *.o
	rm -f mem_fs

format:
	clang-format -style=file -i main.c
