#ifndef OLED_H
#define OLED_H

#include <stdint.h>

void OLED_Init(void);
void OLED_DrawPixel(uint8_t pos_x, uint8_t pos_y);
void OLED_DrawColumn(uint8_t length, uint8_t pos_x);

#endif