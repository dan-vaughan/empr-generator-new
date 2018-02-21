#include <stdlib.h>
#include <string.h>

#include "sequence-manager.h"


sequence_manager :: sequence_manager(){
  void * sequences;
}

//PROTECTED

//PUBLIC
void sequence_manager :: init(){
  sequences = malloc((SLEN*SMAX)+1);   //Allow for PLEN bytes, plus a null terminator
  if (sequences == NULL) throw "Unable to allocate memory";

  memset(sequences, 0, (SLEN*SMAX)+1);
}

int * sequence_manager :: getptr(int sequence_number){
  return (int *)(sequences+sequence_number*SLEN);
}

//PRIVATE
