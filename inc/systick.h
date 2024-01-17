#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

extern uint8_t STK_Clock;

void delay_us(uint32_t timeInMicroSecond);

#endif

