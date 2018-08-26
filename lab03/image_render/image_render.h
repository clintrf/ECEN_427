#include <stdint.h>
#include <stdio.h>

// from image_render.h
#define IMAGE_RENDER_ALIEN_PIXEL_TOTAL 14*10
#define IMAGE_RENDER_ALIEN_ROW 5
#define IMAGE_RENDER_ALIEN_COLUMN 11
#define IMAGE_RENDER_ALIEN_OUT 1
#define IMAGE_RENDER_ALIEN_IN 0
#define IMAGE_RENDER_ALIEN_ALIVE 1
#define IMAGE_RENDER_ALIEN_DESTROYED 0
#define IMAGE_RENDER_LEFT_MOVEMENT 0
#define IMAGE_RENDER_RIGHT_MOVEMENT 1
#define IMAGE_RENDER_NUM_BUNKER_BLOCKS 12
#define FOUR_HIT_POINTS 4
#define ONE_HIT_POINT 1
#define NO_HIT_POINTS 0
#define IMAGE_RENDER_ALIEN_BULLET_TOTAL_PIXELS 7*3


// a struct containing the data for an individual alien
typedef struct Alien Alien;
struct Alien {
  uint32_t image_in[IMAGE_RENDER_ALIEN_PIXEL_TOTAL];
  uint32_t image_out[IMAGE_RENDER_ALIEN_PIXEL_TOTAL];
  uint32_t width;
  uint32_t height;
  uint32_t current_location;
  uint16_t alive;
  uint32_t points;
  uint32_t block_placement;
  uint32_t shooter;
};

// a struct containing the data for an individual bunker
typedef struct Bunker Bunker;
struct Bunker {
  uint32_t position; // location of the top left corner
  uint16_t block_hit_points[IMAGE_RENDER_NUM_BUNKER_BLOCKS]; // status on each of the ten different positions
};

// a struct that contains the data for an individual alien bullet
typedef struct AlienBullet AlienBullet;
struct AlienBullet {
  uint32_t position;
  uint32_t image_in[IMAGE_RENDER_ALIEN_BULLET_TOTAL_PIXELS];
  uint32_t image_out[IMAGE_RENDER_ALIEN_BULLET_TOTAL_PIXELS];
  uint16_t fired;
};

// initializes a few of our necessary variables and starts the screen black
void image_render_init();

// prints the screen black and resets the cursor to the top left corner of the screen
void image_render_print_black_screen();

// creates a single alien object
// image_in : the image of the alien to be created (in)
// image_out : the image of the alien to be created (out)
// starting_location : starting location of the alien
// points : how many points the alien is worth
// placement : where the alien is inside the block
Alien image_render_create_alien(const uint32_t image_in[], const uint32_t image_out[], uint32_t starting_location, uint32_t points, uint32_t placement);

// creates the data for a single bunker object
// pos : the position of the bunker on the screen
Bunker image_render_create_bunker(uint32_t pos);

// creates an individual alien bullet
// pos : position of the bullet
// image_in : inward image
// image_out : outward image
// returns created alien bullet structure
AlienBullet image_render_create_alien_bullet(uint32_t pos, const uint32_t image_in[], const uint32_t image_out[]);

// creates the entire alien block
void image_render_create_alien_block();

// prints the first part of the game over screen
// this will display "GAME OVER" and "Enter your name" and a place to enter the player name
void image_render_print_game_over_screen_enter_name();

// prints the second part of the game over screen
// this will display "GAME OVER" and "High Scores" and the list of high scores
void image_render_print_game_over_screen_high_scores();

// prints the start screen and everything on it
void image_render_print_start_screen();

// moves the tank around the screen
// move : how many pixels to move the tank
// dir : the direction to move the tank
void image_render_tank(uint32_t move, uint16_t dir);

// fires a bullet from the tank position
void image_render_fire_tank_bullet();

// moves the tank bullet up the screen
void image_render_move_tank_bullet();

// fires a bullet from the tank position
void image_render_fire_alien_bullet();

// moves the tank bullet up the screen
void image_render_move_alien_bullet();

// moves the saucer around the screen
void image_render_saucer();

// moves the alien block around the screen
void image_render_move_alien_block();

// aliens fire bullets
void image_render_alien_fire_bullet();

// closes the hdmi connection
void image_render_close();
