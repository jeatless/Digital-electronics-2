/***********************************************
 *
 * Digital-Electronics-2 (BPA-DE2) 2020WS
 * 
 * Lab 2: Control of GPIO, LED, push button
 *
 * Author : Rasit Demir�ren	(226554)
 *
 * Copyright (c) October 2020
 *
 ***********************************************/ 


// Two LEDs and Push button
/* Defines -----------------------------------------------------------*/
#define LED_GREEN PB5 // AVR pin where green LED is connected
#define LED_RED PC0
#define BTN PD0
#define BLINK_DELAY 250
#ifndef F_CPU
#define F_CPU 16000000 // CPU frequency in Hz required for delay
#endif
/* Includes ----------------------------------------------------------*/
#include <util/delay.h> // Functions for busy-wait delay loops
#include <avr/io.h> // AVR device-specific IO definitions
/* Functions ---------------------------------------------------------*/
/**
* Main function where the program execution begins. Toggle two LEDs
* when a push button is pressed.
*/
int main(void)
{
	/* GREEN LED */
	// Set pin as output in Data Direction Register...
	DDRB = DDRB | (1<<LED_GREEN);
	// ...and turn LED off in Data Register
	PORTB = PORTB & ~(1<<LED_GREEN);
	/*RED LED*/
	DDRC = DDRC | (1<<LED_RED);
	PORTC = PORTC & ~(1<<LED_RED);
	DDRD = DDRD & ~(1<<BTN); //Turn OFF
	PORTD = PORTD | (1<<BTN);
	// Infinite loop
	while (1)
	{
		// Pause several milliseconds
		_delay_ms(BLINK_DELAY);
		// WRITE YOUR CODE HERE
		if (bit_is_clear(PIND, BTN))
		{
			// WRITE YOU CODE HERE
			PORTB = PORTB ^ (1<<LED_GREEN);
			PORTC = PORTC ^ (1<<LED_RED);
		}
	}
	// Will never reach this
	return 0;
}