all: build

build:
	make -C ssyp_fs
	make -C mem_fs

test:
	@echo "no test yet"

clean:
	make -C ssyp_fs clean
	make -C mem_fs clean
