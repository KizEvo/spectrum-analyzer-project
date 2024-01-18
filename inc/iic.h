#ifndef IIC_H
#define IIC_H

#include <stdint.h>

void IIC_Init(void);
void IIC_TransmitByte(uint8_t slave_addr, uint8_t data);
void IIC_TransmitMultiBytes(uint8_t slave_addr, uint8_t *datas, uint16_t data_length);

#endif