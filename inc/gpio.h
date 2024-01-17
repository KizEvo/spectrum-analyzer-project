#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

enum GPIO_PORT_MODE 		{INPUT, GP_OUTPUT, ALT_FUNCTION, ANALOG};
enum GPIO_OUTPUT_TYPE		{PUSH_PULL, OPEN_DRAIN};
enum GPIO_OUTPUT_SPEED		{LOW, MEDIUM, HIGH, VERY_HIGH};
enum GPIO_PULL				{NO_PULL, PULL_UP, PULL_DOWN};
enum GPIO_ALT_FUNCTION		{AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15};
enum GPIO_BANK_NAME			{A, B, C, D, E, F, G, H, I, J, K};
enum GPIO_PIN_STATE			{RESET, SET};

typedef struct {
	volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFRL, AFRH;
} GPIO_Register;

#define GPIO_Bank(x)	((GPIO_Register *)(0x40020000 + 0x400 * (x)))

typedef struct {
	uint8_t pin, bank, mode, out_type, out_speed, pull, alt_function;
} GPIO_TypeDef;

void GPIO_Init(GPIO_TypeDef *gpio);
void GPIO_Write(uint8_t pin, enum GPIO_BANK_NAME gpio_bank, enum GPIO_PIN_STATE pin_state);
void GPIO_Toggle(uint8_t pin, enum GPIO_BANK_NAME gpio_bank);

#endif