#include <stdint.h>
#include "gpio.h"

void GPIO_Init(GPIO_TypeDef *gpio)
{
	GPIO_Register *REG = GPIO_Bank(gpio->bank);
	
	uint8_t pin = gpio->pin;
	
	REG->MODER		&= ~(3U << (pin << 1));
	REG->MODER 		|= ((gpio->mode & 3U) << (pin * 2));
	
	REG->OTYPER 	|= ((gpio->out_type & 3U) << pin);
	REG->OSPEEDR	|= ((gpio->out_speed & 3U) << (pin * 2));
	REG->PUPDR		|= ((gpio->pull	& 3U) << (pin * 2));
	
	if (gpio->mode == ALT_FUNCTION)
	{
		if (pin < 8) REG->AFRL |= ((gpio->alt_function & 15U) << (pin * 4));
		else REG->AFRH |= ((gpio->alt_function & 15U) << (pin * 4));
	}
}

void GPIO_Write(uint8_t pin, enum GPIO_BANK_NAME gpio_bank, enum GPIO_PIN_STATE pin_state)
{
	GPIO_Register *REG = GPIO_Bank(gpio_bank);
	if (pin_state == RESET) REG->ODR &= ~(1 << pin);
	else REG->ODR |= (1 << pin);
}

void GPIO_Toggle(uint8_t pin, enum GPIO_BANK_NAME gpio_bank)
{
	GPIO_Register *REG = GPIO_Bank(gpio_bank);
	REG->ODR ^= (1 << pin);
}
