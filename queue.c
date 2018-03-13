//Asher's IC
#include "main.h"


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

int Queue::min()
{
  int min = queue[0];
  for (int i = 1; i < SIZE; i++) {
    if (queue[i] < min) min = queue[i];
  }
  return min;
}

int Queue::max()
{
  int max = queue[0];
  for (int i = 1; i < SIZE; i++) {
    if (queue[i] > max) max = queue[i];
  }
  return max;
}
