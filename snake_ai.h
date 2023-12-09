#include <stdint.h>
//#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"
#include "\msys64\opt\mcb32tools\include\pic32mx.h"
#include "mipslab.h"
#include <stdlib.h>
#include "snake.h"

// Need a function that checks for possible movements at all time to make sure not boxed in and can still move somewhere
// Needs to run 24/7 so that it can react to player movement, runs independently of apple_proximity.
// Add a flag to apple_proximity so that we don't add unnecessary "safe movements" when we have a path to an apple.

int currApple; //The position of the apple that the AI is currently going for
int minDist = 4096; //The distance between the AI head and the apple that the AI is currently going for
int flag = 0; //Flag to check if we have found an apple that is closer to the AI than the player

char* get_safe_moves(int head) {
    char temp[4] = {'0', '0', '0', '0'};
    if (bitmap[head+2] == 0) {
        temp[0] = 'r';
    }
    if (bitmap[head-1] == 0) {
        temp[1] = 'l';
    }
    if (bitmap[head-128] == 0) {
        temp[2] = 'u';
    }
    if (bitmap[head+2*128] == 0) {
        temp[3] = 'd';
    }
    return temp;
}

char go_center(int head, char vektor) {
    //When apple is further away from AI than player, go towards center of board (coords 2048)
    int centerX = 64;
    int centerY = 15;

    int headX = head%128;
    int headY = head/128;

    char* safeMoves = get_safe_moves(head);
    if (headX < centerX) { //To the left of center
        if (headY == centerY && vektor == 'l') {
            if (safeMoves[2] == 'u') {
                return 'u';
            } else if (safeMoves[3] == 'd') {
                return 'd';
            }
        }
        if (vektor != 'l') {
            return 'r';
        }
    }
    if (headX > centerX) { //To the right of center
        if (headY == centerY && vektor == 'r') {
            if (safeMoves[2] == 'u') {
                return 'u';
            } else if (safeMoves[3] == 'd') {
                return 'd';
            }
        }
        if (vektor != 'r') {
            return 'l';
        }
    }
    if (headY < centerY) { //Above center
        if (headX == centerX && vektor == 'u') {
            if (safeMoves[0] == 'r') {
                return 'r';
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
        if (vektor != 'u') {
            return 'd';
        }
    }
    if (headY > centerY) { //Below center
        if (headX == centerX && vektor == 'd') {
            if (safeMoves[0] == 'r') {
                return 'r';
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
        if (vektor != 'd') {
            return 'u';
        }
    }
}

char get_direction(int head, int final_pos, int vektor) {
    //Add wallInfinite = 1 or 0 functionality
    int headX = head%128;
    int headY = head/128;
    int finalX = final_pos%128;
    int finalY = final_pos/128;

    char* safeMoves = get_safe_moves(head); //r, l, u, d
    //If AI's X coordinate is smaller than the apple's X coordinate => (AI is to the left of the apple, needs to go right)
    if (headX < finalX) {
        if (vektor == 'r' && safeMoves[0] == 'r') { // If already travelling to right, continue.
            return vektor;
        }
        if (vektor == 'l' && headY == finalY) { // On same y level but travelling in wrong direction, need to flip (go up or down)
            if (safeMoves[2] == 'u') {
                return 'u';
            } else if (safeMoves[3] == 'd') {
                return 'd';
            } else { // No option but to continue going left
                return vektor;
            }
        }
        if (vektor != 'l' && safeMoves[0] == 'r') { // If not travelling left and can go right, go right
            return 'r';
        }
    }
    
    //If AI's X coordinate is larger than the apple's X coordinate => (AI is to the right of the apple, needs to go left)
    if (headX > finalX) {
        if (vektor == 'l' && safeMoves[1] == 'l') { // If already travelling to left, continue.
            return vektor;
        }
        if (vektor == 'r' && headY == finalY) { // On same y level but travelling in wrong direction, need to flip (go up or down)
            if (safeMoves[2] == 'u') {
                return 'u';
            } else if (safeMoves[3] == 'd') {
                return 'd';
            } else { // No option but to continue going right
                return vektor;
            }
        }
        if (vektor != 'r' && safeMoves[1] == 'l') { // If not travelling right and can go left, go left
            return 'l';
        }
    }

    //If AI's Y coordinate is smaller than the apple's Y coordinate => (AI is above the apple, needs to go down)
    if (headY < finalY) {
        if (vektor == 'd' && safeMoves[3] == 'd') { // If already travelling down, continue.
            return vektor;
        }
        if (vektor == 'u' && headX == finalX) { // On same x level but travelling in wrong direction, need to flip (go left or right)
            if (safeMoves[0] == 'r') {
                return 'r';
            } else if (safeMoves[1] == 'l') {
                return 'l';
            } else { // No option but to continue going up
                return vektor;
            }
        }
        if (vektor != 'u' && safeMoves[3] == 'd') { // If not travelling up and can go down, go down
            return 'd';
        }
    }

    //If AI's Y coordinate is larger than the apple's Y coordinate => (AI is below the apple, needs to go up)
    if (headY > finalY) {
        if (vektor == 'u' && safeMoves[2] == 'u') { // If already travelling up, continue.
            return vektor;
        }
        if (vektor == 'd' && headX == finalX) { // On same x level but travelling in wrong direction, need to flip (go left or right)
            if (safeMoves[0] == 'r') {
                return 'r';
            } else if (safeMoves[1] == 'l') {
                return 'l';
            } else { // No option but to continue going down
                return vektor;
            }
        }
        if (vektor != 'd' && safeMoves[2] == 'u') { // If not travelling down and can go up, go up
            return 'u';
        }
    }

    if (headX == finalX && headY == finalY) {
        return go_center(head, vektor);
    }
    return vektor; //If no other option, continue in same direction (Most likely dead)
}

char apple_proximity(int AI_head, char AI_vektor, int wallInfinite) {
    //Add wallInfinite = 1 or 0 functionality
    int AIheadX = AI_head%128;
    int AIheadY = AI_head/128;
    int playerHeadX = player_head%128;
    int playerHeadY = player_head/128;

    int i = 0;
    for (i; i < appleCount; i++) {
        if (apple_pos[i] == 0) {
            continue;
        }
        if (bitmap[currApple] != 4) {
            minDist = 4096;
            flag = 0;
        }
        if (wallInfinite == 1) {
            int appleX = apple_pos[i]%128;
            int appleY = apple_pos[i]/128;
            
            int diffX = AIheadX - appleX;
            int diffY = AIheadY - appleY;

            if (diffX > 66 || diffX < 66) { // Faster to travel through wall if distance is greater than 66 since then we have time to turn and still have less distance

            }
            if (diffY > 18 || diffY < 18) { // Faster to travel through wall if distance is greater than 18 since then we have time to turn and still have less distance

            }



        } else {
            //Calculate distance between player head and apple (wallInfinite = 0)
            int totalDiffPlayer = player_head - apple_pos[i];
            if (totalDiffPlayer < 0) {
                totalDiffPlayer = -totalDiffPlayer;
            }  
            //Calculate distance between AI head and apple (wallInfinite = 0)
            int totalDiffAI = AI_head - apple_pos[i];    
            if (totalDiffAI < 0) {
                totalDiffAI = -totalDiffAI;
            }


            if (totalDiffAI < minDist && totalDiffAI < totalDiffPlayer) {
                currApple = apple_pos[i];
                minDist = totalDiffAI;
                flag = 1;
            }
        }
    }
    if (flag == 1) {
        return get_direction(AI_head, currApple, AI_vektor);
    }
    return go_center(AI_head, AI_vektor);
}

