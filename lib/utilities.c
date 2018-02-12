#include "i2c.h"
#include "pindef.h"

//PUBLIC
I2C i2c(SDA, SCL);

void delay(int time)
{
	for (int i=0; i<time; i++);
}
