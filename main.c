#include "main.h"

#define TIME 10000000

Serial pc;
DMX dmx;
packet_manager packets;
sequence_manager sequences;
AnalogIn ain;

int manager = 1;
int seq = 0;

char red[5] = {0x00, 0xff, 0x00, 0x00, 0x00};
char green[5] = {0x00, 0x00, 0xff, 0x00, 0x00};
char blue[5] = {0x00, 0x00, 0x00, 0xff, 0x00};
char full[4] = {0x00, 0xff, 0xff, 0xff};
char empty[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

char labels[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

void test(int time)
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
		printstr("2-Edit Packet   ");
		putcustom(0x30);
	}
	else if (screen == 1) {
		printstr("3-Edit Sequence");
		shift_line();
		printstr("4-Music        ");
		putcustom(0x22);
	}
	else if (screen == 2) {
		printstr("5-Game");
		shift_line();
		printstr("6-Send Sequence ");
		putcustom(0x22);
	}
	else if (screen == 3) {
		printstr("7-N/A");
		shift_line();
		printstr("8-N/A          ");
		putcustom(0x12);
	}

}

void sequence_sender(int sequence_number){
	int* seq_ptr = sequences.getptr(sequence_number);
	for (int i = 0; i < 10 ; i++){
		int packetnum = * seq_ptr;
		dmx.send(packets.getptr(packetnum));
		seq_ptr += sizeof(int);
	}
}

void selector(int screen)
{
	if (screen == 0) {
		clear_display();
		return_home();
		printstr("Choose a Packet");
		shift_line();
		printstr("(0-49): ");
	}
	if (screen == 1) {
		clear_display();
		return_home();
		printstr("Choose a Sequence");
		shift_line();
		printstr("(0-9): ");
	}
}

void editor(int packet_number, int slot)
{
	char msg[18];
	char val[3];

	sprintf(msg, "Slot %d:  C", slot);

	clear_display();
	return_home();
	printstr(msg);

	putcustom(0xaa);
	printstr(" D");
	putcustom(0x12);

	shift_line();

	if (slot > 1) {
		putcustom(0x10);
		printchar('A');
	}
	else printstr("  ");
	printstr("            ");

	if (slot < PLEN-1) {
		printchar('B');
		putcustom(0x20);
		cursor_shift(LEFT, 9);
	}
	else {
		cursor_shift(LEFT, 7);	//A lesser shift is needed as "B" not printed
	}

	char * num = packets.getptr(packet_number);

	sprintf(val, "%d", *(num+slot));
	printstr(val);

	int len = strlen(val);
	cursor_shift(LEFT, len);	//Shift back so numbers can be entered
}

void sequencer(int sequence_number, int sequence_position){
	char msg[18];
	char val[3];

	sprintf(msg, "Slot %d:  C", slot);

	clear_display();
	return_home();
	printstr(msg);

	putcustom(0xaa);
	printstr(" D");
	putcustom(0x12);

	shift_line();

	if (sequence > 1) {
		putcustom(0x10);
		printchar('A');
	}
	else printstr("  ");
	printstr("            ");

	if (sequence_position < SLEN-1) {
		printchar('B');
		putcustom(0x20);
		cursor_shift(LEFT, 9);
	}
	else {
		cursor_shift(LEFT, 7);	//A lesser shift is needed as "B" not printed
	}

	//int * packet = packets.getptr(packet_number);

	sprintf(val, "%d", *(num+sequence_position));
	printstr(val);

	int len = strlen(val);
	cursor_shift(LEFT, len);	//Shift back so numbers can be entered
}

void error(int screen)
{
	if (screen == 0) {
		clear_display();
		return_home();
		printstr("Not enough");
		shift_line();
		printstr("memory   (ABCD)");
		putcustom(0x20);
	}
	else if (screen == 1) {
		clear_display();
		return_home();
		printstr("Invalid packet!");
		shift_line();
		char msg[18];
		sprintf(msg, "Max=%d   (ABCD)", PMAX-1);
		printstr(msg);
		putcustom(0x20);
	}
	else if (screen == 2) {
		clear_display();
		return_home();
		printstr("Value too large!");
		shift_line();
		printstr("         (ABCD)");
		putcustom(0x20);
	}
}

void music() {
	//Asher's individual project
	int val = ain.read();

	int sum = val;
	int n = 1;
	int max = val;
	int min = val;
	int avg;
	int diff;
	int maxdiff;

	clear_display();
	return_home();
	printstr("Debug info");
	shift_line();
	printstr("on serial");

	while (1) {
		int val = ain.read();

		if (val < min) min = val;
		else if (val > max) max = val;
		n++;
		sum += val;
		avg = sum/n;
		diff = abs(val - avg);
		if (diff > maxdiff) maxdiff = diff;

		char pack[4] = {0, 0, (int)(diff/maxdiff)*255, 0};

		pc.printf("Dif: %d\n\r", diff);
		dmx.send(pack, 4);


	}
}

void action(int button)
{
	static int mode = 0;	//Menu mode, defualt
	static int choice = 0;	//Used to track packet being edited
	
	if (mode == 0) {
		if (button == 0) test(TIME);	//Option 1 - Test Sequence (G1)

		else if (button == 1) { 			//Option 2 - Packet Manager (G2)
			if (manager == 1) {	//If sufficient memory, enter selector
				mode = 1;
				selector(0);
			}
			else error(0);		//Else, give error message
		}

		else if (button == 2){				//Option 3 - Sequencer (G3)
			if (seq == 1) {	//If sufficient memory, enter selector
				mode = 3;
				selector(1);
			}
			else error(0);		//Else, give error message
		}

		else if (button == 4);				//Option 4 -
		else if (button == 5);				//Option 5 -
		else if (button == 6);				//Option 6 -
		else if (button == 8);				//Option 7 -
		else if (button == 9);				//Option 8 -
		else if (button == 10);				//Option 9 -

		else if (button == 3) menu(0);		//Button A - Screen 0
		else if (button == 7) menu(1);		//Button B - Screen 1
		else if (button == 11) menu(2);		//Button C - Screen 2
		else if (button == 15) menu(3);		//Button D - Screen 3
	}
	else if (mode == 1) {	//Packet selection mode
		static int limit = 0;
		static char input[3] = {0x00, 0x00, 0x00};

		if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13) && limit < 2 )
		{
			printchar(labels[button]);
			input[limit] = labels[button];
			limit++;

			if (limit == 2) {
				choice = atoi(input);
				limit = 0;
				if (choice < PMAX) { mode = 2; editor(choice, 1); }	//Check if packet number exists
				else { mode = 0; error(1); }	//Give error message
			}
		}
		else {
			action(button);
		}
	}
	else if (mode == 2) { //Editor mode
		static int index = 0;	//Equivelent to "limit" above
		static char edit_input[4] = {0x00, 0x00, 0x00, 0x00};
		static int slot = 1;

		if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13) && index < 3 )
		{
			printchar(labels[button]);
			edit_input[index] = labels[button];
			index++;

			if (index == 3) {
				int value = atoi(edit_input);
				if (value > 255) { slot = 1; mode = 0; error(2); }
				else {	//Value is safe
					char * ptr = packets.getptr(choice)+slot;
					*ptr = value;
					editor(choice, slot);
				}
				index = 0;
			}
		}
		else if (button == 3 && slot > 0) editor(choice, --slot);					//If A pressed, move left 1
		else if (button == 7 && slot < PLEN -1)	editor(choice, ++slot);		//If B pressed, move right 1
		else if (button == 11) dmx.send( packets.getptr(choice) , PLEN ); //If C pressed, send packet being edited
		else if (button == 15) { slot = 0; mode = 0; menu(0); }						//If D pressed, exit to main menu
	}
	else if (mode == 3) {//Select a sequence to edit
		if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13) && limit < 2 ){
			sequencer(button, 1);
		}
		else {
			action(button);
		}
	}	//Sequence picker

	else if (mode == 4) {	//Sequence definition mode
		static int sequence_position = 1;
		if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13) && limit < 2 )
		{
			printchar(labels[button]);
			edit_input[index] = labels[button];
			index++;

			if (index == 3) {
				int value = atoi(edit_input);
				if (value > PMAX) { sequence_position = 1; mode = 0; error(2); }
				else {	//Value is safe
					char * ptr = packets.getptr(choice)+slot;
					*ptr = value;
					editor(choice, slot);
				}
				index = 0;
			}
		}
		else if (button == 3 && sequence_position > 0) sequencer(button, - sequence_position);			//If A pressed, move left 1
		else if (button == 7 && sequence_position < SLEN -1)	sequencer(button, + sequence_position);		//If B pressed, move right 1
		else if (button == 15) { sequence_position = 0; mode = 0; menu(0);}		}						//If D pressed, exit to main menu
}

int main ()
{
	try {
		packets.init();
	}
	catch (const char * msg) {
		manager = 0;
	}

	try {
		sequences.init();
	}
	catch (const char * msh){
		seq = 0;
	}

	pc.write("Starting...\n\r");

	dmx.send(empty, 5);

	setup_display();
	menu(0);


	while (1)
	{
		keypad_check(action);
	}
}
