#include "lpc17xx_pinsel.h"

class AnalogIn
{
	protected:
		void init(void);
	public:
		AnalogIn(PINSEL_CFG_Type in);
		uint16_t read(void);
};

class AnalogOut
{
	protected:
		void init(void);
	public:
		AnalogOut(PINSEL_CFG_Type out);
		void write(uint32_t);
};

