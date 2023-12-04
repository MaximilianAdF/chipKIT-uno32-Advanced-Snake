#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int current=1;
int current_menu=0;
char curr_line_1[]="  start";
char curr_line_2[]="  mode";
char curr_line_3[]="  highscore";
void swap(input){
    if(current_menu==0){
        if (input=='d'){
            if(current==1){
                
            }
            
        }
    }
    if (input=='d'){
        if(current ==1){
            curr_line_1[0]=' ';
            display_string(1, curr_line_1);
            curr_line_2[0]='>';
	        display_string(2, curr_line_2);
            display_update();
            current =2;
        }
        else if(current ==2){
            curr_line_2[0]=' ';
            display_string(2, curr_line_2);
            curr_line_3[0]='>';
            display_string(3, curr_line_3);
            display_update();
            current=3;
        }
        else if(current ==3){
            curr_line_1[0]='>';
            display_string(1, curr_line_1);
            curr_line_3[0]=' ';
            display_string(3, curr_line_3);
            display_update();
            current=1;
        }
    }
    if (input=='u'){
        if(current == 1){
            curr_line_1[0]=' ';
            display_string(1, curr_line_1);
            curr_line_3[0]='>';
            display_string(3, curr_line_3);
            display_update();
            current=3;
        }
        else if (current == 3){
            curr_line_2[0]='>';
            display_string(2, curr_line_2);
            curr_line_3[0]=' ';
            display_string(3, curr_line_3);
            display_update();
            current=2;
        }
        else if (current == 2){
            curr_line_1[0]='>';
            display_string(1, curr_line_1);
            curr_line_2[0]=' ';
            display_string(2, curr_line_2);
            display_update();
            current =1;
        }
    }
}

click(){
    if (current==1){
       // game_start;
    }else if (current==2)
    {
       // mode_display;
    }else if (current==3)
    {
        //highscore_display
    }
}
char input=0;
void menu(){
    display_string(0, "Snake");
	display_string(1, "> start");
	display_string(2, "  mode");
	display_string(3, "  highscore");
	display_update();
    while(1){
        input=getbtns(input);
        if (input!=0){
            swap(input);
            click();
            input=0;
            delay(500);
        }
    }
}