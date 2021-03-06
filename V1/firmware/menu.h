/*
 * menu.h
 *
 * Created: November-01-18, 11:23:53 AM
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


#ifndef MENU_H_
#define MENU_H_

/*
         1    11
----5----0----56
A00:00:00 Alarm
01JAN2020 Trim
*/

#define SET_TIME_X		0
#define SET_TIME_Y		0
#define SET_LED_X			10
#define SET_LED_Y			0

#define SET_DATE_X		0
#define SET_DATE_Y		1

#define AL_TIME_X			2
#define AL_TIME_Y			0
#define AL_LENGTH_X		(AL_TIME_X+8)
#define AL_LENGTH_Y		0

#define ADC_X					4
#define ADC_Y					1
#define ADC_LENGTH		4

#define SET_TRIM_X		10
#define SET_TRIM_Y		1
#define TRIM_WIDTH		4

#define TRIMS_X				4
#define TRIMS_Y				0
#define TRIMV_X				5
#define TRIMV_Y				0
#define TRIMA_X				0
#define TRIMA_Y				1

#define DST_X					14
#define DST_Y					0

void Setup(void);
void Display_Trim(UI_Item_t *Item);

#endif
