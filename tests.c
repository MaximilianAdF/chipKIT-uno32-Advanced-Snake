#include <stdlib.h>
#include <stdio.h>


int main(){

    for (int i = 0; i < 10000; i++) {
        int appleX = ((i % 125) + 3) & ~1;  // Ensures appleX is >= 3, even, and < 127
        int appleY = ((i % 29) + 3) & ~1;   // Ensures appleY is >= 3, even, and < 31
        printf("%d\n", appleX);
        printf("%d\n", appleY);
    }

    return 0;
}