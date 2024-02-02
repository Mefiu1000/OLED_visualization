/*
 * SSD1306_OLED.c
 *
 *  Created on: Feb 2, 2024
 *      Author: akun1
 */

#include "main.h"
#include "string.h"

#include "My_library/SSD1306_OLED.h"

I2C_HandleTypeDef  *oled_i2c;

static uint8_t buffer[SSD1306_BUFFER_SIZE]; //OLED memory /8 cuz every pixel is 1 bit

/** SSD1306_Command
  * @brief  Issue single command to SSD1306
  * @param  command The command character to send to the display.
  * @retval None
  */
void SSD1306_Command(uint8_t command)
{
	HAL_I2C_Mem_Write(oled_i2c, SSD1303_ADDRESS << 1, 0x00, 1, &command, 1, SSD1306_TIMEOUT);
}

/** SSD1306_Data
  * @brief  Issue single data to SSD1306
  * @param  data The data character to send to the display.
  * @retval None
  */
void SSD1306_Data(uint8_t *data, uint16_t size)
{
	HAL_I2C_Mem_Write(oled_i2c, SSD1303_ADDRESS << 1, 0x040, 1, data, size, SSD1306_TIMEOUT);
}

void SSD1306_Clear(uint8_t color)
{
	switch(color)
	{
	case SSD1306_BLACK:
		memset(buffer, 0x00, SSD1306_BUFFER_SIZE);
		break;
	case SSD1306_WHITE:
		memset(buffer, 0xFF, SSD1306_BUFFER_SIZE);
		break;
	}
}

void SSD1306_Display(void)
{
	SSD1306_Command(SSD1306_PAGEADDR);
	SSD1306_Command(0); 					// Page start address
	SSD1306_Command(0xFF); 					// Page end (not really, but works here)
	SSD1306_Command(SSD1306_COLUMNADDR); 	// Column start address
	SSD1306_Command(0);
	SSD1306_Command(SSD1306_LCDWIDTH - 1); 	// Column end address

	SSD1306_Data(buffer,SSD1306_BUFFER_SIZE);
}

void SSD1306_Init(I2C_HandleTypeDef *i2c)
{
	oled_i2c = i2c;

	SSD1306_Command(SSD1306_DISPLAYOFF);
	SSD1306_Command(SSD1306_SETDISPLAYCLOCKDIV);
	SSD1306_Command(0x80);
	SSD1306_Command(SSD1306_SETMULTIPLEX);

	SSD1306_Command(SSD1306_LCDHEIGHT - 1);

	SSD1306_Command(SSD1306_SETDISPLAYOFFSET);
	SSD1306_Command(0x00);
	SSD1306_Command(SSD1306_SETSTARTLINE | 0x00);

	SSD1306_Command(SSD1306_CHARGEPUMP);
	SSD1306_Command(0x14); //internal vcc from pump

	SSD1306_Command(SSD1306_MEMORYMODE);
	SSD1306_Command(0x00);
	SSD1306_Command(SSD1306_SEGREMAP | 0x1);
	SSD1306_Command(SSD1306_COMSCANDEC);

	SSD1306_Command(SSD1306_SETCOMPINS);
	SSD1306_Command(0x12);
	SSD1306_Command(SSD1306_SETCONTRAST);
	SSD1306_Command(0xFF);

	SSD1306_Command(SSD1306_SETPRECHARGE);
	SSD1306_Command(0xF1);

	SSD1306_Command(SSD1306_SETVCOMDETECT);
	SSD1306_Command(0x40);
	SSD1306_Command(SSD1306_DISPLAYALLON_RESUME);
	SSD1306_Command(SSD1306_NORMALDISPLAY);
	SSD1306_Command(SSD1306_DEACTIVATE_SCROLL);
	SSD1306_Command(SSD1306_DISPLAYON); // Main screen turn on

}
