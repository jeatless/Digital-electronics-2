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

int main(void)
{	
    // Configure SSD signals
    SEG_init();

	// Test of SSD: display number '3' at position 0
	// SEG_update_shift_regs(3, 0);
     
	/* Configure 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();
   
	// Enables interrupts by setting the global interrupt mask
	sei();
	 
	// Infinite loop
    while(1)
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
	
 	cnt0++;				// Increase cnt0 by one
	if(cnt0 >= 6)		// Count to 6
 	cnt0 = 0;			// Set cnt0 to 0

    SEG_update_shift_regs(cnt0, 0);	
}