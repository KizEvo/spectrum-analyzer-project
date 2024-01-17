#include <stdint.h>
#include "gpio.h"
#include "rcc.h"

extern int main(void);

void GPIOB_Config(void);
void RCC_ClocksConfig(void);
void delay(uint32_t time);

int main(void)
{	
	GPIOB_Config();
	RCC_ClocksConfig();

	while(1)
	{
		GPIO_Write(12, B, SET);
		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
		GPIO_Write(12, B, RESET);
		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
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
	prescaler.HPRE 		= 1U;		
	prescaler.PPRE2		= 2U;		
	prescaler.PPRE1		= 4U;		
	
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

void delay(uint32_t time)
{
	uint32_t temp = time;
	uint32_t temp_2 = time;
	while(time > 0)
	{
		time -= 1;
		while(temp > 0) 
		{
			temp -= 1;
			while(temp_2 > 0)
			{
				temp_2 -= 1;
			}
		}
	}
}