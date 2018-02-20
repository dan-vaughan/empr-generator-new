#include "lpc17xx_uart.h"
#include "lpc_types.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "serial-dmx.h"
#include "pindef.h"
#include "utilities.h"


// CONSTRUCTOR
DMX::DMX()
{
	pinconf(SERIALTX);
	pinconf(SERIALRX);

	init();
}

// PROTECTED
void DMX::init()
{
	UART_CFG_Type cfg;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;

	UART_ConfigStructInit(&cfg);
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	cfg.Baud_rate = 250000;
	cfg.Stopbits = UART_STOPBIT_2;

	UART_Init((LPC_UART_TypeDef *)LPC_UART1, &cfg);
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART1, &UARTFIFOConfigStruct);
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART1, ENABLE);

	send_break();
	delay(1000);
}

char * DMX::pad(char * buf, int length)
{
	char * packet = (char *)malloc(513*sizeof(char));
	if (packet == NULL) throw("Not enough memory");
	memset(packet, 0, 513*sizeof(char));
	memcpy(packet, buf, length);

	return packet;
}

// PUBLIC
void DMX::send(char * buf, int length)
{
	end_break();
	delay(40);	//Precise timing to bodge the end break's high into a MAB
	char * packet = pad(buf, length);
	write(packet, 513);
	free(packet);
	delay(1000);
	send_break();
	delay(1000);

}

int DMX::write(char * buf, int length)
{
	//end_break();
	return(UART_Send((LPC_UART_TypeDef *)LPC_UART1,(uint8_t *)buf,length, BLOCKING));
}

void DMX::send_break()
{
	LPC_UART1->LCR |= UART_LCR_BREAK_EN;	//Set bit 6 to 1
}

void DMX::end_break()
{
	LPC_UART1->LCR &= ~UART_LCR_BREAK_EN;	//Set bit 6 to 0
}
