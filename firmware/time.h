/*
 * time.h
 *
 * Created: October-23-18, 10:13:20 PM
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

#ifndef TIME_H_
#define TIME_H_

typedef struct
{
	uint8_t hour;
	uint8_t min;
	uint8_t	sec;
	uint16_t al_length1;
	uint16_t al_length2;
} time_hms_t;

typedef struct
{
	uint8_t al_hour;
	uint8_t al_min;
	uint16_t al_length1;
	uint16_t al_length2;
	uint16_t adc_threshold;
	int8_t clock_trim;
} alarm_t;

extern volatile uint8_t time_flag,ticks;
extern volatile time_hms_t time;
extern volatile uint16_t countdown;
extern volatile alarm_t Alarm;

void Time_Init(void);
void Timer_Reload(void);
void sec2ms(uint16_t time,uint8_t *min,uint8_t *sec);
void Print_12Hr(uint8_t hour);
void Print_Time(time_hms_t *time,uint8_t display_sec);

// TIM: 6MHz/24000 = 250Hz, 4ms

#define TIM1_PRESCALER		1UL
#define TIM1_RELOAD				60000UL
#define TICKS_PER_SEC			(CPU_CLOCK/TIM1_PRESCALER/TIM1_RELOAD)
#define TICK_RES					144
#define TICK_RES_DIV			100

#define TIM1_PSCR_H				((TIM1_PRESCALER-1) >> 8)
#define TIM1_PSCR_L				((TIM1_PRESCALER-1) & 0xff)

#define TIME_SEPARATOR		':'

#define TIME_HR_MAX				23
#define TIME_HR_MIN				0

#define DISPLAY_SEC				0x01

enum TimeFlagBits
{
	TIME_SEC_FLAG = 0x01, TIME_HALF_SEC	=	0x02, TIME_FULL_SEC = 0x04
};

#endif
