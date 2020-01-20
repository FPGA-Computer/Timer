/*
 * hardware.c
 *
 * Created: October-19-18, 12:25:42 PM
 *  Author: K. C. Lee
 * Copyright (c) 2017 by K. C. Lee
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.

 If not, see http://www.gnu.org/licenses/gpl-3.0.en.html 
 */ 

#include "hardware.h"

void Init_Hardware(void)
{
	// Switch clock enable
	CLK->SWCR |= CLK_SWCR_SWEN;
	// Select HSE
	CLK->SWR = 0xb4;
	// wait for switching
	while(CLK->SWCR & CLK_SWCR_SWBSY)
	  /* */ ;
	
	// Clk divider, CPU divider = 1
	CLK->CKDIVR = 0;
	
	// GPIO for LCD
	LCD_E_PORT->DDR = LCD_E_PORT->CR1 = LCD_E|LCD_DATA;
	LCD_RS_PORT->DDR = LCD_RS_PORT->CR1 = LCD_RS;
	LCD_RW_PORT->DDR = LCD_RW_PORT->CR1 = LCD_RW;
	
	// GPIO for outputs
	MOTOR_OFF();
	LED_OFF();
	
	PERIPH_PORT->DDR |= MOTOR_EN|LED_EN;
	PERIPH_PORT->CR1 |= MOTOR_EN|LED_EN;
	
	// Wait for power to be stabilized
	Delay(0);
	
	// Init peripherals
	LCD_Init();
	ADC_Init();
	Time_Init();	// Timer after ADC
	
	FLASH->DUKR = DATA_KEY1;
	FLASH->DUKR = DATA_KEY2;
	
	// Turn on interrupts
	rim();
}

// rough delay in us
void Delay(uint16_t Delay)
{	
	for(;Delay;Delay--)
	{
		_asm("nop");
		_asm("nop");
		_asm("nop");
		_asm("nop");
	}
}

