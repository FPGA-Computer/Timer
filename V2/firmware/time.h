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

#define DST

typedef struct
{
	uint8_t	ticks;
	uint8_t	sec;
	uint8_t min;
	uint8_t hour;
	uint8_t	day;	
	uint8_t	month;
	uint16_t year;
	uint8_t dayofweek;
		
	// alarm
	uint16_t al_length1;
	uint16_t al_length2;

	uint8_t Tick:1;
	uint8_t LeapYear:1;
	uint8_t SecFlag:1;
	uint8_t HalfSec:1;
	uint8_t FullSec:1;
	
#ifdef DST	
	uint8_t DST_Enable:1;	
	uint8_t DST_Active:1;
	uint8_t DST_Start;
	uint8_t DST_Stop;
#endif

} rtc_t;

extern volatile rtc_t time;

typedef struct
{
	uint8_t al_hour;
	uint8_t al_min;
	uint16_t al_length1;
	uint16_t al_length2;
	uint16_t adc_threshold;
	int16_t DDS_Adj;
	uint8_t TimeOpt;
} prefs_t;

//extern volatile uint8_t time_flag,ticks;
extern volatile uint16_t countdown;
extern volatile prefs_t Prefs;

// North America DST
// https://www.timetemperature.com/northamerica/north_america_daylight_saving_time.shtml

enum DST_
{
	DST_Start_Month = 3,
	DST_Start_Week = 2,
	DST_End_Month = 11,
	DST_End_Week = 1,
	DST_ChangeDay	= 0,			// Sunday (not used in code)
	DST_ChangeTime = 2			// 2am
};

void Time_Init(void);
void RTC(void);
uint8_t MonthDays(uint8_t month, uint16_t year);
uint8_t DayWeek(uint8_t Day, uint8_t Month, uint16_t Year);
void RTC_AnnualUpdate(void);
void DST_Check(void);
void RTC_SetTime(uint8_t Hour, uint8_t Min, uint8_t Sec);
void RTC_SetDate(uint8_t Day, uint8_t Month, uint16_t Year);
void RTC_SetNCO(int16_t Adj);
void sec2ms(uint16_t time,uint8_t *min,uint8_t *sec);
void AdjustAlarm(void);

// DDS target frequency
#define DDS_TICKS_PER_SEC	20UL
#define TICKS_PER_SEC			10UL
#define DDS_TICKS					(TIM1_TICKS/DDS_TICKS_PER_SEC)

#define DDS_BITS					24UL
#define DDS_CARRY					(1UL << DDS_BITS)
#define DDS_MASK					(DDS_CARRY -1UL)

#define DDS_INC						(DDS_CARRY * TICKS_PER_SEC)/DDS_TICKS_PER_SEC

// TIM: 12MHz/(10*60000) = 40Hz, 25ms

#define TICK_MAX					2
#define TIM1_PRESCALER		60000UL
#define TIM1_TICKS				DDS_TICKS_PER_SEC
#define TIM1_RELOAD				(CPU_CLOCK/TIM1_PRESCALER/TIM1_TICKS/TICK_MAX)

#define TIME_SEPARATOR		':'
#define DOW_SEPARATOR			','
#define DATE_SEPARATOR		'-'

#define TIME_HR_MAX				23
#define TIME_HR_MIN				0

#define DISPLAY_SEC				0x01

//#define CENTURY					2000
#define YEAR_START				2021

#endif
