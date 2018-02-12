#include "lpc17xx_pinsel.h"

//Define PinCFG structs as Portnum, Pinnum, Funcnum, Pinmode, OpenDrain
	PINSEL_CFG_Type USBTX = {0, 2, 1, 0, 0};
	PINSEL_CFG_Type USBRX = {0, 3, 1, 0, 0};

	PINSEL_CFG_Type SERIALTX = {0, 15, 1, 0, 0};
	PINSEL_CFG_Type SERIALRX = {0, 16, 1, 0, 0};

	PINSEL_CFG_Type SDA = {0, 0, 3, 0, 0};
	PINSEL_CFG_Type SCL = {0, 1, 3, 0, 0};

void pinconf(PINSEL_CFG_Type PinCfg)
{
	PINSEL_ConfigPin(&PinCfg);
}
