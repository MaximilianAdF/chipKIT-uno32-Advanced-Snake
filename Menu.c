#include <stdint.h>   /* Declarations of uint_32 and the like */
//#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "\msys64\opt\mcb32tools\include\pic32mx.h"
#include "mipslab.h"  /* Declatations for these labs */
char starter='0';
int select=1;
int current_menu=0;

/* values to determin game setings*/
int map_dificulty = 0;
int apples = 1;
int number = 5;
int walls = 1;
int ai = 0;
int poss=0;
int ai_dif=0;

/* test files difficulty*/
char text[]="     Snake      >    start           modes         highscore    ";
char main_text[]="     Snake      >    start           modes         highscore    ";
char mode_text[]=">    MAPS           APPELS           WALL         YOU vs AI    ";
char highscore_text[]="     ___:000         ___:000         ___:000         ___:000    ";
char dificulty_text[]="      MAP              V               5               ^     "; 
char apple_text[]="     Apple             V               1               ^     ";
char wall_text[]="     Walls?                     >     off             on  ";
char ai_text[]="   YOU vs AI?                   >      off             on  ";
char new_highscore_text[]=" New Highscore       V               AAA:___         ^         ";
char ai_dif_text[]=">    LEVEL 1         LEVEL 2         LEVEL 3         LEVEL 3     ";

char new_highscore(int score){
    int i,j;
    for( i = 0; i<4;i++){
        int p=0;
        int sum=0;
        for(j=0; j<3;j++){
            sum +=highscore_text[9+j+i*16]-'0';
        }
        if (score>sum){
            while (p<3) {
            int digit = score % 10;
            new_highscore_text[43-p] = digit + '0'; // Convert digit to character
            p++;
            score /= 10;
            }
            poss=i;

            swap_text(new_highscore_text);
            current_menu=11;
            number=0;
            return;
        }
    }
    //ominte ett nytt highscore
    current_menu=0;
    swap_text(main_text);
    select=1;
}


char gameover(int score,int winner){
    if(winner==2){
            swap_text("    Game over                       YOU LOSE               ");
        }
    else if(winner==3){
            swap_text("    Game over                       YOU WIN               ");
        }
    else{
    swap_text("    Game over                                        ");
    }
    current_menu=10;
}

swap_funk(input, min){
        if(input=='d'){
            text[select*16]=' ';// removes select arrow
            select++;
            if (select > 3){
                select=min;
            }
            text[select*16]='>';// adds new arrow
        }
        else if(input=='u'){
            text[select*16]=' ';// removes
            select--;
            if (select< min){
                select=3;
            }
            text[select*16]='>'; // adds
        }
}

void swap(input){ // moves arrow upp or down
    if(current_menu==0){ // if start menu
        swap_funk(input,1);
    }
    else if(current_menu==3|| current_menu==4){//uppdow menu
        if(input=='d'){
            if(current_menu==3){
                if(number>0){
                    number--;
                    text[39]=number + '0';
                }
            }
            else{
                if(number>1){
                    number--;
                    text[39]=number + '0';
                }
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

    }else if(current_menu==11){
        if(input=='d'){
            if(number>0){
                number--;
                text[37+select]=number + 'A';
            }
        }
        if (input=='u'){
            if (number<25){
                number++;
                text[37+select]=number+ 'A';
            }
        }

    }

    else{ // if not a unik menu
    swap_funk(input,0);
    }
    display_string(0,text);
    display_update();
}

swap_text(char new_text[]){// swaps the select text with disierd text
    int i;
    for (i = 0; i < 65; i++) { 
        text[i] = new_text[i]; 
    }
    select=0;
    display_string(0,text);
    display_update();
    
}

click(char input,int score){
    if(current_menu==10){
        new_highscore(score);

    }else if(input=='r'){
        if(current_menu==0){ // checks if in start menu
            if (select==1){
                starter='1';
            // game_start;
            }else if (select==2){
                current_menu=1;
                swap_text(mode_text);
            }else if (select==3){
            //highscore_display
                current_menu=2;
                swap_text(highscore_text);
            }
        }
        else if (current_menu==1){// checks if in modes menu
            if (select==0){ // game speed
                current_menu=3;
                number=map_dificulty;
                dificulty_text[39]=number+'0';
                swap_text(dificulty_text);
                }

            else if (select==1){ //numbers of apples
                    current_menu=4;
                    number=apples;
                    apple_text[39]=number+'0';
                    swap_text(apple_text);
                }

            else if (select==2){ //no wall
                current_menu=5;
                swap_text(wall_text);
                select=2;
                }

            else if (select==3){ // ai
                current_menu=6;
                swap_text(ai_text);
                select=2;
                }
        }

        else if (current_menu==3){// checks if in game speed menu
            map_dificulty= number;
            current_menu=0;
            swap_text(main_text);
            select=1;
            }

        else if (current_menu==4){// checks if in apple menu
            apples= number;
            current_menu=0;
            swap_text(main_text);
            select=1;
            }
        else if(current_menu==5|| current_menu==6){ // chacks if in ai or walls menue
            if(select==3){
                if(current_menu==5){
                    walls=1;
                    current_menu=0;
                    swap_text(main_text);
                    select=1;
                } else{
                    ai=1;
                    current_menu=7;
                    swap_text(ai_dif_text);
                }
            }else{
                if(current_menu==5){
                    walls=0;
                } else{
                    ai=0;
                }
                current_menu=0;
                swap_text(main_text);
                select=1;
            }
        }
        else if(current_menu==7){
            ai_dif=3-select;
            current_menu=0;
            swap_text(main_text);
            select=1;
            }

        else if(current_menu==11){
            if(select==2){ //save and go to main menu
                char old_score[7];
                while(poss<4){
                    int i ;
                    for (i=0 ; i<7;i++){
                        old_score[i]=highscore_text[5+(poss*16)+i];
                        highscore_text[5+(poss*16)+i]=text[37+i];
                        text[37+i]=old_score[i];
                        }
                    poss++;
                    }
                current_menu=0;
                swap_text(main_text);
                select=1;
            }else{
                text[37+select]=number+'A';
                text[21+select]=' ';
                text[53+select]=' ';
                text[22+select]='V';
                text[54+select]='^';
                select++;
                number=0;
                display_string(0,text);
                display_update();
            }
        }


    }else{ //back
        if(current_menu==1 || current_menu==2){
            current_menu=0;
            swap_text(main_text);
            select=1;
        }
        else if(current_menu==3||current_menu==4||current_menu==5||current_menu==6){
            current_menu = 1;
            swap_text(mode_text);
        }
    }
}


int menu(int *a, int *b, int *c, int *d, int score, char game,char winner,int*e){
    int temp=0;
    char input=0;
    if(game!=0){ // om ett spel har körts
        gameover(score,winner);
        game=0;
    }else{
    display_string(0,text);
	display_update();
    }
    


    while(starter=='0'){
        input=getbtns(input);
        if (input=='u'|| input=='d'){
            swap(input);
            input=0;
            quicksleep(1000000);
        }else if (input!=0){
            click(input,score);
            input=0;
            quicksleep(1000000);
        }
    }
    starter='0';
    *a = map_dificulty;
    *b = apples;
    *c = walls;
    *d = ai;
    *e = ai_dif;
    return;
}