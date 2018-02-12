#include "lpc17xx_pinsel.h"

class DMX
{
	protected:
		void init();

	public:
		DMX();
		int write(char * buf, int length);
		void send(char * buf, int length);
		void send_break();
		void end_break();
};
