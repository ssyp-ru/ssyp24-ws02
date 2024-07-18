#include "allocator.h"
#include <stdbool.h>
#define SIZE 1024
bool map_[SIZE];




block_section allocate_blocks(fs *fs, int num) {
   	block_section b;
  	b.len = num;
        int n = 0;
	fs->super_block->used_blocks += num;//total_bitmap_blocks
        if(fs->super_block->total_bitmap_blocks == 0){
                b.start = 0;
                
                return b;
                }
        for(int i = 0;i < fs->super_block->total_bitmap_blocks;i++){
                if(map_ == 0){
                        n++;
                        continue;
                        }
                n = 0;
                map_[i] = 1;
                }
    // od: save superblock
    	return b;
}


void deallocate_blocks(fs *fs,int start, int num) {
	fs->super_block->used_blocks -= num;//total_bitmap_blocks
        for(int i = 0;i < num;i++){
                map_[num+i] = 0;//i+ num
                }
}
