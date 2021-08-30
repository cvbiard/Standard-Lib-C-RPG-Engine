#define _CRT_SECURE_NO_WARNINGS

#define height 20
#define width 20
#define tile_width 4
#define tile_height 2
#define debug 't'
#define MAX_NAME_SIZE 32
#define MAX_MSG_SIZE 64
#define global_amount_tiles 27
#define global_amount_scenes 2
#define trans_symbol 'X'
#define blank_symbol 'x'

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
};


int calc_screen_size(int border);
void mapping(int tile_map[(height*width)][(tile_height*tile_width)], int screen_size);
void screen_manager(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], struct tile* Tiles, int tile_ids[width][height], int tile_frequency[(width*height)], int *linear_ids, int pos, char player_tile[(tile_width*tile_height)], int screen_size);
void print_screen(int *scrstr, int screen_size);
void load_scene(struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)]);
int move(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], int input, char player_tile[(tile_width*tile_height)], int *linear_ids, struct tile* Tiles, struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)], struct object *player, int screen_size, int msg[1]);
void print_menu(char text[]);
void get_frequency(int tile_ids[width][height], int tile_frequency[(width*height)]);
void debug_printer(int number);
void read_tiles(int amount, struct tile* Tiles);
void read_scenes(int amount, struct asset* scenes);
void ui_manager(int *scrstr, int *bgmap, int pos, struct tile element, int operation, int tile_map[(height*width)][(tile_height*tile_width)]);
void read_message(int id);