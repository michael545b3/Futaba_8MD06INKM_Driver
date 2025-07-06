/*
 * Futaba_8MD06INKM.c
 *
 *  Created on: Feb 16, 2023
 *      Author: Michael
 *		E-Mail: michael.adam4@proton.me
 *
 *      IMPORTANT!!!
 *      SPI Settings:
 *      First Bit = LSB Bit
 *      Freq. = <0.5MHz
 *      CPOL = HIGH
 *      CPHA = 2nd EDGE
 */

// Includes
#include "main.h"
#include <stdint.h>
#include <string.h>

// Defines
// Didplay
#define VFD_DIGITS         		8	// 8 or 16 Digits?

// GPIO Pins
#define VFD_CS_PIN				CS_Pin
#define VFD_RST_PIN				RST_Pin
#define VFD_EN_PIN				EN_Pin

// GPIO Ports
#define VFD_CS_PORT				CS_GPIO_Port
#define VFD_RST_PORT			RST_GPIO_Port
#define VFD_EN_PORT				EN_GPIO_Port

// Registers
#define VFD_TIMING				0xE0
#define	VFD_ST_BY_ACTIVE		0xED
#define	VFD_ST_BY_DEACTIVE		0xEC
#define VFD_DIMM				0xE4
#define VFD_LIGHT_ON			0xE8
#define VFD_LIGHT_OFF			0xE9
#define VFD_DCRAM				0x20

// SPI Handler
extern SPI_HandleTypeDef hspi1;
#define VFD_SPI_PORT hspi1

// Function prototypes
void VFD_Select(void);
void VFD_Un_Select(void);

// Functions
void VFD_Light_Control(uint8_t state)
{
	uint8_t buf[1];

	VFD_Select();
	if (state)
	{
		buf[0] = VFD_LIGHT_ON;
	}
	else
	{
		buf[0] = VFD_LIGHT_OFF;
	}
	HAL_SPI_Transmit(&VFD_SPI_PORT, buf, 1, HAL_MAX_DELAY);
	VFD_Un_Select();
}

void VFD_Stby_Control(uint8_t state)
{
	uint8_t buf[1];

	VFD_Select();
	if (state)
	{
		buf[0] = VFD_ST_BY_ACTIVE;
	}
	else
	{
		buf[0] = VFD_ST_BY_DEACTIVE;
	}
	HAL_SPI_Transmit(&VFD_SPI_PORT, buf, 1, HAL_MAX_DELAY);
	VFD_Un_Select();
}

void VFD_Brightness(uint8_t brightness)
{
	uint8_t buf[2];

	VFD_Select();
	buf[0] = VFD_DIMM;
	buf[1] = brightness;
	HAL_SPI_Transmit(&VFD_SPI_PORT, buf, 2, HAL_MAX_DELAY);
	VFD_Un_Select();
}

void VFD_Clear(void)
{
	uint8_t buf[VFD_DIGITS];

	VFD_Select();
	for (uint8_t i = 0; i < VFD_DIGITS; i++)
	{
		buf[i] = VFD_DCRAM;
	}
	HAL_SPI_Transmit(&VFD_SPI_PORT, buf, VFD_DIGITS, HAL_MAX_DELAY);
	VFD_Un_Select();
}

void VFD_Write_ASCII(uint8_t digit, uint8_t chr)
{
	uint8_t buf[2];

	VFD_Select();

	buf[0] = (VFD_DCRAM + digit);
	buf[1] = (chr);

	HAL_SPI_Transmit(&VFD_SPI_PORT, buf, 2, HAL_MAX_DELAY);
	VFD_Un_Select();
}

void VFD_Write_Str(uint8_t start_pos, uint8_t *s)
{
	while (*s)
	{
		VFD_Write_ASCII(start_pos++, *s++);
	}
}

void VFD_init(void)
{
	/* Setting Digits ****************************************************/
	uint8_t buf[2];

	HAL_GPIO_WritePin(VFD_EN_PORT, VFD_EN_PIN, SET);
	HAL_Delay(40);
	HAL_GPIO_WritePin(VFD_RST_PORT, VFD_RST_PIN, RESET);
	HAL_Delay(40);
	HAL_GPIO_WritePin(VFD_RST_PORT, VFD_RST_PIN, SET);
	HAL_Delay(40);

	VFD_Select();

	buf[0] = VFD_TIMING;            //first byte address for setting DIGITS
	if (VFD_DIGITS == 8)
	{
		buf[1] = 0x07;            //8 DIGITS equals value 7 (hex 0x07)
	}
	if (VFD_DIGITS == 16)
	{
		buf[1] = 0x0F;            //16 DIGITS equals (0x0f)
	}
	HAL_SPI_Transmit(&VFD_SPI_PORT, buf, 2, HAL_MAX_DELAY);
	VFD_Un_Select();

	/* *******************************************************************/
	VFD_Brightness(50);			//setting brightness to initial value
	VFD_Stby_Control(0);		//switch standby off
	VFD_Clear();				//clear the screen
	VFD_Light_Control(1);		//activate the screen
}

void VFD_Select(void)
{
	HAL_GPIO_WritePin(VFD_CS_PORT, VFD_CS_PIN, RESET);
}

void VFD_Un_Select(void)
{
	HAL_GPIO_WritePin(VFD_CS_PORT, VFD_CS_PIN, SET);
}
