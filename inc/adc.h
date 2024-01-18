#ifndef ADC_H
#define ADC_H

#include <stdint.h>

typedef struct {
	volatile uint32_t SR, CR1, CR2, SMPR1, SMPR2, JOFR1, JOFR2, JOFR3, JOFR4, HTR, LTR, SQR1, SQR2, SQR3, JSQR, 
						JDR1, JDR2, JDR3, JDR4, DR;
} ADC_Register;

typedef struct {
	volatile uint32_t CSR, CCR, CDR;
} ADC_COM_Register;

#define ADC_BaseAddress		(0x40012000)
#define ADC1_Address		((ADC_Register *)(ADC_BaseAddress))

extern volatile float ADC_MIC_OFFSET;
extern volatile float ADC_MIC_AMP;
void ADC_Init(void);

#endif