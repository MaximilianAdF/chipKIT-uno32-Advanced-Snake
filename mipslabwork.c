/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "\msys64\opt\mcb32tools\include\pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "snake.h"
int prime = 1234567;    
int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";


int timeoutcount=0;


void next_update(char dead) {
    if (dead == 1) {          // Snake has collided, dead

    }
    else if (dead == 0) {     // Apple has been eaten
        movement();
    } else {                  // Nothing has changed
        movement();
        movement_remove();
    }
}


/* Interrupt Service Routine */
void user_isr( void ) {
    if (IFS(0) & 0x0100) {
        uint8_t btn = getbtns(btn);
        char dead = check_obstacle(); // Check if snake has collided with something

        if(timeoutcount==5){
            next_update(dead);
        }
        
        if (timeoutcount==10){
            next_update(dead);
            pixel_update(btn);
            
            btn = 0;  //Reset btn to fetch new value for next move
            timeoutcount=0;
            }
        IFS(0)&= ~(1 << 8);
        TMR2 = 0;//Set T2IF to 0 - Clear T2 Int. flag.
        timeoutcount++;
    }

    if(IFS(0) & 0x0800){
        IFS(0)&= ~(1 << 11);
    }
    return;
}


/* Lab-specific initialization goes here */
void labinit( void )
{
    push('r');
    push('r');
    push('r');
    push('r');
    push('r');
    push('r');
    volatile unsigned int* TRISE_p = (volatile unsigned int*)0xBF886100;
    TRISDSET = 0xFFF;
    *TRISE_p &= ~0xF;

    T2CON = 0; // Clear Timer2 control register
    T2CONSET = 0x70 ; // Set prescaler to 1:256 (bits 6-4)
    PR2 = 31250;
    TMR2 = 0;
  
    IFS(0) =0;
    IEC(0) = 0x0100; //Timer 2 interrupt enable T2IE
    IPC(2) |= 0x1F; //T2IP
    IPC(2) |= 0x1C0;

    enable_interrupt();


    T2CONSET = 0x8000; // Enable Timer2

    return;
}


/* This function is called repetitively from the main program */

void labwork( void ) {
    display_bit_update();
}

