#include <stdint.h>
#include "adc.h"
#include "nvic.h"
#include "systick.h"
#include "gpio.h"

volatile uint32_t ADC_MIC_OFFSET = 0;
volatile uint8_t ADC_STATE = ADC_RST;
volatile float SAMPLES[SAMPLES_LENGTH] = {0};

void ADC_Init(void)
{
	ADC_Register *ADC1 = ADC1_Address;
	// Data right alignment in ADC_DR
	// Single conversion
	// ADC prescaler = 2
	// Perform 1 conversion in the regular channel conversion sequence
	// Default
	
	// Set channel 8, 9 sampling time = 15 cycles 
	ADC1->SMPR2 |= ((1 & 0x7) << 27) | ((1 & 0x7) << 24);
	
	// Set ADC1 channel 8 (MIC_OFFSET) the first channel to be converted
	ADC1->SQR3 = (8 << 0);
	
	// Turn on ADC
	ADC1->CR2 |= (1 << 0);
	
	// Wait for few clocks t_STAB
	delay_us(50000);
	
	// Get DC offset of MIC
	for(uint16_t i = 0; i <= 255; i++)
	{
		// Start ADC conversion
		ADC1->CR2 |= (1 << 30);
		while(!(ADC1->SR & 1 << 1));
		// A read to ADC_DR clears EOC flag
		ADC_MIC_OFFSET = ADC_MIC_OFFSET + (ADC1->DR & 0xFFFF);
	}	
	// Turn off ADC
	ADC1->CR2 &= ~(1 << 0);
	
	ADC_MIC_OFFSET /= 256;
	
	// Wait for few clocks t_STAB
	delay_us(50000);
	
	// Set ADC1 channel 9 (MIC_AMP) the first channel to be converted
	ADC1->SQR3 &= ~(0x1F << 0);
	ADC1->SQR3 |= (9 << 0); 
	
	// Enable End Of Conversion interrupt
	ADC1->CR1 |= (1 << 5);
	
	// Enable NVIC for ADC
	NVIC_EnableIRQ(18);
	
	// Turn on ADC
	ADC1->CR2 |= (1 << 0);
	
	// Wait for few clocks t_STAB
	delay_us(500);
}

void ADC_Handler(void)
{
	ADC_Register *ADC1 = ADC1_Address;
	static uint16_t sample_count = 0;
	
	if(sample_count >= SAMPLES_LENGTH)
	{
		sample_count = 0;
	}
	
	if(sample_count < SAMPLES_LENGTH)
	{
		// Store samples
		uint16_t temp = (ADC1->DR & 0xFFFF) - ADC_MIC_OFFSET;
		SAMPLES[sample_count] = (float)(temp) * 4096.0 / 3.3;
		sample_count += 1;
	}
	
	if(sample_count == (SAMPLES_LENGTH / 2) - 1)
	{
		ADC_STATE = ADC_HALF;
		GPIO_Write(12, B, SET);
	}
	else if (sample_count == SAMPLES_LENGTH - 1)
	{
		ADC_STATE = ADC_FULL;
		GPIO_Write(12, B, RESET);
	}

	// clears ADC EOC flag
	ADC1->SR &= ~(1 << 1);
}