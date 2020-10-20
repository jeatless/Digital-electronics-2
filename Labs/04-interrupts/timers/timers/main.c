/***********************************************
 *
 * Digital-Electronics-2 (BPA-DE2) 2020WS
 * 
 * Lab 4: Interrupts, timers
 *
 * Control LEDs using functions from GPIO and Timer libraries. Do not
 * use delay library any more.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Author : Rasit Demirören	(226554)
 *
 * Copyright (c) October 2020
 *
 ***********************************************/

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB5			// LED_D1 on PB5
#define LED_D2  PB4			// LED_D2 on PB4
#define LED_D3  PB3			// LED_D3 on PB3
#define LED_D4  PB2			// LED_D4 on PB2
#define BTN		PD0			// Push-Button on PD0

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle three LEDs
 * on Multi-function shield with internal 8- and 16-bit timer modules.
 */

uint8_t array[4] = {LED_D1, LED_D2, LED_D3, LED_D4};
int countLED = 0;
int backwards = 0;

int main(void)
{
    /* LED configurations */
    GPIO_config_output(&DDRB, array[0]);
    GPIO_write_high(&PORTB, array[0]);
	
	GPIO_config_output(&DDRB, array[1]);
    GPIO_write_high(&PORTB, array[1]);
			
	GPIO_config_output(&DDRB, array[2]);	
	GPIO_write_high(&PORTB, array[2]);
			
	GPIO_config_output(&DDRB, array[3]);
	GPIO_write_high(&PORTB, array[3]);


	/* Push-button set-up */
	GPIO_config_input_pullup(&DDRD, BTN);
    
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
		if (GPIO_read(&PIND, BTN))
		{
			/* Configuration of 16-bit Timer/Counter1
			 * Set prescaler and enable overflow interrupt */
			TIM1_overflow_1s();
			TIM1_overflow_interrupt_enable();
		}
		else
		{
			/* Configuration of 16-bit Timer/Counter1
			 * Set prescaler and enable overflow interrupt */
			 TIM1_overflow_262ms();
		     TIM1_overflow_interrupt_enable();
		}
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter1 overflows. Toggle LED D2 on 
 * Multi-function shield. */
ISR(TIMER1_OVF_vect)
{
	/* Toggle (turn on) LED_D1, because it is off at the beginning. 
	   Alternatively I could configure it "low" instead of creating this if() */
	if (GPIO_read(&PORTB,array[countLED]) == 1)
	{
		GPIO_toggle(&PORTB, array[countLED]);
	}
	else
	{
		//Toggle previous LED
		GPIO_toggle(&PORTB, array[countLED]);
	
		//Direction up/down
		if(countLED == 3)
		{
			backwards = 1;
		}
		if(countLED == 0)
		{
			backwards = 0;
		}
		
		if(backwards == 0)
		{
			countLED++;
		}
		else if(backwards == 1)
		{
			countLED--;
		}
	
		//Toggle next LED
		GPIO_toggle(&PORTB, array[countLED]);


	}
}