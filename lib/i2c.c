#include "lpc17xx_i2c.h"
#include <stdio.h>

#include "main.h"
#include "pindef.h"

// CONSTRUCTOR
I2C::I2C(PINSEL_CFG_Type sda, PINSEL_CFG_Type scl)
{
	pinconf(sda);
	pinconf(scl);
	init();
}

// PROTECTED
void I2C::init()
{
	I2C_Init(LPC_I2C1, 100000);
	I2C_Cmd(LPC_I2C1, ENABLE);
}

// PUBLIC
int I2C::write(uint32_t addr, uint8_t * data, uint32_t length)
{
	I2C_M_SETUP_Type setup = {addr, data, length, 0, 0, NULL, 0, 0, 0, NULL};

	return I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
}

int I2C::read(uint32_t addr, uint8_t * data, uint32_t length)
{
	I2C_M_SETUP_Type setup = {addr, NULL, 0, 0, data, length, 0, 0, 0, NULL};

	return I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
}
