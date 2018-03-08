#include "main.h"

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
  if (option == 5){
    clear_display();
    return_home();
    printstr("Congratulations!");
    shift_line();
    printstr("Play again soon?");
  }
}


void game_action(int button){
  my_game.edit_user_sequence(button);
  buzzer(button + 1);
  if (button == 0){
    printstr("R");
  }
  if (button == 1){
    printstr("G");
  }
  if (button == 2){
    printstr("B");
  }
  if (button == 3){
    printstr("C");
  }
  if (button == 4){
    printstr("M");
  }
  if (button == 5){
    printstr("Y");
  }
}

void level_start(int colour_index){

  clear_display();
  return_home();
  putcustom(colour_index + 177);
  printstr(" colours. Play!");
  return_home();
  shift_line();
  for (int i = 0; i <= colour_index; i++){
    putcustom(90);
  }
  return_home();
  shift_line();
}

void buzzer(int tone_factor){
  for (int buzz = 0; buzz < 200 / tone_factor; buzz++) {
    aout.write(60000);
    delay(2500 * tone_factor);
    aout.write(0);
  }
}

void difficulty_display(){
  clear_display();
  return_home();
  printstr("Difficulty (1-5)");
  shift_line();
  putcustom(90);
}

void game_start(int difficulty){
  my_game.set_sequence(difficulty);
  display_options(3);
  delay(TIME);
  int failed;
  for(int colour_index = 0; colour_index < 10; colour_index++){
    display_options(4);
    delay(TIME);
    my_game.send_sequence();
    level_start(colour_index);
    while (!my_game.check_flag()) {
      keypad_check(game_action);
    }
    failed = my_game.test_sequence();
    delay(TIME);
    if (failed){
      display_options(2);
      delay(TIME);
      break;
    }
    if (!failed && colour_index < 9) {
      display_options(1);
    }
    if (!failed && colour_index == 9) {
      display_options(5);
    }
    delay(TIME);
  }
}
