#include "lpc_types.h"

#include "i2c.h"
#include "pindef.h"
#include "utilities.h"

char pressed[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char masks[4] = {0x08, 0x04, 0x02, 0x01};

void checkrow(char b, char mask, int row, void (*action)(int))
{   //action is a function pointer for what to do when button pressed
    int i;
    uint8_t c;

    if ((b & mask) != mask) //Detected a key in row
    {
        //Scan all columns
        for (i = 0; i < 4; i++)
        {
            c = masks[i]*0x10;
            int button = row*4+i;
            i2c.write(KEYPAD, &c, 1);
            i2c.read(KEYPAD, &c, 1);
            if (c == 0)
            { //Button is pressed, check if was already pressed...

                if (pressed[button] == 0)
                {   //Execute action...
                    (*action)(button);
                    pressed[button] = 1;
                }
            }
            else
            {   //key not pressed, set to 0
                pressed[button] = 0;
            }
        }
    }
    else //No key pressed in row, set all to 'not pressed'
    {
        for (i = row*4; i < row*4+4; i++)
        {
            pressed[i] = 0;
        }
    }
}

void keypad_check(void (*action)(int)) {
    //Takes a function to execute when button pressed
    uint8_t b;
    int j;
    b = 0x0F;
    i2c.write(KEYPAD, &b, 1);
    i2c.read(KEYPAD, &b, 1);
    for (j = 0; j < 4; j++)
    {
       checkrow(b, masks[j], j, action);
    }
}
