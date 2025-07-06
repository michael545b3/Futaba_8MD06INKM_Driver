/*
 * Futaba_8MD06INKM.h
 *
 *  Created on: Feb 16, 2023
 *      Author: Michael
 *		E-Mail: michael.adam4@proton.me
 */

#ifndef INC_FUTABA_8MD06INKM_H_
#define INC_FUTABA_8MD06INKM_H_


// Includes
#include <stdint.h>

// Function prototypes
void VFD_Light_Control(uint8_t state);

void VFD_Stby_Control(uint8_t state);

void VFD_Brightness(uint8_t brightness);

void VFD_Clear(void);

void VFD_Write_ASCII(uint8_t digit, uint8_t chr);

void VFD_init(void);

void VFD_Write_Str(uint8_t start_pos, uint8_t *s);

void VFD_Select(void);

void VFD_Un_Select(void);

#endif /* INC_FUTABA_8MD06INKM_H_ */
