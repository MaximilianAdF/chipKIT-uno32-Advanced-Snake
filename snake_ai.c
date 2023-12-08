#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"  /* Declarations of system-specific addresses etc */ ///Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>
#include <snake.h>


// Need a function that checks for possible movements at all time to make sure not boxed in and can still move somewhere
// Needs to run 24/7 so that it can react to player movement, runs independently of apple_proximity.
// Add a flag to apple_proximity so that we don't add unnecessary "safe movements" when we have a path to an apple.

#define SIZE 2048
int front = -1, rear = -1, dir_array[SIZE];     //dir_array is the array that stores the directions of the snake


void push(int x) {
    if (front == -1) {
        front = 0;
    }
    rear = (rear + 1) % SIZE; // Circular increment to prevent array overflow
    dir_array[rear] = x;
}

int pop() {
    if (front == -1) {
        // Queue is empty, handle the situation accordingly
        return -1; // Assuming -1 is an invalid value; you can choose a different approach
    }

    int poppedElem = dir_array[front];
    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % SIZE; // Circular increment to prevent array overflow
    }
    return poppedElem;
}


void generate_opponent() {
    int headAI = 128 * 15 - 8;      //AI snake head
    int endAI = 128 * 15 - 4;       //AI snake tail
    char vektorAI = 'l';            //Travel direction upon init

}

char apple_proximity(int headAI, char vektorAI) {
    int AIheadX = headAI%128;
    int AIheadY = headAI/128;
    int playerHeadX = head%128;
    int playerHeadY = head/128;

    for (int i = 0; i < appleCount; i++) {
        int appleX = apple_pos[i]%128;
        int appleY = apple_pos[i]/128;

        //Calculate distance between AI head and apple
        int xDiffAI = abs(appleX - AIheadX);
        int yDiffAI = abs(appleY - AIheadY);
        int totalDiffAI = xDiffAI + yDiffAI;

        //Calculate distance between player head and apple
        int xDiffPlayer = abs(appleX - playerHeadX);
        int yDiffPlayer = abs(appleY - playerHeadY);
        int totalDiffPlayer = xDiffPlayer + yDiffPlayer;

        //If AI is closer to apple than player, find path to apple
        if (totalDiffAI < totalDiffPlayer) {
            find_path(headAI, apple_pos[i], totalDiffPlayer, totalDiffAI);
        }
    }
}
void find_path(int headAi, int apple_pos, int totalDiffPlayer, int totalDiffAI) {
    //If obstacle in front of AI, find path around it, add extra movements to totalDiffAI and check that still smaller than totalDiffPlayer
    //Should we also find path for a player and compare the two??
}

char get_direction(int headAI) {

}

int main(int argc, char const *argv[])
{
    generate_opponent();
    return 0;
}
