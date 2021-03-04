/*
 * ui.c
 *
 * Created: October-26-18, 8:06:59 PM
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

void Print_uint(uint16_t Value, uint8_t width, uint8_t Format)
{
	const uint16_t DivTbl[]= { 1,10,100,1000,10000 };
	uint16_t Div;

	if(width > sizeof(DivTbl)/sizeof(uint16_t))
		width=sizeof(DivTbl)/sizeof(uint16_t);
	
	Div = DivTbl[width-1];
	
	do
  { 
    if((Format & LeadingZero)||(Value >= Div)||(Div == 1))
    { 
			Format = LeadingZero;
      LCD_Ch('0'+Value/Div);
      Value %= Div;
     }
    else if(Format & RightJustify)
			LCD_Ch(' ');

    Div /= 10;
  } while(Div);
}

void Print2d(uint8_t Value,uint8_t Format)
{
	Print_uint(Value,2,Format|RightJustify);
}
void sec2ms(uint16_t time,uint8_t *min,uint8_t *sec)
{
	*min = time/60;	
	*sec = time-*min*60;
}

void UI_Puts_n(char *str, uint8_t width)
{
	while(width--)
		LCD_Ch(*str?*str++:' ');
}

void Print_12Hr(uint8_t hour)
{
	if(hour<12)	
		LCD_Ch(CH_AM);
	else
	{
		LCD_Ch(CH_PM);
		hour-=12;
	}
	if(!hour)
		Print2d(12,0);
	else 
		Print2d(hour,0);
}

void Print_Time(rtc_t *time,uint8_t display_opt)
{
	Print_12Hr(time->hour);

	LCD_Ch(TIME_SEPARATOR);
	Print2d(time->min,LeadingZero);
	
	if(display_opt & DISPLAY_SEC)
	{
		LCD_Ch(TIME_SEPARATOR);
		Print2d(time->sec,LeadingZero);
	}
}

void Print_Month(uint8_t month, uint8_t width)
{
	const uint8_t *Month[] = { "Jan","Feb","Mar","Apr","May","Jun",
														 "Jul","Aug","Sep","Oct","Nov","Dec" };
  UI_Puts_n(Month[month-1],width);		
}

void Print_Date(rtc_t *time,uint8_t display_opt)
{ 
	const uint8_t *DayOfWeek[]= { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };	
	
  UI_Puts_n(DayOfWeek[time->dayofweek],DOW_WIDTH);
	LCD_Ch(DOW_SEPARATOR);
	Print2d(time->day,0);
	LCD_Ch(DATE_SEPARATOR);
	Print_Month(time->month,MONTH_WIDTH);
}

void UI_PrintItem(UI_Item_t *Item, uint8_t Display)
{
	uint8_t Width;
	
	LCD_Moveto(Item->X,Item->Y);
	
	if(Display)
	{
		switch(Item->Flags)
		{
			case D_12H:
				Print_12Hr(*(uint8_t*)Item->Value);
				break;
			case D_U8:
			case D_U8Z:
				Print_uint(*(uint8_t*)Item->Value,Item->Width,RightJustify|(Item->Flags==D_U8Z));
				break;
			case D_U16:
				Print_uint(*(uint16_t*)Item->Value,Item->Width,RightJustify);
				break;
			case D_Month:
				Print_Month(*(uint8_t*)Item->Value,Item->Width);
				break;
			case D_Menu:
			case D_Function:
				UI_Puts_n(Item->Value,Item->Width);
				break;
			case D_CustomData8:
			case D_CustomData16:
				((FuncPtr_arg)Item->Modified)(Item);
				break;
			case D_SetTime:
				Print_Time((rtc_t *)Item->Value,DISPLAY_SEC);
				break;
		}
	}
	else
		UI_Puts_n("",Item->Width);
}

void UI_PrintItems(UI_Menu_t *Menu)
{
	uint8_t i;

	LCD_Cls();
	LCD_Puts(Menu->Text);

	for(i=0;i<Menu->Size;i++)
		UI_PrintItem(&Menu->Items[i],1);
}

uint8_t UI_EditItem(UI_Item_t *Item)
{
	uint8_t Changed=0,Quit=0,Update=1;
	uint16_t Value;

	if((Item->Flags==D_U16)||(Item->Flags==D_CustomData16))
		Value=*(uint16_t*)Item->Value;	
	else
		Value=*(uint8_t*)Item->Value;
	
	Key_Purge();
	
	LCD_Moveto(Item->X+Item->Width,Item->Y);
	LCD_Ch(CH_ADJ);
	
	do
	{
		switch(Key_Get())
		{
			case KEY_DEC:
				if(Value > Item->Min)
					Value--;
				else
					Value = Item->Max;										// wrap around
					
				Changed |= 1;
				Update=1;
				break;
				
			case KEY_DEC|KEY_LONG:
				if(Value >= (Item->Min+UI_LARGE_STEP))
					Value -= UI_LARGE_STEP;
				else																		// wrap around
					Value = Item->Max /* + (Value-UI_LARGE_STEP-Item->Min) +1*/;
				
				Changed |= 1;
				Update=1;				
				break;
				
			case KEY_INC:
				if(Value < Item->Max)
					Value++;
				else
					Value = Item->Min;										// wrap around
					
				Changed |= 1;
				Update=1;				
				break;
				
			case KEY_INC|KEY_LONG:
				if(Value <= (Item->Max-UI_LARGE_STEP))
					Value += UI_LARGE_STEP;
				else																		// wrap around
					Value = Item->Min /*+ (Value+UI_LARGE_STEP-Item->Max) -1*/;
				
				Changed |= 1;
				Update=1;
				break;
				
			case KEY_MENU:
			case KEY_MENU|KEY_LONG:
				Quit = 1;
				break;
		}
		
		if(Update||Quit)
		{	
			if((Item->Flags==D_U16)||(Item->Flags==D_CustomData16))
				*(uint16_t*)Item->Value=Value;	
			else
				*(uint8_t*)Item->Value=Value;
			
			UI_PrintItem(Item,Update||Quit);
			time.HalfSec=0;
			time.FullSec=0;				
			Update = 0;
		}
			
	} while (!Quit);

	Key_Purge();
	return(Changed);
}

uint8_t UI_Menu(UI_Menu_t *Menu)
{
	uint8_t Modified=0,ItemModified=0,Quit=0,Item=0;
		
	Key_Purge();	

	UI_PrintItems(Menu);
			
	do
	{	
		switch(Key_Get())
		{								
			case KEY_MENU|KEY_LONG:
				Quit = 1;
				break;
				
			case KEY_MENU:
				
				switch(Menu->Items[Item].Flags)
				{
					case D_Menu:
						// Item is a menu
						UI_Menu((UI_Menu_t *)Menu->Items[Item].Modified);
						UI_PrintItems(Menu);					
						break;
					
					case D_Function:
						// Item is a function call
						((FuncPtr)Menu->Items[Item].Modified)();
						UI_PrintItems(Menu);						
						break;
						
					case D_SetTime:					
						if(*(Menu->Items[Item].Modified))
						{
							rtc_t *rtc = Menu->Items[Item].Value;
							
							RTC_SetTime(rtc->hour,rtc->min,rtc->sec);
							*(Menu->Items[Item].Modified) = 0;
							break;						
						}
					
					default:
						ItemModified=UI_EditItem(&Menu->Items[Item]);
						*Menu->Items[Item].Modified|=ItemModified;
						
						UI_PrintItems(Menu);
						Modified|=ItemModified;
				}
			
			//	fall through to next position
				
			case KEY_INC:
			case KEY_INC|KEY_LONG:
				UI_PrintItem(&Menu->Items[Item],1);
				
				if(Item < Menu->Size-1)
					Item++;				
				else
					Item = 0;										// wrap around
				break;
				
			case KEY_DEC:
			case KEY_DEC|KEY_LONG:
				UI_PrintItem(&Menu->Items[Item],1);
				
				if(Item>0)
					Item--;
				else
					Item = Menu->Size-1;				// wrap around
				break;
		}

		// blink
		if(time.HalfSec||time.FullSec)
		{
			UI_PrintItem(&Menu->Items[Item],time.FullSec);
			time.HalfSec=0;
			time.FullSec=0;	
		}		
	} while (!Quit);
	
	Key_Purge();
	LCD_Cls();
	return(Modified);
}
