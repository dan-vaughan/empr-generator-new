#include "main.h"

Serial pc;
DMX dmx;
packet_manager packets;
sequence_manager sequences;
AnalogIn ain;
AnalogOut aout;
GAME my_game;

int manager = 1;
int seq = 1;

void (*repeat)();

char red[5] = {0x00, 0xff, 0x00, 0x00, 0x00};
char green[5] = {0x00, 0x00, 0xff, 0x00, 0x00};
char blue[5] = {0x00, 0x00, 0x00, 0xff, 0x00};
char full[4] = {0x00, 0xff, 0xff, 0xff};
char empty[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
char yellow[5] = {0x00, 0xff, 0xff, 0x00, 0x00};
char magenta[5] = {0x00, 0xff, 0x00, 0xff, 0x00};
char cyan[5] = {0x00, 0x00, 0xff, 0xff, 0x00};
char yellowgreen[5] = {0x00, 0x80, 0xff, 0x00, 0x00};
char orange[5] = {0x00, 0xff, 0x80, 0x00, 0x00};
char rose[5] = {0x00, 0xff, 0x00, 0x80, 0x00};
char violet[5] = {0x00, 0x80, 0x00, 0xff, 0x00};
char azure[5] = {0x00, 0x00, 0x80, 0xff, 0x00};
char seagreen[5] = {0x00, 0x00, 0xff, 0x80, 0x00};

char * colours[6] = {red, green, blue, yellow, magenta, cyan};

char labels[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

void do_nothing() { }

void test()
{

	dmx.send(red, 5);
	delay(TIME);
	dmx.send(green, 5);
	delay(TIME);
	dmx.send(blue, 5);
	delay(TIME);
	dmx.send(empty, 5);
}

void menu(int screen)
{
	clear_display();
	return_home();

	if (screen == 0) {
		printstr("1-Test Sequence");
		shift_line();
		printstr("2-Edit Packet  ");
		putcustom(0x30);
	}
	else if (screen == 1) {
		printstr("3-Edit Sequence");
		shift_line();
		printstr("4-Send Sequence");
		putcustom(0x22);
	}
	else if (screen == 2) {
		printstr("5-Music");
		shift_line();
		printstr("6-Game         ");
		putcustom(0x22);
	}
	else if (screen == 3) {
		printstr("7-N/A          ");
		putcustom(0x12);
		shift_line();
		printstr("8-N/A");
	}
}

void sequence_sender(int sequence_number){
	char val[3];
	int i;
	pc.write("Sending sequence...\n\r");
	char num;
	char * base = sequences.getptr(sequence_number);
	char * ptr;
	for (i = 0; i < 10; i++){
		ptr = base + i;
		pc.printf("base = %d, ptr = %d\n\r", base, ptr);
		num = *ptr;
		pc.printf("Loading packet %d...\n\r", num);

		dmx.send(packets.getptr(num), PLEN);
		delay(TIME);
	}
	dmx.send(empty, 5);
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

	char * packet_ptr = packets.getptr(packet_number);

	sprintf(val, "%d", *(packet_ptr+slot));
	printstr(val);

	int len = strlen(val);
	cursor_shift(LEFT, len);	//Shift back so numbers can be entered
}

void sequencer(int sequence_number, int sequence_position){
	char msg[18];
	char val[2];

	sprintf(msg, "Packet %d: ", sequence_position);

	clear_display();
	return_home();
	printstr(msg);

	printstr(" D");
	putcustom(0x12);

	shift_line();

	if (sequence_position > 0) {
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

	char * sequence_ptr = sequences.getptr(sequence_number);

	sprintf(val, "%d", *(sequence_ptr+sequence_position));
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

void action(int button)
{
	static int mode = 0;	//Menu mode, defualt
	static int choice = 0;	//Used to track packet being edited
	static int current_sequence = 0;

	if (mode == 0) {
		if (button == 0) test();	//Option 1 - Test Sequence (G1)

		else if (button == 1) { 			//O\nption 2 - Packet Manager (G2)
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

		else if (button == 4) { selector(1); mode = 5; }				//Option 4 - Send Sequence (G3)
		else if (button == 5) { mode = 6; repeat = music_repeat; music_start(); }			//Option 5 - Music (IC5)
		else if (button == 6) { mode = 7; difficulty_display(); }					//Option 6 - Game (IC4)
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
			menu(0);
			mode = 0;
		}
	}
	else if (mode == 2) { //Editor mode
		static int index = 0;	//Equivelent to "limit" above
		static char edit_input[4] = {0x00, 0x00, 0x00, 0x00};
		static int slot = 1;
		static int edited = 0;
		if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13) && index < 3 )
		{
			printchar(labels[button]);
			edit_input[index] = labels[button];
			index++;
			edited = 1;
			if (index == 3) {
				int value = atoi(edit_input);
				if (value > 255) { slot = 1; mode = 0; error(2); }
				else {	//Value is safe
					char * ptr = packets.getptr(choice)+slot;
					*ptr = value;
					editor(choice, slot);
				}
				index = 0;
				edited = 0;
			}
		}
		else if (button == 3 && slot > 1 && edited == 0) editor(choice, --slot);					//If A pressed, move left 1
		else if (button == 7 && slot < PLEN -1 && edited == 0)	editor(choice, ++slot);		//If B pressed, move right 1
		else if (button == 11 && edited == 0) dmx.send( packets.getptr(choice) , PLEN ); //If C pressed, send packet being edited
		else if (button == 15) {
			slot = 0;
			mode = 0;
			edit_input[0] = 0x00; edit_input[1] = 0x00; edit_input[2] = 0x00; edit_input[3] = 0x00;
			index = 0;
			edited = 0;
			menu(0); }						//If D pressed, exit to main menu
	}
	else if (mode == 3) {//Select a sequence to edit
		if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13)){
			char buf[2] = {0x00, 0x00};
			buf[0] = labels[button];
			int current_sequence = atoi(buf);
			//pc.printf("Current sequence is %d", current_sequence);
			sequencer(current_sequence, 0);
			mode = 4;
		}
		else {
			mode = 0;
			menu(0);
		}
	}	//Sequence picker

	else if (mode == 4) {	//Sequence editor
		static int index = 0;	//Equivelent to "limit" above
		static char edit_input[3] = {0x00, 0x00, 0x00};
		static int sequence_position = 0;
		static int edited = 0;

		if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13) && index < 2)
		{
			printchar(labels[button]);
			edit_input[index] = labels[button];
			index++;
			edited = 1;
			if (index < 2) putcustom(0x5A);	//Add a '_' character
			if (index == 2) {
				int value = atoi(edit_input);
				pc.printf("Taken %d as input\n\r", value);
				if (value > PMAX-1) { sequence_position = 0; mode = 0; error(2); }
				else {	//Value is safe
					char * ptr = sequences.getptr(current_sequence)+sequence_position;
					*ptr = value;
					pc.printf("Stored in memory at %d we have %x\n\r", ptr, *ptr);
					sequencer(current_sequence, sequence_position);
				}
				index = 0;
				edited = 0;
			}
		}
		else if (button == 3 && sequence_position > 0 && edited == 0) sequencer(current_sequence, --sequence_position);			//If A pressed, move left 1
		else if (button == 7 && sequence_position < SLEN -1 && edited == 0)	sequencer(current_sequence, ++sequence_position);		//If B pressed, move right 1
		else if (button == 15) {//If D pressed, exit to main menu
			sequence_position = 0;
			mode = 0;
			edit_input[0] = 0x00; edit_input[1] = 0x00; edit_input[2] = 0x00; edit_input[3] = 0x00;
			index = 0;
			edited = 0;
			menu(0);
			}
	}
		else if (mode == 5) {//Select a sequence to send
			if ( ( (0 <= button && button < 3) || (4 <= button && button < 7) || (8 <= button && button < 11) || button == 13)){
				char buf[2] = {0x00, 0x00};
				buf[0] = labels[button];
				int seq = atoi(buf);

				sequence_sender(seq);
				mode = 0;
				menu(0);
			}
			else {
				mode = 0;
				menu(0);
			}
		}	//Sequence picker
		else if (mode == 6) {	//Music mode - hook into Asher's individual project
			if (button == 15) { mode = 0; repeat = do_nothing; menu(0); }	//If D pressed, exit to main Menu
			else music_button(button);	//Call the button hook in Asher's individual project
		}
	else if (mode == 7){
		if (0 <= button && button < 3){
			game_start(button);
		}
		else if (button == 4 || button == 5){
			game_start(button - 1);
		}
		mode = 0;
		menu(0);
	}
}

int main ()
{
	try {
		packets.init();
	}
	catch (const char * msg) {
		manager = 0;
	}

	repeat = do_nothing;	//This is a hook for a repeating action. By default it does nothing.

	try {
		sequences.init();
	}
	catch (const char * msg){
		seq = 0;
	}

	pc.write("Starting...\n\r");

	dmx.send(empty, 5);

	setup_display();
	menu(0);


	while (1)
	{
		keypad_check(action);
		(*repeat)();
	}
}
