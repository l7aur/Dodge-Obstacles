#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#define NUMBER_OF_BLOCKS 105
#define NUMBER_OF_LINES 7
#define NUMBER_OF_COLUMNS 15
#define PLAYER_POSITION_NUMBER 46
#define BLOCK_NAME "block"
#define OBSTACLE_NAME "obstacle"
#define PLAYER_NAME "player"
#define OBSTACLE_NAME "obstacle"
#define C_FILE_NAME "__setup.c"
#define O_FILE_NAME "__setup"

int current_player_position_number = PLAYER_POSITION_NUMBER;

void start();
void end();
void * movement(void *arg);

void * create_obstacle( void * arg) {
    for(int i = 0; i < 5; i++) {
        char obstacle_name[100], block_name[100];
        int line_index = rand() % NUMBER_OF_LINES;
        int column_index = 15 * line_index;

        snprintf(obstacle_name, 100, "%d%s.txt", column_index, OBSTACLE_NAME);
        snprintf(block_name, 100, "%d%s.txt", column_index, BLOCK_NAME);
        rename(block_name, obstacle_name);
        sleep(2);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    start();

    end();
    return 0;
}

void start()
{
    char name[100];
    for (int i = 1; i <= PLAYER_POSITION_NUMBER - 1; i++)
    {
        snprintf(name, 100, "%d%s.txt", i, BLOCK_NAME);
        creat(name, 0644);
    }
    snprintf(name, 100, "%d%s.txt", PLAYER_POSITION_NUMBER, PLAYER_NAME);
    creat(name, 0644);
    for (int i = PLAYER_POSITION_NUMBER + 1; i <= NUMBER_OF_BLOCKS; i++)
    {
        snprintf(name, 100, "%d%s.txt", i, BLOCK_NAME);
        creat(name, 0644);
    }

    pthread_t obstacle_thread;
    pthread_create(&obstacle_thread, NULL, create_obstacle, NULL);

    pthread_t movement_thread;
    pthread_create(&movement_thread, NULL, movement, &obstacle_thread);

    pthread_join(movement_thread, NULL);
}

void end()
{
    printf("CLEARING GAME DATA\n");
    DIR * directory;
    struct dirent * directory_entry;
    if((directory = opendir("./")) < 0) {
        printf("Cannot see directory!\n");
        exit(-1);
    }
    while((directory_entry = readdir(directory)) != 0) {
        if( !strcmp(directory_entry->d_name, ".") ||
            !strcmp(directory_entry->d_name, ".."))
            continue;
        if( !strcmp(directory_entry->d_name, C_FILE_NAME) ||
            !strcmp(directory_entry->d_name, O_FILE_NAME))
            continue;
        char name[300];
        snprintf(name, 300, "./%s", directory_entry->d_name);
        remove(name);
    }
    printf("END\n");
}

void * movement(void * arg) {
    pthread_t obstacle_handling_thread = *(pthread_t *) arg;
    struct termios info;
    tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~ICANON;      /* disable canonical mode */
    info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(0, TCSANOW, &info); /* set immediately */

    char ch;
    while (1)
    {
        ch = getchar();
        printf("\nKeyboard event handled!\n");
        if (ch < 0)
        {
            if (ferror(stdin))
                printf("error\n");
            clearerr(stdin);
        }
        else if (ch == 'w')
        {
            if (current_player_position_number - NUMBER_OF_COLUMNS > 0)
            {
                char where_block_goes[100], new_name_block[100];
                char where_player_goes[100], new_name_player[100];
                snprintf(where_block_goes, 100, "%d%s.txt", current_player_position_number, PLAYER_NAME);
                snprintf(new_name_block, 100, "%d%s.txt", current_player_position_number, BLOCK_NAME);

                current_player_position_number -= NUMBER_OF_COLUMNS;

                snprintf(where_player_goes, 100, "%d%s.txt", current_player_position_number, BLOCK_NAME);
                snprintf(new_name_player, 100, "%d%s.txt", current_player_position_number, PLAYER_NAME);

                if (rename(where_player_goes, new_name_player) < 0)
                    printf("Cannot move player!\n");
                if (rename(where_block_goes, new_name_block) < 0)
                    printf("Cannot rename old player!\n");
            }
            else
                printf("Cannot move upper!\n");
        }
        else if (ch == 's')
        {
            if (current_player_position_number + NUMBER_OF_COLUMNS < NUMBER_OF_BLOCKS)
            {
                char where_block_goes[100], new_name_block[100];
                char where_player_goes[100], new_name_player[100];
                snprintf(where_block_goes, 100, "%d%s.txt", current_player_position_number, PLAYER_NAME);
                snprintf(new_name_block, 100, "%d%s.txt", current_player_position_number, BLOCK_NAME);

                current_player_position_number += NUMBER_OF_COLUMNS;

                snprintf(where_player_goes, 100, "%d%s.txt", current_player_position_number, BLOCK_NAME);
                snprintf(new_name_player, 100, "%d%s.txt", current_player_position_number, PLAYER_NAME);

                if (rename(where_player_goes, new_name_player) < 0)
                    printf("Cannot move player!\n");
                if (rename(where_block_goes, new_name_block) < 0)
                    printf("Cannot rename old player!\n");
            }
            else
                printf("Cannot move lower!\n");
        }
        else
            break;
    }
    printf("No more keyboard events handled!\n");

    tcgetattr(0, &info); /* switch back to canonical mode */
    info.c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, &info);
    pthread_cancel(obstacle_handling_thread);
    return NULL;
}
