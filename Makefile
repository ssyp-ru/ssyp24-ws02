all: build

build:
	make -C ssyp_fs
	make -C mem_fs

test:
	@echo "no test yet"

clean:
	make -C ssyp_fs clean
	make -C mem_fs clean

format:
	clang-format -style=file -i ssyp_fs/*.c
	clang-format -style=file -i mem_fs/*.c

check_format:
	clang-format -style=file --dry-run --Werror ssyp_fs/*.c
	clang-format -style=file --dry-run --Werror mem_fs/*.c
