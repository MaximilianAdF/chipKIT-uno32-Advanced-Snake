#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */
//#include "\msys64\opt\mcb32tools\include\pic32mx.h"
#include "mipslab.h"  /* Declatations for these labs */
char starter='0';
int current=1;
int current_menu=0;

/* values to determin game setings*/
int snakespeed = 3;
int apples = 1;
int number = 5;
int walls = 1;
int ai = 0;

/* test files difficulty*/
char text[]="     Snake      >    start           modes         highscore    ";
char main_text[]="     Snake      >    start           modes         highscore    ";
char mode_text[]=">  DIFFICULTY        APPELS          WALLS          1 vs AI    ";
char highscore_text[]=">    ___:---         ___:---         ___:---         ___:---     ";
char dificulty_text[]="   Dificulty           V               5               ^     "; 
char apple_text[]="     Apple             V               1               ^     ";
char wall_text[]="     Walls?                     >     off             on  ";
char ai_text[]="   YOU vs AI?                   >      off             on  ";


swap_funk(input, min){
        if(input=='d'){
            text[current*16]=' ';// removes current arrow
            current++;
            if (current > 3){
                current=min;
            }
            text[current*16]='>';// adds new arrow
        }
        else if(input=='u'){
            text[current*16]=' ';// removes
            current--;
            if (current< min){
                current=3;
            }
            text[current*16]='>'; // adds
        }
}

void swap(input){ // moves arrow upp or down
    if(current_menu==0){ // if start menu
        swap_funk(input,1);
    }
    else if(current_menu==3|| current_menu==4){//uppdow menu
        if(input=='d'){
            if(number>1){
                number--;
                text[39]=number + '0';
            }
        }
        if (input=='u'){
            if (number<5){
                number++;
                text[39]=number+ '0';
            }
        }
    }
    else if(current_menu==5||current_menu==6){ //on / off menue
        swap_funk(input,2);
    }

    else{ // if not a unik menu
    swap_funk(input,0);
    }
    display_string(0,text);
    display_update();
}

swap_text(char new_text[]){// swaps the current text with disierd text
    int i;
    for (i = 0; i < 65; i++) { 
        text[i] = new_text[i]; 
    }
    current=0;
    display_string(0,text);
    display_update();
    
}

click(input){
    if(input=='r'){
        if(current_menu==0){ // checks if in start menu
            if (current==1){
                starter='1';
            // game_start;
            }else if (current==2){
                current_menu=1;
                swap_text(mode_text);
            }else if (current==3){
            //highscore_display
                current_menu=2;
                swap_text(highscore_text);
            }
        }
        else if (current_menu==1){// checks if in modes menu
            if (current==0){ // game speed
                current_menu=3;
                number=snakespeed;
                dificulty_text[39]=number+'0';
                swap_text(dificulty_text);
                }

            else if (current==1){ //numbers of apples
                    current_menu=4;
                    number=apples;
                    apple_text[39]=number+'0';
                    swap_text(apple_text);
                }

            else if (current==2){ //no wall
                current_menu=5;
                swap_text(wall_text);
                current=2;
                }

            else if (current==3){ // ai
                current_menu=6;
                swap_text(ai_text);
                current=2;
                }
                }

        else if (current_menu==3){// checks if in game speed menu
            snakespeed= number;
            current_menu=0;
            swap_text(main_text);
            current=1;

            }
        else if (current_menu==4){// checks if in apple menu
            apples= number;
            current_menu=0;
            swap_text(main_text);
            current=1;

            
            }
        else if(current_menu==5|| current_menu==6){
            if(current==3){
                if(current_menu==5){
                    walls=1;
                } else{
                    ai=1;
                }
            }else{
                if(current_menu==5){
                    walls=0;
                } else{
                    ai=0;
                }
            }
            current_menu=0;
            swap_text(main_text);
            current=1;

        }





    }else{ //back
        if(current_menu==1 || current_menu==2){
            current_menu=0;
            swap_text(main_text);
            current=1;
        }
        else if(current_menu==3||current_menu==4||current_menu==5||current_menu==6){
            current_menu = 1;
            swap_text(mode_text);
        }

    }
}

int menu(int *a, int *b, int *c, int *d){

    char input=0;

    display_string(0,text);
	display_update();
    while(starter=='0'){
        input=getbtns(input);
        if (input=='u'|| input=='d'){
            swap(input);
            input=0;
            quicksleep(1000000);
        }else if (input!=0){
            click(input);
            input=0;
            quicksleep(1000000);
        }
    }
    *a = snakespeed;
    *b = apples;
    *c = walls;
    *d = ai;
    return;
}