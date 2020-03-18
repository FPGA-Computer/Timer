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

#ifdef AUX_SERVO
	Servo_Init();
#endif

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

#ifdef AUX_SERVO

uint8_t Servo_CountDown, ServoFlag;

@far @interrupt void TIM2_IRQ(void)
{
	TIM2->SR1 = ~TIM2_SR1_UIF;								// Clear update IRQ
	
	if (Servo_CountDown >1)
		Servo_CountDown--;
	else if	(Servo_CountDown)
	{
		Servo_CountDown = 0;
		// turn off TIM2
		TIM2->CR1 = 0;	
	}
}

void Servo_State(uint8_t State)
{
	ServoFlag = State;
	TIM2->CR1 = TIM2_CR1_CEN;
	
	if(State)
	{
		TIM2->CCR2H = SERVO_ON >>8;
		TIM2->CCR2L = SERVO_ON & 0xff;
	}
	else
	{
		TIM2->CCR2H = SERVO_OFF >>8;
		TIM2->CCR2L = SERVO_OFF & 0xff;	
	}

	Servo_CountDown = SERVO_SHUTDOWN;
}

void Servo_Init(void)
{
	TIM2->PSCR = TIM2_PSC;
	TIM2->ARRH = (TIM2_RELOAD-1) >> 8;
	TIM2->ARRL = (TIM2_RELOAD-1) & 0xff;
	
	// Update interrupt
	TIM2->IER = TIM2_IER_UIE;
	
	// -- Servo -----
	// Ch2: PWM mode 1, preload enable
	TIM2->CCMR2 = (0x06 <<4)|TIM2_CCMR_OCxPE;
	// Capture pin output enable
	TIM2->CCER1 |= TIM2_CCER1_CC2E;

	Servo_State(0);
}

#endif
