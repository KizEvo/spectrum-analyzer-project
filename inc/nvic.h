#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>

void NVIC_EnableIRQ(uint32_t position);
void NVIC_SetPriority(uint32_t position, uint32_t priority);

#endif