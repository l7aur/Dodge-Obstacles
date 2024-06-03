#include <stddef.h> // Include this to define NULL
#include <string.h>
#include "header.h"

struct bmp_file_format * BMP_FILE_FORMAT_CREATE() {
    struct bmp_file_format * file = (struct bmp_file_format *)malloc(sizeof(struct bmp_file_format));
    
    file->header = (char *)malloc(14 * sizeof(char));
    file->dib_header = (char *)malloc(40 * sizeof(char));
    file->pixel_data = (char *)malloc(3 * WIDTH * HEIGHT * sizeof(char));

    memcpy(file->header, HEADER, 14);
    memcpy(file->dib_header, DIB_HEADER, 40);
    return file;
}

struct bmp_file_format * create_blank_BMP() {
    struct bmp_file_format * file = BMP_FILE_FORMAT_CREATE();
    for(int i = 0; i < HEIGHT * WIDTH * 3; i += 8) {
            file->pixel_data[i] =  
            file->pixel_data[i + 1] = 
            file->pixel_data[i + 2] = 
            file->pixel_data[i + 3] = 
            file->pixel_data[i + 4] =
            file->pixel_data[i + 5] = 0xFF; //2 white pixels
            file->pixel_data[i + 6] =
            file->pixel_data[i + 7] = 0x00; //padding for 4-byte alignment
        }
    return file;
}

struct bmp_file_format * create_player_BMP() {
    return NULL;
}

struct bmp_file_format * create_obstacle_BMP() {
    return NULL;
}

struct bmp_file_format * create_BMP(enum BMP_TYPE type) {
    switch (type)
    {
    case BLANK:
        return create_blank_BMP();
    case PLAYER:
        return create_player_BMP();
    case OBSTACLE:
        return create_obstacle_BMP();
    }
    return NULL;
}