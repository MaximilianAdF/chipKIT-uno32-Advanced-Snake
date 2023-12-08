/*
två dimetionell map av skärmen 128*32 pixlar
ormen:
    en unik punkt som huvudet och en annan unik punkt som svansen + icke unika mitt mitar
    för flyttar sig en pixel fram per uppdatering,
        fram är definerad genom huvud och föredetta platsen huvudet var på
        svänger genom att få instuktion att nästa steg är till höger eller vänster om huvudet

äpplen:
    random position i mappen som inte är huvud,svans eller krop.

mecaniks:
    titta vad nästa position av huvudet är
        väg: dö
        krop:dö
        svans: inget
        äpple: ta inte bort svans, gå fram med huvud och ta bort äpple+ spawna nytt
förflytnings cykel:
    1. titta vart är "fram"
    2. titta input
    3. titta vart hamnar huvud
        1. om äpple
        2. om vägg
        3. om sig själv
        4. om svans
    4. sätt huvud pixel på nästa steg !! stack!! upp = -128; ned = +128; höger= +1; vänster = -1
    5. ta bort gamla
    6.1 om inget äpple,tabort svans och gör den framför till svans
    6.2 om äpple, gör inget
*/
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <stdio.h>
#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */ ///Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>


#define SIZE 2048
#define appleCount 3     // Define how many apples should be on the display at once
#define snakeSpeed 5     // 1 = 2pixel updates per second, 2 = 4 pixel updates per second....
#define wallInfinite 1   // 1 = Infinite wall, 0 = Walls on
#define opponent 0       // 1 = Opponent on, 0 = Opponent off

int apple_pos[appleCount];           // Array to store the position of the apples
int last_apple = -appleCount;        // Variable to store the pos in array of last apple that was eaten
int front = -1, rear = -1, inp_array[SIZE];
int currScore = 0;
int appleCC = appleCount;
int end = 128*14+2;
int head = 128*14+6;
char temp_v = 'r';
char vektor = 'r';  // r = right, l = left, u = up, d = down
/*
1.kolla input
2.ändra konstanten som adderas
*/


/*
 Generate the outline of the snake game (the walls) 
 incase infinite walls game mode has not been selected
*/



int create_apple(int TMR2copy) {
    int appleX = ((TMR2copy % 61) + 1)*2 + 1;  // Ensures appleX is >= 3, odd, and < 127
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

void generate_walls(){
    if (wallInfinite == 0) {
        int i;
        for ( i = 1; i < 127; i++) {
            bitmap[i+128] = 2;
            bitmap[i+128*30] = 2;
        }
        int j;
        for ( j = 1; j < 31; j++) {
            bitmap[j*128 + 1] = 2;
            bitmap[j*128+126] = 2;
        }
    }
    return;
}

/*
 1. Check if snake has hit itself ('1' in bitmap)
 2. Check if snake has hit wall ('2' in bitmap)
 3. Check if snake has it obstacle ('3' in bitmap)
 4. Check if snake has hit apple ('4' in bitmap)
*/
int check_obstacle(){
    int headX = head%128;
    int headY = head/128;
    if (vektor == 'r') {
        if (bitmap[headY*128 + (headX+2)%128] == 1 || bitmap[headY*128 + (headX+2)%128] == 2 || bitmap[headY*128 + (headX+2)%128] == 3) {
            // Game over
            return 1;
        }
        else if (bitmap[headY*128 + (headX+2)%128] == 4) {
            for (int i = 0; i < appleCount; i++) {
                if (apple_pos[i] == headY*128 + (headX+2)%128) {
                    last_apple = i;
                    break;
                }
            }
            return 4;
        }
    }
    else if (vektor == 'l') {
        if (bitmap[headY*128 + (headX+128-1)%128] == 1 || bitmap[headY*128 + (headX+128-1)%128] == 2 || bitmap[headY*128 + (headX+128-1)%128] == 3) {
            // Game over
            return 1;
        }
        else if (bitmap[headY*128 + (headX+128-1)%128] == 4) {
            // Eat apple
            for (int i = 0; i < appleCount; i++) {
                if (apple_pos[i] == headY*128 + (headX+128-1)%128) {
                    last_apple = i;
                    break;
                }
            }
            return 4;
        }
    }
    else if (vektor == 'u') {
        if (bitmap[((headY+32-1)%32)*128 + headX] == 1 || bitmap[((headY+32-1)%32)*128 + headX] == 2 || bitmap[((headY+32-1)%32)*128 + headX] == 3) {
            // Game over
            return 1;
        }
        else if (bitmap[((headY+32-1)%32)*128 + headX] == 4) {
            // Eat apple
            for (int i = 0; i < appleCount; i++) {
                if (apple_pos[i] == ((headY+32-1)%32)*128 + headX) {
                    last_apple = i;
                    break;
                }
            }
            return 4;
        }
    }
    else if (vektor == 'd') {
        if (bitmap[((headY+2)%32)*128 + headX] == 1 || bitmap[((headY+2)%32)*128 + headX] == 2 || bitmap[((headY+2)%32)*128 + headX] == 3) {
            // Game over
            return 1;
        }
        else if (bitmap[((headY+2)%32)*128 + headX] == 4) {
            // Eat apple
            for (int i = 0; i < appleCount; i++) {
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

void push(int x) {
    if (front == -1) {
        front = 0;
    }
    rear = (rear + 1) % SIZE; // Circular increment to prevent array overflow
    inp_array[rear] = x;
}

int pop() {
    if (front == -1) {
        // Queue is empty, handle the situation accordingly
        return -1; // Assuming -1 is an invalid value; you can choose a different approach
    }

    int poppedElem = inp_array[front];
    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % SIZE; // Circular increment to prevent array overflow
    }
    return poppedElem;
}


void movement_remove() { 
    int endX = end%128;
    int endY = end/128;
    int stored_v = pop();

    if(stored_v == 'l'){
        bitmap[endY*128 + (endX+1)%128]=0;
        bitmap[endY*128 + (endX+1)%128 + 128]=0;
        end=endY*128 + (128+endX-1)%128;
    }
    else if(stored_v == 'r'){
        bitmap[endY*128 + endX]=0;
        bitmap[endY*128 + endX+128]=0; 
        end=endY*128 + (endX+1)%128;
    }   
    else if(stored_v == 'd'){
        bitmap[((endY)%32)*128 + endX]=0;
        bitmap[((endY)%32)*128 + endX+1]=0;
        end=((endY+1)%32)*128 + endX;
    }
    else if(stored_v == 'u'){
        bitmap[((endY+1)%32)*128 + endX]=0;
        bitmap[((endY+1)%32)*128 + endX+1]=0;
        end=((endY+32-1)%32)*128 + endX;
    }
}

int movement(uint8_t button){

    if(button=='l' && vektor != 'r'){
        vektor = button;
    }
    if(button=='r' && vektor != 'l'){
        vektor = button;
    }
    if(button=='u' && vektor != 'd'){
        vektor = button;
    }
    if(button=='d' && vektor != 'u'){
        vektor = button;
    }
    
    int next_step = check_obstacle();
    
    if (next_step == 4) {

        currScore++;
        appleCC++;
    }

    if (next_step!=4 && next_step !=5){
        movement_remove();
    }
    
    if (next_step==1){
        return 1;
    }

    push(vektor);
    int headX = head%128;
    int headY = head/128;
    if (vektor == 'l'){
        bitmap[headY*128 + (128+headX-1)%128]=1;
        bitmap[headY*128 + (128+headX-1)%128 + 128]=1;
        head=headY*128 + (128+headX-1)%128;
    }
    else if(vektor == 'r'){
        bitmap[headY*128 + (headX+2)%128]=1;
        bitmap[headY*128 + (headX+2)%128 + 128]=1;
        head=headY*128 + (headX+1)%128;
    }
    else if(vektor == 'd'){
        bitmap[((headY+2)%32)*128 + headX]=1;
        bitmap[((headY+2)%32)*128 + headX+1]=1;
        head=((headY+1)%32)*128 + headX;

    }
    else if(vektor == 'u'){
        bitmap[((headY+32-1)%32)*128 + headX]=1;
        bitmap[((headY+32-1)%32)*128 + headX+1]=1;
        head=((headY+32-1)%32)*128 + headX;
    }
    return 0;
}
















