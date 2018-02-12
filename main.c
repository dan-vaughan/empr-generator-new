#include "main.h"

#define TIME 10000000

Serial pc;
DMX dmx;
packet_manager packets;

char red[5] = {0x00, 0xff, 0x00, 0x00, 0x00};
char green[5] = {0x00, 0x00, 0xff, 0x00, 0x00};
char blue[5] = {0x00, 0x00, 0x00, 0xff, 0x00};
char full[5] = {0x00, 0xff, 0xff, 0xff, 0x00};
char empty[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

char labels[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

void sequence(int time)
{

	dmx.send(red, 5);
	delay(time);
	dmx.send(green, 5);
	delay(time);
	dmx.send(blue, 5);
	delay(time);
	dmx.send(empty, 5);
}

void menu(int screen)
{
	clear_display();
	return_home();

	if (screen == 0) {
		printstr("1-Test Sequence");
		shift_line();
		printstr("2-Editor   B");
		putcustom(0x20);
	}
	if (screen == 1) {
		printstr("3-Send Packet");
		shift_line();
		printstr("4-Sequence C");

	}
	if (screen == 2) {
		printstr("--- Screen C ---");
		shift_line();
		printstr("Not implemented");
	}
	if (screen == 3) {
		printstr("--- Screen D ---");
		shift_line();
		printstr("Not implemented");
	}

}

void editor()
{
	clear_display();
	return_home();
	printstr("Choose a Packet");
	shift_line();
	printstr("(0-99): ");

}

void action(int button)
{
	static int mode = 0;	//Menu mode, defualt

	if (mode == 0) {
		if (button == 0) sequence(TIME);				//Option 1 - Test Sequence
		else if (button == 1) {mode = 1; editor();}	//Option 2 - Editor
		else if (button == 2);												//Option 3 - Send Packet
		else if (button == 4);												//Option 4 - sequence

		else if (button == 3) menu(0);					//Button A - Screen 0
		else if (button == 7) menu(1);					//Button B - Screen 1
		else if (button == 11) menu(2);				//Button C - Screen 2
		else if (button == 15) menu(3);				//Button D - Screen 3
	}

	else if (mode == 1) {
		static int limit = 0;
		static char input[3] = {0x00, 0x00, 0x00};

		if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13) && limit < 2 )
		{
			printchar(labels[button]);
			input[limit] = labels[button];
			limit++;

			if (limit == 2) {
				int choice = atoi(input);
				pc.printf("Input is: %d\n\r", choice);

			}

		}
	}


}

int main ()
{
	dmx.send(empty, 5);

	setup_display();
	menu(0);


	while (1)
	{
		keypad_check(action);
	}
}
