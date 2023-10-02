// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

#define _XTAL_FREQ 20000000     // 20MHz oscillator frequency

unsigned char timer_counter = 0;

void interrupt ISR() {
    if (T0IF == 1) {
        timer_counter++;
        T0IF = 0; // Clear TMR0 interrupt flag
        TMR0 = 59; // Reload TMR0 to achieve a 10ms interrupt period (assuming 20MHz oscillator)
    }
}

void main(void) {
    // Initialize Timer0 and enable interrupts
    OPTION_REG = 0b00000111;   // Prescaler 1:256, TMR0 source is internal instruction cycle clock (Fosc/4)
    TMR0 = 59;                 // Load TMR0 to achieve a 10ms interrupt period
    INTCON = 0b11100000;       // Enable TMR0 interrupt, enable global and peripheral interrupts

    TRISB0 = 0;                // Set RB0 as an output
    PORTBbits.RB0 = 0;         // Initialize RB0 to 0
    
    while (1) {
        if (timer_counter == 0) {
            PORTBbits.RB0 = 1; // Turn on the LED on RB0
        }
        if (timer_counter == 100) {
            PORTBbits.RB0 = 0; // Turn off the LED on RB0
        }
        if (timer_counter == 200) {
            timer_counter = 0; // Reset the timer counter
        }
    }

    return;
}
