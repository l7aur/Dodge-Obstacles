#ifndef UTIL_H
#define UTIL_H

#include <stddef.h> // Include this to define NULL
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

/**======================================================
 * GLOBAL CONSTANTS
 * ======================================================
*/
#define WIDTH 0x64
#define HEIGHT 0x64
#define NUMBER_OF_BLOCKS 105
#define NUMBER_OF_LINES 7
#define NUMBER_OF_COLUMNS 15
#define PLAYER_POSITION_NUMBER 46
#define BMP_DIB_HEADER_SIZE 40
#define BMP_HEADER_SIZE 14
#define BMP_PIXEL_DATA WIDTH * HEIGHT * 3
#define BLOCK_NAME "block"
#define OBSTACLE_NAME "obstacle"
#define PLAYER_NAME "player"
#define OBSTACLE_NAME "obstacle"

/**======================================================
 * TYPES
 * ======================================================
*/
enum BMP_TYPE {BLANK, PLAYER, OBSTACLE};
struct bmp_file_format {
    u_int8_t * header;
    u_int8_t * dib_header;
    u_int8_t * pixel_data;
};

/**======================================================
 * GLOBAL VARIABLES
 * ======================================================
*/
extern const u_int8_t HEADER[BMP_HEADER_SIZE];
extern const u_int8_t DIB_HEADER[BMP_DIB_HEADER_SIZE];
extern int current_player_position_number;

/**======================================================
 * FUNCTION DECLARATIONS
 * ======================================================
*/
int create_BMP(char * file_name, enum BMP_TYPE type);

#endif /* UTIL_H */
