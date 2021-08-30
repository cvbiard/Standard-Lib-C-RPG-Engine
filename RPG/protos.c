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
void mapping(int tile_map[(height*width)][(tile_height*tile_width)], int screen_size)
{
    //Create a temporary 1D array to use for calculation
    int *screen_mapping = (int)malloc(screen_size * sizeof(int));

    //Create a counter to use for translating 2D to 1D
    int count = 0;

    //Writes the top border positions.
    for (int i = 0; i < ((width*tile_width)+2); i++)
    {
        *(screen_mapping + count) = (width*height)+2;
        count = count + 1;
    }

    //This loop maps is the meat of this function.
    for(int l = 0; l <height; l++)
    {
        for (int k = 0; k < tile_height; k++)
        {
            //Writes the left border positions for this line.
           *(screen_mapping + count) = (width*height)+2;
            count = count + 1;

            //Writes the actual mapping position.
            for (int i = 0 + (l*width); i < width+(l*width); i++)
            {
                for (int j = 0; j < tile_width; j++) {
                    *(screen_mapping + count) = i;
                    count = count+1;
                }
            }
            //Writes the right border positions for this line.
            *(screen_mapping + count) = (width*height)+2;
            count = count+1;
        }
    }

    //Writes the bottom border positions.
    for (int i = (screen_size - ((width*tile_width)+2)); i < screen_size; i++)
    {
        *(screen_mapping + count) = (width*height)+2;
        count = count + 1;
    }

    //Sets the tile map to 0
    for(int i = 0; i <(height*width); i++)
    {
        for (int j = 0; j <(tile_height*tile_width); j++)
        {
            tile_map[i][j] = 0;
        }
    }

    //Runs through the tile map and writes the tile number to all positions where that tile would occupy. For example, if our screen was 4 tiles by 2 tiles, and each tile was 2 characters by 2 characters,
    //This loop would leave tile_map looking like this:
    //00112233
    //00112233
    //44556677
    //44556677
    for (int i = 0; i<screen_size; i++)
    {
        for(int j = 0; j<(tile_height*tile_width); j++)
        {
            if (tile_map[*(screen_mapping+i)][j] == 0)
            {
                tile_map[*(screen_mapping+i)][j] = i;
                break;
            }
        }

    }

    //free our temporary 1D array
    free(screen_mapping);

}

//Writes scene specific information to the screen string so that we don't have to do it every time we print the screen.
void screen_manager(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], struct tile* Tiles, int tile_ids[width][height], int tile_frequency[(width*height)], int *linear_ids, int pos, char player_tile[(tile_width*tile_height)], int screen_size)
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
        scrstr[i] = 35;
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

    //Here is where the player tile is printed on to the string. We need to do this before the first time move() is run each scene so that the player appears the first time a scene is printed.
    //On top of this, the player is it's own loop after the background since the player tile supports transparency, so we have to know what tile is behind the player before we add them to the string.
    for(int i = 0; i < (tile_width*tile_height); i++)
    {
        //If we see a transparent symbol, use bgmap to find the character of the tile "behind" the player.
        if(player_tile[i] == trans_symbol)
        {
            scrstr[tile_map[pos][i]] = bgmap[tile_map[pos][i]];
        }
        //If we hit a blank symbol, draw a space (32);
        else if (player_tile[i] == blank_symbol)
        {
            scrstr[tile_map[pos][i]] = 32;
        }
        //If we hit any character that isn't transparent or blank, draw the correct character.
        else
        {
            scrstr[tile_map[pos][i]] = (int) player_tile[i];
        }
    }

}
//Prints the screen
void print_screen(int *scrstr, int screen_size)
{

    //line_pos is created to track when we need to print a newline
    int line_pos = 0;

    //Runs through the scrstr printing the ints as characters, and inserting a newline when we've hit the end of a line.
    for(int i = 0; i<screen_size;i++)
    {
        printf("%c", (char)scrstr[i]);
        if(line_pos == (tile_width*width)+1)
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
int move(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], int input, char player_tile[(tile_width*tile_height)], int *linear_ids, struct tile* Tiles, struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)], struct object *player, int screen_size, int msg[1])
{
    //Linear movement to a 2D space
    //One space down is +10
    //One space up is -10
    //One space right is +1
    //One left is -1

    //A variable to hold the player's position before a warp.
    int prewarppos = 0;

    //If the input is w, we want to go up so we need to get information from the tile directly above the player.
    if (input == 1)
    {
        //Make sure the tile we want to go to is in bounds, and also check if it is a door (warp point).
        if(((player->pos) - width) >= 0 && (Tiles + linear_ids[(player->pos) - width])->flags[1] == 'd')
        {
            //If we are in bounds and we are going in to a door, store the position we were before the warp.
            prewarppos = player->pos;

            //Change the player's position to the post warp position (warp[1] of the warp tile we are stepping on)
            player->pos = (Tiles + linear_ids[(player->pos) - width])->warp[1];

            //Load the new scene we are warping in to. We access the id from the original tile we stepped on's warp[0].
            load_scene((scenes+(Tiles + linear_ids[prewarppos - width])->warp[0]), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);

            //Run screen_manager to load everything for the new scene.
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, (Tiles + linear_ids[prewarppos - width])->warp[1], player_tile, screen_size);

            //return the player's position for the next input cycle.
            return player->pos;
        }
        //If the tile isn't a door, we need to make sure it is in bounds and doesn't have a collision flag (meaning we can't move there)
        if (((player->pos) - width) >= 0 && (Tiles + linear_ids[((player->pos) - width)])->flags[0] != 'c')
        {

            //If the tile we want to go to is in bounds and isn't solid, we need to write the player tile to that position, and restore the background tile from the previous position with the info from bgmap.
            for(int i = 0; i < (tile_width*tile_height); i++)
            {
                scrstr[tile_map[(player->pos)][i]] = bgmap[tile_map[(player->pos)][i]];
                if(player_tile[i] == trans_symbol)
                {
                    scrstr[tile_map[(player->pos) - width][i]] = bgmap[tile_map[(player->pos) - width][i]];
                }
                else if (player_tile[i] == blank_symbol)
                {
                    scrstr[tile_map[(player->pos) - width][i]] = 32;
                }
                else
                {
                    scrstr[tile_map[(player->pos) -width][i]] = (int) player_tile[i];
                }
            }
            //Checking if the tile directly above us is an NPC. If so, access the message they give and print it.
            if((Tiles + linear_ids[((player->pos) - width)-width])->flags[2] == 'n')
            {
                msg[0] = (Tiles + linear_ids[((player->pos) - width)-width])->msg_id;
            }
            //If we don't need to print an NPC message, print a blank message.
            else
            {
                msg[0] = 0;
            }
            //If we make it to this return, the player has successfully moved one tile upward. To set their position correctly, we return their previous position minus one width of the screen.
            return (player->pos) - width;
        }
    }

    //If the input is s, we need to check the information for the tile directly below the player.
    if (input == 3)
    {
        //The only difference between other inputs and w is the direction we look for potential movement, and how we check bounds. There's no way to move down and get to an out of bounds position <0, so we only need to check
        //above the maximum position. Besides that, the only difference is moving down a tile is adding one width to the position instead of subtracting it.
        if((player->pos) + width <= ((width*height)-1) && (Tiles + linear_ids[(player->pos) + width])->flags[1] == 'd')
        {
            //If we hit a door
            prewarppos = player->pos;
            player->pos = (Tiles + linear_ids[(player->pos) + width])->warp[1];
            load_scene((scenes+(Tiles + linear_ids[prewarppos + width])->warp[0]), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, (Tiles + linear_ids[prewarppos + width])->warp[1], player_tile, screen_size);
            return player->pos;
        }
        if ((player->pos) + width <= ((width*height)-1) && (Tiles + linear_ids[(player->pos) + width])->flags[0]!= 'c')
        {
            //Draw player if we aren't colliding
            for(int i = 0; i < (tile_width*tile_height); i++)
            {
                scrstr[tile_map[(player->pos)][i]] = bgmap[tile_map[(player->pos)][i]];
                if(player_tile[i] == trans_symbol)
                {
                    scrstr[tile_map[(player->pos) + width][i]] = bgmap[tile_map[(player->pos) + width][i]];
                }
                else if (player_tile[i] == blank_symbol)
                {
                    scrstr[tile_map[(player->pos) + width][i]] = 32;
                }
                else
                {
                    scrstr[tile_map[(player->pos) + width][i]] = (int) player_tile[i];
                }
            }
            //Check NPC for message
            if((Tiles + linear_ids[((player->pos) + width)-width])->flags[2] == 'n')
            {
                msg[0] = (Tiles + linear_ids[((player->pos) + width)-width])->msg_id;
            }
            else
            {
                msg[0] = 0;
            }
            //Return new position
            return (player->pos) + width;
        }
    }
    //Same deal, but now we want to check to the left, which is -1 instead of adding or subtracting width.
    if (input == 2)
    {
        if((player->pos) - 1 >= 0 && (Tiles + linear_ids[(player->pos) - 1])->flags[1] == 'd')
        {
            //If we hit a door
            prewarppos = player->pos;
            player->pos = (Tiles + linear_ids[(player->pos) - 1])->warp[1];
            load_scene((scenes+(Tiles + linear_ids[prewarppos - 1])->warp[0]), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, (Tiles + linear_ids[prewarppos - 1])->warp[1], player_tile, screen_size);
            return player->pos;
        }
        if ((player->pos) - 1 >= 0 && (player->pos)%width != 0 && (Tiles + linear_ids[(player->pos) - 1])->flags[0]!= 'c')
        {
            //Draw the player if we aren't colliding
            for(int i = 0; i < (tile_width*tile_height); i++)
            {
                scrstr[tile_map[(player->pos)][i]] = bgmap[tile_map[(player->pos)][i]];
                if(player_tile[i] == trans_symbol)
                {
                    scrstr[tile_map[(player->pos) - 1][i]] = bgmap[tile_map[(player->pos) - 1][i]];
                }
                else if (player_tile[i] == blank_symbol)
                {
                    scrstr[tile_map[(player->pos) - 1][i]] = 32;
                }
                else
                {
                    scrstr[tile_map[(player->pos) -1][i]] = (int) player_tile[i];
                }
            }
            //Check NPC message
            if((Tiles + linear_ids[((player->pos) - 1)-width])->flags[2] == 'n')
            {
                msg[0] = (Tiles + linear_ids[((player->pos) - 1)-width])->msg_id;
            }
            else
            {
                msg[0] = 0;
            }
            //Return new position
            return (player->pos) - 1;
        }
    }
    //And finally, now we add one to position instead of subtract to move to the right.
    if (input == 4)
    {
        if((player->pos)%width != (width-1)  && (Tiles + linear_ids[(player->pos) + 1])->flags[1] == 'd')
        {
            //If we hit a door
            prewarppos = player->pos;
            player->pos = (Tiles + linear_ids[(player->pos) + 1])->warp[1];
            load_scene((scenes+(Tiles + linear_ids[prewarppos + 1])->warp[0]), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, (Tiles + linear_ids[prewarppos + 1])->warp[1], player_tile, screen_size);
            return player->pos;
        }
        if ((player->pos) + 1 >= 0 && (player->pos)%width != (width-1)  && (Tiles + linear_ids[(player->pos) + 1])->flags[0]!= 'c')
        {
            //Draw the player
            for(int i = 0; i < (tile_width*tile_height); i++)
            {
                scrstr[tile_map[(player->pos)][i]] = bgmap[tile_map[(player->pos)][i]];
                if(player_tile[i] == trans_symbol)
                {
                    scrstr[tile_map[(player->pos) + 1][i]] = bgmap[tile_map[(player->pos) + 1][i]];
                }
                else if (player_tile[i] == blank_symbol)
                {
                    scrstr[tile_map[(player->pos) + 1][i]] = 32;
                }
                else
                {
                    scrstr[tile_map[(player->pos) + 1][i]] = (int) player_tile[i];
                }
            }
            //Check NPC message
            if((Tiles + linear_ids[((player->pos) + 1)-width])->flags[2] == 'n')
            {
                msg[0] = (Tiles + linear_ids[((player->pos) +1)-width])->msg_id;
            }
            else
            {
                msg[0] = 0;
            }
            //Return new position
            return (player->pos) + 1;
        }
    }

    //Safety return.
    return (player->pos);

}
//Prints the text box and message inside at the bottom of the screen
void print_menu(char text[])
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
        else {
            printf("_");
        }
    }
    printf("\n");

    //prints the left border, the line of text, then the right border
    for (int l = 0; l < (height * tile_height) / 3; l++)
    {
        printf(" ");
        for (int j = 0; j <= (width * tile_width) - 1; j++)
        {
            if (j == 0 || j == (width * tile_width) - 1)
            {
                printf("|");
            }
            else
                {
                if (text[textcounter] > 0 && text[textcounter] <= 127)
                {
                    if (textdone == 0) {
                        printf("%c", text[textcounter]);
                        textcounter = textcounter + 1;
                    } else {
                        printf(" ");
                    }
                } else {
                    printf(" ");
                    textdone = 1;
                }
            }
        }
        printf("\n");
    }

    //Prints the bottom border
    printf(" ");
    for (int i = 0; i < (width * tile_width); i++) {
        if (i == 0 || i == (width * tile_width) - 1) {
            printf("|");
        } else {
            printf("_");
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
        int id = 0;
        int msg = 0;

        //Read into variable
        fscanf(index, "%d\n%s\n%s\n%c\n%c\n%c\n%d\n%d\n%d\n", &id, &name, &file, &flag1, &flag2, &flag3, &warp1, &warp2, &msg);

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
        (Tiles+i)->msg_id = msg;

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
                scrstr[tile_map[pos][i]] = (int) linear_element[i];
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
//Handles storing and reading messages to be displayed in the text box at the bottom of the screen
void read_message(int id)
{
    //Local variables for now, need to work on actually getting these from a file instead of keeping them on the stack
    char message[MAX_MSG_SIZE] = " ";
    char message2[MAX_MSG_SIZE] = "This is a test message from me, a test NPC!";
    char msg_index[5][MAX_MSG_SIZE];

    //Copy messages into index
    strcpy(msg_index[0], message);
    strcpy(msg_index[1], message2);

    //Print out the requested message
    print_menu(msg_index[id]);
}