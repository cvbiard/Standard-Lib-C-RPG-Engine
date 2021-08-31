#define _CRT_SECURE_NO_WARNINGS

#define height 14
#define width 28
#define tile_width 8
#define tile_height 4
#define debug 't'
#define MAX_NAME_SIZE 32
#define MAX_MSG_SIZE 64
#define global_amount_tiles 27
#define global_amount_scenes 2
#define trans_symbol 'X'
#define blank_symbol 'x'

#define BLACK "\x1b[40m"
#define DARK_RED "\x1b[41m"
#define DARK_GREEN "\x1b[42m"
#define DARK_YELLOW "\x1b[43m"
#define DARK_BLUE "\x1b[44m"
#define DARK_MAGENTA "\x1b[45m"
#define DARK_CYAN "\x1b[46m"
#define DARK_WHITE "\x1b[47m"
#define BRIGHT_BLACK "\x1b[100m"
#define BRIGHT_RED "\x1b[101m"
#define BRIGHT_GREEN "\x1b[102m"
#define BRIGHT_YELLOW "\x1b[103m"
#define BRIGHT_BLUE "\x1b[104m"
#define BRIGHT_MAGENTA "\x1b[105m"
#define BRIGHT_CYAN "\x1b[106m"
#define WHITE "\x1b[107m"
#define RESET "\x1b[0m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct object
{
	int pos;
};
struct asset
{
	int id;
	char* file;
};
struct tile
{
	int id;
    char* name;
	char flags[3];
	char* file;
	int warp[2];
	int msg_id;
	int ui_id;
	char ui_dir; //'u', 'd', 'l', or 'r' for up, down, left, or right respectively.
};
struct message
{
    int id;
    char* text;
};


int calc_screen_size(int border);
void mapping(int tile_map[(height*width)][(tile_height*tile_width)], int screen_size);
void screen_manager(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], struct tile* Tiles, int tile_ids[width][height], int tile_frequency[(width*height)], int *linear_ids, int pos, char player_tile[(tile_width*tile_height)], int screen_size);
void print_screen(int *scrstr, int screen_size, char mode);
void load_scene(struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)]);
int move(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], int input, char player_tile[(tile_width*tile_height)], int *linear_ids, struct tile* Tiles, struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)], struct object *player, int screen_size, int *msg);
void print_menu(char text[]);
void get_frequency(int tile_ids[width][height], int tile_frequency[(width*height)]);
void debug_printer(int number);
void read_tiles(int amount, struct tile* Tiles);
void read_scenes(int amount, struct asset* scenes);
void ui_manager(int *scrstr, int *bgmap, int pos, struct tile element, int operation, int tile_map[(height*width)][(tile_height*tile_width)]);
void read_messages(int amount, struct message* Messages);
void display_message(int id, struct message* Messages);