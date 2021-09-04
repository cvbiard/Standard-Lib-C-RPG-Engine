#include "header.h"

int main()
{
	int input = 0;

	while (input !=4)
	{
		printf("What would you like to do?\n\n(1) Write scene\n\n(2) View Tile\n\n(3) Edit Tile\n\n(4) Exit\n");

		input = system("CHOICE /N /C:1234");

		system("cls");

		switch (input)
		{
		case 1: write_scene();
			break;
		case 2: view_tile();
			break;
		case 3: edit_tile();
			break;
		}
	}
	return 0;
}