all: build

build:
	make -C ssyp_fs
	make -C mem_fs

clean:
	make -C ssyp_fs clean
	make -C mem_fs clean

format:
	clang-format -style=file -i ssyp_fs/*.c
	clang-format -style=file -i mem_fs/*.c
	clang-format -style=file -i vis/*.c
	clang-format -style=file -i ptest/*.c

check_format:
	clang-format -style=file --dry-run --Werror ssyp_fs/*.c
	clang-format -style=file --dry-run --Werror mem_fs/*.c
	clang-format -style=file --dry-run --Werror vis/*.c
	clang-format -style=file --dry-run --Werror ptest/*.c

run_test:
	make -C ssyp_fs run_test
	make -C mem_fs run_test
