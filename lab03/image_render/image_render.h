#include <stdint.h>
#include <stdio.h>

#define IMAGE_RENDER_ALIEN_PIXEL_TOTAL 12*8
#define IMAGE_RENDER_ALIEN_ROW 5
#define IMAGE_RENDER_ALIEN_COLUMN 11
#define IMAGE_RENDER_ALIEN_OUT 1
#define IMAGE_RENDER_ALIEN_IN 0
#define IMAGE_RENDER_ALIEN_ALIVE 1
#define IMAGE_RENDER_ALIEN_DESTROYED 0
// #define IMAGE_RENDER_ALIEN_MOVING_RIGHT 1
// #define IMAGE_RENDER_ALIEN_MOVING_LEFT 2
// #define IMAGE_RENDER_ALIEN_MOVING_DOWN 3
// #define IMAGE_RENDER_ALIEN_SHOT 4

// a struct containing the data for an individual alien
typedef struct Alien Alien;
struct Alien {
  uint32_t alien_image[IMAGE_RENDER_ALIEN_PIXEL_TOTAL];
  uint32_t alien_position;
  uint32_t alien_width;
  uint32_t alien_height;
  uint32_t alien_current_location;
  uint16_t alien_alive;
  uint32_t alien_points;
  uint32_t alien_block_placement;
};

// initializes a few of our necessary variables and starts the screen black
void image_render_init();

// prints the screen black and resets the cursor to the top left corner of the screen
void image_render_print_black_screen();

// creates a single alien object
// image : the image of the alien to be created
// starting_location : starting location of the alien
// points : how many points the alien is worth
// placement : where the alien is inside the block
Alien image_render_create_alien(const uint32_t image[], uint32_t starting_location, uint32_t points, uint32_t placement);

// modifies a single alien
// alien : passed in the alien to be modified
// modifer : how the alien is to be modified (moving or destroyed)
void image_render_modify_alien(Alien alien, uint16_t modifier);

// creates the entire alien block
void image_render_create_alien_block();

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
