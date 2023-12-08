/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
//#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "\msys64\opt\mcb32tools\include\pic32mx.h"
#include "mipslab.h"  /* Declatations for these labs */
#include "snake_ai.h"
#include <stdlib.h>


int prime = 1234567;    
int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

int AI;
int wallInfinite;   // 1 = Infinite wall, 0 = Walls on
int snakeSpeed;     // 1 = 2pixel updates per second, 2 = 4 pixel updates per second....

int timeoutcount=0;
char btn = 'r';
int player_dead;
int AI_dead;

char getbtns(char btn){
   uint8_t btn1 = (PORTF >> 1) & 0x1;
   uint8_t btn234 = (PORTD >> 5) & 0x7;
   
   if (btn1){
      return 'r';
   }
   if (btn234 & 0x1){
      return 'd';
   }
   if (btn234 & 0x2){
      return 'u';
   }
   if (btn234 & 0x4){
      return 'l';
   }
   return btn;
}

/* Interrupt Service Routine */
void user_isr( void ) {
    if (IFS(0) & 0x0100 && player_dead!=1) { 
        if(timeoutcount==(6-snakeSpeed)){
            if (AI == 1) {
                char dir = apple_proximity(AI_head, AI_vektor, wallInfinite);
                AI_dead = movement(dir, &AI_head, &AI_end, &AI_vektor, 1);
            }
            player_dead=movement(btn, &player_head, &player_end, &player_vektor, 0);
        }
        
        if (timeoutcount==(6-snakeSpeed)*2){
            if (AI == 1) {
                AI_dead = movement(0, &AI_head, &AI_end, &AI_vektor, 1);
            }
            player_dead=movement(0, &player_head, &player_end, &player_vektor, 0);
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

void game_init(int speed, int apples, int walls, int ai){
    last_apple = -apples;
    appleCount = apples;
    appleCC = apples;
    AI = ai;
    
    snakeSpeed = speed;

    init_snake(player_end); //Create the player's snake
    if (walls==1){
        generate_walls();
    }
    if (ai == 1) {
        init_snake(AI_head); //Create the AI's snake
        int i = 0;
        for (i; i < 4; i++) { //Initial movements that AI does (account initial tail)
            push('l', ai_prev_movm, &ai_front, &ai_rear);
        }
    }

    int i = 0;
    for (i; i < 4; i++) { //Initial movements that player does (account initial tail)
        push('r', player_prev_movm, &player_front, &player_rear);
    }
    player_dead=0;
    AI_dead=0;
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

