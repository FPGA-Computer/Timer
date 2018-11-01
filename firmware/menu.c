/*
 * menu.c
 *
 * Created: November-01-18, 11:22:08 AM
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


static time_hms_t TimeSetting;
static uint8_t Alarm1_min, Alarm1_sec, Alarm2_hour, Alarm2_min, Trim_p, Trim_n;
static uint8_t TimeModified,TimerModified;
static uint16_t Sensivity;

const UI_Item_t LED_Setting[]=
{
	{&Alarm.al_hour,&TimerModified,AL_TIME_X+0,AL_TIME_Y,3,TIME_HR_MIN,TIME_HR_MAX,D_12H},
	{&Alarm.al_min,&TimerModified,AL_TIME_X+4,AL_TIME_Y,2,0,59,D_U8Z},
	{&Alarm1_min,&TimerModified,AL_LENGTH_X,AL_LENGTH_Y,2,0,99,D_U8Z},
	{&Alarm1_sec,&TimerModified,AL_LENGTH_X+3,AL_LENGTH_Y,2,0,59,D_U8Z},
	{&Alarm.adc_threshold,&TimerModified,ADC_X,ADC_Y,ADC_LENGTH,0,1023,D_U16},
	{&Alarm2_hour,&TimerModified,AL_LENGTH_X,AL_LENGTH_Y+1,2,0,17,D_U8Z},	// limited by 16-bit int
	{&Alarm2_min,&TimerModified,AL_LENGTH_X+3,AL_LENGTH_Y+1,2,0,59,D_U8Z},
};

const UI_Menu_t LED_Menu=
{//0---4----9----4-
	"A    :   +  m  s"
	"ADC>     +  h  m"
	,LED_Setting,sizeof(LED_Setting)/sizeof(UI_Item_t)
};

const UI_Item_t TRIM_Setting[]=
{
	{&Trim_p,&TimerModified,TRIMP_X,TRIMP_Y,3,0,127,D_U8},
	{&Trim_n,&TimerModified,TRIMN_X,TRIMN_Y,3,0,127,D_U8}
};

const UI_Menu_t TrimMenu[]=
{//0---4----9----4-
	"Trim +    /-    " 
	"* 1.44 sec/day",
	TRIM_Setting,sizeof(TRIM_Setting)/sizeof(UI_Item_t)
};

const UI_Item_t RTC_Setting[]=
{
	{&TimeSetting.hour,&TimeModified,SET_TIME_X+0,SET_TIME_Y,3,TIME_HR_MIN,TIME_HR_MAX,D_12H},
	{&TimeSetting.min,&TimeModified,SET_TIME_X+4,SET_TIME_Y,2,0,59,D_U8Z},
	{&TimeSetting.sec,&TimeModified,SET_TIME_X+7,SET_TIME_Y,2,0,59,D_U8Z},
	{"Alarm",(uint8_t*)&LED_Menu,SET_LED_X,SET_LED_Y,5,0,0,D_Menu},
	{"Trim",(uint8_t*)&TrimMenu,SET_TRIM_X,SET_TRIM_Y,5,0,0,D_Menu},
};

const UI_Menu_t SettingMenu=
{//0---4----9----4-
	"   :  :",
	RTC_Setting,sizeof(RTC_Setting)/sizeof(UI_Item_t),
};

void Setup(void)
{
	uint16_t countdown;
	
	// This is smaller than a struct copy
	TimeSetting.hour = time.hour;
	TimeSetting.min = time.min;
	TimeSetting.sec = time.sec;
	TimeModified = TimerModified = 0;
	
	Load_Prefs(&Alarm);
	sec2ms(Alarm.al_length1,&Alarm1_min,&Alarm1_sec);
	sec2ms(Alarm.al_length2/60,&Alarm2_hour,&Alarm2_min);

	Trim_n = (Alarm.clock_trim > 0)?Alarm.clock_trim:0;
	Trim_p = (Alarm.clock_trim > 0)?0:Alarm.clock_trim;

	UI_Menu(&SettingMenu);
	
	if(TimeModified)
	{	
		sim();
		time.hour =	TimeSetting.hour;
		time.min =	TimeSetting.min;	
		time.sec =	TimeSetting.sec;
		rim();
	}
	
	if(TimerModified)
	{	
		Alarm.al_length1=Alarm1_min*60+Alarm1_sec;
		Alarm.al_length2=(Alarm2_hour*60+Alarm2_min)*60;
		
	Alarm.clock_trim = Trim_n-Trim_p;
	Timer_Reload();
	
	Save_Prefs(&Alarm);
	}
}
