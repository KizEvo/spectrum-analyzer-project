#include <stdint.h>
#include "systick.h"

typedef struct {
	volatile uint32_t CTRL, LOAD, VAL, CALIB;
} STK_Register;

#define STK_Address ((STK_Register *)(0xE000E010))

uint8_t STK_Clock = 0;

// STK Clock source AHB / 8 by default
// If you change the clock source, change the formula for the STK_Clock global variable in rcc.c
void delay_us(uint32_t timeInMicroSecond)
{
	STK_Register *STK = STK_Address;
	// Delay of 1us * timeInMicroSecond, max 500ms
	if(timeInMicroSecond > 500000) timeInMicroSecond = 500000;
	STK->LOAD = STK_Clock * timeInMicroSecond - STK_Clock;
	// Clear counter value
	STK->VAL &= ~(0xFFFFFF);
	// Enable counter
	STK->CTRL |= (1 << 0);
	// Wait
	while(!((STK->CTRL & 1 << 16) != 0));
	// Disable counter
	STK->CTRL &= ~(1 << 0);
}