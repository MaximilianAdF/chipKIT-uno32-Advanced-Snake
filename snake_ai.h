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

char get_direction(int head, int final_pos, int vektor, int walls) {
    //Add walls = 1 or 0 functionality
    int finalX = final_pos%128;
    int finalY = final_pos/128;
    int headX = head%128;
    int headY = head/128;
    int distX = headX - finalX;
    int distY = headY - finalY;

    get_safe_moves(head); //r, l, u, d
    if (walls == 0) {
        //distX > 64 => (headX > finalX meaning AI is to the right and need to travel through right wall since distance is larger than half the board)
        if (distX > 64) { 
            if (vektor == 'r' && safeMoves[0] == 'r') { // If already travelling to right, continue.
                return vektor;
            }
            if (vektor == 'l' || vektor == 'r') {
                if (safeMoves[2] == 'u') {
                    if (safeMoves[3] != 'd') {
                        return 'u';
                    }
                    else if (-16 <= distY <= 0) {
                        return 'd';
                    }
                    else if (distY < -16) {
                        return 'u';
                    }
                    else if ( 0 < distY <= 16) {
                        return 'u';
                    }
                    else if (distY > 16) {
                        return 'd';
                    }
                } else if (safeMoves[3] == 'd') {
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
                if (safeMoves[2] == 'u') {
                    if (safeMoves[3] != 'd') {
                        return 'u';
                    }
                    else if (-16 <= distY <= 0) {
                        return 'd';
                    }
                    else if (distY < -16) {
                        return 'u';
                    }
                    else if ( 0 < distY <= 16) {
                        return 'u';
                    }
                    else if (distY > 16) {
                        return 'd';
                    }
                } else if (safeMoves[3] == 'd') {
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
            if (vektor == 'u' || vektor == 'd') { // On same x level but travelling in wrong direction, need to flip (go left or right)
                if (safeMoves[0] == 'r') {
                    if (safeMoves[1] != 'l') {
                        return 'r';
                    }
                    else if (-64 <= distX <= 0) { // AI to the left of apple and faster to go right than through left wall
                        return 'r';
                    }
                    else if (distX < -64) { // AI to the left of apple and faster to go through left wall
                        return 'l';
                    }
                    else if ( 0 < distX <= 64) { // AI to the right of apple and faster to go left than through right wall
                        return 'l';
                    }
                    else if (distX > 64) { // AI to the right of apple and faster to go through right wall
                        return 'r';
                    }
                } else if (safeMoves[1] == 'l') {
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
            if (vektor == 'd' || vektor == 'u') { // On same x level but travelling in wrong direction, need to flip (go left or right)
                if (safeMoves[0] == 'r') {
                    if (safeMoves[1] != 'l') {
                        return 'r';
                    }
                    else if (-64 <= distX <= 0) { // AI to the left of apple and faster to go right than through left wall
                        return 'r';
                    }
                    else if (distX < -64) { // AI to the left of apple and faster to go through left wall
                        return 'l';
                    }
                    else if ( 0 < distX <= 64) { // AI to the right of apple and faster to go left than through right wall
                        return 'l';
                    }
                    else if (distX > 64) { // AI to the right of apple and faster to go through right wall
                        return 'r';
                    }

                } else if (safeMoves[1] == 'l') {
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
        if (vektor == 'r' && safeMoves[0] == 'r') { // If already travelling to right, continue.
            return vektor;
        }
        if (vektor != 'l' && safeMoves[0] == 'r') { // If not travelling left and can go right, go right
            return 'r';
        }
        if ((vektor == 'l' || vektor == 'r')) { // On same y level but travelling in wrong direction, need to flip (go up or down)
            if (safeMoves[2] == 'u') {
                if (safeMoves[3] != 'd') {
                    return 'u';
                }
                else if (distY >= 0) {
                    return 'u';
                } 
                else {
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {
                return 'd';
            } else { // No option but to continue going left
                return vektor;
            }
        }
    }
    
    //If AI's X coordinate is larger than the apple's X coordinate => (AI is to the right of the apple, needs to go left)
    if (headX > finalX) {
        //Add logic when travelling up/down and can't go left
        if (vektor == 'l' && safeMoves[1] == 'l') { // If already travelling to left, continue.
            return vektor;
        }
        if (vektor != 'r' && safeMoves[1] == 'l') { // If not travelling right and can go left, go left
            return 'l';
        }
        if ((vektor == 'r' || vektor == 'l')) { // On same y level but travelling in wrong direction, need to flip (go up or down)
            if (safeMoves[2] == 'u') {
                if (safeMoves[3] != 'd') {
                    return 'u';
                }
                else if (distY >= 0) {
                    return 'u';
                } 
                else {
                    return 'd';
                }
            } else if (safeMoves[3] == 'd') {
                return 'd';
            } else { // No option but to continue going right
                return vektor;
            }
        }
    }

    //If AI's Y coordinate is smaller than the apple's Y coordinate => (AI is above the apple, needs to go down)
    if (headY < finalY) {
        if (vektor == 'd' && safeMoves[3] == 'd') { // If already travelling down, continue.
            return vektor;
        }
        if (vektor != 'u' && safeMoves[3] == 'd') { // If not travelling up and can go down, go down
            return 'd';
        }
        if ((vektor == 'u' || vektor == 'd')) { // On same x level but travelling in wrong direction, need to flip (go left or right)
            if (safeMoves[0] == 'r') {
                if (safeMoves[1] != 'l') {
                    return 'r';
                }
                else if (distX < 0) {
                    return 'r';
                } 
                else {
                    return 'l';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            } else { // No option but to continue going down
                return vektor;
            }
        }
    }

    //If AI's Y coordinate is larger than the apple's Y coordinate => (AI is below the apple, needs to go up)
    if (headY > finalY) {
        if (vektor == 'u' && safeMoves[2] == 'u') { // If already travelling up, continue.
            return vektor;
        }
        if (vektor != 'd' && safeMoves[2] == 'u') { // If not travelling down and can go up, go up
            return 'u';
        }
        if ((vektor == 'd' || vektor == 'u')) { // On same x level but travelling in wrong direction, need to flip (go left or right)
            if (safeMoves[0] == 'r') {
                if (safeMoves[1] != 'l') {
                    return 'r';
                }
                else if (distX < 0) {
                    return 'r';
                } 
                else {
                    return 'l';
                }
            } else if (safeMoves[1] == 'l') {
                return 'l';
            } else { // No option but to continue going down
                return vektor;
            }
        }
    }
    return vektor; //If no other option, continue in same direction (Most likely dead)
}

int min_dist(int head, int final_pos, int walls) {
    //walls = 1 or 0 functionality, finds the shortest distance between two points on the board
    int headX = head%128;
    int headY = head/128;
    int finalX = final_pos%128;
    int finalY = final_pos/128;

    int diffX = headX - finalX;
    if (diffX < 0) {
        diffX = -diffX;
    }
    int diffY = headY - finalY;
    if (diffY < 0) {
        diffY = -diffY;
    }

    if (walls == 0) {
        if (diffX > 64) {
            diffX = 128 - diffX;
        }
        if (diffY > 16) {
            diffY = 32 - diffY;
        }
    }

    return diffX + diffY;
}

char init_ai(int AI_head, char AI_vektor, int walls) {
    int totalDiffAI;

    int i = 0;
    for (i; i < appleCount; i++) {
        if (apple_pos[i] == 0) {
            continue;
        }
        if (bitmap[currApple] != 4) {
            minDist = 4096;
        }
        //Calculate distance between AI head and apple (walls = 0,1)
        totalDiffAI = min_dist(AI_head, apple_pos[i], walls);  

        //if (totalDiffAI < minDist && totalDiffAI < totalDiffPlayer) {
        if (totalDiffAI < minDist) {
            currApple = apple_pos[i];
            minDist = totalDiffAI;
        }
    }
    return get_direction(AI_head, currApple, AI_vektor, walls);
}

