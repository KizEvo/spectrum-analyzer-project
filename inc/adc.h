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

enum ADC_SAMPLE_STAGE {ADC_RST, ADC_HALF, ADC_FULL};

#define ADC_BaseAddress		(0x40012000)
#define ADC1_Address		((ADC_Register *)(ADC_BaseAddress))

#define SAMPLES_LENGTH 512

extern volatile uint32_t ADC_MIC_OFFSET;
extern volatile uint8_t ADC_STATE;
extern volatile float SAMPLES[SAMPLES_LENGTH];
void ADC_Init(void);

#endif