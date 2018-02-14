#include "lib/serial.h"
#include "lib/serial-dmx.h"
#include "lib/utilities.h"
#include "lib/keypad.h"
#include "lib/display.h"
#include "packet-manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern I2C i2c;
extern Serial pc;
