#ifndef TIM_H
#define TIM_H

#include <stdint.h>

typedef struct {
	uint16_t ARR;			// Count: 0 -> ARR or ARR -> 0, depends on DIR bit, the time interval is fCK_PSC / (prescaler + 1), so counting to ARR means time interval * ARR = total time before overflow/underflow
	uint16_t PSC;			// Prescaler
	uint8_t ARPE;			// Auto reload preload enable
	uint8_t DIR;			// Count direction up/down
	uint8_t URS;			// Update request source
	uint8_t UIE;			// Update interrupt enable
} TIM_CounterTypeDef;

void TIM_CounterInit(TIM_CounterTypeDef *tim);

#endif

