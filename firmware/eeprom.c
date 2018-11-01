/*
 * eeprom.c
 *
 * Created: October-28-18, 5:29:44 PM
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

void cpy(uint8_t *d, uint8_t *s, uint8_t n)
{
	while(n--)
		*d++=*s++;
}

void Save_Prefs(alarm_t *Prefs)
{
	cpy((uint8_t *)EE_ADDR(EE_Alarm),(uint8_t *)Prefs,sizeof(alarm_t));
}

void Load_Prefs(alarm_t *Prefs)
{
	cpy((uint8_t *)Prefs,(uint8_t *)EE_ADDR(EE_Alarm),sizeof(alarm_t));
}
