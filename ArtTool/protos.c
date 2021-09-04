#include "header.h"

void write_scene()
{
	char name[64];
	int tile = 0;
	int xsize = 0;
	int ysize = 0;

	printf("What would you like to name the file? (Exclude file extention)\n");
	scanf("%s", &name);

	strcat(name, ".txt");

	printf("Creating %s\n", name);

	FILE *scenefile = fopen(name, "w");

	printf("How wide would you like the scene to be?\n");
	scanf("%d", &xsize);

	printf("\n");

	printf("How tall would you like the scene to be?\n");
	scanf("%d", &ysize);

	printf("\n");

	printf("What tile id would you like to fill the file with?\n");
	scanf("%d", &tile);

	printf("\n");

	printf("Creating a scene file of size %d by %d with tile id %d.\n", xsize, ysize, tile);

	printf("\n");

	for (int i = 0; i < ysize; i++)
	{
		for (int j = 0; j < xsize; j++)
		{
			if (j == xsize - 1)
			{
				printf("%d", tile);
				fprintf(scenefile, "%d", tile);
			}
			else
			{
				printf("%d ", tile);
				fprintf(scenefile, "%d ", tile);
			}
		}
		fprintf(scenefile, "\n");
		printf("\n");
	}

	printf("\n");

	fclose(scenefile);
	printf("Complete!\n");
}
void view_tile()
{
	char name[64];
	char tile[TILE_HEIGHT][TILE_WIDTH];

	for (int i = 0; i < TILE_HEIGHT; i++)
	{
		for (int j = 0; j < TILE_WIDTH; j++)
		{
			tile[i][j] = 'x';
			printf("%c", tile[i][j]);
		}
		printf("\n");
	}

	printf("Currently running in mode %c with tile height of %d and tile width of %d.\n", MODE, TILE_HEIGHT, TILE_WIDTH);

	printf("What tile would you like to open? (Exclude file extention)\n");
	scanf("%s", &name);

	strcat(name, ".txt");

	FILE* tilefile = fopen(name, "r");

	printf("\n");

	printf("Opening file %s size %d by %d in mode %c.\n", name, TILE_WIDTH, TILE_HEIGHT, MODE);

	printf("\n");

	for (int i = 0; i < TILE_HEIGHT; i++)
	{
		for (int j = 0; j < TILE_WIDTH; j++)
		{
			fscanf(tilefile, "%c", &tile[i][j]);
		}
		fscanf(tilefile, "%*c");
	}
	
	fclose(tilefile);

	for (int i = 0; i < TILE_HEIGHT; i++)
	{
		for (int j = 0; j < TILE_WIDTH; j++)
		{
			print_color(tile[i][j]);
		}
		printf("\n");
	}


	//free(tile);
}
void print_color(char ch)
{
	switch (ch)
	{
	case 'H': printf(BLACK);
		putchar(' ');
		printf(RESET);
		break;
	case 'R': printf(DARK_RED);
		putchar(' ');
		printf(RESET);
		break;
	case 'G': printf(DARK_GREEN);
		putchar(' ');
		printf(RESET);
		break;
	case 'Y': printf(DARK_YELLOW);
		putchar(' ');
		printf(RESET);
		break;
	case 'B': printf(DARK_BLUE);
		putchar(' ');
		printf(RESET);
		break;
	case 'M': printf(DARK_MAGENTA);
		putchar(' ');
		printf(RESET);
		break;
	case 'C': printf(DARK_CYAN);
		putchar(' ');
		printf(RESET);
		break;
	case 'W': printf(DARK_WHITE);
		putchar(' ');
		printf(RESET);
		break;
	case 'h': printf(BRIGHT_BLACK);
		putchar(' ');
		printf(RESET);
		break;
	case 'r': printf(BRIGHT_RED);
		putchar(' ');
		printf(RESET);
		break;
	case 'g': printf(BRIGHT_GREEN);
		putchar(' ');
		printf(RESET);
		break;
	case 'y': printf(BRIGHT_YELLOW);
		putchar(' ');
		printf(RESET);
		break;
	case 'b': printf(BRIGHT_BLUE);
		putchar(' ');
		printf(RESET);
		break;
	case 'm': printf(BRIGHT_MAGENTA);
		putchar(' ');
		printf(RESET);
		break;
	case 'c': printf(BRIGHT_CYAN);
		putchar(' ');
		printf(RESET);
		break;
	case 'w': printf(WHITE);
		putchar(' ');
		printf(RESET);
		break;
	default: printf("%c", ch);
	}
}
void edit_tile()
{
	char name[64];
	char tile[TILE_HEIGHT][TILE_WIDTH];
	int xpos = 0;
	int ypos = 0;

	int input = 0;

	printf("Currently running in mode %c with tile height of %d and tile width of %d.\n", MODE, TILE_HEIGHT, TILE_WIDTH);

	printf("What tile would you like to open? (Exclude file extention)\n");
	scanf("%s", &name);

	strcat(name, ".txt");

	FILE* tilefile = fopen(name, "r");

	printf("\n");

	printf("Opening file %s size %d by %d in mode %c.\n", name, TILE_WIDTH, TILE_HEIGHT, MODE);

	printf("\n");

	for (int i = 0; i < TILE_HEIGHT; i++)
	{
		for (int j = 0; j < TILE_WIDTH; j++)
		{
			fscanf(tilefile, "%c", &tile[i][j]);
		}
		fscanf(tilefile, "%*c");
	}


	for (int i = 0; i < TILE_HEIGHT; i++)
	{
		for (int j = 0; j < TILE_WIDTH; j++)
		{
			print_color(tile[i][j]);
		}
		printf("\n");
	}

	print_pallete();
	print_options();

	while (input != 21)
	{
		input = system("CHOICE /N /C:wasd12345678ertyuiopcx");

		switch (input)
		{
		case 1: if (ypos - 1 >= 0)
		{
			ypos = ypos - 1;
		}
			  break;
		case 2: if (xpos - 1 >= 0)
		{
			xpos = xpos - 1;
		}
			  break;
		case 3: if (ypos + 1 < TILE_HEIGHT)
		{
			ypos = ypos + 1;
		}
			  break;
		case 4: if (xpos + 1 < TILE_WIDTH)
		{
			xpos = xpos + 1;
		}
			  break;
		case 5: tile[ypos][xpos] = 'H';
			break;
		case 6: tile[ypos][xpos] = 'R';
			break;
		case 7: tile[ypos][xpos] = 'G';
			break;
		case 8: tile[ypos][xpos] = 'Y';
			break;
		case 9: tile[ypos][xpos] = 'B';
			break;
		case 10: tile[ypos][xpos] = 'M';
			break;
		case 11: tile[ypos][xpos] = 'C';
			break;
		case 12: tile[ypos][xpos] = 'W';
			break;
		case 13: tile[ypos][xpos] = 'h';
			break;
		case 14: tile[ypos][xpos] = 'r';
			break;
		case 15: tile[ypos][xpos] = 'g';
			break;
		case 16: tile[ypos][xpos] = 'y';
			break;
		case 17: tile[ypos][xpos] = 'b';
			break;
		case 18: tile[ypos][xpos] = 'm';
			break;
		case 19: tile[ypos][xpos] = 'c';
			break;
		case 20: tile[ypos][xpos] = 'w';
			break;
		case 22: 	
			fclose(tilefile);
			FILE* output = fopen(name, "w");
			for (int i = 0; i < TILE_HEIGHT; i++)
		{
			
			for (int j = 0; j < TILE_WIDTH; j++)
			{
				fprintf(output, "%c", tile[i][j]);
			}
			fprintf(output,"\n");
		}
			input = 21;
			fclose(output);
			break;
		}
	
		system("cls");

		printf("\n");

		for (int i = 0; i < TILE_HEIGHT; i++)
		{
			printf("  ");
			for (int j = 0; j < TILE_WIDTH; j++)
			{
				if (i == ypos && j == xpos)
				{
					set_color(tile[i][j]);
					printf(BOLD);
					printf(BLACK_TEXT"_"RESET);
				}
				else
				{
					print_color(tile[i][j]);
				}
			}
			printf("\n");
		}
		print_pallete();
		print_options();
	}
	fclose(tilefile);
	system("cls");
}
void print_pallete()
{
	printf("\n");
	printf(BLACK" 1 "RESET);
	printf(DARK_RED BLACK_TEXT" 2 "RESET);
	printf(DARK_GREEN BLACK_TEXT" 3 "RESET);
	printf(DARK_YELLOW BLACK_TEXT" 4 "RESET);
	printf(DARK_BLUE BLACK_TEXT" 5 "RESET);
	printf(DARK_MAGENTA BLACK_TEXT" 6 "RESET);
	printf(DARK_CYAN BLACK_TEXT" 7 "RESET);
	printf(DARK_WHITE BLACK_TEXT" 8 "RESET);

	printf("\n");

	printf(BRIGHT_BLACK BLACK_TEXT" e "RESET);
	printf(BRIGHT_RED BLACK_TEXT" r "RESET);
	printf(BRIGHT_GREEN BLACK_TEXT" t "RESET);
	printf(BRIGHT_YELLOW BLACK_TEXT" y "RESET);
	printf(BRIGHT_BLUE BLACK_TEXT" u "RESET);
	printf(BRIGHT_MAGENTA BLACK_TEXT" i "RESET);
	printf(BRIGHT_CYAN BLACK_TEXT" o "RESET);
	printf(WHITE BLACK_TEXT" p "RESET);

	printf("\n");
	printf("\n");
}
void print_options()
{
	printf(BRIGHT_GREEN"(X) Save and exit"RESET);
	printf(BRIGHT_RED" (C) Exit without saving"RESET);
	printf("\n");
}
void set_color(char ch)
{
switch (ch)
{
case 'H': printf(BLACK);
	break;
case 'R': printf(DARK_RED);
	break;
case 'G': printf(DARK_GREEN);
	break;
case 'Y': printf(DARK_YELLOW);
	break;
case 'B': printf(DARK_BLUE);
	break;
case 'M': printf(DARK_MAGENTA);
	break;
case 'C': printf(DARK_CYAN);
	break;
case 'W': printf(DARK_WHITE);
	break;
case 'h': printf(BRIGHT_BLACK);
	break;
case 'r': printf(BRIGHT_RED);
	break;
case 'g': printf(BRIGHT_GREEN);
	break;
case 'y': printf(BRIGHT_YELLOW);
	break;
case 'b': printf(BRIGHT_BLUE);
	break;
case 'm': printf(BRIGHT_MAGENTA);
	break;
case 'c': printf(BRIGHT_CYAN);
	break;
case 'w': printf(WHITE);
	break;
default: printf(BLACK);
}
}