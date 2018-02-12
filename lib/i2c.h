#include "lpc17xx_pinsel.h"

#define DISPLAY 0x3B
#define KEYPAD 0x21

class I2C
{
	protected:
		void init(void);
	public:
		I2C(PINSEL_CFG_Type sda, PINSEL_CFG_Type scl);
		int write(uint32_t addr, uint8_t * data, uint32_t length);
		int read(uint32_t addr, uint8_t * data, uint32_t length);
};

