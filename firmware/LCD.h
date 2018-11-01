/*
 * lcd.h
 *
 * Created: April-08-15, 3:10:36 PM
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
#ifndef LCD_H_
#define LCD_H_

#ifndef HARDWARE_H_
#include "hardware.h"
#endif

#define LCD_NIBH(X)     ((X) & LCD_DATA_MASK)
#define LCD_NIBL(X)     ((X) <<4)

void LCD_Cls(void);
void LCD_Init(void);
void LCD_WR_High_Nibble(uint8_t Code);
void LCD_WR(uint8_t LCD_data);
uint8_t LCD_RD(void);

// Internal functions
void LCD_Wait(void);
void LCD_Data(uint8_t LCD_Data);
void LCD_Cmd(uint8_t Cmd);

void LCD_Moveto(uint8_t X, uint8_t Y);
void LCD_Ch(uint8_t Ch);
void LCD_UserCh(void);
void LCD_Puts(char *str);

#define LCD_CUSTOM_CHR

#define LCD_Addr(X,Y) ((X)|((Y)<<_LCD_YSHIFT))

// LCD commands
#define	_LCD_FuncSet		0x28		// 4-bit interface, 2 lines, 5x7 font
#define	_LCD_FuncSetH		(_LCD_FuncSet & 0xf0)
#define _LCD_Set8bit    0x30		// 8-bit interface

#define	_LCD_CLR				0x01
#define	_LCD_HOME				0x02

#define _LCD_ENTRY      0x06

#define	_LCD_ON					0x08
#define _LCD_DISPLAY_ON	0x04
#define	_LCD_CSR_ON			0x02
#define	_LCD_CSR_BLK		0x01
#define _LCD_CSR_UL			0x00

#define _LCD_SHIFTLEFT  0x10
#define _LCD_SHIFTRIGHT 0x14

#define	_LCD_CG					0x40
#define	_LCD_DD					0x80

// LCD constants
#define _LCD_BUSY				0x80

#define	_LCD_MX0				0x0f
#define	_LCD_MX1				0x4f

#define _LCD_Y1         0x40

#define _LCD_XMASK      0x0f
#define _LCD_YMASK      0x01
#define _LCD_YSHIFT     0x06

#define _LCD_INIT_DELAY1	4100
#define _LCD_INIT_DELAY2	100

enum LCD_Char { CH_AM = 0x08, CH_PM = 0x09, CH_ADJ = 0x0a };

#endif
