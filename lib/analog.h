#include "lpc17xx_pinsel.h"

class AnalogIn
{
	protected:
		void init(void);
	public:
		AnalogIn();
		uint16_t read(void);
};

class AnalogOut
{
	protected:
		void init(void);
	public:
		AnalogOut();
		void write(uint32_t);
};
