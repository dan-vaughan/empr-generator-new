//ASHER's IC
#include "main.h"

int music_mode = 0;

//Used for volume mode:
char volred = 0x16;
char volgreen = 0x00;
char volblue = 0x00;

char rhtred = 0;
char rhtgreen = 0;
char rhtblue = 0;

int transition = 0;	// 0 = Red -> Green, 1 = Green -> Blue, 2 = Blue -> Red

Queue history((int) ain.read());	//Used to 'smooth' out volumes to get an average

void music_menu(int screen)
{
	clear_display();
	return_home();

	if (screen == 0) {
		printstr("1-Volume mode");
		shift_line();
		printstr("2-Rhythm mode");
	}
  else if (screen == 1) {
    putcustom(0xAA);
		printstr(" Volume mode ");
    putcustom(0xAA);
		shift_line();
    putcustom(0x10);
		printstr("C   ");
    putcustom(0x30);
    printstr("D");
	}
  else if (screen == 2) {
    putcustom(0x8B);
		printstr(" Rhythm mode ");
    putcustom(0x8B);
		shift_line();
    putcustom(0x10);
		printstr("C   ");
    putcustom(0x30);
    printstr("D");
	}
}

void music_button(int button) { //Called when button is pressed in music mode
  static int mode = 0;  //Default mode is selection mode

  if (mode == 0) {  //Menu mode
    if (button == 0)      {mode = 1; music_mode = 1; music_menu(1); volume();} //Option 1 - Volume mode
    else if (button == 1) {mode = 1; music_mode = 2; music_menu(2); rhythm();} //Option 2 - Rhythm mode
  }
  else if (mode == 1) { //Other mode
    if (button == 11) {mode = 0; music_mode = 0; music_menu(0);}	//Return to music menu
  }
}

void music_repeat() {
	if (music_mode == 1)		  volume();
	else if (music_mode == 2) rhythm();
}

void music_start() {  //Called when music mode starts
  music_menu(0);
	srand(ain.read());
	int rnd = rand() % 3;
	switch(rnd) {
		case 0:
			rhtred = 255;
			break;
		case 1:
			rhtgreen = 255;
			break;
		case 2:
			rhtblue = 255;
			break;
	}
}

void volume() {
	char smooth[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

	switch (transition) {
		case 0:	//Red -> Green
			volred--;
			volgreen++;
			if (volred == 0) transition = 1;
			break;
		case 1: //Green -> Blue
			volgreen--;
			volblue++;
			if (volgreen == 0) transition = 2;
			break;
		case 2:	//Blue -> Red
			volblue--;
			volred++;
			if (volblue == 0) transition = 0;
			break;
	}
	int vol = getvol();

	smooth[1] = (char)volred * vol;
	smooth[2] = (char)volgreen * vol;
	smooth[3] = (char)volblue * vol;

	pc.printf("RGB: %d %d %d vol: %d\n\r", smooth[1], smooth[2], smooth[3], vol);

	dmx.send(smooth, 5);
}

void change() {	//Change to a random colour which is not adjacent on the colour wheel
	char rhtpack[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	int rnd = rand() % 3;

	if (rhtred == 255) {	//Red
		rhtred = 0;
		switch(rnd) {
			case 0:
				rhtgreen = 255;
				break;
			case 1:
				rhtblue = 255;
				break;
			case 2:
				rhtgreen = 128;
				rhtblue = 128;
				break;
		}
	}
	else if (rhtgreen == 255) {	//Green
		rhtgreen = 0;
		switch(rnd) {
			case 0:
				rhtred = 255;
				break;
			case 1:
				rhtblue = 255;
				break;
			case 2:
				rhtred = 128;
				rhtblue = 128;
				break;
		}
	}
	else if (rhtblue == 255) {	//Blue
		rhtblue = 0;
		switch(rnd) {
			case 0:
				rhtred = 255;
				break;
			case 1:
				rhtgreen = 255;
				break;
			case 2:
				rhtred = 128;
				rhtgreen = 128;
				break;
		}
	}
	else if (rhtred == 128 && rhtgreen == 128) {	//Yellow
		switch(rnd) {
			case 0:
				rhtred = 0;
				rhtgreen = 0;
				rhtblue = 255;
				break;
			case 1:
				rhtred = 0;
				rhtblue = 128;
				break;
			case 2:
				rhtgreen = 0;
				rhtblue = 128;
				break;
		}
	}
	else if (rhtred == 128 && rhtblue == 128) {	//Magenta
		switch(rnd) {
			case 0:
				rhtred = 0;
				rhtgreen = 255;
				rhtblue = 0;
				break;
			case 1:
				rhtred = 0;
				rhtgreen = 128;
				break;
			case 2:
				rhtgreen = 128;
				rhtblue = 0;
				break;
		}
	}
	else if (rhtgreen == 128 && rhtblue == 128) {	//Cyan
		switch(rnd) {
			case 0:
				rhtred = 255;
				rhtgreen = 0;
				rhtblue = 0;
				break;
			case 1:
				rhtred = 128;
				rhtgreen = 0;
				break;
			case 2:
				rhtred = 128;
				rhtblue = 0;
				break;
		}
	}

	rhtpack[1] = rhtred;
	rhtpack[2] = rhtgreen;
	rhtpack[3] = rhtblue;
	dmx.send(rhtpack, 5);
}

void rhythm() {
	int vol = getvol();
	static int prev = vol;
	static int recent = 0;	//Tracks if light has recently changed

	if (vol-prev > 1 && recent == 0) { change(); recent = 300; }
	else if (recent > 0) recent--;

	prev = vol;
}

int maximum(int a, int b) {
	if (a > b) return a;
	else return b;

}



int getvol() {
	static int setup = SIZE;	//Do not send packets during first round of the queue

	history.add((int) ain.read());
	int mymin = history.min();
	int mymax = history.max();
	int diff = ((mymax - mymin) >> 8) - 2;
	if (diff < 0) diff = 0;
	if (diff > 16) diff = 16;

	if (setup == 0) return diff;
	else { setup--; return 0; }
}
