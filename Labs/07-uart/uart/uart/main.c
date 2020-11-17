/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
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
#include "uart.h"           // Peter Fleury's UART library
#include "lcd_definitions.h"
#ifndef F_CPU
#define F_CPU   16000000
#endif


/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */

int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");    // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");    // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");    // Put button name here

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
    ADMUX |= (1 << REFS0);
    // Set input channel to ADC0
    ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0)); // 0000: ADC0
    // Enable ADC module
    ADCSRA |= (1 << ADEN);    
    // Enable conversion complete interrupt
    ADCSRA |= (1 << ADIE);    
    // Set clock prescaler to 128
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();
    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

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
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);


}

/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. Display value on LCD
 * and send it to UART.
 */


ISR(ADC_vect)
{

    uint16_t value = 0;
    char lcd_string[4] = "0000";
    int i=0;
    int j=0;
    uint8_t count_ones = 0; 
    uint16_t reminder [16];

    value = ADC;    // Copy ADC result to 16 bit variable

  
    itoa(value, lcd_string, 10);    // Convert to string in decimal
    lcd_gotoxy(8,0);
    // Clear the position
    lcd_puts("    ");
    lcd_gotoxy(8,0);
    // Update the position 
    lcd_puts(lcd_string);
  
    if (value < 700)
    {
    uart_puts("Button was pressed: ");
    uart_puts(lcd_string);
    uart_puts("\r\n");
    }     
    
    itoa(value, lcd_string, 16);    // Convert to string in hex 
    lcd_gotoxy(13,0);
    lcd_puts("   ");
    lcd_gotoxy(13,0);
    lcd_puts(lcd_string);

    // Approximately +- 10 Interval    
    if ((value >= 0) | (value < 5))
    {
        lcd_gotoxy(8,1);
        lcd_puts("      ");
        lcd_gotoxy(8,1);
        lcd_puts("Right");    
    }
    if((value > 95) && (value < 105) )
    {
        lcd_gotoxy(8,1);
        lcd_puts("      ");
        lcd_gotoxy(8,1);
        lcd_puts("Up");
    }  
    if((value > 240) && (value < 250))
    {
        lcd_gotoxy(8,1);
        lcd_puts("      ");
        lcd_gotoxy(8,1);
        lcd_puts("Down");
          
    }
    
    if((value > 395) && (value < 410))
    {
        lcd_gotoxy(8,1);
        lcd_puts("      ");
        lcd_gotoxy(8,1);
        lcd_puts("Left");  
        
        
    }
    if((value > 645) && (value < 655))
    {
        lcd_gotoxy(8,1);
        lcd_puts("      ");
        lcd_gotoxy(8,1);
        lcd_puts("Select");
               
    }
    if (value >= 1015)
    {
        lcd_gotoxy(8,1);
        lcd_puts("      ");
        lcd_gotoxy(8,1);
        lcd_puts("None");
   
    }
    
    // Calculating parity bit
    for(i=0; value>0; i++)
    {
         
        reminder[i]=value%2; // store the 1's and 0's
        value=value/2;   
     
    }
    for (j=i-1 ;j>0; j--)
    {
        if (reminder[j]==1) 
        {
            count_ones++;  // we want to count only the 1's
        }
         
    } 
     
    lcd_gotoxy(15,1);
    if ((count_ones%2)==1) // if the result of this is equal 1 then it means this is ODD number of 1's
    {
           
        lcd_puts("1");
     
    }
    else
    {
        lcd_puts("0");    // EVEN
   
    }
                
    
}


