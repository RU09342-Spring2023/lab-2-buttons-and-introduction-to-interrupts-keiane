/*
 * File: OccupancyDetector.c
 * Author/Editor: Keiane Balicanta
 * Course (Section): Embedded Systems (Section 3)
 * Assignment: Lab 2 - Part 4
 * Date: 1 FEB 2023
 * Version: 1.0
 * Description: This is part 4 of the lab 2 is an occupancy door alert system meant to cycle between 3 modes
 * depending on the detection of an individual within specific time frames.
 */

#include <msp430.h>
#define ARMED_STATE 0
#define WARNING_STATE 1
#define ALERT_STATE 2


void gpioInit();

char state = ARMED_STATE;
int x = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    gpioInit();                 // Initialize all GPIO Pins for the project


    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings


    while (1){
        switch(state){
        case ARMED_STATE:
                if(!(P2IN & BIT3)){     // if P2.3 pressed
                    state = WARNING_STATE;  // move into warning state
                    P6OUT &= ~BIT6;         // turns off green LED
                    __delay_cycles(500000);
                }
                else {
                    P6OUT ^= BIT6;                  // toggle green LED
                    P1OUT &= ~BIT0;                 // sets red LED to off
                    __delay_cycles(3000000);        // delay 3 seconds
                }
            break;

        case WARNING_STATE:
            if (!(P2IN & BIT3)){        // if P2.3 pressed
                P1OUT ^= BIT0;      // toggle red LED
                P6OUT &= ~BIT6;     // sets green LED to off
                x++;                // global x variable increments
                if (x == 10){       // if x reaches 10, move to ALERT state
                    state = ALERT_STATE;
                    x = 0;          // resets x to 0
                    __delay_cycles(100000);
                }
                else {
                    state = ARMED_STATE;    // go back to ARMED state if x doesn't equal 10 while button is held down
                    __delay_cycles(100000);
                }
                __delay_cycles(500000);
            }
            else {
                state = ARMED_STATE;        // go back to ARMED_state if button 2.3 is released
                x = 0;
                __delay_cycles(100000);
            }

            break;

        case ALERT_STATE:
            P1OUT |= BIT0;       // turn on red LED to a solid red
            if(!(P4IN & BIT1)){                 // to reset to ARMED_STATE (if P4.1 button pressed)
                state = ARMED_STATE;        // reset to ARMED state
                __delay_cycles(100000);
            }
            break;
        }


    }
}

void gpioInit()
{
    // Setting Directions of Pins

           P1DIR |= BIT0;              // Configure P1.0 to an Output
           P6DIR |= BIT6;              // Configure P6.6 to an Output
           P2DIR &= ~BIT3;             // Configure P2.3 to an Input
           P4DIR &= ~BIT1;             // Configure P4.1 to an Input

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

           P4REN |= BIT1;               // Enable Resistor on P4.1
           P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup
}



/*
 * PSUEDOCODE
 * STATE ARMED:
 *      if(p2.3 pressed){
 *          change to WARNING state
 *      }
 *      else {
 *          blink once every 3 seconds
 *      }
 *
 * STATE WARNING:
 *      if(p2.3 pressed){
 *          start a 10 second timer with x starting at 0 and incrementing to 10
 *          if (timer equal to 10){
 *              change to ALERT state
 *          }
 *          else {
 *              change back to ARMED state
 *          }
 *      }
 *      else {
 *          change back to ARMED state
 *      }
 *
 * STATE ALERT:
 *      if (p4.1 pressed){
 *          change state to ARMED state
 *      }
 *
 */
