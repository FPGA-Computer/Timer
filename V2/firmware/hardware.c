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

uint8_t power=1, debugger;

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
	debugger = RST->SR & RST_SR_SWIMF;
	
	// GPIO for outputs
	MOTOR_OFF();
	LED_OFF();
	
	PERIPH_PORT->DDR |= MOTOR_EN|LED_EN;
	PERIPH_PORT->CR1 |= MOTOR_EN|LED_EN;
	
	// Wait for power to be stabilized
	Delay(100);
	
	// Init peripherals
	LCD_Init();
	ADC_Init();
	Time_Init();
	Servo_Init();

	FLASH->DUKR = DATA_KEY1;
	FLASH->DUKR = DATA_KEY2;
	
	// Turn on interrupts
	rim();
}

void PowerDetect(void)
{
	uint8_t detect;

	// LCD data is internally pulled up to +5V
	detect=(LCD_DATA_PORT->IDR & LCD_DATA)!=0;

	if(power!=detect)		// power status change
	{
		if(detect)				// power returns
		{
			CLK->CKDIVR = CPU_DIV;
			// power up all peripherals
			CLK->PCKENR1 = CLK_PCKENR1_RESET_VALUE;
			CLK->PCKENR2 = CLK_PCKENR2_RESET_VALUE;
			// Wait for power to be stabilized
			Delay(1000);			
			LCD_Init();
			Servo_Init();			
		}
		else							// power outage
		{
			LCD_Off();
			Servo_Off();
			
			CLK->CKDIVR = CPU_SLOW;
			// power down all peripherals except TIM1 needed for RTC
			CLK->PCKENR1 = CLK_PCKENR1_TIM1;
			CLK->PCKENR2 = 0;
		}
		power = detect;
	}
}

// use DELAY_ADJ() for rough value in us

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
		TIM2->CCR1H = SERVO_ON >>8;
		TIM2->CCR1L = SERVO_ON & 0xff;
	}
	else
	{
		TIM2->CCR1H = SERVO_OFF >>8;
		TIM2->CCR1L = SERVO_OFF & 0xff;	
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
	TIM2->CCMR1 = (0x06 <<4)|TIM2_CCMR_OCxPE;
	// Capture pin output enable
	TIM2->CCER1 |= TIM2_CCER1_CC1E;

	Servo_State(0);
}

void Servo_Off(void)
{
	TIM2->CCER1 &= ~TIM2_CCER1_CC1E;
	TIM2->CR1 = 0;
	TIM2->IER = 0;
}
