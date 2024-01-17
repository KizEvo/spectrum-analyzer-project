#include <stdint.h>
#include "nvic.h"

typedef struct {
	volatile uint32_t ISER[8];
} NVIC_ISER_Register;

typedef struct {
	volatile uint32_t ICER[8];
} NVIC_ICER_Register;

typedef struct {
	volatile uint32_t ISPR[8];
} NVIC_ISPR_Register;

typedef struct {
	volatile uint32_t ICPR[8];
} NVIC_ICPR_Register;

typedef struct {
	volatile uint32_t IABR[8];
} NVIC_IABR_Register;

typedef struct {
	volatile uint32_t IPR[60];
} NVIC_IPR_Register;

typedef struct {
	volatile uint32_t STIR;
} NVIC_STIR_Register;

#define NVIC_BaseAddress 	(0xE000E100)
#define NVIC_ISER_Address	((NVIC_ISER_Register *)(NVIC_BaseAddress))
#define NVIC_ICER_Address	((NVIC_ICER_Register *)(NVIC_BaseAddress + 0x180))
#define NVIC_ISPR_Address	((NVIC_ISPR_Register *)(NVIC_BaseAddress + 0x200))
#define NVIC_ICPR_Address	((NVIC_ICPR_Register *)(NVIC_BaseAddress + 0x280))
#define NVIC_IABR_Address	((NVIC_IABR_Register *)(NVIC_BaseAddress + 0x300))
#define NVIC_IPR_Address	((NVIC_IPR_Register  *)(NVIC_BaseAddress + 0x400))

void NVIC_EnableIRQ(uint32_t position)
{
	NVIC_ISER_Register *NVIC = NVIC_ISER_Address;
	// Read previous enabled IRQ
	uint32_t prev_enabled_irq = NVIC->ISER[position >> 5];
	// Write the previous enabled IRQ with the new IRQ position
	NVIC->ISER[position >> 5] = prev_enabled_irq | (1 << (position - 32*(position >> 5)));
}

void NVIC_SetPriority(uint32_t position, uint32_t priority)
{
	NVIC_IPR_Register *NVIC = NVIC_IPR_Address;
	uint8_t ip_offset = position % 4;
	uint8_t idx_ip_number = position / 4;
	NVIC->IPR[idx_ip_number] = ((NVIC->IPR[idx_ip_number]) | ((priority << (8*ip_offset)) << 4));
}