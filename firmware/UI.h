/*
 * ui.h
 *
 * Created: October-26-18, 8:07:24 PM
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
 
#ifndef UI_H_
#define UI_H_

void Setup(void);

typedef struct
{	
	void	*Value;
	uint8_t	*Modified;
	uint8_t X;
	uint8_t	Y;
	uint8_t Width;
	uint16_t Min;
	uint16_t Max;
	uint8_t Flags;
} UI_Item_t;

typedef struct
{	
	uint8_t		*Text;
	UI_Item_t *Items;
	uint8_t		Size;
} UI_Menu_t;

typedef void (*FuncPtr)(void);
typedef void (*FuncPtr_arg)(void *);

enum Gfx_Put_Format
{
	RightJustify = 0x02, LeadingZero = 0x01
};

void LCD_Hex(uint8_t n);
void LCD_Hex2(uint8_t n);

void Print2d(uint8_t Value,uint8_t Format);
void Print_uint(uint16_t Value, uint8_t width, uint8_t Format);

void UI_PrintItem(UI_Item_t *Item, uint8_t Display);
uint8_t UI_EditItem(UI_Item_t *Item);
uint8_t UI_Menu(UI_Menu_t *Menu);

#define UI_LARGE_STEP		10

enum DataFlags { D_12H, D_U8, D_U8Z, D_U16, D_Menu, D_Function, D_CustomData };

#endif
