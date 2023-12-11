


#include <stdint.h>   /* Declarations of uint_32 and the like */
//#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */ ///Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h
#include "\msys64\opt\mcb32tools\include\pic32mx.h"
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>


#define SIZE 1024

int apple_pos[5] = {0,0,0,0,0};         // Array to store all the positions of the apples
int appleCount;                         // Initial appleCount set from main menu
int last_apple;                         // Variable to store the pos in array of last apple that was eaten
int appleCC;                            // The amount of apples that need to be generated to reach appleCount

int player_front = -1, player_rear = -1; 
char player_prev_movm[SIZE];
int currScore = 0;

int player_head = 128 * 14 + 6; // Player snake pos  
int player_end = 128 * 14 + 2;  // Player snake tail
char player_vektor = 'r';       // r = right, l = left, u = up, d = down

int ai_front = -1, ai_rear = -1;
char ai_prev_movm[SIZE];

int AI_head = 128 * 14 + 120;      //AI snake pos
int AI_end = 128 * 14 + 124;       //AI snake tail
char AI_vektor = 'l';            // r = right, l = left, u = up, d = down

int ai_score=0;

/*
 Generate the outline of the snake game (the walls) 
 incase infinite walls game mode has not been selected
*/
void generate_walls(){
    int i;
    for ( i = 1; i < 127; i++) {
        bitmap[i+128] = 1;
        bitmap[i+128*30] = 1;
    }
    int j;
    for ( j = 1; j < 31; j++) {
        bitmap[j*128 + 1] = 1;
        bitmap[j*128+126] = 1;
    }
}

/*
 Generates obstacle at with the top left corner at tl
 4x4 obstacles with hollow 2x2 centers.
*/
void generate_bomb(int tl) {
    bitmap[tl+1+128]=1;
    bitmap[tl+2+2*128]=1;
    int i;
    //Bottom & top
    for (i = 0; i < 4;i++) {
        bitmap[tl+i] = 1;
        bitmap[tl+i+3*128] = 1;
    }
    int j;
    //Left & right
    for (j = 1; j < 3;j++) {
        bitmap[tl+j*128] = 1;
        bitmap[tl+j*128+3] = 1;
    }}


/*
 Generates a map with different difficulties depending on the input from menu
 Ranges from map 1 (standard map) to map 5 (hardest map)
*/
void generate_map(int map){
    if(map==1) { 
        generate_bomb(60+12*128);
        
    }
    if(map==2) {
        generate_bomb(62+128*4);
        generate_bomb(62+24*128);
    }
    if(map==3){
        generate_bomb(62+128*4);
        generate_bomb(62+24*128);
        generate_bomb(62+128*8);
        generate_bomb(62+20*128);
    }if(map==4){
        int i;
        for (i=0;i<2;i++){
        generate_bomb(32+128*4 +60*i);
        generate_bomb(32+24*128+60*i);
        generate_bomb(32+128*8+60*i);
        generate_bomb(32+20*128+60*i);
        }
    }if(map==5){
        int i;
        for (i=0;i<2;i++){
        generate_bomb(32+128*4 +60*i);
        generate_bomb(32+24*128+60*i);
        generate_bomb(32+128*8+60*i);
        generate_bomb(32+20*128+60*i);
        }
        for(i=0;i<4;i++){
            generate_bomb(54+12*128+i*4);
            generate_bomb(54+16*128+i*4);
        
        }
        
    }

}

/*
 Pushes an element to the queue
*/
void push(int x, char arr[], int *front, int *rear) {
    if (*front == -1) {
        *front = 0;
        *rear = 0;
    }
    *rear = (*rear + 1) % SIZE; // Circular increment to prevent array overflow
    arr[*rear] = x;
}

/*
 Pops an element from the queue
*/
int pop(char arr[], int *front, int *rear) {
    if (*front == -1) {
        return -1; // Assuming -1 is an invalid value; you can choose a different approach
    }

    int poppedElem = arr[*front];
    if (*front == *rear) {
        *front = *rear = -1;
    } else {
        *front = (*front + 1) % SIZE; // Circular increment to prevent array overflow
    }
    return poppedElem;
}

/*
 Initializes the snake at the position pos
*/
void init_snake(int pos) {
    int i = 0;
    for (i; i < 6; i++) {
        bitmap[pos+i] = 1;
        bitmap[pos+i+128] = 1;
    }
}

/*
 Creates an apple (2x2) at a random position on the map using the value of TMR2 as seed
*/
int create_apple(int TMR2copy) {
    int appleX = ((TMR2copy % 61) + 1)*2;  // Ensures appleX is >= 3, odd, and < 127
    int appleY = ((TMR2copy % 13) + 1)*2;   // Ensures appleY is >= 2, even, and < 31
 
    if (bitmap[appleX+appleY*128] != 0 || bitmap[appleX+appleY*128+1] != 0 || bitmap[appleX+appleY*128+128] != 0 || bitmap[appleX+appleY*128+1+128] != 0) {
        return 0;
    } else {
        bitmap[appleX+appleY*128] = 4; 
        bitmap[appleX+appleY*128+1] = 5;
        bitmap[appleX+appleY*128+128] = 5;
        bitmap[appleX+appleY*128+1+128] = 5;
        if (last_apple < 0) {  //Initialize all apples (Start of game)
            apple_pos[appleCount+last_apple] = appleX+appleY*128;
            last_apple++;
        } else {
            apple_pos[last_apple] = appleX+appleY*128;
        }
        appleCC--;
        return 1;
    }
}

/*
 Checks if the snake has collided with an obstacle in the next pixel update
*/
int check_obstacle(int *pos, char *vektor){
    int headX = *pos%128;
    int headY = *pos/128;
    if (*vektor == 'r') {
        if (bitmap[headY*128 + (headX+2)%128] == 1 || bitmap[headY*128 + (headX+2)%128] == 2) {
            // Game over
            return 1;
        }
        else if (bitmap[headY*128 + (headX+2)%128] == 4) {
            int i = 0;
            for (i; i < appleCount; i++) {
                if (apple_pos[i] == headY*128 + (headX+2)%128) {
                    last_apple = i;
                    break;
                }
            }
            return 4;
        }
    }
    else if (*vektor == 'l') {
        if (bitmap[headY*128 + (headX+128-1)%128] == 1 || bitmap[headY*128 + (headX+128-1)%128] == 2) {
            // Game over
            return 1;
        }
        else if (bitmap[headY*128 + (headX+128-1)%128] == 4) {
            // Eat apple
            int i = 0;
            for (i; i < appleCount; i++) {
                if (apple_pos[i] == headY*128 + (headX+128-1)%128) {
                    last_apple = i;
                    break;
                }
            }
            return 4;
        }
    }
    else if (*vektor == 'u') {
        if (bitmap[((headY+32-1)%32)*128 + headX] == 1 || bitmap[((headY+32-1)%32)*128 + headX] == 2) {
            // Game over
            return 1;
        }
        else if (bitmap[((headY+32-1)%32)*128 + headX] == 4) {
            // Eat apple
            int i = 0;
            for (i; i < appleCount; i++) {
                if (apple_pos[i] == ((headY+32-1)%32)*128 + headX) {
                    last_apple = i;
                    break;
                }
            }
            return 4;
        }
    }
    else if (*vektor == 'd') {
        if (bitmap[((headY+2)%32)*128 + headX] == 1 || bitmap[((headY+2)%32)*128 + headX] == 2) {
            // Game over
            return 1;
        }
        else if (bitmap[((headY+2)%32)*128 + headX] == 4) {
            // Eat apple
            int i = 0;
            for (i; i < appleCount; i++) {
                if (apple_pos[i] == ((headY+2)%32)*128 + headX) {
                    last_apple = i;
                    break;
                }
            }
            return 4;
        }
    }
    return 0;
}

/*
 Removes the last pixels (2x1) of the snake and updates the end position of the snake
*/
void movement_remove(int *end, int AI) { 
    int endX = *end%128;
    int endY = *end/128;
    int stored_v;
    if (AI == 1) { // AI movement
        stored_v = pop(ai_prev_movm, &ai_front, &ai_rear);
    } else {       // Player movement
        stored_v = pop(player_prev_movm, &player_front, &player_rear);
    }

    if(stored_v == 'l'){
        bitmap[endY*128 + (endX+1)%128]=0;
        bitmap[endY*128 + (endX+1)%128 + 128]=0;
        *end=endY*128 + (128+endX-1)%128;
    }
    else if(stored_v == 'r'){
        bitmap[endY*128 + endX]=0;
        bitmap[endY*128 + endX+128]=0; 
        *end=endY*128 + (endX+1)%128;
    }   
    else if(stored_v == 'd'){
        bitmap[((endY)%32)*128 + endX]=0;
        bitmap[((endY)%32)*128 + endX+1]=0;
        *end=((endY+1)%32)*128 + endX;
    }
    else if(stored_v == 'u'){
        bitmap[((endY+1)%32)*128 + endX]=0;
        bitmap[((endY+1)%32)*128 + endX+1]=0;
        *end=((endY+32-1)%32)*128 + endX;
    }
}

/*
 Updates the snake's head postion (2x1) 
*/
int movement(uint8_t button, int *pos, int *end, char *vektor, int AI){

    if(button=='l' && *vektor != 'r'){
        *vektor = button;
    }
    if(button=='r' && *vektor != 'l'){
        *vektor = button;
    }
    if(button=='u' && *vektor != 'd'){
        *vektor = button;
    }
    if(button=='d' && *vektor != 'u'){
        *vektor = button;
    }
    
    int next_step = check_obstacle(pos, vektor);
    
    if (next_step == 4) {
        if (AI != 1) {
            currScore++;
        }else if (AI==1){
            ai_score++;
        }
        appleCC++;
    }

    if (next_step!=4 && next_step !=5){
        movement_remove(end, AI);
    }
    
    if (next_step==1){
        return 1;
    }
    if (AI == 1) {
        push(*vektor, ai_prev_movm, &ai_front, &ai_rear);
    } else {
        push(*vektor, player_prev_movm, &player_front, &player_rear);
    }
    int headX = *pos%128;
    int headY = *pos/128;
    if (*vektor == 'l'){
        bitmap[headY*128 + (128+headX-1)%128]=1;
        bitmap[headY*128 + (128+headX-1)%128 + 128]=1;
        *pos=headY*128 + (128+headX-1)%128;
    }
    else if(*vektor == 'r'){
        bitmap[headY*128 + (headX+2)%128]=1;
        bitmap[headY*128 + (headX+2)%128 + 128]=1;
        *pos=headY*128 + (headX+1)%128;
    }
    else if(*vektor == 'd'){
        bitmap[((headY+2)%32)*128 + headX]=1;
        bitmap[((headY+2)%32)*128 + headX+1]=1;
        *pos=((headY+1)%32)*128 + headX;

    }
    else if(*vektor == 'u'){
        bitmap[((headY+32-1)%32)*128 + headX]=1;
        bitmap[((headY+32-1)%32)*128 + headX+1]=1;
        *pos=((headY+32-1)%32)*128 + headX;
    }
    return 0;
}
















