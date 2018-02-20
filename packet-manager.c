#include <stdlib.h>
#include <string.h>

#include "packet-manager.h"


packet_manager :: packet_manager(){
  void * packets;
}

//PROTECTED

//PUBLIC
void packet_manager :: init(){
  packets = malloc((PLEN*PMAX)+1);   //Allow for PLEN bytes, plus a null terminator
  if (packets == NULL) throw "Unable to allocate memory";

  memset(packets, 0, (PLEN*PMAX)+1);
}

char * packet_manager :: getptr(int packet_number){
  return (char *)(packets+packet_number*PLEN);
}

//PRIVATE
