/***********************************************
 *
 * Digital-Electronics-2 (BPA-DE2) 2020WS
 * 
 * Lab 3: User library for GPIO control
 *
 * Author : Rasit Demirören	(226554)
 *
 * Copyright (c) October 2020
 *
 ***********************************************/ 

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define LED_RED     PC0
#define BTN			PD0
#define BLINK_DELAY 500
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include "gpio.h"           // GPIO library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle two LEDs 
 * when a push button is pressed. Functions from user-defined GPIO
 * library is used instead of low-level logic operations.
 */
int main(void)
{
	
	/* RED LED */
	GPIO_config_output(&DDRC, LED_RED);
    GPIO_write_low(&PORTC, LED_RED); //LED off, because active-low

    /* GREEN LED */
    GPIO_config_output(&DDRB, LED_GREEN);
    GPIO_write_low(&PORTB, LED_GREEN); //LED off, because active-high
	
    /* push button */
    GPIO_config_input_pullup(&DDRD, BTN);

    // Infinite loop
    while(1)
    {
		
		// Pause several milliseconds
		_delay_ms(BLINK_DELAY);
		if (GPIO_read(&PIND, BTN)) //Check if the button is pressed
		{
	
			GPIO_toggle(&PORTB, LED_GREEN);		// Revert the green LED
			GPIO_toggle(&PORTC, LED_RED);		// Revert the red LED
			
		}
		
    }

    // Will never reach this
    return 1;
}