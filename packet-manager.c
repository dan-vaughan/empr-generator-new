#include <stdlib.h>

packet_manager :: packet_manager(){
  init();
}

//PROTECTED

void packet_manager :: init(){
  int i;
  char * * packets = (char * *) malloc(sizeof(char) * 8 * 100);//Allocates that entire block
  short * sequences = (short *) malloc(sizeof(short) * 100 * 100); //Allocates entire block
  for (i = 0; i < 100; i++){
    packets[i] = (char *) malloc(sizeof(char) * 8);//splits the block up by each packet
    packets[i][0] = 0x00;//writes the start code for each packet
  }
}

//PUBLIC

void packet_manager :: write_packet(int packet_number, char * buf){
  int i;
  for (i = 1; i < 8; i++){
    packets[packet_number][i] = buf[i];
  }
}

char * packet_manager :: read_packet(int packet_number){
  return packets[packet_number];
}

//PRIVATE
