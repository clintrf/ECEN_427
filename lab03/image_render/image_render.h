#include <stdint.h>
#include <stdio.h>

// from image_render.h
#define IMAGE_RENDER_ALIEN_PIXEL_TOTAL 14*10                                    // total pixels that every alien has
#define IMAGE_RENDER_ALIEN_ROW 5                                                // number of rows of aliens
#define IMAGE_RENDER_ALIEN_COLUMN 11                                            // number of columns of aliens
#define IMAGE_RENDER_ALIEN_OUT 1                                                // flag for when to print the out movment of the aliens
#define IMAGE_RENDER_ALIEN_IN 0                                                 // flag for when to print the in movment of the aliens
#define IMAGE_RENDER_ALIEN_ALIVE 1                                              // flag when aliens are alive
#define IMAGE_RENDER_ALIEN_DESTROYED 0                                          // flag when aliens are dead
#define IMAGE_RENDER_LEFT_MOVEMENT 0                                            // flag for moving the aliens to the left
#define IMAGE_RENDER_RIGHT_MOVEMENT 1                                           // flag for moving the aliens to the right
#define IMAGE_RENDER_NUM_BUNKER_BLOCKS 12                                       // flag for number of sections of the bunker


// a struct containing the data for an individual alien
typedef struct Alien Alien;
struct Alien {
  uint32_t image_in[IMAGE_RENDER_ALIEN_PIXEL_TOTAL];                            // image of the alien moving in
  uint32_t image_out[IMAGE_RENDER_ALIEN_PIXEL_TOTAL];                           // image of the alien moving out
  uint32_t width;                                                               // width of the alien
  uint32_t height;                                                              // height of the alien
  uint32_t current_location;                                                    // current position of the alien
  uint16_t alive;                                                               // if the alien is alive or not
  uint32_t points;                                                              // number of points the alien is worth
  uint32_t block_placement;                                                     // where to place the alien in the block
  uint32_t shooter;                                                             // if the alien is a shooter
};

// a struct containing the data for an individual bunker
typedef struct Bunker Bunker;
struct Bunker {
  uint32_t position;                                                            // location of the top left corner
  uint16_t block_hit_points[IMAGE_RENDER_NUM_BUNKER_BLOCKS];                    // status on each of the ten different positions
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
// returns : a newly created alien struct
Alien image_render_create_alien(const uint32_t image_in[], const uint32_t image_out[], uint32_t starting_location, uint32_t points, uint32_t placement);

// creates the data for a single bunker object
// pos : the position of the bunker on the screen
// returns : a newly created Bunker struct
Bunker image_render_create_bunker(uint32_t pos);

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

// fires a bullet from the first of the alien bullets from a random position
void image_render_fire_alien_bullet_0();

// fires a bullet from the second of the alien bullets from a random position
void image_render_fire_alien_bullet_1();

// fires a bullet from the third of the alien bullets from a random position
void image_render_fire_alien_bullet_2();

// fires a bullet from the fourth of the alien bullets from a random position
void image_render_fire_alien_bullet_3();

// moves the alien bullet down the screen
void image_render_move_alien_bullet_0();

// moves the alien bullet down the screen
void image_render_move_alien_bullet_1();

// moves the alien bullet down the screen
void image_render_move_alien_bullet_2();

// moves the alien bullet down the screen
void image_render_move_alien_bullet_3();

// moves the saucer around the screen
void image_render_saucer();

// moves the alien block around the screen
void image_render_move_alien_block();

// aliens fire bullets
void image_render_alien_fire_bullet_0();

// aliens fire bullets
void image_render_alien_fire_bullet_1();

// aliens fire bullets
void image_render_alien_fire_bullet_2();

// aliens fire bullets
void image_render_alien_fire_bullet_3();

// closes the hdmi connection
void image_render_close();
