#include "lpc17xx_pinsel.h"

extern PINSEL_CFG_Type USBTX;
extern PINSEL_CFG_Type USBRX;
extern PINSEL_CFG_Type SERIALTX;
extern PINSEL_CFG_Type SERIALRX;

extern PINSEL_CFG_Type SDA;
extern PINSEL_CFG_Type SCL;

extern PINSEL_CFG_Type ADC;
extern PINSEL_CFG_Type DAC;

extern PINSEL_CFG_Type PWM_pin;

void pinconf(PINSEL_CFG_Type);
