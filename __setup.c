#include "util.h"

void start();
void end();
void * movement(void *arg);

void update_obstacle_position(struct list * prim) {
    struct list * prev = NULL;
    while(prim != NULL) {
        if(prim->value == -INF){
            prev = prim;
            prim = prim->next;
            continue;
        }
        char name[100];

        if(prim->value % NUMBER_OF_COLUMNS == 1) { //delete form list
            struct list * next_node = prim->next;
            if(prim->next == NULL) {
                snprintf(name, 100, "%d%s.bmp", prim->value, OBSTACLE_NAME);
                remove(name); //remove old position
                snprintf(name, 100, "%d%s.bmp", prim->value, BLOCK_NAME);
                if(create_BMP(name, BLOCK) < 0) { //create new block in old position
                    perror("Error!");
                }
                free(prim);
                prev->next = next_node;
                prim = next_node;
                continue;
            }
            snprintf(name, 100, "%d%s.bmp", prim->value, OBSTACLE_NAME);
            remove(name); //remove old position
            snprintf(name, 100, "%d%s.bmp", prim->value, BLOCK_NAME);
            if(create_BMP(name, BLOCK) < 0) { //create new block in old position
                perror("Error!");
            }
            free(prim);
            prev->next = next_node;
            prim = next_node;
            continue;
        }
        
        snprintf(name, 100, "%d%s.bmp", prim->value, OBSTACLE_NAME);
        remove(name); //remove old position
        snprintf(name, 100, "%d%s.bmp", prim->value, BLOCK_NAME);
        if(create_BMP(name, BLOCK) < 0) { //create new block in old position
            perror("Error!");
        }
        snprintf(name, 100, "%d%s.bmp", --prim->value, OBSTACLE_NAME);
        if(create_BMP(name, OBSTACLE) < 0) { //create new block in new position
            perror("Cannot create block!");
        }
        snprintf(name, 100, "%d%s.bmp", prim->value, BLOCK_NAME);
        remove(name); //remove block in new position
        prev = prim;
        prim = prim->next;
    }
}

void * create_obstacle( void * arg) {
    sleep(3);
    struct list * l = (struct list *)malloc(sizeof(struct list));
    struct list * prim = l;
    l->next = NULL; //sentinel
    l->value = -INF;
    while(1) {
        update_obstacle_position(prim);

        char obstacle_name[100], block_name[100];
        int line_index = rand() % NUMBER_OF_LINES;
        int column_index = 15 * line_index;

        struct list * new_node = (struct list *)malloc(sizeof(struct list));
        l->next = new_node;
        new_node->next = NULL;
        new_node->value = column_index;
        l = l->next;

        snprintf(block_name, 100, "%d%s.bmp", column_index, BLOCK_NAME);
        remove(block_name);
        snprintf(obstacle_name, 100, "%d%s.bmp", column_index, OBSTACLE_NAME);
        if(create_BMP(obstacle_name, OBSTACLE) < 0) {
            perror("Cannot create block!");
        }
        sleep(1);
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
    for (int i = 1; i <= NUMBER_OF_BLOCKS; i++)
    {
        if(i == PLAYER_POSITION_NUMBER)
            continue;
        snprintf(name, 100, "%d%s.bmp", i, BLOCK_NAME);
        if(create_BMP(name, BLOCK) < 0)
            perror("Cannot create a file!");
    }
    snprintf(name, 100, "%d%s.bmp", PLAYER_POSITION_NUMBER, PLAYER_NAME);
    if(create_BMP(name, PLAYER) < 0) {
        perror("Cannot create player!");
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
    while((directory_entry = readdir(directory)) != 0)
        if( strstr(directory_entry->d_name, BLOCK_NAME) ||
       	    strstr(directory_entry->d_name, PLAYER_NAME) ||
       	    strstr(directory_entry->d_name, OBSTACLE_NAME)) {
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
                snprintf(where_block_goes, 100, "%d%s.bmp", current_player_position_number, PLAYER_NAME);
                snprintf(new_name_block, 100, "%d%s.bmp", current_player_position_number, BLOCK_NAME);

                current_player_position_number -= NUMBER_OF_COLUMNS;

                snprintf(where_player_goes, 100, "%d%s.bmp", current_player_position_number, BLOCK_NAME);
                snprintf(new_name_player, 100, "%d%s.bmp", current_player_position_number, PLAYER_NAME);

                if(remove(where_player_goes) < 0) {
                    perror("Error!");
                }
                if(create_BMP(new_name_player, PLAYER) < 0) {
                    perror("Cannot create player!");
                }

                if(remove(where_block_goes) < 0) {
                    perror("Error!");
                }
                if(create_BMP(new_name_block, BLOCK) < 0) {
                    perror("Cannot create block!");
                }
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
                snprintf(where_block_goes, 100, "%d%s.bmp", current_player_position_number, PLAYER_NAME);
                snprintf(new_name_block, 100, "%d%s.bmp", current_player_position_number, BLOCK_NAME);

                current_player_position_number += NUMBER_OF_COLUMNS;

                snprintf(where_player_goes, 100, "%d%s.bmp", current_player_position_number, BLOCK_NAME);
                snprintf(new_name_player, 100, "%d%s.bmp", current_player_position_number, PLAYER_NAME);
                
                if(remove(where_player_goes) < 0) {
                    perror("Error!");
                }
                if(create_BMP(new_name_player, PLAYER) < 0) {
                    perror("Cannot create player!");
                }

                if(remove(where_block_goes) < 0) {
                    perror("Error!");
                }
                if(create_BMP(new_name_block, BLOCK) < 0) {
                    perror("Cannot create block!");
                }
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
