/*
 * lcd.c
 *
 * Created: April-08-15, 3:09:59 PM
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
#include "LCD.h"

// minimum delay for 12MHz
uint8_t LCD_X, LCD_Y;

void LCD_Short_Delay(void)
{
		_asm("nop");
		_asm("nop");
		_asm("nop");
		_asm("nop");
		_asm("nop");		
}

void LCD_WR_High_Nibble(uint8_t Code)
{
	LCD_DATA_PORT->DDR |= LCD_DATA;
	LCD_DATA_PORT->ODR = Code;
	LCD_Short_Delay();
	LCD_DATA_PORT->ODR = LCD_E|Code;
	LCD_Short_Delay();	
	LCD_E_PORT->ODR &= ~LCD_E;
	Delay(2);
}

void LCD_WR(uint8_t LCD_data)
{ 
  // DDR = Output
  LCD_RW_PORT->ODR &= ~LCD_RW;
  LCD_WR_High_Nibble(LCD_data & 0xf0);
  LCD_WR_High_Nibble(LCD_data << 4);
  // DDR = Input  
  LCD_DATA_PORT->DDR &= ~LCD_DATA;
	LCD_Short_Delay();
 }

uint8_t LCD_RD(void)
{ uint8_t LCD_Data, i;

	LCD_RW_PORT->ODR |= LCD_RW;
  LCD_E_PORT->ODR |= LCD_E;
	Delay(1);	
  LCD_Data = LCD_DATA_PORT->IDR & LCD_DATA;	
  LCD_E_PORT->ODR &= ~LCD_E;
	Delay(1);    
  LCD_E_PORT->ODR |= LCD_E;
	Delay(1);	
  LCD_Data |= LCD_DATA_PORT->IDR >> 4;
  LCD_E_PORT->ODR &= ~LCD_E;
	Delay(1);	
  return(LCD_Data);
 }

void LCD_Wait(void)
{ 
  LCD_RS_PORT->ODR = 0;
	
  while (LCD_RD() & _LCD_BUSY)
    /* WAIT */ ;
 }

void LCD_Data(uint8_t LCD_Data)
{ 
  LCD_Wait();
  LCD_RS_PORT->ODR = LCD_RS;
  LCD_WR(LCD_Data);
 }

void LCD_Cmd(uint8_t Cmd)
{ 
  LCD_Wait();
  LCD_RS_PORT->ODR = 0;
  LCD_WR(Cmd);
 }

// Cursor support
void LCD_Moveto(uint8_t X, uint8_t Y)
{ 
	LCD_X = X & _LCD_XMASK;
	LCD_Y = Y & _LCD_YMASK;
	
	LCD_Cmd(_LCD_DD|LCD_Addr(LCD_X,LCD_Y));
 }

void LCD_Ch(uint8_t Ch)
{
	LCD_Data(Ch);
	LCD_X++;
	
	if(LCD_X > _LCD_XMASK)
	{
		LCD_X = 0;
		LCD_Y = 1;
		
		LCD_Moveto(0,1);
	}
 }

#ifdef LCD_CUSTOM_CHR

LCD_CUSTOM_CHR
const uint8_t LCD_CG[]=
{ 
	0x0e,0x11,0x1f,0x11,0x00,0x11,0x1b,0x15,		// AM
	0x1e,0x11,0x1e,0x10,0x00,0x11,0x1b,0x15,		// PM
	0x04,0x0e,0x1f,0x00,0x1f,0x0e,0x04,0x00,		// Adj
	0x08,0x05,0x03,0x17,0x08,0x05,0x03,0x07,		// Light
};

void LCD_UserCh(void)
{ uint8_t i;

  LCD_Cmd(_LCD_CG);
  
  for(i=0;i<sizeof(LCD_CG);i++)
    LCD_Data(LCD_CG[i]);

  LCD_Cmd(_LCD_DD);  
 }

#endif

void LCD_Puts(char *str)
{
	while(*str)
	  LCD_Ch(*str++);
}

void LCD_Cls(void)
{
	LCD_Cmd(_LCD_CLR);	
	LCD_X = LCD_Y = 0;
}

// Power On LCD reset    
void LCD_Init(void)
{ 
	LCD_WR_High_Nibble(_LCD_Set8bit);
	Delay(_LCD_INIT_DELAY1);
	
	LCD_WR_High_Nibble(_LCD_Set8bit);
	Delay(_LCD_INIT_DELAY2);
	
	LCD_WR_High_Nibble(_LCD_Set8bit);
	LCD_WR_High_Nibble(_LCD_FuncSetH);

	LCD_Cmd(_LCD_FuncSet);
	LCD_Cmd(_LCD_ON|_LCD_DISPLAY_ON);	

	LCD_Cls();
	
	#ifdef LCD_CUSTOM_CHR
	LCD_UserCh();
	#endif
	
	LCD_Cmd(_LCD_ENTRY);
 }

 
 
 