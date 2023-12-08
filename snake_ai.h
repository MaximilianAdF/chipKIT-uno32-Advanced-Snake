#include <stdint.h>
#include "/Applications/mcb32tools.app/Contents/Resources/Toolchain/include/pic32mx.h"
#include "mipslab.h"
#include <stdlib.h>
#include "snake.h"

// Need a function that checks for possible movements at all time to make sure not boxed in and can still move somewhere
// Needs to run 24/7 so that it can react to player movement, runs independently of apple_proximity.
// Add a flag to apple_proximity so that we don't add unnecessary "safe movements" when we have a path to an apple.

char get_direction(int head, int apple_pos, int vektor) {
    //Add wallInfinite = 1 or 0 functionality
    int headX = head%128;
    int headY = head/128;
    int appleX = apple_pos%128;
    int appleY = apple_pos/128;

    if (headX < appleX && vektor != 'l') {
        return 'r';
    } else if (headX > appleX && vektor != 'r') {
        return 'l';
    } else if (headY < appleY && vektor != 'd') {
        return 'u';
    } else if (headY > appleY && vektor != 'u') {
        return 'd';
    }
}

void find_path(int AI_head, int apple_pos, int totalDiffPlayer, int totalDiffAI, char AI_vektor) {
    //If obstacle in front of AI, find path around it, add extra movements to totalDiffAI and check that still smaller than totalDiffPlayer
    //Should we also find path for a player and compare the two??
    
    int player_pos = player_head;
    int AI_pos = AI_head;
    while (AI_pos != apple_pos && totalDiffAI < totalDiffPlayer) { // Check if apple has been reached or if AI is further away than player
        char travel_dir = get_direction(player_pos, apple_pos, player_vektor); // Get directions to apple for player
        char travel_dir_AI = get_direction(AI_pos, apple_pos, AI_vektor); // Get directions to apple for AI
        
    }
}

char apple_proximity(int AI_head, char AI_vektor, int wallInfinite) {
    //Add wallInfinite = 1 or 0 functionality
    int AIheadX = AI_head%128;
    int AIheadY = AI_head/128;
    int playerHeadX = player_head%128;
    int playerHeadY = player_head/128;

    int i = 0;
    for (i; i < appleCount; i++) {
        int appleX = apple_pos[i]%128;
        int appleY = apple_pos[i]/128;



        //Calculate distance between AI head and apple (wallInfinite = 0)
        int xDiffAI = appleX - AIheadX;
        int yDiffAI = appleY - AIheadY;
        int totalDiffAI = xDiffAI + yDiffAI;

        //Calculate distance between player head and apple (wallInfinite = 0)
        int xDiffPlayer = appleX - playerHeadX;
        int yDiffPlayer = appleY - playerHeadY;
        int totalDiffPlayer = xDiffPlayer + yDiffPlayer;

        if (wallInfinite == 1) {

        }

        //If AI is closer to apple than player, find path to apple
        if (totalDiffAI < totalDiffPlayer) {
            find_path(AI_head, apple_pos[i], totalDiffPlayer, totalDiffAI, AI_vektor);
        }
    }
}

