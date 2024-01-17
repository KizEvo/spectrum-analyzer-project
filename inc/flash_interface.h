#ifndef FLASH_INTERFACE_H
#define FLASH_INTERFACE_H

#include <stdint.h>

typedef struct {
	volatile uint32_t ACR, KEYR, OPTKEYR, SR, CR, OPTCR;
} FLASHINT_Register;

#define FlashInt_Address	((FLASHINT_Register *)(0x40023C00))

#endif