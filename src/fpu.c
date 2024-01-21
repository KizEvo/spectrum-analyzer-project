#include <stdint.h>
#include "fpu.h"

#define CPACR_Address	(0xE000ED88)
#define FPCCR_Address	(0xE000EF34)
#define FPCAR_Address	(0xE000EF38)
#define FPDSCR_Address	(0xE000EF3C)

void FPU_Init(void)
{
	volatile uint32_t *CPACR = (volatile uint32_t *)CPACR_Address;
	(*CPACR) = (*CPACR) | (0xF << 20);
}
