
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "\msys64\opt\mcb32tools\include\pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int getsw( void ){
    return (PORTD >> 8) & 0xF;

 }


 int getbtns(int btn){
   uint8_t btnVal = (PORTD >> 5) & 0x7;
   if (btnVal & 0x1 || btnVal & 0x2 || btn == 0) {
      return btnVal;
   } else {
      return btn;
   }
}