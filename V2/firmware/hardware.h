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

#define LCD_E										PC3
#define LCD_DATA								(PC4|PC5|PC6|PC7)
#define LCD_RS									PA3
#define LCD_RS_PORT							GPIOA
#define LCD_E_PORT							GPIOC
#define LCD_DATA_PORT						GPIOC

#define KEY_PORT								LCD_DATA_PORT

#define I2C_SDA									PB5
#define I2C_SCK									PB4
#define I2C_PINS								(I2C_SDA|I2C_SCK)
#define I2C_PORT								GPIOB

// CPU divider = 1 -> 12MHz
//#define CPU_DIV					0

// CPU divider = 16 -> 750kHz
//#define CPU_DIV					0x04
// CPU divider = 4 -> 3MHz
#define CPU_DIV					0x00
// CPU divider = 128 -> 93.75kHz
#define CPU_SLOW				0x07							

enum _KEY_CODES
{	KEY_MENU=PC5, KEY_DEC=PC6, KEY_INC=PC7, KEY_MASK=(PC5|PC6|PC7) };

#define NRF_IRQ									LCD_RS
#define NRF_IRQ_PORT						LCD_RS_PORT

#define NRF_CE									PD2
#define NRF_CS									PD3
#define NRF_PORT								GPIOD

#define MOTOR_EN								PD4
#define LED_EN									PD6
#define PERIPH_PORT							GPIOD

#define LED_OFF()								PERIPH_PORT->ODR &= ~LED_EN
#define LED_ON()								PERIPH_PORT->ODR |= LED_EN

#define ADC_SENSE								5
#define ADC_MAX									1023
#define ADC_HYSTERESIS					5

#define DATA_KEY1								0xAE
#define DATA_KEY2								0x56

#define SERVO_FREQ							50
#define TIM2_DIV								4
#define TIM2_PSC								2			// 2^2
#define TIM2_RELOAD							(CPU_CLOCK/(TIM2_DIV*SERVO_FREQ))

// 12MHz/4 = 3MHz, resolution: 333ns
// Servo : 1.5ms + 0.5ms* X/90 
// Actual position determined empirically

#define SERVO_OFF								4600
#define SERVO_ON								1100

// wait 8 sec before shut down
#define SERVO_SHUTDOWN					(SERVO_FREQ*10)

extern uint8_t 	ServoFlag;
void Servo_Init(void);
void Servo_State(uint8_t State);
void Servo_Off(void);

#define MOTOR_OFF()							Servo_State(0)
#define MOTOR_ON()							Servo_State(1)
#define MOTOR_FLAG							ServoFlag

#define DELAY_ADJ(X)						(X*0.79)

#include "adc.h"
#include "LCD.h"
#include "time.h"
#include "key.h"
#include "ui.h"
#include "eeprom.h"
#include "menu.h"

void Init_Hardware(void);
void Delay(uint16_t Delay);
void PowerDetect(void);

extern uint16_t ADC_Result;
extern uint8_t power, debugger;
#endif
