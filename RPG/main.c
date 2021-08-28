#include "header.h"


int main(void)
{

    //Declare stack variables
    int scr_size = calc_screen_size(1);
    int msg[1] = {0};
    int tile_amount = 0;
    int scene_amount = 0;
    FILE *ta = fopen("TileAmount.txt", "r");
    fscanf(ta, "%d", &tile_amount);
    fclose(ta);
    printf("%d\n", tile_amount);
    debug_printer(100);
    FILE *sa = fopen("SceneAmount.txt", "r");
    fscanf(sa, "%d", &scene_amount);
    fclose(sa);
    printf("%d\n", scene_amount);
    //debug_printer(1);
	char input = '\0';
	int scrstr = (int) malloc(scr_size * sizeof(int));
	int bgmap = (int) malloc(scr_size * sizeof(int));
	//debug_printer(2);
	int linear_ids = (int) malloc((height*width) * sizeof(int));
    int tile_map[(height*width)][(tile_height*tile_width)] = {0};
    //debug_printer(3);
    mapping(tile_map, scr_size);

	char TestText[] = "This is test text, and I'm making it long to see what happens :)";
	int linear_pos[(width*height)][2] = {0};
	map_pos(linear_pos);
	int tile_ids[width][height], tile_frequency[(width*height)] = { 0 };
    int exit = 0;
    struct object player;
    player.pos = 50;

    char player_tile[(tile_width*tile_height)];
    //debug_printer(2);

    //Add to global tiles array
	struct tile* Tiles = (struct tile*) malloc(tile_amount*sizeof(struct tile));

    read_tiles(tile_amount, Tiles);
    for(int i = 0; i < tile_amount; i++)
    {
        printf("%d\n%s\n%s\n%c\n%c\n%c\n%d\n%d\n",(Tiles+i)->id, (Tiles+i)->name, (Tiles+i)->file, (Tiles+i)->flags[0], (Tiles+i)->flags[1], (Tiles+i)->flags[2], (Tiles+i)->warp[0], (Tiles+i)->warp[1]);
    }

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
    //Declare global scenes
    struct asset FirstScreen;
    FirstScreen.file = "FirstScreen.txt";

    struct asset Inside;
    Inside.file = "Inside.txt";

    struct asset* scenes = (struct asset*) malloc(scene_amount*sizeof(struct asset));
//    *(scenes) = FirstScreen;
//    *(scenes+1) = Inside;

    read_scenes(scene_amount, scenes);
    if(debug == 't')
    {
        printf("Made it here\n");
        system("PAUSE");
    }

    //Load scene and get frequency
    load_scene((scenes+0), tile_ids, tile_frequency);
    get_frequency(tile_ids, tile_frequency);

    if(debug == 't')
    {
        printf("Made it out of load tiles\n");
        system("PAUSE");
    }


    screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, player.pos, player_tile, scr_size);

    //debug_printer(5);


    print_screen(scrstr, scr_size);
    print_menu(TestText);

    //system("PAUSE");

    int added = 0;

	while (exit != 1)
	{

		input = _getch();

		if (input == 'p')
        {
            free(scrstr);
            free(bgmap);
            free(Tiles);
            free(scenes);
            return 0;
        }
        if (input == 'r' && debug == 't')
        {
            atexit(system("RPG.exe"));
            break;
//            FILE* ta2 = fopen("TileAmount.txt", "r");
//            fscanf(ta2, "%d", &tile_amount);
//            fclose(ta2);
//            printf("%d\n", tile_amount);
//            debug_printer(200);
//            read_tiles(tile_amount, Tiles);
//            load_scene((scenes+0), tile_ids, tile_frequency);
//            get_frequency(tile_ids, tile_frequency);
//            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, player.pos, player_tile, scr_size);

        }
        if (input == 't' && debug == 't')
        {
            load_scene((scenes+0), tile_ids, tile_frequency);
            get_frequency(tile_ids, tile_frequency);
            screen_manager(scrstr, bgmap, tile_map, Tiles, tile_ids, tile_frequency, linear_ids, player.pos, player_tile, scr_size);
        }
        if(input == 'u')
        {
            ui_manager(scrstr, bgmap, tile_ids, 88, *(Tiles+28), 0, tile_map);
        }
        if(input == 'i')
        {
            ui_manager(scrstr, bgmap, tile_ids, 88, *(Tiles+28), 1, tile_map);
        }
        ui_manager(scrstr, bgmap, tile_ids, 205, *(Tiles+30), 0, tile_map);
        player.pos = move(scrstr, bgmap, tile_map, input, player_tile, linear_ids, linear_pos, Tiles, scenes, tile_ids, tile_frequency, &player, scr_size, msg);
		input = '\0';
		system("cls");
        print_screen(scrstr, scr_size);
        read_message(msg[0]);
        if(debug == 't')
        {
            printf("Player position is %d.\n", player.pos);
        }
	}

	free(scrstr);
	free(bgmap);
    free(Tiles);
    free(scenes);
    return 0;
}