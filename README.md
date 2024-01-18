# Snake Game for ChipKIT Uno32 - C/Assembly Implementation

## Objective and Requirements

Embark on a challenging adventure with our Snake Game developed for the ChipKIT Uno32 board. The primary goal of this project is to recreate the classic "snake" game, where players navigate a growing snake through a pixelated world, consuming apples while avoiding collisions with walls and the snake's own tail. The project's main requirements include two primary game modes (singleplayer and player vs AI), a highscore system, adjustable difficulty levels, control using ChipKIT's built-in buttons, LED display for the current score, pixel-by-pixel graphical updates for the snake, different obstacles based on difficulty, customizable apple count, and the option for wall-less gameplay.

## Solution

The game leverages the ChipKIT Uno32 board and the Basic I/O shield, with the snake displayed on the 128x32 pixel screen. The rectangular display created several challenges along the way, specifically for the user, such as the snake's movement feeling faster vertically than horizontally. The snake's movement is controlled using the board's buttons (BTN1, BTN2, BTN3, BTN4), corresponding to left, up, down, and right directions. The game's refresh rate relies on TMR2 interrupts to ensure smooth display updates. The optimal refresh rate was determined to be 2hz meaning that the snake moves 2 pixels per second. The implementation is in the C and Assembly language, compiled using MCB32tools.

## Verification

We meticulously tested the game through various combinations of game modes and inputs on the ChipKIT board, identifying and addressing potential issues throughout the development. Extensive testing ensured that the game met its intended functionality. We documented problems and conflicts encountered during development, resolving them to guarantee a seamless gaming experience.

## Contributions

The project's development was a collaborative effort between Maximilian and Alfred. Maximilian focused on extending snake movements, collision checks, apple creation, player vs AI mode, and the current score system. Alfred handled the initial movement solution, highscore system, main menu, and death/win screens. Both team members contributed to different maps, difficulty levels, and the walls on/off functionality.

## Reflections

The Snake Game project provided an enjoyable and demanding experience, deepening our understanding of the ChipKIT Uno32 Board and I/O Shield's functionalities and enhancing our proficiency in C programming. The development journey, though time-consuming, resulted in a valuable learning experience, shedding light on the complexities and intricacies of game development in a constrained environment. While certain optional features were left out due to time constraints, the project significantly contributed to our familiarity with C/Assembly programming and hardware interfaces.
