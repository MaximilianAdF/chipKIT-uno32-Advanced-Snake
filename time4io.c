
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */ //\msys64\opt\mcb32tools\include\pic32mx.h
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>

int getsw( void ){
    return (PORTD >> 8) & 0xF;

};


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