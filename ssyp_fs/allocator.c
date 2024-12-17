#include "allocator.h"

uint8_t map[4096];
void create_map(fs_t *fs){
    for(int i = fs->super_block->first_bitmap_blocks;i < fs->super_block->total_bitmap_blocks;i++){
        map[i] = 0;
        }
    map[0] = 1;
    map[1] = 1;
    }

uint8_t* dec_in_bin(uint8_t num){
    
    
    uint8_t bin[8];
    int i = 0;
    while (num){
        bin[i] = (num % 2);
        i++;
        num /= 2;
        }
        return bin;
    }
uint8_t bin_to_dec(uint8_t *bin, uint8_t off, uint8_t offoff){
    uint8_t num = 0;
    uint8_t k = 1;
    for(int i = off;i < offoff;i++){
        bin[i] = 1;
        }
    for(int i = 0;i < 8;i++){
        num += bin[7-i] * k;
        k = k * 2;
        }
    return k;
    }
block_section allocate_blocks(fs_t *fs, int num) {
   	
    block_section b;
  	b.len = num;
    uint8_t n = 0;
    int cnt = 0;
	fs->super_block->used_blocks += num; 
    for(int g = 0;g < fs->super_block->total_bitmap_blocks;g++){
            n = map[g];
            uint8_t bin[8];
            bin = dec_in_bin(n);
            for(int i = 0;i < 8;i++){
                if(bin[i] == 0){
                    cnt++;
                    if(num == cnt){
                        b.start = g * 8 + i - num;
                        map[g] = bin_to_dec(bin,0,i);
                        map[g-num/8] = bin_to_dec(dec_in_bin(map[g-num/8]),(g * 8 + i)%8,8);
                         
                        return b;
                        }
                    continue;
                    }
                if(bin[i] == 1){
                    cnt = 0;
                    }
                }
            }
    return b;
}
/*
void de_blocks(fs_t *fs, int num, int start){
    if(start == 0 || start == 1){
        return;
        }

    for(int i = 0;i < start/8;i++){
        if(map[g] == 255){
            map_[i] = 0;
            }
        }
    for(int i = 0;i <)

    }*/
