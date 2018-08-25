#include <stdint.h>
#include <stdio.h>

// initializes a few of our necessary variables and starts the screen black
void image_render_init();

// prints the screen black and resets the cursor to the top left corner of the screen
void image_render_print_black_screen();

void image_render_print_game_over_screen();

void image_render_print_start_screen();

void image_render_update_screen();

void image_render_print_high_scores();

void image_render_print_gameover();

void image_render_test_image();

// closes the hdmi connection
void image_render_close();
