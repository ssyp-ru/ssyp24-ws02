#include <stdint.h>
#include <stdio.h>

#define BYTE_RANGE 256;

typedef struct {
    char id_length;  // length of id field (number of bytes - max 255)
    char map_type;   // colormap field (0 or 1; no map or 256 entry palette)
    char image_type; // ( 0 - no image data included
                     //	  1 - uncompressed, color mapped image
                     //	  2 - uncompressed, RGB image
                     //	  3 - uncompressed, black & white image
                     //	  9 - run-length encoded(RLE-lossless compression),color mapped image
                     //	 10 - RLE, RGB image
                     //	 11 - compressed, black & white image )

    int map_first;
    int map_length;
    char map_entry_size;

    int x;
    int y;

    int width;
    int height;

    char bits_per_pixel;

    char misc;
} targa_header;

int little_endianify(int number) {
    return number % BYTE_RANGE;
}

int big_endianify(int number) {
    return number / BYTE_RANGE;
}

void write_tga_header(targa_header header, FILE *tga) {
    fputc(header.id_length, tga);
    fputc(header.map_type, tga);
    fputc(header.image_type, tga);

    fputc(little_endianify(header.map_first), tga);
    fputc(big_endianify(header.map_first), tga);

    fputc(little_endianify(header.map_length), tga);
    fputc(big_endianify(header.map_length), tga);

    fputc(header.map_entry_size, tga);

    fputc(little_endianify(header.x), tga);
    fputc(big_endianify(header.x), tga);
    fputc(little_endianify(header.y), tga);
    fputc(big_endianify(header.y), tga);

    fputc(little_endianify(header.width), tga);
    fputc(big_endianify(header.width), tga);
    fputc(little_endianify(header.height), tga);
    fputc(big_endianify(header.height), tga);

    fputc(header.bits_per_pixel, tga);
    fputc(header.misc, tga);
}

void write_tga(char *filename, int width, int height, int channels, uint8_t *data) {
    FILE *tga;
    targa_header header;

    int x,y;

    //set header values 

    header.id_length = 0;
    header.map_type = 0;
    header.image_type = 2; // uncompressed RGB image

    header.map_first = 0;
    header.map_length = 0;
    header.map_entry_size = 0;

    header.x = 0;
    header.y = 0;
    header.width = width;
    header.height = height;

    header.bits_per_pixel = 24;
    header.misc = 0x20;

    // start to write file 

    tga = fopen(filename, "wb");
    write_tga_header(header, tga);
 
    //// magic happens here -- write the pixels

    for (int i = 0; i < width * height * channels; i++) {
        fputc(data[i], tga);
    }

    fclose(tga);
}                                                                                        

#define width 100
#define height 100

int main() {
    uint8_t data[width * height * 3];
    for (int i = 0; i < width * height; i++) {
	   for (int j =0; j <width; j++);{
       	data[((i * width + j)* 3)] = 255;
       	data[((i * width + j)* 3) + 1] = 0;
	data[((i * width + j)* 3) + 2] = 0;
    }
    }

	for (int i = 0; i < width; i++) {
		   for (int j = 0; j < width; j++);{
		   

        		data[((i * width + j)* 3)] = 255;
        		data[((i * width + j)* 3) + 1] = 255;
        		data[((i * width + j)* 3) + 2] = 255;
	
         	}
		}
    write_tga("test.tga", width, height, 3, data);
    return 0;
}
