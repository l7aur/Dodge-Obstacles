#ifndef HEADER_H
#define HEADER_H

#include <stddef.h> // Include this to define NULL
#include <string.h>

#define WIDTH 0x64
#define HEIGHT 0x64

enum BMP_TYPE {BLANK, PLAYER, OBSTACLE};

struct bmp_file_format {
    char * header;
    char * dib_header;
    char * pixel_data;
};

const char HEADER[14] = {
        0x42, 0x4D,                 //signature
        0xFC, 0xFF, 0x00, 0x00,     //file syze in bytes 54 + 3 * WIDTH * HEIGHT
        0x00, 0x00, 0x00, 0x00,     //reserved
        0x36, 0x00, 0x00, 0x00      //data offset
};
const char DIB_HEADER[40] = {
        0x28, 0x00, 0x00, 0x00,     //DIB header size
        WIDTH, 0x00, 0x00, 0x00,    //BMP width
        HEIGHT, 0x00, 0x00, 0x00,   //BMP height
        0x01, 0x00,                 //number od color planes
        0x18, 0x00,                 //bits per pixel
        0x00, 0x00, 0x00, 0x00,     //compression - none
        0x00, 0x00, 0x00, 0x00,     //image real size, can be 0 if no compression
        0x00, 0x00, 0x00, 0x00,     //x pixels per meter (unspecified)
        0x00, 0x00, 0x00, 0x00,     //y pixels per meter (unspecified)
        0x00, 0x00, 0x00, 0x00,     //number of colors in color table (no color table)
        0x00, 0x00, 0x00, 0x00      //important color count (all colors are important)
};

struct bmp_file_format * BMP_FILE_FORMAT_CREATE();
struct bmp_file_format * create_blank_BMP();
struct bmp_file_format * create_player_BMP();
struct bmp_file_format * create_obstacle_BMP();
struct bmp_file_format * create_BMP(enum BMP_TYPE type);

#endif //HEADER_H
