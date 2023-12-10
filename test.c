#include <stdio.h>
char bitmap[4096] = {};

char* get_safe_moves(int head) {
    static char temp[4] = {'0', '0', '0', '0'};
    if (bitmap[head+2] != 1) {
        temp[0] = 'r';
    }
    if (bitmap[head-1] != 1) {
        temp[1] = 'l';
    }
    if (bitmap[head-128] != 1) {
        temp[2] = 'u';
    }
    if (bitmap[head+2*128] != 1) {
        temp[3] = 'd';
    }
    return temp;
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
        if (vektor == 'r' && safeMoves[0] == 'r') { // If already travelling to right, continue.
            return vektor;
        }
        if (vektor != 'l' && safeMoves[0] == 'r') { // If not travelling left and can go right, go right
            return 'r';
        }
        if ((vektor == 'l' || vektor == 'r')) { // On same y level but travelling in wrong direction, need to flip (go up or down)
            if (safeMoves[2] == 'u' && distY >= 0) {
                return 'u';
            } else if (safeMoves[3] == 'd' && distY < 0) {
                return 'd';
            } else { // No option but to continue going left
                return vektor;
            }
        }
    }
    
    //If AI's X coordinate is larger than the apple's X coordinate => (AI is to the right of the apple, needs to go left)
    if (headX > finalX) {
        if (vektor == 'l' && safeMoves[1] == 'l') { // If already travelling to left, continue.
            return vektor;
        }
        if (vektor != 'r' && safeMoves[1] == 'l') { // If not travelling right and can go left, go left
            return 'l';
        }
        if ((vektor == 'r' || vektor == 'l')) { // On same y level but travelling in wrong direction, need to flip (go up or down)
            if (safeMoves[2] == 'u' && distY >= 0) {
                return 'u';
            } else if (safeMoves[3] == 'd' && distY < 0) {
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
            if (safeMoves[0] == 'r' && distX < 0) {
                return 'r';
            } else if (safeMoves[1] == 'l' && distX >= 0) {
                return 'l';
            } else { // No option but to continue going up
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
            if (safeMoves[0] == 'r' && distX < 0) {
                return 'r';
            } else if (safeMoves[1] == 'l' && distX >= 0) {
                return 'l';
            } else { // No option but to continue going down
                return vektor;
            }
        }
    }
    return vektor; //If no other option, continue in same direction (Most likely dead)
}

int main() {
    int currPos = 128 * 14 + 120;
    int finalPos = 128 * 4 + 4;

    char l = get_direction(128*12 + 132, finalPos, 'r', 0);
    printf("%c\n", l);
    return 0;
}
