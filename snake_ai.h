#include <stdint.h>
//#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"
#include "\msys64\opt\mcb32tools\include\pic32mx.h"
#include "mipslab.h"
#include <stdlib.h>
#include "snake.h"

char safeMoves[4] = {'0', '0', '0', '0'};
int currApple = 2048; //The position of the apple that the AI is currently going for
int minDist = 4096; //The distance between the AI head and the apple that the AI is currently going for

void get_safe_moves(int head) {
    int headX = head % 128;
    int headY = head / 128;

    if (bitmap[headY*128 + (headX+2)%128] != 1) {
        safeMoves[0] = 'r';
    } else {
        safeMoves[0] = '0';
    }

    if (bitmap[headY*128 + (headX+128-1)%128] != 1) {
        safeMoves[1] = 'l';
    } else {
        safeMoves[1] = '0';
    }

    if (bitmap[((headY+32-1)%32)*128 + headX] != 1) {
        safeMoves[2] = 'u';
    } else {
        safeMoves[2] = '0';
    }

    if (bitmap[((headY+2)%32)*128 + headX] != 1) {
        safeMoves[3] = 'd';
    } else {
        safeMoves[3] = '0';
    }
}

char go_center(int head, char vektor) {
    // Constants
    int centerX = 64;
    int centerY = 15;

    // Head coordinates
    int headX = head % 128;
    int headY = head / 128;

    // Update get safe moves
    get_safe_moves(head);

    // Logic to go towards the center
    if (headX < centerX) { // To the left of center
        if (headY == centerY) {
            if (safeMoves[2] == 'u') {
                return 'u';
            } else if (safeMoves[3] == 'd') {
                return 'd';
            }
        }
        if (safeMoves[0] == 'r' && vektor != 'l') {
            return 'r';
        }
    } else if (headX > centerX) { // To the right of center
        if (headY == centerY) {
            if (safeMoves[2] == 'u') {
                return 'u';
            } else if (safeMoves[3] == 'd') {
                return 'd';
            }
        }
        if (safeMoves[1] == 'l' && vektor != 'r') {
            return 'l';
        }
    } else if (headY < centerY) { // Above center
        if (headX == centerX) {
            if (safeMoves[0] == 'r') {
                return 'r';
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
        if (safeMoves[2] == 'u' && vektor != 'd') {
            return 'u';
        }
    } else if (headY > centerY) { // Below center
        if (headX == centerX) {
            if (safeMoves[0] == 'r') {
                return 'r';
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
        if (safeMoves[3] == 'd' && vektor != 'u') {
            return 'd';
        }
    }
    return vektor;
}

char get_direction(int head, int final_pos, int vektor, int wallInfinite) {
    //Add wallInfinite = 1 or 0 functionality
    int finalX = final_pos%128;
    int finalY = final_pos/128;
    int headX = head%128;
    int headY = head/128;
    int distX = headX - finalX;
    int distY = headY - finalY;
    printf("headX: %d, headY: %d, finalX: %d, finalY: %d, distX: %d, distY: %d\n", headX, headY, finalX, finalY, distX, distY);

    char* safeMoves = get_safe_moves(head); //r, l, u, d
    if (wallInfinite == 0) {
        //distX > 64 => (headX > finalX meaning AI is to the right and need to travel through right wall since distance is larger than half the board)
        if (distX > 64) { 
            if (vektor == 'r' && safeMoves[0] == 'r') { // If already travelling to right, continue.
                return vektor;
            }
            if (vektor == 'l' || vektor == 'r') {
                if (safeMoves[2] == 'u' && distY >= 0) {
                    return 'u';
                } else if (safeMoves[3] == 'd' && distY < 0) {
                    return 'd';
                } else { // No option but to continue going left/right
                    return vektor;
                }
            }
            if (vektor != 'l' && safeMoves[0] == 'r') { // If not travelling left and can go right, go right
                return 'r';
            }
        }

        //distX < -64 => (headX < finalX meaning AI is to the left and need to travel through left wall since distance is larger than half the board)
        if (distX < -64) {
            if (vektor == 'l' && safeMoves[1] == 'l') { // If already travelling to left, continue.
                return vektor;
            }
            if (vektor == 'r' || vektor == 'l') { // On same y level but travelling in wrong direction, need to flip (go up or down)
                if (safeMoves[2] == 'u' && distY >= 0) {
                    return 'u';
                } else if (safeMoves[3] == 'd' && distY < 0) {
                    return 'd';
                } else { // No option but to continue going left/right
                    return vektor;
                }
            }
            if (vektor != 'r' && safeMoves[1] == 'l') { // If not travelling right and can go left, go left
                return 'l';
            }
        }

        //distY > 16 => (headY > finalY meaning AI is below and need to travel through bottom wall since distance is larger than half the board)
        if (distY > 16) {
            if (vektor == 'd' && safeMoves[3] == 'd') { // If already travelling down, continue.
                return vektor;
            }
            if ((vektor == 'u' || vektor == 'd')) { // On same x level but travelling in wrong direction, need to flip (go left or right)
                if (safeMoves[0] == 'r' && distX < 0) {
                    return 'r';
                } else if (safeMoves[1] == 'l' && distX >= 0) {
                    return 'l';
                } else { // No option but to continue going up/down
                    return vektor;
                }
            }
            if (vektor != 'u' && safeMoves[3] == 'd') { // If not travelling up and can go down, go down
                return 'd';
            }
        }

        //distY < -16 => (headY < finalY meaning AI is above and need to travel through top wall since distance is larger than half the board)
        if (distY < -16) {
            if (vektor == 'u' && safeMoves[2] == 'u') { // If already travelling up, continue.
                return vektor;
            }
            if ((vektor == 'd' || vektor == 'u')) { // On same x level but travelling in wrong direction, need to flip (go left or right)
                if (safeMoves[0] == 'r' && distX < 0) {
                    return 'r';
                } else if (safeMoves[1] == 'l' && distX >= 0) {
                    return 'l';
                } else { // No option but to continue going up/down
                    return vektor;
                }
            }
            if (vektor != 'd' && safeMoves[2] == 'u') { // If not travelling down and can go up, go up
                return 'u';
            }
        }
    }

    //If AI's X coordinate is smaller than the apple's X coordinate => (AI is to the left of the apple, needs to go right)
    if (headX < finalX) {
        if (vektor == 'r') { // Can only go up, down or right
            if (safeMoves[0] == 'r') { // If already travelling to right, continue.
                return vektor;
            } else if (safeMoves[2] == 'u') { //If can go up
                if (safeMoves[3] != 'd') { //If down is not an option, must go up regardless of path deviation
                    return 'u';
                } else if (distY >= 0) { // Apple is upwards, go up.
                    return 'u';
                } else { // Apple is downwards, go down.
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {//If can go down
                return 'd'; //Doesn't matter if apple is up/down.
            }
        }
        if (vektor == 'l') { // Can only go up, down or left
            if (safeMoves[1] == 'l') { // If already travelling to left, continue.
                return vektor;
            } else if (safeMoves[2] == 'u') { //If can go up
                if (safeMoves[3] != 'd') { //If down is not an option, must go up regardless of path deviation
                    return 'u';
                } else if (distY >= 0) { // Apple is upwards, go up.
                    return 'u';
                } else { // Apple is downwards, go down.
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {//If can go down
                return 'd'; //Doesn't matter if apple is up/down.
            }
        }
        if (vektor == 'u') { // Can only go right, left or up
            if (safeMoves[2] == 'u') { // If already travelling to up, continue.
                return vektor;
            } else if (safeMoves[0] == 'r') { //If can go right
                if (safeMoves[1] != 'l') { //If left is not an option, must go right regardless of path deviation
                    return 'r';
                } else if (distX >= 0) {
                    return 'l';
                } else { 
                    return 'r';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
        if (vektor == 'd') {
            if (safeMoves[3] == 'd') { // If already travelling to down, continue.
                return vektor;
            } else if (safeMoves[0] == 'r') { //If can go right
                if (safeMoves[1] != 'l') { //If left is not an option, must go right regardless of path deviation
                    return 'r';
                } else if (distX >= 0) {
                    return 'l';
                } else { 
                    return 'r';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
    }
    
    //If AI's X coordinate is larger than the apple's X coordinate => (AI is to the right of the apple, needs to go left)
    if (headX > finalX) {
        if (vektor == 'r') { // Can only go up, down or right
            if (safeMoves[0] == 'r') { // If already travelling to right, continue.
                return vektor;
            } else if (safeMoves[2] == 'u') { //If can go up
                if (safeMoves[3] != 'd') { //If down is not an option, must go up regardless of path deviation
                    return 'u';
                } else if (distY >= 0) { // Apple is upwards, go up.
                    return 'u';
                } else { // Apple is downwards, go down.
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {//If can go down
                return 'd'; //Doesn't matter if apple is up/down.
            }
        }
        if (vektor == 'l') { // Can only go up, down or left
            if (safeMoves[1] == 'l') { // If already travelling to left, continue.
                return vektor;
            } else if (safeMoves[2] == 'u') { //If can go up
                if (safeMoves[3] != 'd') { //If down is not an option, must go up regardless of path deviation
                    return 'u';
                } else if (distY >= 0) { // Apple is upwards, go up.
                    return 'u';
                } else { // Apple is downwards, go down.
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {//If can go down
                return 'd'; //Doesn't matter if apple is up/down.
            }
        }
        if (vektor == 'u') { // Can only go right, left or up
            if (safeMoves[2] == 'u') { // If already travelling to up, continue.
                return vektor;
            } else if (safeMoves[0] == 'r') { //If can go right
                if (safeMoves[1] != 'l') { //If left is not an option, must go right regardless of path deviation
                    return 'r';
                } else if (distX >= 0) {
                    return 'l';
                } else { 
                    return 'r';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
        if (vektor == 'd') {
            if (safeMoves[3] == 'd') { // If already travelling to down, continue.
                return vektor;
            } else if (safeMoves[0] == 'r') { //If can go right
                if (safeMoves[1] != 'l') { //If left is not an option, must go right regardless of path deviation
                    return 'r';
                } else if (distX >= 0) {
                    return 'l';
                } else { 
                    return 'r';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
    }

    //If AI's Y coordinate is smaller than the apple's Y coordinate => (AI is above the apple, needs to go down)
    if (headY < finalY) {
        if (vektor == 'r') { // Can only go up, down or right
            if (safeMoves[0] == 'r') { // If already travelling to right, continue.
                return vektor;
            } else if (safeMoves[2] == 'u') { //If can go up
                if (safeMoves[3] != 'd') { //If down is not an option, must go up regardless of path deviation
                    return 'u';
                } else if (distY >= 0) { // Apple is upwards, go up.
                    return 'u';
                } else { // Apple is downwards, go down.
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {//If can go down
                return 'd'; //Doesn't matter if apple is up/down.
            }
        }
        if (vektor == 'l') { // Can only go up, down or left
            if (safeMoves[1] == 'l') { // If already travelling to left, continue.
                return vektor;
            } else if (safeMoves[2] == 'u') { //If can go up
                if (safeMoves[3] != 'd') { //If down is not an option, must go up regardless of path deviation
                    return 'u';
                } else if (distY >= 0) { // Apple is upwards, go up.
                    return 'u';
                } else { // Apple is downwards, go down.
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {//If can go down
                return 'd'; //Doesn't matter if apple is up/down.
            }
        }
        if (vektor == 'u') { // Can only go right, left or up
            if (safeMoves[2] == 'u') { // If already travelling to up, continue.
                return vektor;
            } else if (safeMoves[0] == 'r') { //If can go right
                if (safeMoves[1] != 'l') { //If left is not an option, must go right regardless of path deviation
                    return 'r';
                } else if (distX >= 0) {
                    return 'l';
                } else { 
                    return 'r';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
        if (vektor == 'd') {
            if (safeMoves[3] == 'd') { // If already travelling to down, continue.
                return vektor;
            } else if (safeMoves[0] == 'r') { //If can go right
                if (safeMoves[1] != 'l') { //If left is not an option, must go right regardless of path deviation
                    return 'r';
                } else if (distX >= 0) {
                    return 'l';
                } else { 
                    return 'r';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
    }

    //If AI's Y coordinate is larger than the apple's Y coordinate => (AI is below the apple, needs to go up)
    if (headY > finalY) {
        if (vektor == 'r') { // Can only go up, down or right
            if (safeMoves[0] == 'r') { // If already travelling to right, continue.
                return vektor;
            } else if (safeMoves[2] == 'u') { //If can go up
                if (safeMoves[3] != 'd') { //If down is not an option, must go up regardless of path deviation
                    return 'u';
                } else if (distY >= 0) { // Apple is upwards, go up.
                    return 'u';
                } else { // Apple is downwards, go down.
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {//If can go down
                return 'd'; //Doesn't matter if apple is up/down.
            }
        }
        if (vektor == 'l') { // Can only go up, down or left
            if (safeMoves[1] == 'l') { // If already travelling to left, continue.
                return vektor;
            } else if (safeMoves[2] == 'u') { //If can go up
                if (safeMoves[3] != 'd') { //If down is not an option, must go up regardless of path deviation
                    return 'u';
                } else if (distY >= 0) { // Apple is upwards, go up.
                    return 'u';
                } else { // Apple is downwards, go down.
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {//If can go down
                return 'd'; //Doesn't matter if apple is up/down.
            }
        }
        if (vektor == 'u') { // Can only go right, left or up
            if (safeMoves[2] == 'u') { // If already travelling to up, continue.
                return vektor;
            } else if (safeMoves[0] == 'r') { //If can go right
                if (safeMoves[1] != 'l') { //If left is not an option, must go right regardless of path deviation
                    return 'r';
                } else if (distX >= 0) {
                    return 'l';
                } else { 
                    return 'r';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
        if (vektor == 'd') {
            if (safeMoves[3] == 'd') { // If already travelling to down, continue.
                return vektor;
            } else if (safeMoves[0] == 'r') { //If can go right
                if (safeMoves[1] != 'l') { //If left is not an option, must go right regardless of path deviation
                    return 'r';
                } else if (distX >= 0) {
                    return 'l';
                } else { 
                    return 'r';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            }
        }
    }
    return vektor; //If no other option, continue in same direction (Most likely dead)
}

char init_ai(int AI_head, char AI_vektor, int wallInfinite) {
    int totalDiffPlayer;
    int totalDiffAI;


    int i = 0;
    for (i; i < appleCount; i++) {
        if (apple_pos[i] == 0) {
            continue;
        }
        if (bitmap[currApple] != 4) {
            minDist = 4096;
            flag = 0;
        }

        //Calculate distance between player head and apple (wallInfinite = 0,1)
        totalDiffPlayer = min_dist(player_head, apple_pos[i], wallInfinite);
        //Calculate distance between AI head and apple (wallInfinite = 0,1)
        totalDiffAI = min_dist(AI_head, apple_pos[i], wallInfinite);  

        //if (totalDiffAI < minDist && totalDiffAI < totalDiffPlayer) {
        if (totalDiffAI < minDist) {
            currApple = apple_pos[i];
            minDist = totalDiffAI;
        }
    }
    return get_direction(AI_head, currApple, AI_vektor, wallInfinite);
}

