/*
 * main.c
 *
 * Created: October-19-18, 2:38:15 PM
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
#include "main.h"

void DisplayTask(void)
{
	LCD_Moveto(CLOCK_X,CLOCK_Y);
	Print_Time(&time,DISPLAY_SEC);

  LCD_Moveto(STATUS_X,STATUS_Y);
	LCD_Puts((PERIPH_PORT->ODR & LED_EN)?"LED ":"    ");
	LCD_Puts((PERIPH_PORT->ODR & MOTOR_EN)?"    ":"Pump");
	LCD_Moveto(9,1);
	LCD_Puts("ADC");
	Print_uint(ADC.Result[ADC_Sense],4,LeftJustify);
}

void main(void)
{	
	// load Alarm
	Load_Prefs(&Alarm);
	Init_Hardware();

	while(1)
	{
		if(Key_Get() == (KEY_MENU|KEY_LONG))
		  Setup();
		
		if(time_flag&TIME_SEC_FLAG)
		{
			time_flag &= ~TIME_SEC_FLAG;
			DisplayTask();
		}
	}
}
