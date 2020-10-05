/***********************************************
 *
 * Digital-Electronics-2 (BPA-DE2) 2020WS
 * 
 * Lab 2: Control of GPIO, LED, push button
 *
 * Author : Rasit Demirören	(226554)
 *
 * Copyright (c) October 2020
 *
 ***********************************************/ 


/* Defines -----------------------------------------------------------*/

#define LED_RED    PC0
#define LED_GREEN  PC1
#define LED_YELLOW PC2 
#define LED_ORANGE PB1 
#define LED_BLUE   PB2 
#define BTN		   PD0


#define BLINK_DELAY 500
#define BLINK_DELAY2 50

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif
/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
void YELLOW () 
{
	
	PORTC=PORTC ^ (1<< LED_YELLOW);
	_delay_ms(BLINK_DELAY);
	PORTC = PORTC & ~(1<<LED_YELLOW);
	
}

void GREEN ()
{
	
	PORTC=PORTC ^ (1<<LED_GREEN);
	_delay_ms(BLINK_DELAY);
	PORTC = PORTC & ~(1<<LED_GREEN);
	
}

void RED ()
{

	PORTC=PORTC ^ (1<<LED_RED);
	_delay_ms(BLINK_DELAY);
	PORTC = PORTC & ~(1<<LED_RED);

}

void BLUE ()
{

	PORTB=PORTB ^ (1<< LED_BLUE);
	_delay_ms(BLINK_DELAY);
	PORTB = PORTB & ~(1<<LED_BLUE);

}

void ORANGE ()
{

	PORTB=PORTB ^ (1<< LED_ORANGE);
	_delay_ms(BLINK_DELAY);
	PORTB = PORTB & ~(1<<LED_ORANGE);

}

int main(void)
{
	/*YELLOW LED */
	DDRC = DDRC | (1<<LED_YELLOW);
	PORTC = PORTC & ~(1<<LED_YELLOW);
	
    /* GREEN LED */
    DDRC = DDRC | (1<<LED_GREEN);
    PORTC = PORTC & ~(1<<LED_GREEN);

    /* RED LED */
	DDRC = DDRC | (1 << LED_RED );  
	PORTC = PORTC & ~(1<<LED_RED);

	/*BLUE LED*/
	DDRB = DDRB | (1<<LED_BLUE);
	PORTB = PORTB & ~(1<<LED_BLUE);
	
	/* ORANGE LED */
	DDRB = DDRB | (1<<LED_ORANGE);
	PORTB = PORTB & ~(1<<LED_ORANGE);
	   
	DDRD = DDRD & ~(1<<BTN); //Turn OFF
	PORTD = PORTD | (1<<BTN);
 
    // Infinite loop
    while (1)
	{
        // Pause several milliseconds
		_delay_ms(BLINK_DELAY2);
		
  		if (bit_is_clear(PIND, BTN))
	  
		{
				YELLOW();
			
			
			if (bit_is_clear(PIND, BTN))
			
			{
				
				GREEN();
				
				if (bit_is_clear(PIND, BTN))
				
				{
					
					RED();
					
					if (bit_is_clear(PIND, BTN))
					
					{
						
						BLUE();
						
						if (bit_is_clear(PIND, BTN))
						
						{
							
							ORANGE();
							
							if (bit_is_clear(PIND, BTN))
							
							{
								
								BLUE();
							
								if (bit_is_clear(PIND, BTN))
								
								{
									
									RED();
								
									if (bit_is_clear(PIND, BTN))
									
									{
										
										GREEN();
										
									}	
								}	
							}	
						}		
					}
				}	
			}
		}	

	}
    
    return 0;
}