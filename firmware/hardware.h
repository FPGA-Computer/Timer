/*
 * hardware.h
 *
 * Created: October-19-18, 12:25:42 PM
 *  Author: K. C. Lee
 * Copyright (c) 2016 by K. C. Lee
 
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

#ifndef HARDWARE_H_
#define HARDWARE_H_

#ifndef __CSMC__
#define __CSMC__
#endif
#define STM8S003

#include "stm8s.h"
#include <stdio.h>

// STM8S003F3P6
enum _PA { PA1=0x02, PA2=0x04, PA3=0x08 };
enum _PB { PB4=0x10, PB5=0x20 };
enum _PC { PC3=0x08, PC4=0x10, PC5=0x20, PC6=0x40, PC7=0x80 };
enum _PD { PD1=0x02, PD2=0x04, PD3=0x08, PD4=0x10, PD5=0x20, PD6=0x40 };

#include "irq.h"

#define CPU_CLOCK								12000000UL
#define EE_Addr									0x4000

// Board specific - trims the real time clock by CLOCK_TRIM * 1.8 sec/day
#define CLOCK_TRIM							12

#define LCD_E										PC3
#define LCD_RW									PD4
#define LCD_DATA								(PC4|PC5|PC6|PC7)
#define LCD_RS									PA3

#define LCD_RW_PORT							GPIOD
#define LCD_RS_PORT							GPIOA
#define LCD_E_PORT							GPIOC
#define LCD_DATA_PORT						GPIOC

#define MOTOR_EN								PD3
#define LED_EN									PD6
#define PERIPH_PORT							GPIOD

#define MOTOR_OFF()							PERIPH_PORT->ODR |= MOTOR_EN
#define MOTOR_ON()							PERIPH_PORT->ODR &= ~MOTOR_EN
#define LED_OFF()								PERIPH_PORT->ODR &= ~LED_EN
#define LED_ON()								PERIPH_PORT->ODR |= LED_EN

#define ADC_KEYS								3
#define ADC_SENSE								5

#define ADC_MAX									1023
#define ADC_HYSTERESIS					5

#define DATA_KEY1								0xAE
#define DATA_KEY2								0x56

#include "LCD.h"
#include "time.h"
#include "adc.h"
#include "key.h"
#include "ui.h"
#include "eeprom.h"
#include "menu.h"

void Init_Hardware(void);
void Delay(uint16_t Delay);


#endif
