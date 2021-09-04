#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TILE_HEIGHT 5
#define TILE_WIDTH 5
#define MODE 'c'

#define BLACK "\x1b[40m"
#define BLACK_TEXT "\x1b[30m"
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
#define BLACK_TEXT "\x1b[30m"
#define BOLD "\x1b[1m"


void write_scene();
void view_tile();
void print_color(char ch);
void edit_tile();
void print_pallete();
void print_options();
void set_color(char ch);