#include "lib/serial.h"
#include "lib/serial-dmx.h"
#include "lib/utilities.h"
#include "lib/keypad.h"
#include "lib/display.h"
#include "lib/analog.h"
#include "packet-manager.h"
#include "sequence-manager.h"

#include "queue.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

extern I2C i2c;
extern Serial pc;

extern DMX dmx;
extern AnalogIn ain;
extern char labels[16];

extern char red[5];
extern char green[5];
extern char blue[5];
extern char empty[5];
extern char yellow[5];

