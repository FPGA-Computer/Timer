/*
 * time.c
 *
 * Created: October-23-18, 10:12:33 PM
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

volatile uint8_t time_flag,ticks;
volatile time_hms_t time;
volatile alarm_t Alarm;

void sec2ms(uint16_t time,uint8_t *min,uint8_t *sec)
{
	*min = time/60;	
	*sec = time-*min*60;
}

void Print_12Hr(uint8_t hour)
{
	if(hour<12)	
		LCD_Ch(CH_AM);
	else
	{
		LCD_Ch(CH_PM);
		hour-=12;
	}
	if(!hour)
		Print2d(12,0);
	else 
		Print2d(hour,0);
}

void Print_Time(time_hms_t *time,uint8_t display_opt)
{
	Print_12Hr(time->hour);

	LCD_Ch(TIME_SEPARATOR);
	Print2d(time->min,LeadingZero);
	
	if(display_opt & DISPLAY_SEC)
	{
		LCD_Ch(TIME_SEPARATOR);
		Print2d(time->sec,LeadingZero);
	}
}

void Timer_Reload(void)
{
	uint16_t ARR;
	
	ARR = TIM1_RELOAD+Alarm.clock_trim-1;
	
	// Autoreload
	TIM1->ARRH = ARR >> 8;
	TIM1->ARRL = ARR & 0xff;
}

void Time_Init(void)
{
	// Master mode TRGO = Update -> ADC
	TIM1->CR2 = (0x02)<<4;
	// Update Interrupt
	TIM1->IER = TIM1_IER_UIE;
	// prescaler
	TIM1->PSCRH = TIM1_PSCR_H;
	TIM1->PSCRL = TIM1_PSCR_L;
	// Preload, Counter enable
	TIM1->CR1 = TIM1_CR1_ARPE|TIM1_CR1_CEN;
}

void TimeTask(void)
{
	time.sec++;
	
	if(time.sec >= 60)
	{
		time.sec = 0;
		time.min++;
	
		if(time.min >= 60)
		{
			time.min = 0;			
			time.hour++;
		}
		if(time.hour > TIME_HR_MAX)
			time.hour = TIME_HR_MIN;
			
		if((Alarm.al_hour == time.hour)&&(Alarm.al_min == time.min))
		{
			time.al_length1 = Alarm.al_length1;
			time.al_length2 = Alarm.al_length2;			
			MOTOR_ON();
		}
	}
	
	if(time.al_length1)
	{
		if(time.al_length1==1)
			MOTOR_OFF();
			
		time.al_length1--;
	}
	
	if(time.al_length2)
	{
		if(time.al_length2==1)
			LED_OFF();
		else
		{
			// handle on/off
			if(ADC.Result[ADC_Sense]>Alarm.adc_threshold+ADC_HYSTERESIS)
				LED_ON();
			else if(ADC.Result[ADC_Sense]<Alarm.adc_threshold)
				LED_OFF();				
		}
		time.al_length2--;
	}
}

@far @interrupt void TIM1_IRQ(void)
{
	// clear UIF
	TIM1->SR1 &= ~TIM1_SR1_UIF;
	
	if(ticks)
	{
		if(ticks==TICKS_PER_SEC/2)
			time_flag |= TIME_HALF_SEC;
			
		ticks--;
	}
	else
	{
		ticks = TICKS_PER_SEC-1;
		TimeTask();

		time_flag |= TIME_SEC_FLAG|TIME_FULL_SEC;
	}
}
