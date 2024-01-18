#include <stdint.h>
#include "tim.h"
#include "gpio.h"
#include "adc.h"

// General purpose TIM2 to TIM5
typedef struct {
	volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR1, CCMR2, CCER, CNT, PSC, ARR, _RESERVED_0,
						CCR1, CCR2, CCR3, CCR4, _RESERVED_1, DCR, DMAR, OR;
} TIM2_TIM5_Register;

#define TIM2_Address ((TIM2_TIM5_Register *)(0x40000000))

void TIM_CounterInit(TIM_CounterTypeDef *tim)
{
	TIM2_TIM5_Register *TIM2 = TIM2_Address;
	
	TIM2->PSC = tim->PSC;
	
	TIM2->ARR = tim->ARR;
	
	TIM2->CR1 |= (tim->ARPE << 7);
	
	TIM2->CR1 |= (tim->DIR << 4);
	
	TIM2->CR1 |= (tim->URS << 2);
	
	TIM2->DIER |= (tim->UIE << 0);
	
	// Enable counter
	TIM2->CR1 |= (1 << 0);
}

void TIM2_Handler(void)
{
	TIM2_TIM5_Register *TIM2 = TIM2_Address;
	ADC_Register *ADC1 = ADC1_Address;
	// Toggle pin state
	GPIO_Toggle(12, B);
	// Start ADC conversion
	ADC1->CR2 |= (1 << 30);
	// Clear TIM interrupt flag
	TIM2->SR &= ~(1 << 0);
}