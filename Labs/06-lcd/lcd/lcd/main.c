/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */
/* Variables ---------------------------------------------------------*/
// Custom defined character definitions (Loading effect)
uint8_t customChar[] = {

	// .....
	0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
	// |....
	0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
	// ||...
	0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
	// |||..
	0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
	// ||||.
	0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110,
	// |||||
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
	// Custom designed arrow (Left)
	0b10100, 0b01010, 0b00101, 0b11111, 0b00101, 0b01010, 0b10100, 0b00000,
	// Custom designed arrow (Right)
	0b00101, 0b01010, 0b10100, 0b11111, 0b10100, 0b01010, 0b00101, 0b00000,
	

};

int main(void)
{
	// Initialize LCD display
	lcd_init(LCD_DISP_ON);

	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 64; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
	// Put string(s) at LCD display
	lcd_gotoxy(1, 0);		// column: 1, row: 0 
	lcd_puts("00:00.0");	// put string: all zero(s)

 	lcd_gotoxy(0,0);
 	lcd_putc(6);
 	lcd_gotoxy(8,0);
 	lcd_putc(7);
	lcd_gotoxy(11, 1);		// column: 11, row: 1 
	lcd_putc('c');			// put character 'c'
		
    // Configure 8-bit Timer/Counter0 for Stopwatch
    // Enable interrupt and set the overflow prescaler to 16 ms
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();
    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Enable interrupt and set the overflow prescaler to 16 ms
    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
	static uint8_t tens = 0;		// Tenths of a second
	static uint8_t secs = 0;		// Seconds
	static uint8_t mins = 0;		// Minutes
	static uint16_t square = 0;
	char lcd_string[4] = "    ";    // String to convert numbers by itoa()
	
	
    number_of_overflows++;
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;

		// Increment the tenths
		tens++;
		if(tens == 10)
		{
			tens = 0;
			secs++;
		
			if(secs == 60)
			{
				secs = 0;
				lcd_gotoxy(4, 0);
				lcd_putc('0');
				mins++;
			
				if(mins == 60)
				{
					mins = 0;
					lcd_gotoxy(1, 0);
					lcd_putc('0');
				}
			}
		}
		
		// Printing the tenths of a second
		itoa(tens, lcd_string, 10);
		lcd_gotoxy(7, 0);
		lcd_puts(lcd_string);
		
		// Printing the seconds
		itoa(secs, lcd_string, 10);
		if(secs < 10)
		{
			lcd_gotoxy(5, 0);
		}
		else
		{
			lcd_gotoxy(4, 0);
		}
		lcd_puts(lcd_string);

		// Printing the minutes
		itoa(mins, lcd_string, 10);
		if (mins < 10)
		{
			lcd_gotoxy(2, 0);
		}
		else
		{
			lcd_gotoxy(1, 0);
		}
		lcd_puts(lcd_string);

		// Printing the squares of seconds (second^2)
		square = secs * secs;
		itoa(square, lcd_string, 10);
		if (secs == 0)
		{
			lcd_gotoxy(11, 0);
			lcd_putc('0');			//  if secs == 0 put 0
			lcd_puts("   ");		// then clear last 3 digits
		}
		else
		{
			lcd_gotoxy(11, 0);
			lcd_puts(lcd_string);
		}

    }
}

/**
 * ISR starts when Timer/Counter0 overflows. Create a progress
 * bar on the LCD display for every counted second
 */
ISR(TIMER0_OVF_vect)
{
	static uint8_t symbol = 0;
	static uint8_t position = 0;
	uint8_t i = 0;

	symbol++;
	if(symbol > 5)
	{
		symbol = 0;
		position++;
		if(position > 9)
		{
			position = 0;

			lcd_gotoxy(1,1);
			// while loop to clear
			while(i < 10)
			{
				lcd_putc(0);
				i++;
			}
		}
		
	}
	
	lcd_gotoxy(1 + position, 1);
	lcd_putc(symbol);
	
}
