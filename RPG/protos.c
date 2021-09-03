#include "header.h"

//Calculates the size of the screen in characters, with a boolean argument for whether or not we want a pound sign border.
int calc_screen_size(int border)
{
    //Calculate the size based on macros
    int size = 0;
    size = ((width * height)*(tile_height*tile_width));

    //Print size before adding a border
    printf("Size pre border: %d\n", size);

    //If we want a border, include it.
    if (border == 1)
    {
        printf("Border size %d\n", ((((width + tile_width) + 2) * 2)+((height * tile_height)*2)));
        size = size + (((width*tile_width)+2)*2) + ((height*tile_height)*2);
        printf("Total size %d\n", size);
    }
    //Pause so we can read debug info
    system("PAUSE");

    //Return complete calculation
    return size;
}
//Maps where a character in a 2D array should be written to a 1D array. If we wanted to figure out where to write a character at 5, 4 on the 2D array to on the 1D array, after running this function we could access
//tile_map[5][4]. Even better, we can just write to scrstr[tile_map[5][4]].
void mapping(int tile_map[(height*width)][(tile_height*tile_width)], int screen_size, int position_map[height][width])
{
    //Create a temporary 1D array to use for calculation
    int *screen_mapping = malloc(screen_size * sizeof(int*));
    memset(screen_mapping, 0, screen_size * sizeof(int*));

    //Create a counter to use for translating 2D to 1D
    int count = 0;

    //This loop maps is the meat of this function.
    for(int l = 0; l <height; l++)
    {
        for (int k = 0; k < tile_height; k++)
        {
            //Writes the actual mapping position.
            for (int i = 0 + (l*width); i < width+(l*width); i++)
            {
                for (int j = 0; j < tile_width; j++) {
                    screen_mapping[count] = i;
                    count = count+1;
                }
            }
        }
    }


    //Sets the tile map to 0
    for(int i = 0; i <(height*width); i++)
    {
        for (int j = 0; j <(tile_height*tile_width); j++)
        {
            tile_map[i][j] = 0;
        }
    }

    debug_printer(2);


    //Runs through the tile map and writes the tile number to all positions where that tile would occupy. For example, if our screen was 4 tiles by 2 tiles, and each tile was 2 characters by 2 characters,
    //This loop would leave tile_map looking like this:
    //00112233
    //00112233
    //44556677
    //44556677
    for (int q = 0; q < screen_size; q++)
    {
        for(int j = 0; j<(tile_height*tile_width); j++)
        {
            if (tile_map[screen_mapping[q]][j] == 0)
            {
                tile_map[screen_mapping[q]][j] = q;
                break;
            }
        }

    }

    int tick =0;
    for(int i = 0; i<height;i++)
    {
        for(int j = 0; j<width;j++)
        {
            position_map[i][j] = tick;
            tick = tick+1;
        }
    }

    //free our temporary 1D array
    free(screen_mapping);

}
//Writes scene specific information to the screen string so that we don't have to do it every time we print the screen.
void screen_manager(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], struct tile* Tiles, int tile_ids[width][height], int tile_frequency[(width*height)], int *linear_ids, int screen_size, char mode)
{
    //unique_count is for counting how many unique tiles are being used. If we only had 2 tiles and they were both id 2, unique_count would be 1. If we had two tiles and they were id 1 and id 2, unique_count would be 2.
    int unique_count = 0;

    //Stores the ids of the tiles being used.
    int used_tiles[(width*height)] = {0};

    //Create a counter for translating 2D to 1D.
    int tick = 0;

    //Transforms tile_ids from 2D to 1D
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            linear_ids[tick] = tile_ids[j][i];
            tick = tick+1;
        }
    }

    //Sets the whole screen to # (35). Safety measure.
    for(int i = 0; i<screen_size; i++)
    {
        if(mode == 'p')
        {
            scrstr[i] = 104;
        }
        else{
            scrstr[i] = 35;
        }

    }

    //Log the ID of each unique tile that will need to be loaded, as well as count how many
    for(int i = 0; i<(width*height); i++)
    {
        if(tile_frequency[i]>0)
        {
            for(int j = 0; j<(width*height); j++)
            {
                if(used_tiles[j] == 0)
                {
                    unique_count = unique_count + 1;
                    used_tiles[j] = i;
                    break;
                }
            }
        }
    }

    //Opens the file for the current tile being loaded, and loads it into a linear array while eating newlines
    for(int i = 0; i<unique_count; i++)
    {
        //Access the file for the tile we are currently working with
        FILE* loaded = fopen((Tiles+(used_tiles[i]))->file, "r");

        //prenewline is the first step of reading in the tile. This step includes newline characters which we want to weed out.
        char prenewline[(tile_width*tile_height)+tile_height];

        //current is where we will write the tile information excluding newlines
        char current[(tile_width*tile_height)];

        //Reads tile into prenewline
        for(int n = 0; n < ((tile_width*tile_height)+tile_height); n++)
        {

            fscanf(loaded, "%c", &prenewline[n]);
        }
        fclose(loaded);

        //Reset counter
        tick = 0;

        //Read prenewline into current, excluding newline characters (10)
        for(int n = 0; n < ((tile_width*tile_height)+tile_height); n++)
        {

            if(prenewline[n] != 10)
            {
                current[tick] = prenewline[n];
                tick = tick+1;
            }
        }


        //Scrubs through tile_ids in order, and if any of them match ids with the currently loaded tile, uses the tile_map to write each line of the tile to the right position in the screen string.
        //This loop is for actually putting the current tile's characters in the right place on the scrstr.
       for (int m = 0; m <(width*height); m++)
       {
           if(linear_ids[m] == (Tiles+used_tiles[i])->id)
           {
               for(int p = 0; p<(tile_width*tile_height); p++)
               {
                   if((int)current[p] == (int)blank_symbol)
                   {
                       scrstr[tile_map[m][p]] = 32;
                   }
                   else
                   {
                       scrstr[tile_map[m][p]] = (int)current[p];
                   }
               }
           }
       }
    }

    //Updates the background map for future use to restore a tile on the screen that gets changed from moving tiles
    for (int i =0; i<screen_size; i++)
    {
        bgmap[i] = scrstr[i];
    }

    //Now that bgmap is ready to be accessed to restore tiles, we can draw the UI layer.
    for(int k = 0; k<unique_count; k++)
    {
        for (int m = 0; m <(width*height); m++) {
            if (linear_ids[m] == (Tiles + used_tiles[k])->id) {
                if ((Tiles + used_tiles[k])->ui_id > 0) {
                    if ((Tiles + used_tiles[k])->ui_dir == 'u') {
                        ui_manager(scrstr, bgmap, (m - width), *(Tiles + (Tiles + used_tiles[k])->ui_id), 0, tile_map);
                    }
                    if ((Tiles + used_tiles[k])->ui_dir == 'd') {
                        ui_manager(scrstr, bgmap, (m + width), *(Tiles + (Tiles + used_tiles[k])->ui_id), 0, tile_map);
                    }
                    if ((Tiles + used_tiles[k])->ui_dir == 'l') {
                        ui_manager(scrstr, bgmap, (m - 1), *(Tiles + (Tiles + used_tiles[k])->ui_id), 0, tile_map);
                    }
                    if ((Tiles + used_tiles[k])->ui_dir == 'r') {
                        ui_manager(scrstr, bgmap, (m + 1), *(Tiles + (Tiles + used_tiles[k])->ui_id), 0, tile_map);
                    }
                }
            }
        }
    }

}
//Prints the screen
void print_screen(int *scrstr, int screen_size, char mode)
{

    //line_pos is created to track when we need to print a newline
    int line_pos = 0;

    //Runs through the scrstr printing the ints as characters, and inserting a newline when we've hit the end of a line.
    for(int i = 0; i<screen_size;i++)
    {

        if(mode == 'p')
        {
            switch((char)scrstr[i])
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
                default : printf("%c", (char)scrstr[i]);
            }
        }
        else
        {
            printf( "%c", (char)scrstr[i]);
        }

        if(line_pos == (tile_width*width)-1)
        {
            printf("\n");
            line_pos = -1;

        }
        line_pos = line_pos+1;
    }
}
//Accesses a scene an reads it to tile_ids.
void load_scene(struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)])
{

    //Open the file
    FILE *scene_file = fopen(scenes->file, "r");

    //Error check
    if (scene_file != NULL) {
        //Read directly in to tile_ids.
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                fscanf(scene_file, "%d", &tile_ids[j][i]);
            }
        }
    }
    //Close file
	fclose(scene_file);
}
//Handles everything related to movement, including collisions, warping (changing scene), and getting a message from a tile.
void move(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], int input, char player_tile[(tile_width*tile_height)], int *linear_ids, struct tile* Tiles, struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)], struct object *player, int screen_size, int *msg, char mode, int position_map[height][width], struct message *Messages, int *had_message)
{
   int prewarppos[2] ={0};
    if(input == 1)
    {
        if(player->pos[0]-1 >= 0 && (Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->flags[0] != 'c')
        {
            if((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->flags[1] == 'd')
            {
                prewarppos[0] = player->pos[0];
                prewarppos[1] = player->pos[1];
                player->pos[0] = (Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->warp[1];
                player->pos[1] = (Tiles+((tile_ids[(prewarppos[1])][(prewarppos[0])-1])))->warp[2];
                load_scene((scenes+(Tiles+((tile_ids[(prewarppos[1])][(prewarppos[0])-1])))->warp[0]), tile_ids, tile_frequency);
                get_frequency(tile_ids, tile_frequency);
                screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, screen_size, mode);
                system("cls");
                print_screen(scrstr, screen_size, mode);
                display_message(0, Messages, mode);
                print_player(player->pos, player_tile);
            }
            else
            {
                restore_to_pos(player->pos, scrstr, tile_map, position_map);
                player->pos[0] = player->pos[0]-1;
                print_player(player->pos, player_tile);
            }
            if((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->flags[2] == 'n')
            {
                jump_to(height*tile_height, 0);
                printf("\x1b[0J");
                display_message((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->msg_id, Messages, mode);
                *had_message = 1;
            }
            else if(*had_message == 1)
            {
                jump_to(height*tile_height, 0);
                printf("\x1b[0J");
                display_message(0, Messages, mode);
                *had_message = 0;
            }

        }
    }
    if(input == 2)
    {
        if(player->pos[1]-1 >= 0 && (Tiles+((tile_ids[(player->pos[1]-1)][(player->pos[0])])))->flags[0] != 'c')
        {
            if((Tiles+((tile_ids[(player->pos[1]-1)][(player->pos[0])])))->flags[1] == 'd')
            {
                prewarppos[0] = player->pos[0];
                prewarppos[1] = player->pos[1];
                player->pos[0] = (Tiles+((tile_ids[(player->pos[1]-1)][(player->pos[0])])))->warp[1];
                player->pos[1] = (Tiles+((tile_ids[(prewarppos[1]-1)][(prewarppos[0])])))->warp[2];
                load_scene((scenes+(Tiles+((tile_ids[(prewarppos[1]-1)][(prewarppos[0])])))->warp[0]), tile_ids, tile_frequency);
                get_frequency(tile_ids, tile_frequency);
                screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, screen_size, mode);
                system("cls");
                print_screen(scrstr, screen_size, mode);
                display_message(0, Messages, mode);
                print_player(player->pos, player_tile);
            }
            else
            {
                restore_to_pos(player->pos, scrstr, tile_map, position_map);
                player->pos[1] = player->pos[1]-1;
                print_player(player->pos, player_tile);
            }
            if((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->flags[2] == 'n')
            {
                jump_to(height*tile_height, 0);
                printf("\x1b[0J");
                display_message((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->msg_id, Messages, mode);
                *had_message = 1;
            }
            else if(*had_message == 1)
            {
                jump_to(height*tile_height, 0);
                printf("\x1b[0J");
                display_message(0, Messages, mode);
                *had_message = 0;
            }
        }
    }
    if(input == 3)
    {
        if (player->pos[0]+1 < height && (Tiles+((tile_ids[(player->pos[1])][(player->pos[0])+1])))->flags[0] != 'c')
        {
            if((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])+1])))->flags[1] == 'd')
            {
                prewarppos[0] = player->pos[0];
                prewarppos[1] = player->pos[1];
                player->pos[0] = (Tiles+((tile_ids[(player->pos[1])][(player->pos[0])+1])))->warp[1];
                player->pos[1] = (Tiles+((tile_ids[(prewarppos[1])][(prewarppos[0])+1])))->warp[2];
                load_scene((scenes+(Tiles+((tile_ids[(prewarppos[1])][(prewarppos[0])+1])))->warp[0]), tile_ids, tile_frequency);
                get_frequency(tile_ids, tile_frequency);
                screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, screen_size, mode);
                system("cls");
                print_screen(scrstr, screen_size, mode);
                display_message(0, Messages, mode);
                print_player(player->pos, player_tile);
            }
            else
            {
                restore_to_pos(player->pos, scrstr,tile_map, position_map);
                player->pos[0] = player->pos[0]+1;
                print_player(player->pos, player_tile);
            }
            if((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->flags[2] == 'n')
            {
                jump_to(height*tile_height, 0);
                printf("\x1b[0J");
                display_message((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->msg_id, Messages, mode);
                *had_message = 1;
            }
            else if(*had_message == 1)
            {
                jump_to(height*tile_height, 0);
                printf("\x1b[0J");
                display_message(0, Messages, mode);
                *had_message = 0;
            }
        }
    }
    if(input == 4)
    {
        if(player->pos[1]+1<width && (Tiles+((tile_ids[(player->pos[1])+1][(player->pos[0])])))->flags[0] != 'c')
        {
            if((Tiles+((tile_ids[(player->pos[1])+1][(player->pos[0])])))->flags[1] == 'd')
            {
                prewarppos[0] = player->pos[0];
                prewarppos[1] = player->pos[1];
                player->pos[0] = (Tiles+((tile_ids[(player->pos[1])+1][(player->pos[0])])))->warp[1];
                player->pos[1] = (Tiles+((tile_ids[(prewarppos[1])+1][(prewarppos[0])])))->warp[2];
                load_scene((scenes+(Tiles+((tile_ids[(prewarppos[1])+1][(prewarppos[0])])))->warp[0]), tile_ids, tile_frequency);
                get_frequency(tile_ids, tile_frequency);
                screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, screen_size, mode);
                system("cls");
                print_screen(scrstr, screen_size, mode);
                display_message(0, Messages, mode);
                print_player(player->pos, player_tile);
            }
            else
            {
                restore_to_pos(player->pos, scrstr, tile_map, position_map);
                player->pos[1] = player->pos[1]+1;
                print_player(player->pos, player_tile);
            }
            if((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->flags[2] == 'n')
            {
                jump_to(height*tile_height, 0);
                printf("\x1b[0J");
                display_message((Tiles+((tile_ids[(player->pos[1])][(player->pos[0])-1])))->msg_id, Messages, mode);
                *had_message = 1;
            }
            else if(*had_message == 1)
            {
                jump_to(height*tile_height, 0);
                printf("\x1b[0J");
                display_message(0, Messages, mode);
                *had_message = 0;
            }
        }
    }
}
//Prints the text box and message inside at the bottom of the screen
void print_menu(char text[], char mode)
{
    //textdone is a boolean for whether or not we are done with printing text. textcounter counts how much text we have printed so far.
    int textdone = 0, textcounter = 0;

    //prints an initial space, I just like how the menu looks being thinner by one character than the game on each side
    printf(" ");

    //prints the top border
    for (int i = 0; i < width * tile_width; i++)
    {
        if (i == 0 || i == ((width * tile_width) - 1))
        {
            printf(" ");
        }
        else
            {
            if(mode == 'p')
            {
                printf(BRIGHT_BLACK" "RESET);
            }
            else
            {
                printf("_");
            }
        }
    }
    printf("\n");

    //prints the left border, the line of text, then the right border
    for (int l = 0; l < (height * tile_height) / MENU_HEIGHT; l++)
    {
        printf(" ");
        for (int j = 0; j <= (width * tile_width) - 1; j++)
        {
            if (j == 0 || j == (width * tile_width) - 1)
            {
                if(mode == 'p')
                {
                    printf(BRIGHT_BLACK" "RESET);
                }
                else
                {
                    printf("|");
                }
            }
            else
                {
                if (text[textcounter] > 0 && text[textcounter] <= 127)
                {
                    if(mode == 'p')
                    {
                        if (textdone == 0) {
                            printf(WHITE);
                            printf(BLACK_TEXT"%c"RESET, text[textcounter]);
                            textcounter = textcounter + 1;
                        }
                        else {
                            printf(WHITE" "RESET);
                        }
                    }
                    else
                    {
                        if (textdone == 0) {
                            printf("%c", text[textcounter]);
                            textcounter = textcounter + 1;
                        }
                        else {
                            printf(" ");
                        }
                    }
                } else if(mode == 'p'){
                    printf(WHITE" "RESET);
                    textdone = 1;
                }
                else {
                    printf(" ");
                    textdone = 1;
                }
            }
        }
        printf("\n");
    }

    //Prints the bottom border
    if(mode =='p')
    {
        printf(" ");
        for (int i = 0; i < (width * tile_width); i++) {
            if (i == 0 || i == (width * tile_width) - 1) {
                printf(" ");
            } else {
                printf(BRIGHT_BLACK" "RESET);
            }
        }
    }
    else
    {
        printf(" ");
        for (int i = 0; i < (width * tile_width); i++) {
            if (i == 0 || i == (width * tile_width) - 1) {
                printf("|");
            } else {
                printf("_");
            }
        }
    }

    printf("\n");
}
//Gets the frequency of each tile in a scene
void get_frequency(int tile_ids[width][height], int tile_frequency[(width*height)])
{
    //Just a frequency table for the contents of tile_ids.
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			tile_frequency[tile_ids[j][i]] = tile_frequency[tile_ids[j][i]] + 1;
		}
	}
}
//A little debug printer I found helpful
void debug_printer(int number)
{
    printf("You've hit debug print number %d.\n", number);
    system("PAUSE");
}
//Reads tiles out of the index file
void read_tiles(int amount, struct tile* Tiles)
{
    //Open the index file
    FILE *index = fopen("TileIndex.txt", "r");
    for(int i = 0; i < amount; i++)
    {
        //Temporary variables to make things easier as apposed to reading directly into the array of Tiles struct (scary :Z)
        char name[MAX_NAME_SIZE];
        char file[MAX_NAME_SIZE];
        char flag1 = '\0';
        char flag2 = '\0';
        char flag3 = '\0';
        int warp1 = 0;
        int warp2 = 0;
        int warp3 = 0;
        int id = 0;
        int msg = 0;
        int ui_id = 0;
        char ui_dir = '\0';

        //Read into variable
        fscanf(index, "%d\n%s\n%s\n%c\n%c\n%c\n%d\n%d\n%d\n%d\n%d\n%c\n", &id, &name, &file, &flag1, &flag2, &flag3, &warp1, &warp2, &warp3, &msg, &ui_id, &ui_dir);

        //Set id
        (Tiles+i)->id = id;

        //Allocate correct amount of memory for the two strings
        Tiles[i].name = malloc(sizeof(char) * strlen(name));
        Tiles[i].file = malloc(sizeof(char) * strlen(file));

        //Copy in strings
        strcpy((Tiles+i)->name, name);
        strcpy((Tiles+i)->file, file);

        //Write in the rest of the data
        (Tiles+i)->flags[0] = flag1;
        (Tiles+i)->flags[1] = flag2;
        (Tiles+i)->flags[2] = flag3;
        (Tiles+i)->warp[0] = warp1;
        (Tiles+i)->warp[1] = warp2;
        (Tiles+i)->warp[2] = warp3;
        (Tiles+i)->msg_id = msg;
        (Tiles+i)->ui_id = ui_id;
        (Tiles+i)->ui_dir = ui_dir;

        //Clear the strings for the next round
        memset(name, 0, sizeof(char)*strlen(name));
        memset(file, 0, sizeof(char)*strlen(file));


    }
    //Close the file
    fclose(index);
}
//Reads scenes out of the scene index file
void read_scenes(int amount, struct asset* scenes)
{
    //Open the file
    FILE *index = fopen("SceneIndex.txt", "r");

    for(int i = 0; i < amount; i++)
    {
        //Temporary variables so I don't have to deal with reading right into the array of structs
        char file[MAX_NAME_SIZE];
        int id = 0;

        //Read into the variables
        fscanf(index, "%d\n%s\n", &id, &file);

        //Set id
        (scenes+i)->id = id;

        //Allocate correct amount of memory for string
        scenes[i].file = malloc(sizeof(char) * strlen(file));

        //Copy in string
        strcpy((scenes+i)->file, file);

        //Clear string for next cycle
        memset(file, 0, sizeof(char)*strlen(file));

    }
    //Close file
    fclose(index);
}
//Manages writing UI elements to the screen string. This has to be done seperately from other functions because like the player, UI elements support transparency.
void ui_manager(int *scrstr, int *bgmap, int pos, struct tile element, int operation, int tile_map[(height*width)][(tile_height*tile_width)])
{
    //Operation is a boolean, 0 for drawing an element, 1 for removing.
    if (operation == 0)
    {
        //Create a 1D verision of the element
        char linear_element[(tile_width*tile_height)];

        //Open the file
        FILE* pt = fopen(element.file, "r");

        //Counter
        int tick = 0;

        //Read into 1D array
        for(int i = 0; i<tile_height; i++)
        {
            for(int j = 0; j <tile_width;j++)
            {
                fscanf(pt, "%c", &linear_element[tick]);
                tick = tick + 1;
            }
            fscanf(pt, "%*c");
        }
        //Close file
        fclose(pt);

        //Draw element to correct position with transparency
        for(int i = 0; i < (tile_width*tile_height); i++)
        {
            if(linear_element[i] == trans_symbol)
            {
                scrstr[tile_map[pos][i]] = bgmap[tile_map[pos][i]];
            }
            else if (linear_element[i] == blank_symbol)
            {
                scrstr[tile_map[pos][i]] = 32;
            }
            else
            {
                scrstr[tile_map[pos][i]] = (int)linear_element[i];
            }
        }
    }

    //If we are removing an element, restore the position on scrstr with info from bgmap
    else if(operation == 1)
    {
        for(int i = 0; i < (tile_width*tile_height); i++)
        {
            scrstr[tile_map[pos][i]] = bgmap[tile_map[pos][i]];
        }
    }
}
//Handles reading in messages from text file
void read_messages(int amount, struct message* Messages)
{
    FILE *messages = fopen("Messages.txt", "r");
    char message[MAX_MSG_SIZE];
    int id;

    for(int i = 0; i<amount; i++)
    {
        fscanf(messages, "%d\n", &id);
        fscanf(messages, "%[^\n]%*c", message);

        (Messages+i)->id = id;
        Messages[i].text = malloc(sizeof(char) * strlen(message));

        strcpy((Messages+i)->text, message);
        memset(message, 0, sizeof(char)*strlen(message));
    }

}
//Handles displaying a message to the menu
void display_message(int id, struct message* Messages, char mode)
{
    if (id == 0)
    {
        print_menu(" ", mode);
    }
    else
    {
        print_menu((Messages+id)->text, mode);
    }
}
void clear_screen()
{
    printf("\x1b[%d;%df", 0, 0);
    printf("\x1b[0J");
}
void print_player(int pos[2], char player_tile[(tile_width*tile_height)])
{
    int tick = 0;
    for(int i = 0; i<tile_height;i++)
    {
        for(int j = 0; j<tile_width;j++)
        {
            jump_to(((pos[0]*tile_height)+i), ((pos[1]*tile_width+j)));
            switch(player_tile[tick])
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
            }
            tick = tick+1;
            jump_to((height*tile_height)+(MENU_HEIGHT*tile_height)+3, 0);
            printf("\x1b[0J");
        }
    }
}
void restore_to_pos(int pos[2], int *scrstr, int tile_map[(height*width)][(tile_height*tile_width)], int position_map[height][width])
{
    int tick = 0;
    for(int i = 0; i<tile_height;i++)
    {
        for(int j = 0; j<tile_width;j++)
        {
            jump_to(((pos[0]*tile_height)+i), ((pos[1]*tile_width+j)));

            switch((char)scrstr[tile_map[position_map[pos[0]][pos[1]]][tick]])
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
            }
            tick = tick+1;
            jump_to((height*tile_height)+(MENU_HEIGHT*tile_height)+3, 0);
            printf("\x1b[0J");
        }
    }
}
void jump_to(int row, int col)
{
    printf("\x1b[%d;%df", row+1, col+1);
}