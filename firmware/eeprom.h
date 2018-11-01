/*
 * eeprom.h
 *
 * Created: October-28-18, 5:30:41 PM
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
 

#ifndef EEPROM_H_
#define EEPROM_H_

enum EEPROM_ADDR
{
	EE_Alarm_H,
	EE_Alarm_M,
	EE_Count1,
	EE_Count2_H,
	EE_Count2_L,
	//---
	EE_SIZE
};

#define EE_ADDR(X)	(EE_Addr+(X))

#define EE_Alarm	EE_Alarm_H
#define EE_Count	EE_Count1

void cpy(uint8_t *d, uint8_t *s, uint8_t n);
void Save_Prefs(alarm_t *Prefs);
void Load_Prefs(alarm_t *Prefs);

#endif
