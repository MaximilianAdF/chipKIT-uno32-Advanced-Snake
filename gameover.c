#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*everyting after you die*/
int curr=0;
int num=0;
char new_highscore_text[]=" New Highscore       V                AAA:___           ^       ";


char highscore_swap(char text[],int score,int poss){
    // Extract digits one by one
    char buffer[3];
    char buffer2[3];
    int i=0;
    while (i<3) {
        int digit = score % 10;
        buffer[2-i] = digit + '0'; // Convert digit to character
        i++;
        score /= 10;
    }
    for ( i=0; i<3;i++){
        
        text[9+i+poss*16]= buffer[i];
    }
}


char new_highscore(char text[],int score){
    int i;
    int j;
    for( i = 0; i<4;i++){
        int sum=0;
        for(j=0; j<3;j++){
            sum +=text[9+j+i*16]-'0';
        }
        if (score>sum){
            highscore_swap(text,score,i);
            score = sum;
            return ;
        }
    }
}


void gameover(char text[],int score,int oppownent){
    if(oppownent>0){
        if(score<oppownent){
            display_string(0,">   Game over                       YOU LOSE               ");
	        display_update();
        }
        else{
            display_string(0,">   Game over                       YOU WIN               ");
	        display_update();
        }
    }else{
    display_string(0,">   Game over                                        ");
	display_update();
    }
    char x=0;

    while(x=0){
        x=getbtns(0);
    }

    //click ok
    char new_scores[66]=new_highscore(text,score);
    // gå till main screen
}