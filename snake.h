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
#include "\msys64\opt\mcb32tools\include\pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */


#define appleCount 1    // Define how many apples should be on the display at once
#define snakeSpeed 1    // 1 = 2pixel updates per second, 2 = 4 pixel updates per second....
#define wallInfinite 0  // 1 = Infinite wall, 0 = Walls on
#define obstacles 0     // 1 = Obstacles on, 0 = Obstacles off
#define opponent 0      // 1 = Opponent on, 0 = Opponent off


int end = 128*15;
int head = 128*15+6;
char temp_v = 'r';
char vektor = 'r';
/*
1.kolla input
2.ändra konstanten som adderas

*/


/*
 Generate the outline of the snake game (the walls) 
 incase infinite walls game mode has not been selected
*/
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

}

/*
 1. Check if snake has hit itself ('1' in bitmap)
 2. Check if snake has hit wall ('2' in bitmap)
 3. Check if snake has it obstacle ('3' in bitmap)
 4. Check if snake has hit apple ('4' in bitmap)
*/
int check_obstacle(){
    if (vektor == 'r') {
        if (bitmap[head+2] == 1 || bitmap[head+2] == 2 || bitmap[head+2] == 3) {
            // Game over
            return 1;
        }
        else if (bitmap[head+2] == 4) {
            // Eat apple
            return 0;
        }
    }
    else if (vektor == 'l') {
        if (bitmap[head-1] == 1 || bitmap[head-1] == 2 || bitmap[head-1] == 3) {
            // Game over
            return 1;
        }
        else if (bitmap[head-1] == 4) {
            // Eat apple
            return 0;
        }
    }
    else if (vektor == 'u') {
        if (bitmap[head-128] == 1 || bitmap[head-128] == 2 || bitmap[head-128] == 3) {
            // Game over
            return 1;
        }
        else if (bitmap[head-128] == 4) {
            // Eat apple
            return 0;
        }
    }
    else if (vektor == 'd') {
        if (bitmap[head+128*2] == 1 || bitmap[head+128*2] == 2 || bitmap[head+128*2] == 3) {
            // Game over
            return 1;
        }
        else if (bitmap[head+128*2] == 4) {
            // Eat apple
            return 0;
        }
    }
    return;
}


/*
 Apples are defined as '4' in the bitmap.
  
*/



#define SIZE 1025
int front = -1, rear = -1, inp_array[SIZE];

void push(int x) {
    if (front == -1){
        front = 0;
    }
    rear = rear + 1;
    inp_array[rear] = x;
}

int pop() {
    int poppedElem = inp_array[front];
    if (front == rear) { 
        front = rear = -1;
    } else {
        front = front + 1;
    }
    return poppedElem;
}





void pixel_update(button){
    if(button & 0x2){ // Left
      if(vektor=='l'){
         temp_v = 'd';
      }
      if(vektor=='d'){
         temp_v = 'r';
      }
      if(vektor=='r'){
         temp_v = 'u';
      }
      if(vektor=='u'){
         temp_v = 'l';
      }
    }
   else if(button & 0x1){ // Right
      if(vektor=='l'){
         temp_v = 'u';
      }
      if(vektor=='u'){
         temp_v = 'r';
      }
      if(vektor=='r'){
         temp_v = 'd';
      }
      if(vektor=='d'){
         temp_v = 'l';
      }
    }

    
}


void movement(){
    vektor=temp_v;
    push(vektor);
    if(vektor == 'l'){
        bitmap[head-1]=1;
        bitmap[head-1+128]=1;
        head=head -1;
    }
    else if(vektor == 'r'){
        bitmap[head+2]=1;
        bitmap[head+2+128]=1;
        head=head +1;
    }
    else if(vektor == 'd'){
        bitmap[head+128*2]=1;
        bitmap[head+1+128*2]=1;
        head=head +128;

    }
    else if(vektor == 'u'){
        bitmap[head-128]=1;
        bitmap[head+1-128]=1;
        head=head -128;
    }
}

void movement_remove() { 
    int stored_v = pop();
    if(stored_v == 'l'){
        bitmap[end+1]=0;
        bitmap[end+1+128]=0;
        end=end -1;
    }
    else if(stored_v == 'r'){
        bitmap[end]=0;
        bitmap[end+128]=0;
        end=end +1;
    }
    else if(stored_v == 'd'){
        bitmap[end]=0;
        bitmap[end+1]=0;
        end=end +128;
    }
    else if(stored_v == 'u'){
        bitmap[end+128]=0;
        bitmap[end+1+128]=0;
        end=end -128;
    }
}














