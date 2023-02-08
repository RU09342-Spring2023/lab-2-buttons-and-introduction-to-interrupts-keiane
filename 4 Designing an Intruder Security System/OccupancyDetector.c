#include <msp430.h>
#define ARMED_STATE 0
#define WARNING_STATE 1
#define ALERT_STATE 2


void gpioInit();

/*
 * File: OccupancyDetector.c
 * Author/Editor: Keiane Balicanta
 * Course (Section): Embedded Systems (Section 3)
 * Assignment: Lab 2 - Part 4
 * Date: 1 FEB 2023
 * Version: 1.0
 * Description: This is part 4 of the lab 2 is an occupancy door alert system meant to cycle between 3 modes
 * depending on the detection of an individual within specific timeframes.
 */
char state = ARMED_STATE;
char sensorDetect = 0x01;                       // Global Variable to track if the LED should be on or off
char alertReset = 0x01;                       // Global Variable to track if the LED should be on or off
unsigned int x;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    gpioInit();                 // Initialize all GPIO Pins for the project


    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    __bis_SR_register(GIE);                 // Enter LPM3 w/interrupt

    while (1){
        switch(state){
        case ARMED_STATE:
            P6OUT ^= BIT6;                  // toggle green LED
            if(sensorDetect){               // if sensor detects someone (if P2.3 button pressed)
                state = WARNING_STATE;      // enter warning state

            }
            __delay_cycles(100000);
            break;

        case WARNING_STATE:
            P1OUT ^= BIT0;              // toggle red LED
            // controller stops here. input timer somehow
            __delay_cycles(100000);
            break;

        case ALERT_STATE:
            if(alertReset){                 // to reset to ARMED_STATE (if P4.1 button pressed)
                P1OUT |= BIT0;              // toggle red LED

                state = ARMED_STATE;        // reset to armed state
            }
            __delay_cycles(100000);
            break;
        }


    }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO You might need to modify this based on your approach to the lab
    P2IFG &= ~BIT3;                         // Clear P1.3 IFG
    sensorDetect ^= 0x01;                   // Enable if the toggle should be active

}

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    // @TODO You might need to modify this based on your approach to the lab
    P4IFG &= ~BIT1;                         // Clear P1.3 IFG
    alertReset ^= 0x01;                   // Enable if the toggle should be active

}

void gpioInit()
{
   // Setting Directions of Pins

       P1DIR |= BIT0;              // Configure P1.0 to an Output
       P6DIR |= BIT6;              // Configure P6.6 to an Output
       P2DIR &= ~BIT3;             // Configure P2.3 to an Input
       P4DIR &= ~BIT1;             // Configure P4.1 to an Input

       P1OUT &= ~BIT0;
       P6OUT &= ~BIT6;

   // Configuring Pullup Resistors per MSP430FR2355 Family User Guide
   /*
    *   PXDIR | PXREN | PXOUT | I/O Configuration
    *     0       0       X     Input
    *     0       1       0     Input with Pull Down Resistor
    *     0       1       1     Input With Pull Up Resistor
    *     1       X       X     Output
    */

       P2REN |= BIT3;               // Enable Resistor on P2.3
       P2OUT |= BIT3;               // Configure Resistor on P2.3 to be Pullup
       P2IES &= ~BIT3;                         // P2.3 Low --> High edge
       P2IE |= BIT3;                           // P2.3 interrupt enabled

       P4REN |= BIT1;               // Enable Resistor on P4.1
       P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup
       P4IES &= ~BIT1;                         // P2.3 Low --> High edge
       P4IE |= BIT1;                           // P2.3 interrupt enabled
}



/*
 * PSUEDOCODE
 * Blink once every 3 seconds:
 *      P6OUT ^= BIT6;
 *
 * If (person detected)
 *      Enter warning state:
 *              Stop blinking green
 *              Start blinking red
 *
 *         if (P2IN & BIT3)            // If S2 (P2.3) is pressed
            P6OUT ^= BIT6;          // Toggle P6.6
        if (P4IN & BIT1)            // If S1 (P4.1) is pressed
            P1OUT ^= BIT0;          // Toggle P1.0
 */
