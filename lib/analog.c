#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"

#include "main.h"
#include "pindef.h"

#define RATE 200000 //100kHz
#define CHANNEL ADC_CHANNEL_4

// CONSTRUCTOR
AnalogIn::AnalogIn()
{
	pinconf(ADC);
	init();
}

AnalogOut::AnalogOut()
{
	pinconf(DAC);
	init();
}

// PROTECTED
void AnalogIn::init()
{
	ADC_Init(LPC_ADC, RATE);
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN2, DISABLE);
	ADC_ChannelCmd(LPC_ADC, CHANNEL, ENABLE);
}

void AnalogOut::init()
{
	DAC_Init(LPC_DAC);
}

// PUBLIC
uint16_t AnalogIn::read()
{
	ADC_StartCmd(LPC_ADC, ADC_START_NOW);

	while (!(ADC_ChannelGetStatus(LPC_ADC, CHANNEL, ADC_DATA_DONE)));
	return ADC_ChannelGetData(LPC_ADC, CHANNEL);

}

void AnalogOut::write(uint32_t value)
{
	DAC_UpdateValue(LPC_DAC, value);
}
