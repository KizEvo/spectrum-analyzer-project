#include <stdint.h>
#include "rcc.h"

void RCC_SystemClockInit(RCC_SysClkTypeDef *rcc)
{
	RCC_Register *REG = RCC_Address;
	if (rcc->clock == HSI) 
	{
		// HSI = 16 MHz
		while(!((REG->CR & 1 << 1) != 0));
		return;
	}
	else if (rcc->clock == HSE)
	{
		// Turn HSE on
		REG->CR |= (1 << 16);
		// Wait until HSERDY flag set
		while(!((REG->CR & 1 << 17) != 0));
		// Switch System clock to HSE
		REG->CFGR |= (1 << HSE);
	}
	else
	{
		// Implement later
	}
}

void RCC_PeripheralClockEnable(enum RCC_PERIPHERAL_CLK clock, uint8_t peripheral)
{
	RCC_Register *REG = RCC_Address;
	if(clock == AHB1) REG->AHB1ENR |= (1 << peripheral);
	else if (clock == APB2) REG->APB2ENR |= (1 << peripheral);
}