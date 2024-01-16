#include <stdint.h>
#include "gpio.h"
#include "rcc.h"

extern int main(void);

void GPIOB_Config(void);
void delay(uint32_t time);

int main(void)
{	
	GPIOB_Config();

	while(1)
	{
		GPIO_Write(12, B, SET);
		delay(500000);
		GPIO_Write(12, B, RESET);
		delay(500000);
	}
	return 0;
}

void GPIOB_Config(void)
{
	GPIO_TypeDef led;
	
	led.pin 		= 12;
	led.bank 		= B;
	led.mode 		= GP_OUTPUT;
	led.out_type 	= PUSH_PULL;
	led.out_speed 	= LOW;
	led.pull		= NO_PULL;

	RCC_PeripheralClockEnable(AHB1, GPIOBEN);
	GPIO_Init(&led);
}

void delay(uint32_t time)
{
	uint32_t temp = time;
	while(time > 0)
	{
		time -= 1;
		while(temp > 0) 
		{
			temp -= 1;
		}
	}
}