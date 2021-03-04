/*
 * adc.c
 *
 * Created: October-24-18, 1:07:15 PM
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

void ADC_Init(void)
{	
	// Disable schmitt trigger on ADC input
	ADC1->TDRL = (1<<ADC_SENSE);
	// set MUX for first conversion
	ADC1->CSR = ADC_SENSE;
	// External trigger, TIM1 TRGO, right align, no scan mode
	ADC1->CR2 = ADC1_CR2_EXTTRIG|ADC1_CR2_ALIGN;	
	// ADC on, 1MHz
	ADC1->CR1|=ADC1_CR1_ADON|ADC_CLKDIV;
}

uint16_t Read_ADC(void)
{	uint8_t RL;

	// clear EOC flag
	ADC1->CSR &= ~ADC1_CSR_EOC;
	ADC1->CR1|=ADC1_CR1_ADON|ADC_CLKDIV;
	
	while(!(ADC1->CSR & ADC1_CSR_EOC))
		/* wait */;
		
	// Cosmic bug work around 
	RL = ADC1->DRL;
	return (ADC1->DRH<<8)|RL;
}
