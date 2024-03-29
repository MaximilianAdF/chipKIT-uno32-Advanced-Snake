/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   modified 2023-12-10 by Alfred Malmström


   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */


#include <stdint.h>   /* Declarations of uint_32 and the like */
//#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "\msys64\opt\mcb32tools\include\pic32mx.h"
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>

int main(void) {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;


	display_init();

	int num1,num2,num3,num4,num5;
	int game = 0;
	int score = 0;
	char winner=0;
	while(1){
		
		menu(&num1, &num2, &num3, &num4,score,game,winner,&num5);
		winner=0;
		game_init(num1, num2, num3, num4,&game,num5);
		score = 0;
		labinit(); /* Do any lab-specific initialization */
		
		
			while( winner==0 )
			{
				winner=labwork(&score); /* Do lab-specific things again and again */
			}
		
		disable_interrupt();
		game_reset();
		}	
	return 0;
}
