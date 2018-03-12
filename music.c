//ASHER's IC
#include "main.h"

int music_mode = 0;

//Used for volume mode:
char volred = 0x16;
char volgreen = 0x00;
char volblue = 0x00;

int transition = 0;	// 0 = Red -> Green, 1 = Green -> Blue, 2 = Blue -> Red

Queue history(0);	//Used to 'smooth' out volumes to get an average

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
	float vol = getvol();

	smooth[1] = (char)volred * vol;
	smooth[2] = (char)volgreen * vol;
	smooth[3] = (char)volblue * vol;

	dmx.send(smooth, 5);
}

void rhythm() {
	dmx.send(yellow, 5);
}

int maximum(int a, int b) {
	if (a > b) return a;
	else return b;

}

float getvol() {
	static int max = 0;

	int val = ain.read();
	history.add(val);
	int avg = history.avg();
	int cor = avg - 2300;

	if (cor < 0) cor = 0;
	if (cor > 1024) cor = 1024;

	cor /= 64;

	pc.printf("avg %d cor %d\n\r", avg, cor);

	return cor;
}

/*void music() {
	//Asher's individual project
	int val = ain.read();

	int sum = val;
	int n = 1;
	int max = val;
	int min = val;
	int avg;
	int diff;
	int maxdiff;
	int intensity;
	int strength;
	int previous;		//Track previous strength
	char * old; 				//Tracks the current colour to ensure a new colour is chosen
	int recent = 0;

	char * pack = colours[rand() % 6];
	char mypack[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

	clear_display();
	return_home();
	printstr("Debug info");
	shift_line();
	printstr("on serial");

	Queue history(ain.read());	//recent measurements to detect sudden changes

	int seed = ain.read();
	srand(seed);

	while (1) {
		int val = ain.read();
		if (val < min) min = val;
		else if (val > max) max = val;
		n++;
		sum += val;
		avg = sum/n;
		diff = abs(val - avg);
		if (diff > maxdiff) maxdiff = diff;

		history.add(diff);

		intensity = pow(history.avg()/64, 2)-3;	//50 is a good threshold for speech. 0 is better if lab is quiet and inconspicuous noises are needed.

		if (intensity > 255) intensity = 255;
		if (intensity < 0) intensity = 0;

		pc.printf("%d\n\r", intensity);


		if (intensity - previous > 20 && recent == 0) {			//intensity >= 150 && previous < 150) {	//Values for a quiet lab. Will need adjustment for music.
			old = pack;
			while (old == pack) {
				pack = colours[rand() % 6];
			}
			recent = 5;
		}
		else if (recent > 0) recent--;
		for (int i = 1; i < 4; i++) {	//Apply intensity values to each
			mypack[i] = floor(pack[i] * intensity/255);
		}
		dmx.send(mypack, 4);
		previous = intensity;
	}
}*/
