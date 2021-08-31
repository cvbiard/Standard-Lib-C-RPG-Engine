#include "header.h"


/*STANDARD C LIBRARY RPG ENGINE by Clark Biard
 * This software is currently in development, but free to use for any purpose! Documentation coming soon.
 * Contact: cvbiard@hotmail.com
 * Version: 0.0.3
 * Notes: Full color implemented with mode p. At this point, I think I need to just start working on building a game with these tools.
*/


int main(void)
{

    //Initializing the game
    char mode = 'p';
    //First, we need to calculate the size of the screen in terms of individual characters for future use. The argument for calc_screen_size is a boolean for whether or not we want a border of pound signs around the screen.
    int scr_size = calc_screen_size(1);

    //msg is the id of the text phrase to be printed in the text box. It needs to be a pointer but I haven't done that yet oops.
    int msg = 0;

    //Create tile_amount, scene_amount, and msg_amount (the amount of tiles, scenes, and messages we should expect to read from files), then read out of the respective files.
    int tile_amount = 0;
    int scene_amount = 0;
    int msg_amount = 0;
    FILE *ta = fopen("TileAmount.txt", "r");
    fscanf(ta, "%d", &tile_amount);
    fclose(ta);
    printf("%d\n", tile_amount);
    FILE *sa = fopen("SceneAmount.txt", "r");
    fscanf(sa, "%d", &scene_amount);
    fclose(sa);
    printf("%d\n", scene_amount);
    FILE *ma = fopen("MessageAmount.txt", "r");
    fscanf(ma, "%d", &msg_amount);
    fclose(ma);
    printf("%d\n", msg_amount);

    //Create the input variable to store player input in.
	int input = 0;

	//Allocate heap memory for the screen string (the string that is actually printed to the screen) and the background map (the string that holds the background characters so that the screen string can be restored after
	//some destructive action such as the player moving on to a tile)
	int scrstr = (int) malloc(scr_size * sizeof(int));
	int bgmap = (int) malloc(scr_size * sizeof(int));

	//Allocate heap memory for linear ids, a one dimentional array for storing tile ids of the scene we are currently loading.
	int linear_ids = (int) malloc((height*width) * sizeof(int));

	//Create the tile map. This 2D array stores the 1D position of each character based on the screen string. For example, if we wanted to figure out where in scrstr the character being printed at 5, 4 on the screen is,
	//we could access tile_map[5][4]. This is very useful for going from 2D to 1D, where 2D is easier to understand mentally, but 1D is much more efficient to work with and print.
    int tile_map[(height*width)][(tile_height*tile_width)] = {0};
    mapping(tile_map, scr_size);

    debug_printer(1);

    //TextText is a test message
	char TestText[] = "This is test text, and I'm making it long to see what happens :)";

	//Create tile_ids and tile_frequency. tile_ids is simply a 2D map of the screen using the tile ids, which is useful for accessing tile properties and flags without having to deal with all the extra visual information
	//tile_frequency is used to figure out how many unique tiles there are in a scene. This is used so that we don't have to allocate memory for a massive array, but instead can size some arrays to be only as large as we need
	int tile_ids[width][height], tile_frequency[(width*height)] = { 0 };

	//exit is for the main while loop, probably don't need it anymore
    int exit = 0;

    //create the player object and assign a default position
    struct object player;
    player.pos = 3;

    //create the player tile. I figure it's easier to do here instead of along with the other tiles since we will always need it.
    char player_tile[(tile_width*tile_height)];

    //Create and allocate memory for the array of tiles. This is where tile_amount is useful.
	struct tile* Tiles = (struct tile*) malloc(tile_amount*sizeof(struct tile));

	//Read in the tile information from TileIndex.txt, which specifically is the id, name, file, flags, warp, and message id for each tile. Reading from a file is how I was able to solve the issue of wanting to be able
	//to create new tiles without having to rebuild the game every time.
    read_tiles(tile_amount, Tiles);

    //Debug prints all the tile data we just read in.
    for(int i = 0; i < tile_amount; i++)
    {
        printf("%d\n%s\n%s\n%c\n%c\n%c\n%d\n%d\n%d\n%c\n",(Tiles+i)->id, (Tiles+i)->name, (Tiles+i)->file, (Tiles+i)->flags[0], (Tiles+i)->flags[1], (Tiles+i)->flags[2], (Tiles+i)->warp[0], (Tiles+i)->warp[1], (Tiles+i)->ui_id, (Tiles+i)->ui_dir);
    }


    //Now that we can access the player tile's file, read it in to the player tile array. Like all tiles, we read from a 2D format into a 1D format as again, 1D is easier to print and work with despite making less sense visually.
    FILE* pt = fopen((Tiles+1)->file, "r");
    int tick = 0;
    for(int i = 0; i<tile_height; i++)
    {
        for(int j = 0; j <tile_width;j++)
        {
            fscanf(pt, "%c", &player_tile[tick]);
            tick = tick + 1;
        }
        fscanf(pt, "%*c");
    }
    fclose(pt);


    //Now, we give scenes the same treatment as tiles. Create an array of the correct size, and read in the information (specifically only the id and file name) from SceneIndex.txt.
    struct asset* scenes = (struct asset*) malloc(scene_amount*sizeof(struct asset));

    //Again reading from a file is nice because it allows for scene creation without having to rebuild.
    read_scenes(scene_amount, scenes);

    //Once more, like Tiles and scenes, for messages
    struct message* Messages = (struct message*) malloc(msg_amount*sizeof(struct message));

    //Read from file
    read_messages(msg_amount, Messages);

    //Little debug print to let me know I got out of the initialization
    if(debug == 't')
    {
        printf("Initialization complete\n");
        system("PAUSE");
    }

    //Now we start specific work. Loading the first scene, and getting the frequency of the tiles.
    load_scene((scenes+0), tile_ids, tile_frequency);
    get_frequency(tile_ids, tile_frequency);

    //Little debug print to let me know we got through loading the scene and getting tile frequency.
    if(debug == 't')
    {
        printf("Loaded the scene and got tile frequency.\n");
        system("PAUSE");
    }

    //Running the screen manager. This function writes the correct tile characters to the correct place on scrstr and bgmap. Splitting this functionality from print_screen was very important, as doing this work every time
    //we needed to print the screen was super slow and buggy. This function essentially handles all work that needs to be done once per scene.
    screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, player.pos, player_tile, scr_size, mode);


    //Prints the screen and menu below it.
    print_screen(scrstr, scr_size, mode);
    display_message(0, Messages, mode);



    //The main gameplay loop.
	while (exit != 1)
	{
        //Get input from the player. Using system command because it doesn't require pressing enter after an input like scanf does. The return value is an integer starting from 1 of what key was
        //pressed in the order they were listed. For example, if the user entered w, we would get 1. If they entered p, we would get 5.
		input = system("CHOICE /N /C:wasdprtui");

		//Exit input
		if (input == 5)
        {
            free(scrstr);
            free(bgmap);
            free(Tiles);
            free(scenes);
            return 0;
        }

		//Reload input. This reload entirely restarts the program, meaning memory is freed and reallocated so new tiles and scenes we created will be included.
        if (input == 6 && debug == 't')
        {
            atexit(system("RPG.exe"));
            break;
        }

        //Visual reload. All this does is reload the scene which is useful for changing tiles in a scene to ones that already exist, or changing how a tile looks.
        if (input == 7 && debug == 't')
        {
            load_scene((scenes+0), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, player.pos, player_tile, scr_size, mode);
            ui_manager(scrstr, bgmap, 199, *(Tiles+1), 1, tile_map);
        }


        //Processing player input into movement.
        player.pos = move(scrstr, bgmap, tile_map, input, player_tile, linear_ids, Tiles, scenes, tile_ids, tile_frequency, &player, scr_size, &msg, mode);

        //Reset input
		input = 0;

		//Clear the previous step display
		system("cls");

		//Reprint the screen and menu with updated information
        print_screen(scrstr, scr_size, mode);
        display_message(msg, Messages, mode);

        //Debug print to show player position
        if(debug == 't')
        {
            printf("Player position is %d.\n", player.pos);
        }
	}

	//Free heap memory and exit :)
	free(scrstr);
	free(bgmap);
    free(Tiles);
    free(scenes);
    return 0;
}