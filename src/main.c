#include <stdint.h>
#include "gpio.h"
#include "rcc.h"
#include "systick.h"
#include "tim.h"
#include "nvic.h"

extern int main(void);

void GPIOB_Config(void);
void RCC_ClocksConfig(void);
void TIM_ADC_SampleTime_Config(void);

int main(void)
{	
	RCC_ClocksConfig();
	GPIOB_Config();
	TIM_ADC_SampleTime_Config();
	
	while(1)
	{
	}
	return 0;
}

void RCC_ClocksConfig(void)
{
	RCC_SysClkTypeDef clk_init;
	RCC_APBx_AHB_PreTypeDef prescaler;
	
	clk_init.clock 		= PLLCLK;
	// f(VCO out) = 16 MHz (HSI) * PLLN / PLLM
	// 100 MHz < f(VCO out) < 432 MHz
	clk_init.PLLSRC		= HSI;
	clk_init.PLLN		= 120U;
	clk_init.PLLM		= 8U;
	// f(PLL general clock output) = f(VCO clock) / PLLP
	// This must be <= 168 MHz
	clk_init.PLLP		= 2U;
	// f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ
	// This must be <= 48 MHz
	clk_init.PLLQ	 	= 4U;
	
	// AHB, APB prescaler
	prescaler.HPRE 		= 1U;	// 120 / 1
	prescaler.PPRE2		= 2U;	// 120 / 2 = 60 MHz
	prescaler.PPRE1		= 4U;	// 120 / 4 = 30 MHz
	
	RCC_SystemClockInit(&clk_init, &prescaler);
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

void TIM_ADC_SampleTime_Config(void)
{
	TIM_CounterTypeDef adc_tim;
	
	uint16_t sample_time_hz = 1000;
	// Divide by 2 means the APBx timer clock is multiply by 2 if the APBx prescaler = 1 => x1 else x2
	adc_tim.PSC = (((STK_Clock * 8) / 2) * 1000000) / sample_time_hz;
	adc_tim.PSC = adc_tim.PSC - 1;
	adc_tim.ARR = 500;	// 500ms
	adc_tim.ARPE = 1;
	adc_tim.DIR = 0;
	adc_tim.URS = 1;
	adc_tim.UIE = 1;
	
	RCC_PeripheralClockEnable(APB1, TIM2);
	NVIC_EnableIRQ(28);
	TIM_CounterInit(&adc_tim);
}
