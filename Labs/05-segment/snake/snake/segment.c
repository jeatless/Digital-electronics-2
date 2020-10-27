/***********************************************
 *
 * Digital-Electronics-2 (BPA-DE2) 2020WS
 * 
 * Lab 5: Display devices, 7-segment display
 *
 * Author : Rasit Demirören	(226554)
 *
 * Copyright (c) October 2020
 *
 ***********************************************/

/* Includes ----------------------------------------------------------*/
#define  F_CPU 16000000
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Variables ---------------------------------------------------------*/
uint8_t segment_value1[] = {
	// abcdefgDP
	
	0b01111111,     // Segment A
	0b10111111,     // Segment B
	0b11011111,     // Segment C
	0b11101111,     // Segment D
	0b11110111,     // Segment E
	0b11111011,     // Segment F

};

uint8_t segment_position1[] = {
	// p0....
	0b00010000,     // Position 0
	0b00100000,     // Position 1
	0b01000000,     // Position 2
	0b10000000      // Position 3
};

/* Function definitions ----------------------------------------------*/
void SEG_init(void)
{
    /* Configuration of SSD signals */
    GPIO_config_output(&DDRD, SEGMENT_LATCH);
    GPIO_config_output(&DDRD, SEGMENT_CLK);
    GPIO_config_output(&DDRB, SEGMENT_DATA);
}

/*--------------------------------------------------------------------*/
void SEG_update_shift_regs(uint8_t segments, uint8_t position)
{
    uint8_t bit_number;
	
	segments = segment_value1[segments];     // A,B,C,D,E,F...
	position = segment_position1[position];

  	
    // Pull LATCH, CLK, and DATA low
	GPIO_write_low(&PORTD, SEGMENT_LATCH);	// LATCH
	GPIO_write_low(&PORTD, SEGMENT_CLK);	// CLK
	GPIO_write_low(&PORTB, SEGMENT_DATA);	// DATA
	 
    // Wait 1 us
	_delay_us(1);

    // Loop through the 1st byte (segments)
    // a b c d e f g DP (active low values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Output DATA value (bit 0 of "segments")
		if((segments % 2)==0) // LSB is 0
			GPIO_write_low(&PORTB, SEGMENT_DATA);
		else   
			GPIO_write_high(&PORTB, SEGMENT_DATA);
			
			// Wait 1 us
			_delay_us(1);
		    // Pull CLK high
			GPIO_write_high(&PORTD, SEGMENT_CLK);
			// Wait 1 us
			_delay_us(1);
			// Pull CLK low
			GPIO_write_low(&PORTD, SEGMENT_CLK);
		    // Shift "segments"
			
        segments = segments >> 1;
			
    }

    // Loop through the 2nd byte (position)
    // p3 p2 p1 p0 . . . . (active high values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
		// Output DATA value (bit 0 of "position")
		if((position % 2)==0)// LSB is 0	
		
			GPIO_write_low(&PORTB, SEGMENT_DATA);
		
		else
			GPIO_write_high(&PORTB, SEGMENT_DATA);
		    
		// Wait 1 us
		_delay_us(1);
		// Pull CLK high
		GPIO_write_high(&PORTD, SEGMENT_CLK);
		// Wait 1 us
		_delay_us(1);
		// Pull CLK low
		GPIO_write_low(&PORTD, SEGMENT_CLK);
        // Shift "position"
		
        position = position >> 1;
		
	}

    // Pull LATCH high
	GPIO_write_high(&PORTD, SEGMENT_LATCH);
    // Wait 1 us
	_delay_us(1);
 
}

/*--------------------------------------------------------------------*/
/* SEG_clear */
void SEG_clear()
{

	SEG_update_shift_regs(0b11111111, 0b11111111);
	
}

/*--------------------------------------------------------------------*/
/* SEG_clk_2us */
void SEG_clk_2us ()
{
	GPIO_write_high(&PORTD, SEGMENT_CLK);	 // CLK
	_delay_us(1);
	GPIO_write_low(&PORTD, SEGMENT_CLK);
	_delay_us(1);
}