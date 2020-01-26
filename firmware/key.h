/*
 * key.h
 *
 * Created: October-24-18, 4:39:46 PM
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
 
#ifndef KEY_H_
#define KEY_H_

#define ButtonTolerance	5

typedef struct
{
	volatile uint8_t code;
	volatile uint8_t count;
} key_t;

extern key_t key;
uint8_t ADC_Button(void);
void Key_Task(void);
uint8_t Key_Get(void);
uint8_t Key_Purge(void);

#define KEY_REPEAT_RATE		4
#define KEY_THRESHOLD			2
#define KEY_RELOAD				50
#define KEY_CNT_MAX				(KEY_RELOAD+TIM1_TICKS/2/KEY_REPEAT_RATE)

enum keys { KEY_MENU=0x04, KEY_DEC=0x02, KEY_INC=0x01, KEY_MASK=0x07, 
						KEY_LONG=0x20, KEY_REPORTED=0x40, KEY_FLAG=0x80 };

#endif
