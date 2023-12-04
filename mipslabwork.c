/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "snake.h"
#include <stdlib.h>

int prime = 1234567;    
int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";


int timeoutcount=0;
char btn = 'r';
int dead=1;
/* Interrupt Service Routine */
void user_isr( void ) {
    if (IFS(0) & 0x0100 && dead!=1) { 
        if(timeoutcount==(8-snakeSpeed)){
            dead=movement(0);
        }
        
        if (timeoutcount==(8-snakeSpeed)*2){
            dead=movement(btn);
            timeoutcount=0;
            }
        IFS(0)&= ~(1 << 8);
        TMR2 = 0;//Set T2IF to 0 - Clear T2 Int. flag.
        timeoutcount++;
    }


    return;
}


/* Lab-specific initialization goes here */
void labinit( void )
{
    volatile uint32_t *TRISE_ptr = (volatile uint32_t*)0xbf886100;
    *TRISE_ptr &= ~0xff;
    TRISDSET = 0xFF;

    T2CON = 0; // Clear Timer2 control register
    T2CONSET = 0x70; // Set prescaler to 1:256 (bits 6-4)
    PR2 = 6250;
    TMR2 = 0;
  
    IFS(0) =0;
    IEC(0) = 0x0100; //Timer 2 interrupt enable T2IE
    IPC(2) |= 0x1F; //T2IP
    IPC(2) |= 0x1C0;

    enable_interrupt();

    T2CONSET = 0x8000; // Enable Timer2

    return;
}
void game_init(void){
    generate_walls();
    push('r');
    push('r');
    push('r');
    push('r');
    dead=0;

}

/* This function is called repetitively from the main program */



void labwork( void ) {
    volatile uint32_t *PORTE_ptr = (volatile uint32_t*)0xbf886110; //Define leds
    *PORTE_ptr = currScore; //Update the leds with score

    int TMR2copy = TMR2;
    if (appleCC > 0) {
        create_apple(TMR2copy);
    }
    btn = getbtns(btn);
    display_bit_update();
}

