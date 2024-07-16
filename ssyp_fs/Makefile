build: ssyp_fs

CFLAGS=-Werror -Wall -pedantic `pkg-config fuse3 --cflags --libs`

ssyp_fs: main.c fs_command.o
	gcc main.c fs_command.o -o ssyp_fs $(CFLAGS) 

fs_command.o: fs_command.c fs_command.h
	gcc -c fs_command.c $(CFLAGS) 

run: ssyp_fs
	./ssyp_fs -f mount

clean:
	rm -f *.o
	rm -f ssyp_fs

format:
	clang-format -style=file -i main.c
