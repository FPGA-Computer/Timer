/*
 * key.c
 * October-24-18, 4:39:27 PM
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

key_t key;

void Key_Task(void)
{
	uint8_t new_code;
	
	new_code = (~KEY_PORT->IDR & KEY_MASK);

  if(new_code==(key.code & KEY_MASK))
	{
		if(new_code)
		{
			if(key.count == KEY_CNT_MAX)
			{	
				if(!(new_code&KEY_MENU))						// no auto-repeat for menu key
					key.count = KEY_RELOAD;
				
				key.code |= KEY_LONG|KEY_FLAG;
			}
			else
				key.count++;
		}
	}
	else
	{
		if(!new_code&&key.code&&(key.count>KEY_THRESHOLD)&&!(key.code&(KEY_REPORTED|KEY_LONG)))
			key.code|=KEY_FLAG;
		else
		{
			key.code = new_code;
			key.count = 0;
		}
	}
}

uint8_t Key_Get(void)
{ 
	if(time.Tick)
	{
		Key_Task();
		time.Tick =0;
	}

  if(key.code & KEY_FLAG)
	{
		key.code = (key.code & ~KEY_FLAG)|KEY_REPORTED;
		return(key.code & (KEY_MASK|KEY_LONG));
	}
	return(0);
}

uint8_t Key_Purge(void)
{
	key.count = 0;
}
