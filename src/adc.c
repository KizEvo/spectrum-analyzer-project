#include <stdint.h>
#include "adc.h"
#include "nvic.h"
#include "systick.h"

volatile float ADC_MIC_OFFSET = 0;
volatile float ADC_MIC_AMP = 0;

void ADC_Init(void)
{
	ADC_Register *ADC1 = ADC1_Address;
	// Data right alignment in ADC_DR
	// Single conversion
	// Default
	
	// Perform 1 conversion in the regular channel conversion sequence
	ADC1->SQR1 &= ~(0xF << 20);
	
	// Set channel 8, 9 sampling time = 15 cycles of APB2 clock(60 MHz)
	ADC1->SMPR2 |= ((1 & 0x7) << 27) | ((1 & 0x7) << 24);
	
	// Set ADC1 channel 8 (MIC_OFFSET) the first channel to be converted
	ADC1->SQR3 |= (8 << 0);
	
	// Turn on ADC
	ADC1->CR2 |= (1 << 0);
	
	// Wait for few clocks t_STAB
	delay_us(500);
	
	// Get DC offset of MIC
	for(uint16_t i = 0; i <= 255; i++)
	{
		while(!((ADC1->SR & 1 << 1) != 0));
		// clears ADC EOC flag
		ADC1->SR &= ~(1 << 1);
		ADC_MIC_OFFSET += (ADC1->DR & 0xFFFF);
	}
	
	// Turn off ADC
	ADC1->CR2 &= ~(1 << 0);
	
	ADC_MIC_OFFSET /= 256.0;
	ADC_MIC_OFFSET = ADC_MIC_OFFSET * 3.3 / 4096.0; // 12-bit resolution, Vref = 3.3V
	
	// Wait for few clocks
	delay_us(500);
	
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
	// Calculate voltage
	ADC_MIC_AMP = (ADC1->DR & 0xFFFF) * 3.3 / 4096.0;
	// clears ADC EOC flag
	ADC1->SR &= ~(1 << 1);
}