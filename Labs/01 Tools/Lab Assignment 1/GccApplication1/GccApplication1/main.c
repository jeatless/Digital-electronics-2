 /*
 * Digital-Electronics-2
 *
 * Lab exercise 01
 *
 * Author : Rasit Demiroren (226554)
 */ 


#define LED_GREEN PB5						// AVR pin where green LED is connected
#define ONE_UNIT_DELAY 500					// Delay in milliseconds
#define THREE_UNITS_DELAY 1500
#define SEVEN_UNITS_DELAY 3500
#ifndef  F_CPU
#define F_CPU 16000000						// CPU frequency in Hz required for delay function
#endif


#include <util/delay.h>						// Functions for busy-wait delay loops
#include <avr/io.h>

void long_blink()							// This function realizes so called "dash" of morse code
{
	PORTB = PORTB | (1<<LED_GREEN);			// Turns the LED on by setting PB5 "1" through shifting
	_delay_ms(THREE_UNITS_DELAY);			// Three units "dash"
	PORTB = PORTB & ~(1<<LED_GREEN);		// Turns the LED off with "And" operation and inverse
}


void short_blink()							// This function realizes so called "dot" of morse code
{
	PORTB = PORTB | (1<<LED_GREEN);			// Turns the LED on by setting PB5 "1" through shifting
	_delay_ms(ONE_UNIT_DELAY);				// One second "dot"
	PORTB = PORTB & ~(1<<LED_GREEN);		// Turns the LED off with "And" operation and inverse
}


void gap()									// We put gap between letters to make it more clear to understand where the letter ends
{
	PORTB = PORTB & ~(1<<LED_GREEN);		// Turns the LED off
	_delay_ms(THREE_UNITS_DELAY);			// Three units "gap"
}

int main(void)
{
	// Set pin as output in Data Direction Register
	// DDRB = DDRB or 0010 0000
	DDRB = DDRB | (1<<LED_GREEN);			

	// Set pin LOW in Data Register (LED off)
	// PORTB = PORTB and 1101 1111
	PORTB = PORTB & ~(1<<LED_GREEN);		

	// Infinite loop
	
	while (1)
	{
		
		
		// (The letter "D" in morse code is: -..)
		long_blink();
		_delay_ms(ONE_UNIT_DELAY);			// Wait one unit between parts
		short_blink();
		_delay_ms(ONE_UNIT_DELAY);		
		short_blink();						// End of letter "D"
		
		gap();								// Wait between letters
		// (The letter "E" in morse code is: .)
		short_blink();						// End of letter "E"
		
		gap();								// Wait between letters
		
		// (The number "2" in morse code is: ..---)
		short_blink();						
		_delay_ms(ONE_UNIT_DELAY);		
		short_blink();
		_delay_ms(ONE_UNIT_DELAY);		
		long_blink();
		_delay_ms(ONE_UNIT_DELAY);		
		long_blink();
			_delay_ms(ONE_UNIT_DELAY);	
		long_blink();						// End of number "2"
			_delay_ms(SEVEN_UNITS_DELAY); 
		
	}

	return 0;
}

