#include "header.h"

int calc_screen_size(int border)
{
    int size = 0;
    size = ((width * height)*(tile_height*tile_width));
    printf("Size pre border: %d\n", size);
    if (border == 1)
    {
        printf("Border size %d\n", ((((width + tile_width) + 2) * 2)+((height * tile_height)*2)));
        size = size + (((width*tile_width)+2)*2) + ((height*tile_height)*2);
        printf("Total size %d\n", size);
    }
    system("PAUSE");

    //debug_printer(3);
    return size;
}
void map_pos(int linear_pos[(height*width)][2])
{
    for (int i = 0; i < height*width; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            linear_pos[i][j] = j;
        }
    }
}
void print_screen_map()
{
    int screen_map[1984] = {0};
    int count = 0;
    FILE* outfile = fopen("screen_map.txt", "w");
    for (int i = 0; i < 62; i++)
    {
        //printf("#");
        screen_map[count] = 102;
        count = count + 1;
    }
    for(int l = 0; l <10; l++)
    {
        for (int k = 0; k < 3; k++) {
            //printf("#");
            screen_map[count] = 102;
            count = count + 1;
            for (int i = 0 + (l*10); i < 10+(l*10); i++) {

                for (int j = 0; j < 6; j++) {
                    //printf("%d", i);
                    screen_map[count] = i;
                    count = count+1;
                }
            }
            screen_map[count] = 102;
            count = count+1;
            //printf("#");
        }
    }
    for (int i = 1922; i < 1984; i++)
    {
        //printf("#");
        screen_map[count] = 102;
        count = count + 1;
    }
    for (int i = 0; i<1984; i++)
    {
        fprintf(outfile, "%d, ", screen_map[i]);
    }
    fclose(outfile);
}
void mapping(int tile_map[(height*width)][(tile_height*tile_width)], int screen_size)
{
    int *screen_mapping = (int)malloc(screen_size * sizeof(int));
    int count = 0;

    for (int i = 0; i < ((width*tile_width)+2); i++)
    {
        //printf("#");
        *(screen_mapping + count) = ((width*height)+2); //was 102
        count = count + 1;
    }
    for(int l = 0; l <height; l++)
    {
        for (int k = 0; k < tile_height; k++) {
            //printf("#");
            *(screen_mapping + count) = ((width*height)+2); //was 102
            count = count + 1;
            for (int i = 0 + (l*width); i < width+(l*width); i++) {

                for (int j = 0; j < tile_width; j++) {
                    //printf("%d", i);
                    *(screen_mapping + count) = i;
                    count = count+1;
                }
            }
            *(screen_mapping + count) = ((width*height)+2); //was 102 idk why
            count = count+1;
            //printf("#");
        }
    }
    for (int i = (screen_size - ((width*tile_width)+2)); i < screen_size; i++)
    {
        //printf("#");
        *(screen_mapping + count) = ((width*height)+2); //was 102, not sure why
        count = count + 1;
    }


    for(int i = 0; i <(height*width); i++)
    {
        for (int j = 0; j <(tile_height*tile_width); j++)
        {
            tile_map[i][j] = 0;
        }
    }

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

    free(screen_mapping);

}
void screen_manager(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], struct tile* Tiles, int tile_ids[width][height], int tile_frequency[(width*height)], int *linear_ids, int pos, char player_tile[(tile_width*tile_height)], int screen_size)
{
    int unique_count = 0;
    int used_tiles[(width*height)] = {0};
    int tick = 0;

    //Transforms tile_ids from 10x10 grid to a linear 100
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            linear_ids[tick] = tile_ids[j][i];
            tick = tick+1;
        }
    }

    //debug_printer(20);
    //system("PAUSE");

    //Sets the whole screen to # (35). If overwriting works, this should just leave a border of #
    for(int i = 0; i<screen_size; i++)
    {
        scrstr[i] = 35;
    }

   //debug_printer(30);

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
        FILE* loaded = fopen((Tiles+(used_tiles[i]))->file, "r");
        char prenewline[(tile_width*tile_height)+tile_height];
        char current[(tile_width*tile_height)];
        for(int n = 0; n < ((tile_width*tile_height)+tile_height); n++)
        {

            fscanf(loaded, "%c", &prenewline[n]);
            //printf("%c", current[n]);
        }
        fclose(loaded);

        int tick = 0;
        for(int n = 0; n < ((tile_width*tile_height)+tile_height); n++)
        {

            if(prenewline[n] != 10)
            {
                current[tick] = prenewline[n];
                tick = tick+1;
            }
        }
        //debug_printer(20);

        //Scrubs through tile ids in order, and if any of them match ids with the currently loaded tile, uses the tile_map to write each line of the tile to the right position in the screen string.
       for (int m = 0; m <(width*height); m++)
       {
           //printf("%d\n", linear_ids[m]);
           //debug_printer(40);
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
    //debug_printer(30);

    //Updates the background map for future use to restore a tile on the screen that gets changed from moving tiles
    for (int i =0; i<screen_size; i++)
    {
        bgmap[i] = scrstr[i];
    }

    //Here is where the player tile is printed on to the string... Can't do UI here since we don't want to rerun screen_manager every step but we could use this code in print_screen maybe.
    for(int i = 0; i < (tile_width*tile_height); i++)
    {
        if(player_tile[i] == trans_symbol)
        {
            scrstr[tile_map[pos][i]] = bgmap[tile_map[pos][i]];
        }
        else if (player_tile[i] == blank_symbol)
        {
            scrstr[tile_map[pos][i]] = 32;
        }
        else
        {
            scrstr[tile_map[pos][i]] = (int) player_tile[i];
        }
    }

}
void print_screen(int *scrstr, int screen_size)
{

    int line_pos = 0;
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
void load_scene(struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)])
{

    FILE *scene_file = fopen(scenes->file, "r");
    if (scene_file != NULL) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                fscanf(scene_file, "%d", &tile_ids[j][i]);
                //ref[j][i] = tile_ids[j][i];
            }
        }
    }
	fclose(scene_file);
    //tile_ids[pos[1]][pos[0]] = 1;
}
int move(int *scrstr, int *bgmap, int tile_map[(height*width)][(tile_height*tile_width)], char input, char player_tile[(tile_width*tile_height)], int *linear_ids, int linear_pos[(height*width)][2], struct tile* Tiles, struct asset* scenes, int tile_ids[width][height], int tile_frequency[(width*height)], struct object *player, int screen_size, int msg[1])
{
    //Linear movement to a 2D space
    //One space down is +10
    //One space up is -10
    //One space right is +1
    //One left is -1
    int prewarppos = 0;

    if (input == 'w' || input == 'W')
    {
        if(((player->pos) - width) >= 0 && (Tiles + linear_ids[(player->pos) - width])->flags[1] == 'd')
        {
            //debug_printer(9);
            prewarppos = player->pos;
            player->pos = (Tiles + linear_ids[(player->pos) - width])->warp[1];
            load_scene((scenes+(Tiles + linear_ids[prewarppos - width])->warp[0]), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, (Tiles + linear_ids[prewarppos - width])->warp[1], player_tile, screen_size);
            return player->pos;
        }
        if (((player->pos) - width) >= 0 && (Tiles + linear_ids[((player->pos) - width)])->flags[0] != 'c')
        {
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
            if((Tiles + linear_ids[((player->pos) - width)-width])->flags[2] == 'n')
            {
                msg[0] = (Tiles + linear_ids[((player->pos) - width)-width])->msg_id;
                //debug_printer(100);
            }
            else
            {
                msg[0] = 0;
            }
            return (player->pos) - width;
        }
    }
    if (input == 's' || input == 'S')
    {
        if((player->pos) + width <= ((width*height)-1) && (Tiles + linear_ids[(player->pos) + width])->flags[1] == 'd')
        {
            prewarppos = player->pos;
            player->pos = (Tiles + linear_ids[(player->pos) + width])->warp[1];
            load_scene((scenes+(Tiles + linear_ids[prewarppos + width])->warp[0]), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, (Tiles + linear_ids[prewarppos + width])->warp[1], player_tile, screen_size);
            return player->pos;
        }
        if ((player->pos) + width <= ((width*height)-1) && (Tiles + linear_ids[(player->pos) + width])->flags[0]!= 'c')
        {
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
            if((Tiles + linear_ids[((player->pos) + width)-width])->flags[2] == 'n')
            {
                msg[0] = (Tiles + linear_ids[((player->pos) + width)-width])->msg_id;
                //debug_printer(100);
            }
            else
            {
                msg[0] = 0;
            }
            return (player->pos) + width;
        }
    }
    if (input == 'a' || input == 'A' )
    {
        if((player->pos) - 1 >= 0 && (Tiles + linear_ids[(player->pos) - 1])->flags[1] == 'd')
        {
            prewarppos = player->pos;
            player->pos = (Tiles + linear_ids[(player->pos) - 1])->warp[1];
            load_scene((scenes+(Tiles + linear_ids[prewarppos - 1])->warp[0]), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, (Tiles + linear_ids[prewarppos - 1])->warp[1], player_tile, screen_size);
            return player->pos;
        }
        if ((player->pos) - 1 >= 0 && (player->pos)%width != 0 && (Tiles + linear_ids[(player->pos) - 1])->flags[0]!= 'c')
        {
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
            if((Tiles + linear_ids[((player->pos) - 1)-width])->flags[2] == 'n')
            {
                msg[0] = (Tiles + linear_ids[((player->pos) - 1)-width])->msg_id;
                //debug_printer(100);
            }
            else
            {
                msg[0] = 0;
            }
            return (player->pos) - 1;
        }
    }
    if (input == 'd' || input == 'D')
    {
        if((player->pos)%width != (width-1)  && (Tiles + linear_ids[(player->pos) + 1])->flags[1] == 'd')
        {
            prewarppos = player->pos;
            player->pos = (Tiles + linear_ids[(player->pos) + 1])->warp[1];
            load_scene((scenes+(Tiles + linear_ids[prewarppos + 1])->warp[0]), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, (Tiles + linear_ids[prewarppos + 1])->warp[1], player_tile, screen_size);
            return player->pos;
        }
        if ((player->pos) + 1 >= 0 && (player->pos)%width != (width-1)  && (Tiles + linear_ids[(player->pos) + 1])->flags[0]!= 'c')
        {
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
            if((Tiles + linear_ids[((player->pos) + 1)-width])->flags[2] == 'n')
            {
                msg[0] = (Tiles + linear_ids[((player->pos) +1)-width])->msg_id;
                //debug_printer(100);
            }
            else
            {
                msg[0] = 0;
            }
            return (player->pos) + 1;
        }
    }

    return (player->pos);

}
void print_menu(char text[]) {
    int textdone = 0, textcounter = 0;
    printf(" ");
    for (int i = 0; i < width * tile_width; i++) {
        if (i == 0 || i == ((width * tile_width) - 1))
        {
            printf(" ");
        }
        else {
            printf("_");
        }
    }
    printf("\n");

    for (int l = 0; l < (height * tile_height) / 3; l++) {
        printf(" ");
        for (int j = 0; j <= (width * tile_width) - 1; j++) {
            if (j == 0 || j == (width * tile_width) - 1) {
                printf("|");
            } else {
                if (text[textcounter] > 0 && text[textcounter] <= 127) {
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
void get_frequency(int tile_ids[width][height], int tile_frequency[(width*height)])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			tile_frequency[tile_ids[j][i]] = tile_frequency[tile_ids[j][i]] + 1;
		}
	}
}
void debug_printer(int number)
{
    printf("You've hit debug print number %d.\n", number);
    system("PAUSE");
}
void read_tiles(int amount, struct tile* Tiles)
{
    FILE *index = fopen("TileIndex.txt", "r");
    for(int i = 0; i < amount; i++)
    {
        char name[MAX_NAME_SIZE];
        char file[MAX_NAME_SIZE];
        char flag1 = '\0';
        char flag2 = '\0';
        char flag3 = '\0';
        int warp1 = 0;
        int warp2 = 0;
        int id = 0;
        int msg = 0;

        fscanf(index, "%d\n%s\n%s\n%c\n%c\n%c\n%d\n%d\n%d\n", &id, &name, &file, &flag1, &flag2, &flag3, &warp1, &warp2, &msg);
        (Tiles+i)->id = id;
        Tiles[i].name = malloc(sizeof(char) * strlen(name));
        Tiles[i].file = malloc(sizeof(char) * strlen(file));
        strcpy((Tiles+i)->name, name);
        strcpy((Tiles+i)->file, file);
        (Tiles+i)->flags[0] = flag1;
        (Tiles+i)->flags[1] = flag2;
        (Tiles+i)->flags[2] = flag3;
        (Tiles+i)->warp[0] = warp1;
        (Tiles+i)->warp[1] = warp2;
        (Tiles+i)->msg_id = msg;

        memset(name, 0, sizeof(char)*strlen(name));
        memset(file, 0, sizeof(char)*strlen(file));


    }
    fclose(index);
    debug_printer(1);
}
void read_scenes(int amount, struct asset* scenes)
{
    FILE *index = fopen("SceneIndex.txt", "r");
    for(int i = 0; i < amount; i++)
    {
        char file[MAX_NAME_SIZE];
        int id = 0;

        fscanf(index, "%d\n%s\n", &id, &file);
        (scenes+i)->id = id;
        scenes[i].file = malloc(sizeof(char) * strlen(file));
        strcpy((scenes+i)->file, file);

        memset(file, 0, sizeof(char)*strlen(file));

    }
    fclose(index);
    debug_printer(1);
}
void ui_manager(int *scrstr, int *bgmap, int tile_ids[width][height], int pos, struct tile element, int operation, int tile_map[(height*width)][(tile_height*tile_width)])
{
    if (operation == 0)
    {
        char linear_element[(tile_width*tile_height)];
        FILE* pt = fopen(element.file, "r");
        int tick = 0;
        for(int i = 0; i<tile_height; i++)
        {
            for(int j = 0; j <tile_width;j++)
            {
                fscanf(pt, "%c", &linear_element[tick]);
                tick = tick + 1;
            }
            fscanf(pt, "%*c");
        }
        fclose(pt);

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
    else if(operation == 1)
    {
        for(int i = 0; i < (tile_width*tile_height); i++)
        {
            scrstr[tile_map[pos][i]] = bgmap[tile_map[pos][i]];
        }
    }
}
void read_message(int id)
{
    char message[MAX_MSG_SIZE] = " ";
    char message2[MAX_MSG_SIZE] = "This is a test message from me, a test NPC!";
    char msg_index[5][MAX_MSG_SIZE];

    strcpy(msg_index[0], message);
    strcpy(msg_index[1], message2);

    print_menu(msg_index[id]);
}