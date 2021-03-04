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

rtc_t Set;
uint8_t Alarm1_min,Alarm1_sec,Alarm2_hour,Alarm2_min;
uint8_t TimeModified,DateModified,DST_Modified,TimerModified,Trim_Polarity,DST_Enable;
uint16_t Sensivity,Trim_Value;

// Custom data
void Display_Trim(UI_Item_t *Item)
{
	// handle sign
	if(Item->Width==1)
		LCD_Ch(*((uint8_t*)Item->Value)?'-':'+');
	else
		//	print raw value
		Print_uint(*((uint16_t*)Item->Value),TRIM_WIDTH,RightJustify);
}

const UI_Item_t LED_Setting[]=
{
	{&Prefs.al_hour,&TimerModified,AL_TIME_X+0,AL_TIME_Y,3,TIME_HR_MIN,TIME_HR_MAX,D_12H},
	{&Prefs.al_min,&TimerModified,AL_TIME_X+4,AL_TIME_Y,2,0,59,D_U8Z},
	{&Alarm1_min,&TimerModified,AL_LENGTH_X,AL_LENGTH_Y,2,0,99,D_U8Z},
	{&Alarm1_sec,&TimerModified,AL_LENGTH_X+3,AL_LENGTH_Y,2,0,59,D_U8Z},
	{&Prefs.adc_threshold,&TimerModified,ADC_X,ADC_Y,ADC_LENGTH,0,1023,D_U16},
	{&Alarm2_hour,&TimerModified,AL_LENGTH_X,AL_LENGTH_Y+1,2,0,17,D_U8Z},	// limited by 16-bit int
	{&Alarm2_min,&TimerModified,AL_LENGTH_X+3,AL_LENGTH_Y+1,2,0,59,D_U8Z},
};

const UI_Menu_t LED_Menu=
{//0---4----9----4-
	"@    :   +  m  s"
	"ADC>     +  h  m"
	,LED_Setting,sizeof(LED_Setting)/sizeof(UI_Item_t)
};

const UI_Item_t TRIM_Setting[]=
{
	{&Trim_Polarity,(uint8_t*)Display_Trim,TRIMS_X,TRIMS_Y,1,0,1,D_CustomData8},
	{&Trim_Value,(uint8_t*)Display_Trim,TRIMV_X,TRIMV_Y,4,0,9999,D_CustomData16},
	{&DST_Enable,&DST_Modified,DST_X,DST_Y,1,0,1,D_U8}
};

const UI_Menu_t TrimMenu[]=
{//0---4----9----4-
	"NCO       DST",
	TRIM_Setting,sizeof(TRIM_Setting)/sizeof(UI_Item_t)
};

const UI_Item_t RTC_Setting[]=
{
	{&Set.hour,&TimeModified,SET_TIME_X+0,SET_TIME_Y,3,TIME_HR_MIN,TIME_HR_MAX,D_12H},
	{&Set.min,&TimeModified,SET_TIME_X+4,SET_TIME_Y,2,0,59,D_U8Z},
	{&Set.sec,&TimeModified,SET_TIME_X+7,SET_TIME_Y,2,0,59,D_U8Z},
	{&Set,&TimeModified,SET_TIME_X+0,SET_TIME_Y,9,0,0,D_SetTime},
	
	{&Set.day,&DateModified,SET_DATE_X+0,SET_DATE_Y,2,1,31,D_U8},
	{&Set.month,&DateModified,SET_DATE_X+2,SET_DATE_Y,MONTH_WIDTH,1,12,D_Month},
	{&Set.year,&DateModified,SET_DATE_X+5,SET_DATE_Y,4,YEAR_START,9999,D_U16},	
	
	{"Alarm",(uint8_t*)&LED_Menu,SET_LED_X,SET_LED_Y,5,0,0,D_Menu},
	{"Misc",(uint8_t*)&TrimMenu,SET_TRIM_X,SET_TRIM_Y,4,0,0,D_Menu},
};

const UI_Menu_t SettingMenu=
{//0---4----9----4-
	"   :  :",
	RTC_Setting,sizeof(RTC_Setting)/sizeof(UI_Item_t),
};

void Setup(void)
{
	uint16_t countdown;
	int16_t new_trim;
	
	// snapshot of time
	Set = time;
	Set.sec = 0;
	
	TimeModified = DateModified = TimerModified = DST_Modified = 0;
	
	Load_Prefs(&Prefs);
	sec2ms(Prefs.al_length1,&Alarm1_min,&Alarm1_sec);
	sec2ms(Prefs.al_length2/60,&Alarm2_hour,&Alarm2_min);

	if(Prefs.DDS_Adj >= 0)
	{
		Trim_Polarity = 0;
		Trim_Value = Prefs.DDS_Adj;
	}
	else
	{
		Trim_Polarity = 1;
		Trim_Value = -Prefs.DDS_Adj;		
	}
	
	DST_Enable =	time.DST_Enable;

	UI_Menu(&SettingMenu);
	
	new_trim = Trim_Polarity?-Trim_Value:Trim_Value;
	
	if(new_trim!=Prefs.DDS_Adj)
		RTC_SetNCO(new_trim);
	
	if(TimeModified)
	{
		RTC_SetTime(Set.hour,Set.min,Set.sec);
		AdjustAlarm();
	}
	
	if(DateModified || DST_Modified)
	{
		time.DST_Enable = DST_Enable;
		RTC_SetDate(Set.day,Set.month,Set.year);
	}
	
	if(TimerModified||(new_trim!=Prefs.DDS_Adj)||DST_Modified)
	{	
		Prefs.al_length1 = Alarm1_min*60U+Alarm1_sec;
		Prefs.al_length2 = (Alarm2_hour*60U+Alarm2_min)*60U;

		Prefs.DDS_Adj = new_trim;
		Prefs.TimeOpt = DST_Enable;
		Save_Prefs(&Prefs);
	}
}
