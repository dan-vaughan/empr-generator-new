#include "game.h"
#include "main.h"

#define TIME 10000000

static int input;

void display_options(int option){
  if (option == 0){
    clear_display();
    return_home();
    printstr("1-Red, 2-Grn");
    shift_line();
    printstr("3-Blu, 4-Yel");
  }
  if (option == 1){
    clear_display();
    return_home();
    printstr("Well done!");
    shift_line();
    printstr("Next level...");
  }
  if (option == 2){
    clear_display();
    return_home();
    printstr("Better luck");
    shift_line();
    printstr("next time");
  }
  if (option == 3){
    clear_display();
    return_home();
    printstr("Welcome to");
    shift_line();
    printstr("Simple Simon");
  }
  if (option == 4){
    clear_display();
    return_home();
    printstr("Get ready!");
  }
}

void game_action(int button){
  input = labels[button];
}

int pseudo_random(int range){
  int val = ain.read();
  return val % range;
}

void send_game_sequence(int * colour_sequence, int i){
  for (int j = 0; j <= i; j++){
    if(colour_sequence[j] == 1){
      dmx.send(red, 5);
    }
    if(colour_sequence[j] == 2){
      dmx.send(green, 5);
    }
    if(colour_sequence[j] == 3){
      dmx.send(blue, 5);
    }
    if(colour_sequence[j] == 4){
      dmx.send(yellow, 5);
    }
    delay(TIME);
    dmx.send(empty, 5);
  }
}



void game_start(){
  display_options(3);
  delay(TIME);
  int i = 0;
  int failed = 0;
  int colour_index;
  int * colour_sequence = (int*)malloc(sizeof(int));
  int * user_sequence = (int*)malloc(sizeof(int));
  colour_sequence[0] = pseudo_random(4) + 1;
  while(!failed){
    display_options(4);
    delay(TIME);
    send_game_sequence(colour_sequence, i);
    display_options(0);

    for (colour_index = 0; colour_index <= i; colour_index++){
      keypad_check(game_action);
      user_sequence[colour_index] = input;
      //send tone to speaker
    }

    for (colour_index = 0; colour_index <= i; colour_index++){
        if (colour_sequence[colour_index] != user_sequence[colour_index]){
          failed = 1;
        }
    }
    if (failed) {
      display_options(2);
    }
    else{
      display_options(1);
    }
    delay(TIME);
    i += 1;
    int * colour_sequence = (int *)realloc(colour_sequence, sizeof(int) * (i + 1));
    int * user_sequence = (int *)realloc(colour_sequence, sizeof(int) * (i + 1));
    colour_sequence[i] = pseudo_random(4) + 1;
  }
}
