//Daniel's IC
#include "main.h"

GAME::GAME(){
  flag = 0;
  index = 0;
  int sequence[9] = {0,0,0,0,0,0,0,0,0};
  int user_sequence[9] = {0,0,0,0,0,0,0,0,0};
};

void GAME::set_sequence(int difficulty){
  for (int i = 0; i < 10; i++){
    sequence[i] = ain.read() % (2 + difficulty) + 1; //0 is red, green, 1 is red green blue
  }
  index = 0;
  limit = 0;
}

int GAME::test_sequence(){
  for (int i = 0; i < limit; i++){
    if (sequence[i] != user_sequence[i]){
      return 1;
    }
  }
  return 0;
}

void GAME::edit_user_sequence(int input){
  user_sequence[index] = input + 1;
  index++;
  if (index > limit){
    limit ++;
    index = 0;
    flag = 1;
  }
}

int GAME::check_flag(){
  if (flag == 0){
    return 0;
  }
  else{
    flag = 0;
    return 1;
  }
}

void GAME::send_sequence(){
  for (int j = 0; j <= limit; j++){
    if(sequence[j] == 1){
      dmx.send(red, 5);
    }
    if(sequence[j] == 2){
      dmx.send(green, 5);
    }
    if(sequence[j] == 3){
      dmx.send(blue, 5);
    }
    if(sequence[j] == 4){
      dmx.send(cyan, 5);
    }
    if(sequence[j] == 5){
      dmx.send(magenta, 5);
    }
    if(sequence[j] == 6){
      dmx.send(yellow, 5);
    }
    delay(TIME);
    dmx.send(empty, 5);
  }
}
