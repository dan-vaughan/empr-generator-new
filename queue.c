#include "main.h"

//PART OF ASHER'S INDIVIDUAL COMPONENT

//CONSTRUCTOR
Queue::Queue(int initial)
{ //Initial is the value to populate the queue with. SIZE is defined in the header file.
  memset(queue, initial, SIZE*sizeof(int));
  pos = 0;
}

//PROTECTED

//PUBLIC
void Queue::add(int val)
{
  queue[pos] = val;
  pos = (pos + 1) % SIZE;  //Increment pos by 1, modulo SIZE
}

int Queue::avg()
{ //Return the average of all values in the queue
  int sum = 0;
  for (int i=0; i < SIZE; i++) {
    sum += queue[i];
  }

  return sum/SIZE;
}
