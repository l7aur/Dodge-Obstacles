#include "util.h"

/**======================================================
 * INITIALIZATIONS
 * ======================================================
*/
int current_player_position_number = PLAYER_POSITION_NUMBER;
const u_int8_t HEADER[14] = {
        0x42, 0x4D,                 //signature
        0xFC, 0xFF, 0x00, 0x00,     //file syze in bytes 54 + 3 * WIDTH * HEIGHT
        0x00, 0x00, 0x00, 0x00,     //reserved
        0x36, 0x00, 0x00, 0x00      //data offset
};
const u_int8_t DIB_HEADER[40] = {
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

/**======================================================
 * FUNCTION BODIES
 * ======================================================
*/
struct bmp_file_format * BMP_FILE_FORMAT_CREATE() {
    struct bmp_file_format * file = (struct bmp_file_format *)malloc(sizeof(struct bmp_file_format));
    
    file->header = (u_int8_t *)malloc(BMP_HEADER_SIZE * sizeof(u_int8_t));
    file->dib_header = (u_int8_t *)malloc(BMP_DIB_HEADER_SIZE * sizeof(u_int8_t));
    file->pixel_data = (u_int8_t *)malloc(BMP_PIXEL_DATA * sizeof(u_int8_t));

    memcpy(file->header, HEADER, BMP_HEADER_SIZE);
    memcpy(file->dib_header, DIB_HEADER, BMP_DIB_HEADER_SIZE);
    return file;
}

struct bmp_file_format * create_blank_BMP() {
    struct bmp_file_format * file = BMP_FILE_FORMAT_CREATE();
    for(int i = 0; i < BMP_PIXEL_DATA; i++) 
        file->pixel_data[i] = 0xFF;
    return file;
}

struct bmp_file_format * create_player_BMP() {
    return NULL;
}

struct bmp_file_format * create_obstacle_BMP() {
    return NULL;
}

int create_BMP(char * file_name, enum BMP_TYPE type) {
    struct bmp_file_format * data = NULL;
    switch (type)
    {
    case BLANK:
        data = create_blank_BMP();
        break;
    case PLAYER:
        data = create_player_BMP();
        break;
    case OBSTACLE:
        data = create_obstacle_BMP();
        break;
    default:
        return -1;
    }

    if(data == NULL)
        return -1;
        
    int fd = open(file_name, O_CREAT | O_WRONLY, 0644);
    write(fd, data->header, BMP_HEADER_SIZE);
    write(fd, data->dib_header, BMP_DIB_HEADER_SIZE);
    write(fd, data->pixel_data, BMP_PIXEL_DATA);
    close(fd);
    free(data->dib_header);
    free(data->header);
    free(data->pixel_data);
    free(data);
    return 0;
}
