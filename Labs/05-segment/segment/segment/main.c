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
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC


/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Display decimal 
 * counter values on SSD (Seven-segment display) when 16-bit 
 * Timer/Counter1 overflows.
 */

/* Variables */
uint8_t cnt0 = 0;	// Decimal counter value on position 0
uint8_t cnt1 = 0;	// Decimal counter value on position 1

int main(void)
{	
    // Configure SSD signals
    SEG_init();

    // Test of SSD: display number '3' at position 0
   // SEG_update_shift_regs(3, 0);
   
	/* Configure 8-bit Timer/Counter0
     * Set prescaler and enable overflow interrupt */	
	TIM0_overflow_4ms();
	TIM0_overflow_interrupt_enable();
     
	/* Configure 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
	TIM1_overflow_1s();
	TIM1_overflow_interrupt_enable();
   
  // SEG_clear(cnt0,cnt1);
  
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
 * ISR starts when Timer/Counter1 overflows. Increment decimal counter
 * value and display it on SSD.
 */

ISR(TIMER1_OVF_vect)	
{
	// SEG_update_shift_regs(cnt0,0);
  	
	cnt0++;					// Increase cnt0 by one
	
	if(cnt0 >= 10)			// Count to 9
	{
		cnt0 = 0;			// Then restart the count on position 0
		cnt1++;				// Increase cnt1 by one
		
		if(cnt1 >= 6)		// Count to 5, then restart the count on position 1
		{
			cnt1 = 0;		// Then restart the count on position 1
		} 
	}
}


 ISR(TIMER0_OVF_vect)
 {
	 
 	static uint8_t pos = 0;

 	uint8_t digitPosition = 0; 
	 
 	if(pos == 0)
 	{
 		SEG_update_shift_regs(cnt0, digitPosition);
 		pos = 1;
 	}
 	else
 	{  
 		digitPosition++;
 		SEG_update_shift_regs(cnt1, digitPosition);
 		pos = 0;
 	}
	 
 }
