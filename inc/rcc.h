#ifndef RCC_H
#define RCC_H

#include <stdint.h>

enum RCC_SYSCLK_IN		{HSI, HSE, PLLCLK};
enum RCC_AHB1_CLK_POS	{GPIOAEN, GPIOBEN, GPIOCEN, GPIODEN, GPIOEEN, GPIOFEN, GPIOGEN, GPIOHEN, GPIOIEN, GPIOJEN, GPIOKEN,
							CRCEN = 12, BKPSRAMEN = 18, CCMDATARAMEN = 20, DMA1EN, DMA2EN, DMA2DEN, ETHMACEN = 25, ETHMACTXEN, ETHMACRXEN,
							ETHMACPTPEN, OTGHSEN, OTHGHSULPIEN};
enum RCC_APB2_CLK_POS	{TIM1EN, TIM8EN, USART1EN = 4, USART6EN, ADC1EN = 8, ADC2EN, ADC3EN, SDIOEN, SPI1EN, SPI4EN, SYSCFGEN, TIM9EN = 16,
							TIM10EN, TIM11EN, SPI5EN = 20, SPI6EN, SAI1EN, LTDCEN = 26};
enum RCC_PERIPHERAL_CLK {AHB1, AHB2, AHB3, APB1, APB2, AHB1LP, AHB2LP, AHB3LP, APB1LP, APB2LP};

typedef struct {
	volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR, 
					_RESERVED_0, APB1RSTR, APB2RSTR, _RESERVED_1, _RESERVED_2,
					AHB1ENR, AHB2ENR, AHB3ENR, _RESERVED_3, APB1ENR, APB2ENR,
					_RESERVED_4, _RESERVED_5, AHB1LPENR, AHB2LPENR, AHB3LPENR,
					_RESERVED_6, APB1LPENR, APB2LPENR, _RESERVED_7, _RESERVED_8,
					BDCR, CSR, _RESERVED_9, _RESERVED_10, SSCGR, PLLI2SCFGR;
} RCC_Register;

#define RCC_Address		((RCC_Register *)(0x40023800))

typedef struct {
	uint16_t PLLN;
	uint8_t PLLQ;			// Main PLL division factor for USB OTG FS, SDIO, RNG clk
	uint8_t PLLSRC; 		// Main PLL and audio PLL (PLLI2S) clk source
	uint8_t PLLP; 			// Main PLL division factor for main system clock
	uint8_t PLLM;			// Main PLL multiplication factor for VCO
	uint8_t clock;			// Clock source for main system clock (HSI, HSE, PLLCLK)
} RCC_SysClkTypeDef;

// ###### PLL clock outputs formulas ######
// f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
// f(PLL general clock output) = f(VCO clock) / PLLP
// f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ

void RCC_SystemClockInit(RCC_SysClkTypeDef *rcc);
void RCC_PeripheralClockEnable(enum RCC_PERIPHERAL_CLK clock, uint8_t peripheral);

#endif