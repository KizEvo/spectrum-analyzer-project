#include <stdint.h>
#include "rcc.h"
#include "flash_interface.h"
#include "systick.h"

extern void delay(uint32_t time);

void RCC_SystemClockInit(RCC_SysClkTypeDef *rcc, RCC_APBx_AHB_PreTypeDef *prescaler)
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
		REG->CFGR |= (HSE << 0);
		// Turn HSI off
		REG->CR &= ~(1 << 0);
		return;
	}
	else
	{
		if (rcc->PLLSRC == HSE)
		{
			// Turn HSE on
			REG->CR |= (1 << 16);
			// Wait until HSERDY flag set
			while(!((REG->CR & 1 << 17) != 0));
		}
		else
		{
			// Wait until HSI flag set
			while(!((REG->CR & 1 << 1) != 0));
		}
		
		// Get flash wait states based on main system clock freq and voltage level - check the datasheet for different range of voltage input, this is 2.7-3.6V
		uint32_t freq_range_mhz = ((uint32_t)(CLK_FREQ * rcc->PLLN) / (uint32_t)(rcc->PLLM * rcc->PLLP));
		uint8_t wait_state = (freq_range_mhz <= 30) ? 0 :
							 (freq_range_mhz <= 60) ? 1 :
							 (freq_range_mhz <= 90) ? 2 :
							 (freq_range_mhz <= 120)? 3 :
							 (freq_range_mhz <= 150)? 4 : 5;
		// Calculate STK_Clock
		STK_Clock = ((freq_range_mhz / prescaler->HPRE) / 8);
		
		rcc->PLLQ = (rcc->PLLQ < 2 || rcc->PLLQ > 15) ? 0b0100U : rcc->PLLQ;
		rcc->PLLP = (rcc->PLLP == 2) ? 0b00 :
					(rcc->PLLP == 4) ? 0b01 :
					(rcc->PLLP == 6) ? 0b10 : 0b11;
		rcc->PLLN = (rcc->PLLN < 50 || rcc->PLLN > 432) ? 0x30 : rcc->PLLN;
		rcc->PLLM = (rcc->PLLM < 2 || rcc->PLLM > 63) ? 0x10 : rcc->PLLM;
		
		REG->PLLCFGR = ((rcc->PLLQ & 0xF) << 24) | ((rcc->PLLSRC & 0x1) << 22) | ((rcc->PLLP & 0x3) << 16) | ((rcc->PLLN & 0x1FF) << 6) | ((rcc->PLLM & 0x3F) << 0);
		
		// Wait for hardware to lock PLL
		while((REG->CR & 1 << 25) != 0);
		
		FLASHINT_Register *FLASH = FlashInt_Address;					 
		FLASH->ACR |= ((wait_state) << 0);
		
		// AHBx divider
		prescaler->PPRE2 = (prescaler->PPRE2 == 2) ? 0b100U :			// prescaler 2
						   (prescaler->PPRE2 == 4) ? 0b101U :			// 4
						   (prescaler->PPRE2 == 8) ? 0b110U : 0b111U;	// 8 and 16 // prescaler = 16 by default incase PLL is used and APBx prescaler is not set correctly

		prescaler->PPRE1 = (prescaler->PPRE1 == 2) ? 0b100U :			// prescaler 2
						   (prescaler->PPRE1 == 4) ? 0b101U :			// 4
						   (prescaler->PPRE1 == 8) ? 0b110U : 0b111U;	// 8 and 16
	
		prescaler->HPRE  = (prescaler->HPRE <  2)  ? 0b0000U :
						   (prescaler->HPRE == 2)  ? 0b1000U :
						   (prescaler->HPRE == 4)  ? 0b1001U :
						   (prescaler->HPRE == 8)  ? 0b1010U :
						   (prescaler->HPRE == 16) ? 0b1011U :
						   (prescaler->HPRE == 64) ? 0b1100U :
						   (prescaler->HPRE == 128)? 0b1101U :
						   (prescaler->HPRE == 256)? 0b1110U : 0b1111U;

		REG->CFGR |= (prescaler->PPRE2 << 13) | (prescaler->PPRE1 << 10) | (prescaler->HPRE << 4);
		
		// Turn PLL on
		REG->CR |= (1 << 24);
		
		// Wait for hardware to unlock PLL
		while(!((REG->CR & 1 << 25) != 0));
		
		// Switch System clock to PLL
		REG->CFGR |= ((0b10U & 0x3) << 0);
	}
}

void RCC_PeripheralClockEnable(enum RCC_PERIPHERAL_CLK clock, uint8_t peripheral)
{
	RCC_Register *REG = RCC_Address;
	if(clock == AHB1) REG->AHB1ENR |= (1 << peripheral);
	else if (clock == APB2) REG->APB2ENR |= (1 << peripheral);
	else if (clock == APB1) REG->APB1ENR |= (1 << peripheral);
}