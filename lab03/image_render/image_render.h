
#include <stdint.h>
#include <stdio.h>

// initializes a few of our necessary variables and starts the screen black
void image_render_init();

// prints the screen black and resets the cursor to the top left corner of the screen
void image_render_print_black_screen();

// prints the first part of the game over screen
// this will display "GAME OVER" and "Enter your name" and a place to enter the player name
void image_render_print_game_over_screen_enter_name();

// prints the second part of the game over screen
// this will display "GAME OVER" and "High Scores" and the list of high scores
void image_render_print_game_over_screen_high_scores();

void image_render_print_start_screen();

void image_render_update_screen();

// this is simply a test to see if we could print some characters... which we can!
void image_render_test_image();

// closes the hdmi connection
void image_render_close();
