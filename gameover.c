#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*everyting after you die*/
int curr=0;
int num=0;
char Highscore_text[]=" New Highscore       V                AAA:___           ^       ";
void gameover(int score,int oppownent){
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
	display_update();}

    //click ok

    char current_scores[66]=read();
    char new_scores[66]=new_highscore(current_scores);
    write(new_scores);
    // gå till main screen
}




char write(){

}

char read() {
    FILE *filePointer;
    char ch;
    char highscores[66];
    int i=0;
    // Open the file in read mode
    filePointer = fopen("highscore.txt", "r");
    // Check if the file opened successfully
    if (filePointer == NULL) {
        printf("Unable to open the file.\n");
        return 1; // Exit the program with an error code
    }
    // Read and put the content in a list the content character by character
    while ((ch = fgetc(filePointer)) != EOF) {
        highscores[i]=ch;
        i++;
    }
    // Close the file
    fclose(filePointer);
    return highscores; // Exit the program successfully
}


char highscore_swap(char *highscores[],int score,int poss){
    // Extract digits one by one
    char buffer[3];
    int i=0;
    while (i<3) {
        int digit = score % 10;
        buffer[2-i] = digit + '0'; // Convert digit to character
        i++;
        score /= 10;
    }
    for (int i=0; i<3;i++){
        *highscores[9+i+poss*16]= buffer[i];
    }
}


char new_highscore(score){
    char highscores[]=read();
    for(int i = 0; i<4;i++){
        int sum=0;
        for(int j=0; j<3;j++){
            sum +=atoi(highscores[9+j+i*16])*pow(10,2-j);
        }
        if (score>sum){
            highscore_swap(&highscores,score,i);
            score = sum;
        }
    }
    return highscores;
}