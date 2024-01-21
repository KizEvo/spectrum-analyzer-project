#include <stdint.h>
#include "iic.h"
#include "oled.h"

#define OLED_ADDRESS					(0x78U)
#define OLED_COMMAND_FRAME				(0x00U)
#define OLED_DATA_FRAME					(0x40U)
#define OLED_BUFFER_LEN					(1024U + 1U) // 1 extra byte for control byte
#define OLED_WRITECOMMAND(command)		OLED_I2C_Write((OLED_ADDRESS), (OLED_COMMAND_FRAME), (command))
#define OLED_WRITEDATA(data)			OLED_I2C_Write((OLED_ADDRESS), (OLED_DATA_FRAME), (data))

#define CHECK_RANGE(value, min, max)	((value < min) ? min : (value > max) ? max : value)							

uint8_t OLED_SCREEN_BUFFER[1024 + 1]; 

static void OLED_I2C_MultiWrite(uint8_t slave_addr, uint8_t control_byte, uint8_t *data_frame, uint16_t data_length)
{
	data_frame[0] = control_byte;
	IIC_TransmitMultiBytes(slave_addr, data_frame, data_length);
}

static void OLED_I2C_Write(uint8_t slave_addr, uint8_t control_byte, uint8_t data_byte)
{
	uint8_t data_frame[2] = {control_byte, data_byte};
	IIC_TransmitMultiBytes(slave_addr, data_frame, 2);
}

static uint8_t get_bit_1(short length)
{
	uint8_t result = 0;
	length -= 1;
	while(length >= 0)
	{
		result |= (1 << length);
		length -= 1;
	}
	return result;
}

void OLED_UpdateScreen(void)
{
	OLED_I2C_MultiWrite(OLED_ADDRESS, OLED_DATA_FRAME, OLED_SCREEN_BUFFER, 128 * 8 + 1);
}

void OLED_DrawPixel(uint8_t pos_x, uint8_t pos_y)
{
	pos_y = CHECK_RANGE(pos_y, 1, 64);
	pos_x = CHECK_RANGE(pos_x, 1, 128);
	
	uint8_t page_y = (uint8_t)(pos_y / 8);
	uint8_t pos_y_offset = pos_y - (page_y * 8);
	
	OLED_SCREEN_BUFFER[pos_x * page_y] = OLED_SCREEN_BUFFER[pos_x * page_y] | (1 << pos_y_offset);
}

void OLED_DrawColumn(uint8_t length, uint8_t pos_x)
{
	length = CHECK_RANGE(length, 1, 64);
	pos_x = CHECK_RANGE(pos_x, 1, 128);
	
	uint8_t page = 0;
	for(short i = (short)length; page < 8; i -= 8)
	{
		OLED_SCREEN_BUFFER[pos_x + page * 128] = (i >= 8) ? 0xFF : 
												 (i < 0)  ? 0x00 : get_bit_1(i);
		page += 1;
	}
}

void OLED_Init(void)
{
	OLED_WRITECOMMAND(0xAE); // display off
	
	/*=== Addressing Setting ===*/
	OLED_WRITECOMMAND(0x20); // Set Memory Addressing Mode   
	OLED_WRITECOMMAND(0x10); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode; 10b,Page Addressing Mode (RESET); 11b,Invalid
	
	OLED_WRITECOMMAND(0xB0); // Set Page Start Address for Page Addressing Mode,0-7

	OLED_WRITECOMMAND(0x00); // set low column address

	OLED_WRITECOMMAND(0x10); // set high column address
	/*=== Addressing Setting ===*/
	
	/*=== Hardware Configuration ===*/
	OLED_WRITECOMMAND(0x40); // set start line address
	
	OLED_WRITECOMMAND(0xA1); // set column address 127 is mapped to SEG0 
	
	OLED_WRITECOMMAND(0xA8); // set multiplex ratio
	OLED_WRITECOMMAND(0x3F); // 63d => 64 multiplex
	
	OLED_WRITECOMMAND(0xC0); // Set COM Output Scan Direction - COM0 -> COM[N-1] where N is the multiplex ratio
	
	OLED_WRITECOMMAND(0xD3); // set display offset
	OLED_WRITECOMMAND(0x00); // no offset
	
	OLED_WRITECOMMAND(0xDA); // set com pins hardware configuration
	OLED_WRITECOMMAND(0x12); // A[4]=1b(RESET), Alternative COM pin configuration; A[5]=0b(RESET), Disable COM Left/Right remap
	/*=== Hardware Configuration ===*/
	
	/*=== Fundamental Command ===*/
	OLED_WRITECOMMAND(0x81); // set contrast control register
	OLED_WRITECOMMAND(0xFF); // max contrast
	
	OLED_WRITECOMMAND(0xA6); // set normal display
	OLED_WRITECOMMAND(0xA4); // normal display on; output follows RAM content
	/*=== Fundamental Command ===*/
	
	/*=== Timing & Driving Scheme Setting ===*/
	OLED_WRITECOMMAND(0xD5); // set display clock divide ratio/oscillator frequency
	OLED_WRITECOMMAND(0xF0); // set divide ratio, A[3:0] denominator, 0x0 = 1; A[7:4] numerator, 0xF = max frequency range
	
	OLED_WRITECOMMAND(0xD9); // set pre-charge period
	OLED_WRITECOMMAND(0x22); // set value same as datasheet

	OLED_WRITECOMMAND(0xDB); // set Vcomh deselect level
	OLED_WRITECOMMAND(0x20); // 0x20, 0.77*Vcc, same as datasheet
	/*=== Timing & Driving Scheme Setting ===*/
	
	/*=== Charge Bump Setting Sequence ===*/
	OLED_WRITECOMMAND(0x8D); // set DC-DC enable
	OLED_WRITECOMMAND(0x14); //
	OLED_WRITECOMMAND(0xAF); // turn display on
	/*=== Charge Bump Setting Sequence ===*/
	
	OLED_WRITECOMMAND(0x2E); // Deactivate scroll, so we can access RAM and change scroll setup parameters. /pg.46
	
	// Clear garbage data in RAM after deactivating scroll
	for(uint8_t i = 1; i <= 128; i++)
	{
		OLED_DrawColumn(0, i);
	}
	OLED_UpdateScreen();
}