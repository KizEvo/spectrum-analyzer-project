#include <stdint.h>
#include "iic.h"

typedef struct {
	volatile uint32_t CR1, CR2, OAR1, OAR2, DR, SR1, SR2, CCR, TRISE, FLTR;
} IIC_Register;

#define IIC_Address	((IIC_Register *)(0x40005400))

void IIC_Init(void)
{
	IIC_Register *I2C1 = IIC_Address;
	// Config I2C to work in Master mode
	// Program the peripheral input clock same as APB1, 30 MHz
	I2C1->CR2 |= (30 << 0);
	
	// I2C fast mode
	I2C1->CCR |= (1 << 15);
	
	// DUTY = 0, T_total = T_high + T_low = 3 * CCR * T_PCLK;
	// T_total = 400 KHz, T_PCLK = 1/30e6 -> CCR = 25d;
	I2C1->CCR |= ((25 & 0xFFF) << 0);
	
	// Config maximum allowed SCL rise time in Fast mode, 300ns (based on SSD1306)
	// 300ns / T_PCLK = 9, increment by 1
	I2C1->TRISE = ((9 + 1) & 0x3F);
	
	// Enable I2C, ready to start transmission
	I2C1->CR1 |= (1 << 0);
}

void IIC_TransmitByte(uint8_t slave_addr, uint8_t data)
{
	IIC_Register *I2C1 = IIC_Address;
	// Wait until BUSY bit is clear, if it's set there is ongoing communication on the SDA
	while((I2C1->SR2 & 1 << 1) != 0);
	// Send start condition
	I2C1->CR1 |= (1 << 8);
	// Wait for start condition to be sent
	while(!((I2C1->SR1 & 1 << 0) != 0));
	// Transmit address frame, LSB is reset to enter write mode
	I2C1->DR = slave_addr & 0xFE;
	// Wait for address frame to be sent, set ADDR
	while(!((I2C1->SR1 & 1 << 1) != 0));
	// Read SR2 to clear ADDR flag
	uint8_t is_data_received = (I2C1->SR2 & 1 << 2) != 0 ? 0 : 1;
	// Send data byte
	I2C1->DR = data & 0xFF;
	// If either TxE or BTF set, it means one byte transmission is finished
	while(!((I2C1->SR1 & 1 << 7) != 0) || !((I2C1->SR1 & 1 << 2) != 0));
	// Stop generation
	I2C1->CR1 |= (1 << 9);
}

void IIC_TransmitMultiBytes(uint8_t slave_addr, uint8_t *datas, uint16_t data_length)
{
	IIC_Register *I2C1 = IIC_Address;
	// Wait until BUSY bit is clear, if it's set there is ongoing communication on the SDA
	while((I2C1->SR2 & 1 << 1) != 0);
	// Send start condition
	I2C1->CR1 |= (1 << 8);
	// Wait for start condition to be sent
	while(!((I2C1->SR1 & 1 << 0) != 0));
	// Transmit address frame, LSB is reset to enter write mode
	I2C1->DR = slave_addr & 0xFE;
	// Wait for address frame to be sent, set ADDR
	while(!((I2C1->SR1 & 1 << 1) != 0));
	// Read SR2 to clear ADDR flag
	uint8_t is_data_received = (I2C1->SR2 & 1 << 2) != 0 ? 0 : 1;
	// Send data bytes
	for(uint16_t i = 0; i < data_length; i++)
	{
		I2C1->DR = datas[i] & 0xFF;
		// If either TxE or BTF set, it means one byte transmission is finished
		while(!((I2C1->SR1 & 1 << 7) != 0) || !((I2C1->SR1 & 1 << 2) != 0));
	}
	// Stop generation
	I2C1->CR1 |= (1 << 9);
}