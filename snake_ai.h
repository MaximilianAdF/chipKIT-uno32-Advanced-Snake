char get_direction_to_apple() {
    int appleX, appleY;
    for (int i = 0; i < SIZE; i++) {
        if (bitmap[i] == 4) {
            appleX = i % 128;
            appleY = i / 128;
            break;
        }
    }

    int headX = head % 128;
    int headY = head / 128;

    if (appleX > headX) return 'r';
    if (appleX < headX) return 'l';
    if (appleY > headY) return 'd';
    return 'u';
}

char get_safe_direction() {
    char directions[] = {'r', 'l', 'u', 'd'};
    for (int i = 0; i < 4; i++) {
        vektor = directions[i];
        if (check_obstacle() == 0) {
            return directions[i];
        }
    }
    return 0;  // No safe direction found
}

void ai_move() {
    char direction_to_apple = get_direction_to_apple();
    vektor = direction_to_apple;
    if (check_obstacle() != 0) {
        vektor = get_safe_direction();
    }
    if (vektor != 0) {
        movement(vektor);
    }
}