#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "score_board.h"
#include "hdmi.h"
#include "image_render.h"
#include "sprites.h"
#include "../globals/globals.h"
#include <inttypes.h>

/********************************** macros ***********************************/
#define IMAGE_RENDER_SCREEN_WIDTH 640                                           // number of pixels for width of screen
#define IMAGE_RENDER_SCREEN_HEIGHT 480                                          // number of pixels for height of screen
#define IMAGE_RENDER_BYTES_PER_PIXEL 3                                          // number of bytes in pixel
#define IMAGE_RENDER_WHOLE_SCREEN IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_SCREEN_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL //number of pixels in the entire screen
#define LETTER_A 11                                                             // mapping for letter A
#define ALIEN_DEAD 0                                                            // flag for if alien is dead
#define ALIEN_DEPTH ALIEN_SIZE*SPRITES_ALIEN_HEIGHT                             // after scaling, height of alien
#define ALIEN_BLOCK_ROW_0 (15+(IMAGE_RENDER_SCREEN_WIDTH*3)*39)                 // where to start the first row of aliens
#define ALIEN_BLOCK_ROW_1 ALIEN_BLOCK_ROW_0+(IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15) // where to start the second row of aliens
#define ALIEN_BLOCK_ROW_2 ALIEN_BLOCK_ROW_1+(IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15) // where to start the third row of aliens
#define ALIEN_BLOCK_ROW_3 ALIEN_BLOCK_ROW_2+(IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15) // where to start the fourth row of aliens
#define ALIEN_BLOCK_ROW_4 ALIEN_BLOCK_ROW_3+(IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15) // where to start the fifth row of aliens
#define FIFTH_LIFE_START_LOCATION ((IMAGE_RENDER_SCREEN_WIDTH*3)-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9) // where to start the tank life position
#define FOURTH_LIFE_START_LOCATION (FIFTH_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9) // where to start the tank life position
#define THIRD_LIFE_START_LOCATION (FOURTH_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9) // where to start the tank life position
#define SECOND_LIFE_START_LOCATION (THIRD_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9) // where to start the tank life position
#define FIRST_LIFE_START_LOCATION (SECOND_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9) // where to start the tank life position
#define S_START_LOCATION_LVS (FIRST_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9) // where to start the S
#define E_START_LOCATION_LVS (S_START_LOCATION_LVS-(SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_WIDTH*3)-9) // where to start the E
#define V_START_LOCATION_LVS (E_START_LOCATION_LVS-(SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_WIDTH*3)-9) // where to start the V
#define I_START_LOCATION_LVS (V_START_LOCATION_LVS-(SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_WIDTH*3)-9) // where to start the I
#define L_START_LOCATION_LVS (I_START_LOCATION_LVS-(SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_WIDTH*3)-9) // where to start the L
#define SAUCER_POINTS 250                                                       // number of points for saucer
#define ALIEN_TOP_POINTS 40                                                     // points for the top row of aliens
#define ALIEN_MIDDLE_POINTS 20                                                  // points for the middle row of aliens
#define ALIEN_BOTTOM_POINTS 10                                                  // points for the bottom row of aliens
#define ROW_1 11                                                                // index for where to start the 1st row of aliens
#define ROW_2 22                                                                // index for where to start the 2nd row of aliens
#define ROW_3 33                                                                // index for where to start the 3rd row of aliens
#define ROW_ONE 0                                                               // index for row one
#define ROW_TWO 1                                                               // index for row two
#define ROW_THREE 2                                                             // index for row three
#define ROW_FOUR 3                                                              // index for row four
#define ROW_FIVE 4                                                              // index for row five
#define FIVE_ROWS 5                                                             // number of rows total
#define ELEVEN_COLUMNS 11                                                       // number of columns total
#define COLUMN_0_LEFT 0                                                         // column index
#define COLUMN_1 1                                                              // column index
#define COLUMN_2 2                                                              // column index
#define COLUMN_3 3                                                              // column index
#define COLUMN_4 4                                                              // column index
#define COLUMN_5 5                                                              // column index
#define COLUMN_6 6                                                              // column index
#define COLUMN_7 7                                                              // column index
#define COLUMN_8 8                                                              // column index
#define COLUMN_9 9                                                              // column index
#define COLUMN_10_RIGHT 10                                                      // column index
#define ALIEN_BUFFER_AREA 15                                                    // area of buffered sprite alien
#define ALIEN_SIZE 2                                                            // scaler for aliens
#define ALIEN_OFFSET ((SPRITES_ALIEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE)+30) // offset for aliens
#define ALIEN_BLOCK_SIZE 55                                                     // total number of aliens in block
#define S_START_LOCATION_SC 60                                                  // start location of S
#define C_START_LOCATION_SC S_START_LOCATION_SC+54                              // start location of C
#define O_START_LOCATION_SC C_START_LOCATION_SC+54                              // start location of O
#define R_START_LOCATION_SC O_START_LOCATION_SC+54                              // start location of R
#define E_START_LOCATION_SC R_START_LOCATION_SC+54                              // start location of E
#define SCORE_0_START_LOCATION E_START_LOCATION_SC+108                          // start location of 0
#define SCORE_1_START_LOCATION SCORE_0_START_LOCATION+54                        // start location of 1
#define SCORE_2_START_LOCATION SCORE_1_START_LOCATION+54                        // start location of 2
#define SCORE_3_START_LOCATION SCORE_2_START_LOCATION+54                        // start location of 3
#define SCORE_4_START_LOCATION SCORE_3_START_LOCATION+54                        // start location of 4
#define BOTTOM_LEFT_CORNER_OF_SCREEN 640*3*450                                  // position of bottom left corner of screen
#define CHARACTER_ZERO 0                                                        // defining character 0
#define LEFT_BOUND 2                                                            // left most bound of screen
#define RIGHT_BOUND_TANK 604                                                    // right most bound for tank
#define COLUMN_ZERO_LEFT_BOUND ALIEN_BLOCK_ROW_0                                // Bound for alien column
#define COLUMN_ZERO_RIGHT_BOUND ALIEN_BLOCK_ROW_0+ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL*SPRITES_ALIEN_WIDTH
#define COLUMN_ONE_LEFT_BOUND COLUMN_ZERO_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_ONE_RIGHT_BOUND COLUMN_ZERO_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_TWO_LEFT_BOUND COLUMN_ONE_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_TWO_RIGHT_BOUND COLUMN_ONE_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_THREE_LEFT_BOUND COLUMN_TWO_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_THREE_RIGHT_BOUND COLUMN_TWO_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_FOUR_LEFT_BOUND COLUMN_THREE_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_FOUR_RIGHT_BOUND COLUMN_THREE_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_FIVE_LEFT_BOUND COLUMN_FOUR_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_FIVE_RIGHT_BOUND COLUMN_FOUR_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_SIX_LEFT_BOUND COLUMN_FIVE_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_SIX_RIGHT_BOUND COLUMN_FIVE_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_SEVEN_LEFT_BOUND COLUMN_SIX_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_SEVEN_RIGHT_BOUND COLUMN_SIX_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_EIGHT_LEFT_BOUND COLUMN_SEVEN_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_EIGHT_RIGHT_BOUND COLUMN_SEVEN_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_NINE_LEFT_BOUND COLUMN_EIGHT_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_NINE_RIGHT_BOUND COLUMN_EIGHT_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_TEN_LEFT_BOUND COLUMN_NINE_LEFT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define COLUMN_TEN_RIGHT_BOUND COLUMN_NINE_RIGHT_BOUND+ALIEN_OFFSET               // Bound for alien column
#define TANK_START_POSITION 200                                                 // start position of tank
#define ALIEN_OUT 0                                                             // alien out flag
#define ALIEN_PIXEL_MOVEMENT 2                                                  // how many spaces to move alien
#define FULL_ALIEN_MOVEMENT (IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_PIXEL_MOVEMENT) // how many pixels to move alien
#define SAUCER_END_POSITION 580                                                 // right most position of saucer
#define BUNKER_HEIGHTS 360*IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL //height of bunker
#define BUNKER_ONE (360*(IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)+(72*3)) // location of bunker 1
#define BUNKER_TWO (360*(IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)+(3*72*3)) //  location of bunker 2
#define BUNKER_THREE (360*(IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)+(5*72*3)) // location of bunker 3
#define BUNKER_FOUR (360*(IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)+(7*72*3)) // location of bunker 4
#define BUNKER1 0                                                               // index of bunker
#define BUNKER2 1                                                               // index of bunker
#define BUNKER3 2                                                               // index of bunker
#define BUNKER4 3                                                               // index of bunker
#define TOP_LEFT_CORNER_OF_SCREEN 0                                             // top left corner of screen
#define TANK_BULLET_START_POS BOTTOM_LEFT_CORNER_OF_SCREEN+(tank_pos*IMAGE_RENDER_BYTES_PER_PIXEL)-(SPRITES_BULLET_HEIGHT*640*3*ALIEN_SIZE-8*3*ALIEN_SIZE) // postion of where tank bullets start from
#define BULLET_MOVEMENT_TWO_PIXELS 640*3*2                                      // how many pixels to move bullets
#define BULLET_MOVEMENT_ONE_PIXELS 640*3                                        // anouther option for how many pixels to move bullets
#define SAUCER_SHOT 0                                                           // flag for if saucer is shot
#define ALIEN_EXPLOSION_TIMER 1000000                                           // how long to make the alien explode for
#define TANK_EXPLOSION_TIMER 50000000                                           // how long to make the tank explode for
#define ALIEN_ALIVE 1                                                           // flag if alien is alive
#define LEFT 1                                                                  // flag to indicate
#define RIGHT 0                                                                 // indicate right
#define FOUR_HIT_POINTS 4                                                       // indicate 4 hit points
#define ONE_HIT_POINT 1                                                         // indicate 1 hit points
#define NO_HIT_POINTS 0                                                         // indicate 0 hit points
#define BUNKER_BLOCK_0 0                                                        // Block bunker number
#define BUNKER_BLOCK_3 3                                                        // Block bunker number
#define BUNKER_BLOCK_5 5                                                        // Block bunker number
#define BUNKER_BLOCK_6 6                                                        // Block bunker number
#define BUNKER_BLOCK_9 9                                                        // Block bunker number
#define BUNKER_BLOCK_10 10                                                        // Block bunker number
#define NUM_BUNKERS 4                                                           // Block bunker number
#define BLOCK_ROWS_PER_BUNKER 3                                                 // rows in each bunker
#define BLOCKS_PER_ROW 4                                                        // blocks per row of bunker
#define BLOCK_THREE_OFFSET SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL*3 // offset for bunker
#define BLOCK_FIVE_OFFSET ((SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL)+IMAGE_RENDER_SCREEN_WIDTH*SPRITES_BUNKER_DAMAGE_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE) // offset for 5th block in bunkers
#define BLOCK_SIX_OFFSET ((SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL*2)+IMAGE_RENDER_SCREEN_WIDTH*SPRITES_BUNKER_DAMAGE_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE) // offset for the 6th block in bunker
#define THREE_HIT_POINTS 3                                                        // Indicates 3 hitpoints
#define TWO_HIT_POINTS 2                                                        // indicates 2 high points
#define TANK_START_DEPTH 440                                                    // tank start depth postion
#define TANK_FIND_DEPTH 450                                                     // tank start depth plus 10
#define RANDOM_SHOOTER 10                                                       // set random shooter
#define ROW_COUNTER 4                                                           // counter for rows
#define ALIEN_SHOOTING 1                                                        // indicates aliens is shooting
#define ALIEN_BULLET_0 8                                                        // index for 0th alien bullet
#define ALIEN_BULLET_1 9                                                        // index for 1st alien bullet
#define ALIEN_BULLET_2 10                                                        // index for 2nd alien bullet
#define ALIEN_BULLET_3 12                                                        // index for 3rd alien bullet
#define TANK_BULLET 11                                                          // index for the tank bullet
#define ONE_PIXEL 3                                                             // bytes per pixel
#define TWO_PIXELS 6                                                            // bytes per 2 pixels
#define THREE_PIXELS 9                                                          // bytes per 3 pixels
#define FOUR_PIXELS 12                                                          // bytes per 4 pixels
#define FIVE_PIXELS 15                                                          // bytes per 5 pixels
#define TRUE 1                                                                  // true
#define FALSE 0                                                                 // false

/********************************** globals **********************************/
/* global arrays */
char full_screen_black[IMAGE_RENDER_WHOLE_SCREEN];
Alien alien_block[ALIEN_BLOCK_SIZE];
/* colors!! */
uint32_t black[IMAGE_RENDER_BYTES_PER_PIXEL] = {0x00, 0x00, 0x00};
uint32_t pink[IMAGE_RENDER_BYTES_PER_PIXEL] = {0xFF, 0x69, 0xB4};
uint32_t white[IMAGE_RENDER_BYTES_PER_PIXEL] = {0xFF, 0xFF, 0xFF};
uint32_t green[IMAGE_RENDER_BYTES_PER_PIXEL] = {0x00,0x80,0x00};
uint32_t red[IMAGE_RENDER_BYTES_PER_PIXEL] = {0xFF,0x00,0x00};
/* global variables */
uint32_t tank_pos;
uint32_t alien_block_right_bound;
uint32_t alien_block_left_bound;
uint16_t current_left_column;
uint16_t current_right_column;
uint16_t current_alien_direction;
uint32_t maximum_bound_right_alien;
uint32_t maximum_bound_left_alien;
uint16_t current_alien_position;
uint32_t odd_0;                       // tracks alien pop wiggles
uint32_t odd_1;                       // tracks alien pop wiggles
uint32_t odd_2;                       // tracks alien pop wiggles
uint32_t odd_3;                       // tracks alien pop wiggles
uint16_t alien_killed;
uint32_t total_shooters_0;
uint32_t total_shooters_1;
uint32_t total_shooters_2;
uint32_t total_shooters_3;
uint32_t current_shooter_0;
uint32_t current_shooter_1;
uint32_t current_shooter_2;
uint32_t current_shooter_3;
Bunker bunker_set[NUM_BUNKERS];

/**************************** function prototypes ****************************/
void image_render_print_black_screen();
void image_render_create_alien_block();

/********************************* functions *********************************/
// initializes a few of our necessary variables and starts the screen black
void image_render_init() {
  hdmi_init(HDMI_FILE_PATH); /* opens a path to the HDMI driver (enables/read write) */
  /* Set all the global variable start values */
  tank_pos = TANK_START_POSITION;
  alien_block_right_bound = COLUMN_TEN_RIGHT_BOUND;
  alien_block_left_bound = COLUMN_ZERO_LEFT_BOUND;
  current_alien_direction = IMAGE_RENDER_RIGHT_MOVEMENT;
  maximum_bound_right_alien = ALIEN_BLOCK_ROW_0+IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL-30;
  maximum_bound_left_alien = ALIEN_BLOCK_ROW_0;
  current_alien_position = ALIEN_OUT;
  alien_killed = 0;
  odd_0 = ALIEN_OUT;
  odd_1 = ALIEN_OUT;
  odd_2 = ALIEN_OUT;
  odd_3 = ALIEN_OUT;
  // initializes a buffer that contains all zeroes to print black to the entire screen
  for(uint32_t i = 0; i < IMAGE_RENDER_WHOLE_SCREEN; i++) {
    full_screen_black[i++] = 0x00; // if all bits are set to 0x00, the pixel will be black
    full_screen_black[i++] = 0x00;
    full_screen_black[i] = 0x00;
  }
  image_render_print_black_screen(); // call the print screen function
  image_render_create_alien_block(); // creates the original alien block
}

// prints the screen black and resets the cursor to the top left corner of the screen
void image_render_print_black_screen() {
  hdmi_set_offset(TOP_LEFT_CORNER_OF_SCREEN); /* this will set the fd offset back to the top left of the screen */
  hdmi_write(full_screen_black,IMAGE_RENDER_WHOLE_SCREEN); /* this will write a black screen to the hdmi monitor */
}

// creates a single alien object
// image_in : the image of the alien to be created (in)
// image_out : the image of the alien to be created (out)
// starting_location : starting location of the alien
// points : how many points the alien is worth
// placement : where the alien is inside the block
// returns : a newly created alien struct
Alien image_render_create_alien(const uint32_t image_in[], const uint32_t image_out[], uint32_t starting_location, uint32_t points, uint32_t placement) {
  Alien alien;
  for (int i = 0; i < IMAGE_RENDER_ALIEN_PIXEL_TOTAL; i++) { // sets the alien image
    alien.image_in[i] = image_in[i];
  }
  for (int i = 0; i < IMAGE_RENDER_ALIEN_PIXEL_TOTAL; i++) { // sets the alien image
    alien.image_out[i] = image_out[i];
  }
  alien.current_location = starting_location;
  alien.points = points;
  alien.block_placement = placement;
  alien.width = SPRITES_ALIEN_WIDTH;
  alien.height = SPRITES_ALIEN_HEIGHT;
  alien.alive = IMAGE_RENDER_ALIEN_ALIVE;
  alien.shooter = 0;
  return alien;
}

// creates the data for a single bunker object
// pos : the position of the bunker on the screen
// returns : a newly created Bunker struct
Bunker image_render_create_bunker(uint32_t pos) {
  Bunker bunker;
  bunker.position = pos;
  for(int i = 0; i < IMAGE_RENDER_NUM_BUNKER_BLOCKS; i++) { // runs through the amount of blocks in each bunker
    if(i == BUNKER_BLOCK_5 || i == BUNKER_BLOCK_6) { // if it is one of the lower middle diagonal blocks, go here
      bunker.block_hit_points[i] = TWO_HIT_POINTS;
    }
    else if(i == BUNKER_BLOCK_9 || i == BUNKER_BLOCK_10) { // if it is one of the empty bunker blocks, go here
      bunker.block_hit_points[i] = NO_HIT_POINTS;
    }
    else {
      bunker.block_hit_points[i] = FOUR_HIT_POINTS; // all other bunker blocks have 4 hp points
    }
  }
  return bunker;
}

// creates the entire alien block
void image_render_create_alien_block() {
  int r = 0;
  for(int i=0; i<FIVE_ROWS; i++){ // interates through every row of aliens
    for(int c=0; c<ELEVEN_COLUMNS; c++){ // iterates through each column within each row
      if(i==ROW_ONE){ // if we are on the top row of aliens
        alien_block[r+c] = image_render_create_alien(alien_top_in_14x10,alien_top_out_14x10,ALIEN_BLOCK_ROW_0+i*((IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)*(ALIEN_DEPTH+ALIEN_BUFFER_AREA))+ALIEN_OFFSET*c,ALIEN_TOP_POINTS,r+c);
      }
      if(i==ROW_TWO || i==ROW_THREE){ // if we are in the middle rows of the block
        alien_block[r+c] = image_render_create_alien(alien_middle_in_14x10,alien_middle_out_14x10,ALIEN_BLOCK_ROW_0+i*((IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)*(ALIEN_DEPTH+ALIEN_BUFFER_AREA))+ALIEN_OFFSET*c,ALIEN_MIDDLE_POINTS,r+c);
      }
      if(i==ROW_FOUR || i==ROW_FIVE){ // if we are in the bottom two rows of the block
        alien_block[r+c] = image_render_create_alien(alien_bottom_in_14x10,alien_bottom_out_14x10,ALIEN_BLOCK_ROW_0+i*((IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)*(ALIEN_DEPTH+ALIEN_BUFFER_AREA))+ALIEN_OFFSET*c,ALIEN_BOTTOM_POINTS,r+c);
      }
    }
    r = r+ROW_1;
  }
  /* sets all initial parameters, or resets them when we recreate a new alien_block */
  alien_block_right_bound = COLUMN_TEN_RIGHT_BOUND;
  alien_block_left_bound = COLUMN_ZERO_LEFT_BOUND;
  current_left_column = COLUMN_0_LEFT;
  current_right_column = COLUMN_10_RIGHT;
}

// prints the first part of the game over screen
// this will display "GAME OVER" and "Enter your name" and a place to enter the player name
void image_render_print_game_over_screen_enter_name() {
  image_render_print_black_screen(); // print the whole screen black
  /* print out the words GAME OVER */
  sprites_render_buffer(letterG_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_G_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_buffer(letterA_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_A_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_buffer(letterM_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_M_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_buffer(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_O_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_buffer(letterV_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_V_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E2_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_buffer(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  /* print out the words ENTER YOUR NAME */
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterN_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_N_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterT_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_T_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterY_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_Y_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_O_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterU_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_U_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterN_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_N2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterA_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_A_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterM_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_M_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E3_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  /* print out three original characters for player name */
  sprites_render_buffer(char_array[LETTER_A],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_CHARACTER_ONE_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(char_array[LETTER_A],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_CHARACTER_TWO_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(char_array[LETTER_A],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_CHARACTER_THREE_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,white);
}

// prints the second part of the game over screen
// this will display "GAME OVER" and "High Scores" and the list of high scores
void image_render_print_game_over_screen_high_scores() {
  /* clear the bottom portion of the screen */
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterN_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_N_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterT_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_T_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterY_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_Y_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_O_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterU_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_U_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterN_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_N2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterA_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_A_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterM_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_M_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E3_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  /* print high scores image */
  sprites_render_buffer(letterH_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_H_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterI_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_I_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterG_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_G_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterH_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_H2_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterS_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_S_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterC_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_C_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_O_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterS_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_S2_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
}

// prints the start screen and everything on it
void image_render_print_start_screen() {
  /* Print the word score in the top left corner and then print the score */
  sprites_render_buffer(letterS_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,S_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterC_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,C_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,O_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,R_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,E_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  /* Print the current score on the screen (starts at 00000) */
  sprites_render_buffer(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_0_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  sprites_render_buffer(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_1_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  sprites_render_buffer(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_2_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  sprites_render_buffer(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_3_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  sprites_render_buffer(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_4_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  /* Print the lives section of the start screen */
  sprites_render_buffer(letterL_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,L_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterI_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,I_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterV_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,V_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,E_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(letterS_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,S_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-ALIEN_SIZE,SPRITES_TANK_HEIGHT-ALIEN_SIZE,FIRST_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-ALIEN_SIZE,SPRITES_TANK_HEIGHT-ALIEN_SIZE,SECOND_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-ALIEN_SIZE,SPRITES_TANK_HEIGHT-ALIEN_SIZE,THIRD_LIFE_START_LOCATION,ALIEN_SIZE,green);
  /* Prints bunkers */
  sprites_render_buffer(bunker_24x18,SPRITES_BUNKER_WIDTH,SPRITES_BUNKER_HEIGHT,BUNKER_ONE,ALIEN_SIZE,green);
  bunker_set[BUNKER1] = image_render_create_bunker(BUNKER_ONE);
  sprites_render_buffer(bunker_24x18,SPRITES_BUNKER_WIDTH,SPRITES_BUNKER_HEIGHT,BUNKER_TWO,ALIEN_SIZE,green);
  bunker_set[BUNKER2] = image_render_create_bunker(BUNKER_TWO);
  sprites_render_buffer(bunker_24x18,SPRITES_BUNKER_WIDTH,SPRITES_BUNKER_HEIGHT,BUNKER_THREE,ALIEN_SIZE,green);
  bunker_set[BUNKER3] = image_render_create_bunker(BUNKER_THREE);
  sprites_render_buffer(bunker_24x18,SPRITES_BUNKER_WIDTH,SPRITES_BUNKER_HEIGHT,BUNKER_FOUR,ALIEN_SIZE,green);
  bunker_set[BUNKER4] = image_render_create_bunker(BUNKER_FOUR);
  /* Prints the saucer as a starter */
  sprites_render_buffer(saucer_18x9,SPRITES_SAUCER_WIDTH,SPRITES_SAUCER_HEIGHT,GLOBALS_SAUCER_ROW_START_LOCATION,ALIEN_SIZE,red);
  /* Prints starting tank*/
  sprites_render_buffer(tank_17x10,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,BOTTOM_LEFT_CORNER_OF_SCREEN+(tank_pos*IMAGE_RENDER_BYTES_PER_PIXEL),ALIEN_SIZE,green);
  /* Prints the alien block completely */
  for(int i = 0; i < ALIEN_BLOCK_SIZE; i++) {
    Alien alien_temp = alien_block[i];
    sprites_render_buffer(alien_temp.image_out,alien_temp.width,alien_temp.height,alien_temp.current_location,ALIEN_SIZE,pink);
  }
}

// moves the tank around the screen
// move : how many pixels to move the tank
// dir : the direction to move the tank
void image_render_tank(uint32_t move, uint16_t dir){
  if(dir == IMAGE_RENDER_RIGHT_MOVEMENT && tank_pos <= RIGHT_BOUND_TANK) { // moves the tank to the right until it hits the edge of the screen
    tank_pos = tank_pos+move;
    sprites_render_buffer(tank_17x10,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,BOTTOM_LEFT_CORNER_OF_SCREEN+(tank_pos*IMAGE_RENDER_BYTES_PER_PIXEL),ALIEN_SIZE,green);
  }
  else if(dir == IMAGE_RENDER_LEFT_MOVEMENT && tank_pos >= LEFT_BOUND) { // moves the tank to the left until it hits the edge of the screen
    tank_pos = tank_pos-move;
    sprites_render_buffer(tank_17x10,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,BOTTOM_LEFT_CORNER_OF_SCREEN+(tank_pos*IMAGE_RENDER_BYTES_PER_PIXEL),ALIEN_SIZE,green);
  }
}

// fires a bullet from the tank position
void image_render_fire_tank_bullet() {
  globals_fire_tank_bullet(); // says that the bullet has been fired
  globals_set_tank_bullet_position(TANK_BULLET_START_POS);
  sprites_render_buffer(tankbullet_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,TANK_BULLET_START_POS,ALIEN_SIZE,white);
}

// fires a bullet from the first of the alien bullets from a random position
void image_render_fire_alien_bullet_0() {
  globals_fire_alien_bullet_0(); // says that the bullet has been fired
  globals_set_alien_bullet_position_0(current_shooter_0);
  if(odd_0){
    sprites_render_buffer(alienbullet2_up_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_shooter_0,ALIEN_SIZE,white);
  }
  else{
    sprites_render_buffer(alienbullet2_down_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_shooter_0,ALIEN_SIZE,white);
  }
}

// fires a bullet from the second of the alien bullets from a random position
void image_render_fire_alien_bullet_1() {
  globals_fire_alien_bullet_1(); // says that the bullet has been fired
  globals_set_alien_bullet_position_1(current_shooter_1);
  if(odd_1){
    sprites_render_buffer(alienbullet2_up_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_shooter_1,ALIEN_SIZE,green);
  }
  else{
    sprites_render_buffer(alienbullet2_down_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_shooter_1,ALIEN_SIZE,green);
  }
}

// fires a bullet from the third of the alien bullets from a random position
void image_render_fire_alien_bullet_2() {
  globals_fire_alien_bullet_2(); // says that the bullet has been fired
  globals_set_alien_bullet_position_2(current_shooter_2);
  if(odd_2){
    sprites_render_buffer(alienbullet2_up_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_shooter_2,ALIEN_SIZE,pink);
  }
  else{
    sprites_render_buffer(alienbullet2_down_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_shooter_2,ALIEN_SIZE,pink);
  }
}

// fires a bullet from the fourth of the alien bullets from a random position
void image_render_fire_alien_bullet_3() {
  globals_fire_alien_bullet_3(); // says that the bullet has been fired
  globals_set_alien_bullet_position_3(current_shooter_3);
  if(odd_3){
    sprites_render_buffer(alienbullet2_up_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_shooter_3,ALIEN_SIZE,red);
  }
  else{
    sprites_render_buffer(alienbullet2_down_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_shooter_3,ALIEN_SIZE,red);
  }
}


// checks to see if the location of the saucer and the tank bullet match
// current_pos : the current position of the tank bullet
void image_render_check_for_saucer(uint32_t current_pos) {
  uint32_t saucer_pos = globals_get_saucer_pos();
  for(int sh = 0; sh < SPRITES_SAUCER_HEIGHT*ALIEN_SIZE; sh++) { // checks along the saucer height
    for(int sw = 0; sw < SPRITES_SAUCER_WIDTH*ALIEN_SIZE; sw++) { // checks along the saucer width
      // if the tank bullet position is the same as the saucer position, kill the saucer
      if(current_pos == saucer_pos+(sw*IMAGE_RENDER_BYTES_PER_PIXEL)+(sh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH)) {
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        sprites_render_buffer(saucer_18x9,SPRITES_SAUCER_WIDTH,SPRITES_SAUCER_HEIGHT,saucer_pos,ALIEN_SIZE,black);
        globals_set_saucer_pos(GLOBALS_SAUCER_ROW_START_LOCATION);
        globals_set_saucer_status(SAUCER_SHOT);
        globals_tank_bullet_stopped();
        globals_add_to_current_score(SAUCER_POINTS);
      }
    }
  }
}

// goes through each column to check the bound
// column : the column to check
// bound : the current bound on the block
void image_render_check_column(uint16_t column, uint16_t bound) {
  uint16_t column_count = 0;
    for(int i = 0; i < FIVE_ROWS; i++) { // go through the entire column
      if(alien_block[column].alive == ALIEN_DEAD) { // check to see if they are alive
        column_count++;
      }
      if(column_count == FIVE_ROWS && bound == LEFT) { // if all of the aliens are dead, then set a new left bound
        alien_block_left_bound = alien_block_left_bound+ALIEN_OFFSET;
        current_left_column++;
      }
      else if(column_count == FIVE_ROWS && bound == RIGHT) { // if all of the aliens are dead, then set a new right bound
        alien_block_right_bound = alien_block_right_bound-ALIEN_OFFSET;
        current_right_column--;
      }
      column += ELEVEN_COLUMNS; // increment down to the next one
    }
}

// checks each of the columns on the left hand side to see if we need to move the boundaries over
void image_render_set_left_bound() {
  uint16_t j = current_left_column;
  image_render_check_column(j,LEFT);
}

// checks each of the columns on the right hand side to see if we need to move the boundaries over
void image_render_set_right_bound() {
  uint16_t j = current_right_column;
  image_render_check_column(j,RIGHT);
}

// checks to see if the location of an alien and the tank bullet match
// current_pos : the current position of the tank bullet
void image_render_check_for_aliens(uint32_t current_pos) {
  for(int i = 0; i < ALIEN_BLOCK_SIZE; i++) { // iterates through every single alien in the block
    uint32_t alien_pos = alien_block[i].current_location;
    for(int ah = 0; ah < SPRITES_ALIEN_HEIGHT*ALIEN_SIZE; ah++) { // parses through alien height
      for(int aw = 0; aw < SPRITES_ALIEN_WIDTH*ALIEN_SIZE; aw++) { // parses through alien width
        /* if the alien's position matches the tip of the bullet, then run this code */
        if(current_pos == alien_pos+(aw*IMAGE_RENDER_BYTES_PER_PIXEL)+(ah*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && alien_block[i].alive == 1) {
          sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
          sprites_render_buffer(alien_explosion_14x12,SPRITES_ALIEN_WIDTH,SPRITES_ALIEN_HEIGHT,alien_pos,ALIEN_SIZE,red);
          alien_block[i].alive = 0;
          alien_block[i].shooter = 0;
          globals_tank_bullet_stopped(); // stops the tank bullet
          globals_add_to_current_score(alien_block[i].points); // adds the alien's points to the score
          globals_decrement_total_alien_count(); // decreases total alien count
          alien_killed = TRUE;
          globals_set_dead_alien_loc(alien_block[i].current_location);
          uint32_t counter = 0;
          while(counter < ALIEN_EXPLOSION_TIMER) { counter++; } // timer to display the explosion
        }
      }
    }
  }
}

// blows the tank up
// bullet_id : tracks which bullet shot the tank
void image_render_blow_tank_up(uint16_t bullet_id) {
  if(bullet_id == ALIEN_BULLET_0) { // need to determine which bullet shot the tank
    globals_alien_bullet_stopped_0();
    sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,globals_get_alien_bullet_position_0(),ALIEN_SIZE,black);
  }
  else if(bullet_id == ALIEN_BULLET_1) { // need to determine which bullet shot the tank
    globals_alien_bullet_stopped_1();
    sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,globals_get_alien_bullet_position_1(),ALIEN_SIZE,black);
  }
  else if(bullet_id == ALIEN_BULLET_2) { // need to determine which bullet shot the tank
    globals_alien_bullet_stopped_2();
    sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,globals_get_alien_bullet_position_2(),ALIEN_SIZE,black);
  }
  else { // need to determine which bullet shot the tank
    globals_alien_bullet_stopped_3();
    sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,globals_get_alien_bullet_position_3(),ALIEN_SIZE,black);
  }
  sprites_render_buffer(tank_explosion2_17x10,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,(IMAGE_RENDER_SCREEN_WIDTH*TANK_FIND_DEPTH+tank_pos)*IMAGE_RENDER_BYTES_PER_PIXEL,ALIEN_SIZE,green);
  globals_decrement_current_lives();
  uint32_t counter = 0;
  while(counter < TANK_EXPLOSION_TIMER) { counter++; } // timer to display the explosion
  sprites_render_buffer(tank_17x10,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,(IMAGE_RENDER_SCREEN_WIDTH*TANK_FIND_DEPTH+tank_pos)*IMAGE_RENDER_BYTES_PER_PIXEL,ALIEN_SIZE,green);
}

// checks for the position of the tank
// current_pos : current position of the alien bullet
// bullet_id : used to trank which bullet shot the tank
void image_render_check_for_tank(uint32_t current_pos, uint16_t bullet_id) {
  current_pos = (current_pos / IMAGE_RENDER_BYTES_PER_PIXEL);
  for(int i = 0; i < ALIEN_SIZE; i++){
    for(int j = 0; j < SPRITES_TANK_WIDTH*ALIEN_SIZE; j++){
      if(current_pos == IMAGE_RENDER_SCREEN_WIDTH*TANK_START_DEPTH+tank_pos+j+i*IMAGE_RENDER_SCREEN_WIDTH) { // if the tank bullet position is the same as the saucer position, kill the saucer
        image_render_blow_tank_up(bullet_id);
        break;
      }
      else if(current_pos+IMAGE_RENDER_BYTES_PER_PIXEL == IMAGE_RENDER_SCREEN_WIDTH*TANK_START_DEPTH+tank_pos+j+i*IMAGE_RENDER_SCREEN_WIDTH) { // if the tank bullet position is the same as the saucer position, kill the saucer
        image_render_blow_tank_up(bullet_id);
        break;
      }
      else if(current_pos-IMAGE_RENDER_BYTES_PER_PIXEL == IMAGE_RENDER_SCREEN_WIDTH*TANK_START_DEPTH+tank_pos+j+i*IMAGE_RENDER_SCREEN_WIDTH) { // if the tank bullet position is the same as the saucer position, kill the saucer
        image_render_blow_tank_up(bullet_id);
        break;
      }
      else if(current_pos+IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE == IMAGE_RENDER_SCREEN_WIDTH*TANK_START_DEPTH+tank_pos+j+i*IMAGE_RENDER_SCREEN_WIDTH) { // if the tank bullet position is the same as the saucer position, kill the saucer
        image_render_blow_tank_up(bullet_id);
        break;
      }
      else if(current_pos-IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE == IMAGE_RENDER_SCREEN_WIDTH*TANK_START_DEPTH+tank_pos+j+i*IMAGE_RENDER_SCREEN_WIDTH) { // if the tank bullet position is the same as the saucer position, kill the saucer
        image_render_blow_tank_up(bullet_id);
        break;
      }
      else if(current_pos+THREE_PIXELS == IMAGE_RENDER_SCREEN_WIDTH*TANK_START_DEPTH+tank_pos+j+i*IMAGE_RENDER_SCREEN_WIDTH) { // if the tank bullet position is the same as the saucer position, kill the saucer
        image_render_blow_tank_up(bullet_id);
        break;
      }
    }
  }
}

// checks the status of a block within the bunker
// bunker_num : which bunker we are checking
// block_num : which block within the bunker we are checking
// bullet_id : identifies alien bullet or tank bullet
void image_render_check_bunker_block(uint16_t bunker_num, uint16_t block_num, uint16_t row, uint16_t column, uint32_t current_pos, uint32_t bullet_id) {
  uint16_t hp = bunker_set[bunker_num].block_hit_points[block_num];
  if(block_num == BUNKER_BLOCK_0) { // top left corner case
    if(hp == FOUR_HIT_POINTS) { // if the block hasn't been hit yet
      sprites_render_buffer(bunker_upper_left_damage2_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position,ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if one of the alien bullets shot the bunker
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }

      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == THREE_HIT_POINTS) { // if the block has been hit once already
      sprites_render_buffer(bunker_upper_left_damage1_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position,ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == TWO_HIT_POINTS) { // if the block has been hit twice already
      sprites_render_buffer(bunker_upper_left_damage0_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position,ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == ONE_HIT_POINT) { // if the block has been hit thrice already
      sprites_render_buffer(bunker_upper_left_gone_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position,ALIEN_SIZE,black);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
  }
  else if(block_num == BUNKER_BLOCK_3) { // the right corner case
    if(hp == FOUR_HIT_POINTS) { // if the block hasn't been hit yet
      sprites_render_buffer(bunker_upper_right_damage2_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+BLOCK_THREE_OFFSET,ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == THREE_HIT_POINTS) { // if the block has been hit once already
      sprites_render_buffer(bunker_upper_right_damage1_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+BLOCK_THREE_OFFSET,ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == TWO_HIT_POINTS) { // if the block has been hit twice already
      sprites_render_buffer(bunker_upper_right_damage0_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+BLOCK_THREE_OFFSET,ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == ONE_HIT_POINT) { // if the block has been hit thrice already
      sprites_render_buffer(bunker_upper_right_gone_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+BLOCK_THREE_OFFSET,ALIEN_SIZE,black);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
  }
  else if(block_num == BUNKER_BLOCK_5) { // lower middle left case
    if(hp == TWO_HIT_POINTS) { // if the block hasn't been hit yet
      sprites_render_buffer(bunker_lower_middle_left_damage0_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+BLOCK_FIVE_OFFSET,ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == ONE_HIT_POINT) { // if the block hasn't been hit yet
      sprites_render_buffer(bunker_lower_middle_left_gone_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+BLOCK_FIVE_OFFSET,ALIEN_SIZE,black);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
  }
  else if(block_num == BUNKER_BLOCK_6) { // lower middle right case
    if(hp == TWO_HIT_POINTS) { // if the block hasn't been hit yet
      sprites_render_buffer(bunker_lower_middle_right_damage0_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+BLOCK_SIX_OFFSET,ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == ONE_HIT_POINT) { // if the block hasn't been hit yet
      sprites_render_buffer(bunker_lower_middle_right_gone_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+BLOCK_SIX_OFFSET,ALIEN_SIZE,black);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
  }
  else if(block_num == BUNKER_BLOCK_9 || block_num == BUNKER_BLOCK_10) {} // empty blocks
  else { // all other blocks (normal)
    if(hp == FOUR_HIT_POINTS) { // if the block hasn't been hit yet
      sprites_render_buffer(bunkerDamage2_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+((SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL*column)+SPRITES_BUNKER_DAMAGE_HEIGHT*IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*row),ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == THREE_HIT_POINTS) { // if the block has been hit once already
      sprites_render_buffer(bunkerDamage1_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+((SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL*column)+SPRITES_BUNKER_DAMAGE_HEIGHT*IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*row),ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == TWO_HIT_POINTS) { // if the block has been hit twice already
      sprites_render_buffer(bunkerDamage0_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+((SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL*column)+SPRITES_BUNKER_DAMAGE_HEIGHT*IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*row),ALIEN_SIZE,green);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
    else if(hp == ONE_HIT_POINT) { // if the block has been hit thrice already
      sprites_render_buffer(bunkerDamage3_6x6,SPRITES_BUNKER_DAMAGE_WIDTH,SPRITES_BUNKER_DAMAGE_HEIGHT,bunker_set[bunker_num].position+((SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL*column)+SPRITES_BUNKER_DAMAGE_HEIGHT*IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*row),ALIEN_SIZE,black);
      if(bullet_id == TANK_BULLET) { // if the tank bullet shot the bunker
        sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        globals_tank_bullet_stopped();
      }
      else { // if the bunker was hit with an alien bullet
        sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
        if(bullet_id == ALIEN_BULLET_0){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_0();
        }
        if(bullet_id == ALIEN_BULLET_1){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_1();
        }
        if(bullet_id == ALIEN_BULLET_2){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_2();
        }
        if(bullet_id == ALIEN_BULLET_3){ // determine which bullet shot the bunker
          globals_alien_bullet_stopped_3();
        }
      }
      bunker_set[bunker_num].block_hit_points[block_num]--;
    }
  }
}

// checks to see if the location of a bullet and the bunker match
// current_pos : position of the bullet
// bullet_id : identifies alien bullet or tank bullet
void image_render_check_for_bunker(uint32_t current_pos, uint32_t bullet_id) {
  if(bullet_id == TANK_BULLET) { // if the bullet that was shot was the tank bullet
    for(int b = 0; b < NUM_BUNKERS; b++) { // checks each of the four bunkers
      uint32_t bunker_pos = bunker_set[b].position;
      for(int i = 0; i < BLOCK_ROWS_PER_BUNKER; i++) { // iterates through each block row in the bunker
        for(int j = 0; j < BLOCKS_PER_ROW; j++) { // iterates through each block column in the bunker
          for(int bh = 0; bh < SPRITES_BUNKER_DAMAGE_HEIGHT*ALIEN_SIZE; bh++) { // iterate through each pixel height
            for(int bw = 0; bw < SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE; bw++) { // iterate through each pixel width
              /* if the bullet position matches one of the bunker block's positions */
              bunker_pos = bunker_set[b].position+SPRITES_BUNKER_DAMAGE_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*j+IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*IMAGE_RENDER_SCREEN_WIDTH*SPRITES_BUNKER_DAMAGE_HEIGHT*i;
              if(current_pos == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH)) {
                image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos,bullet_id);
              }
            }
          }
        }
      }
    }
  }
  else { // if the bullet that was shot was one of the alien bullets
    current_pos = current_pos+SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE;
    for(int b = 0; b < NUM_BUNKERS; b++) { // checks each of the four bunkers
      uint32_t bunker_pos = bunker_set[b].position;
      uint16_t bunker_hit = 0;
      for(int i = 0; i < BLOCK_ROWS_PER_BUNKER; i++) { // iterates through each block row in the bunker
        for(int j = 0; j < BLOCKS_PER_ROW; j++) { // iterates through each block column in the bunker
          for(int bh = 0; bh < SPRITES_BUNKER_DAMAGE_HEIGHT*ALIEN_SIZE; bh++) { // iterate through each pixel height
            for(int bw = 0; bw < SPRITES_BUNKER_DAMAGE_WIDTH*ALIEN_SIZE; bw++) { // iterate through each pixel width
              /* if the bullet position matches one of the bunker block's positions */
              bunker_pos = bunker_set[b].position+SPRITES_BUNKER_DAMAGE_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*j+IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*IMAGE_RENDER_SCREEN_WIDTH*SPRITES_BUNKER_DAMAGE_HEIGHT*i;
              // this says that the bunker hasn't been hit yet with this bullet
              // this is necessary because the alien bullets have a three pixel width
              if(bunker_hit == FALSE) {
                // check the position of the point
                if(current_pos == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && bunker_set[b].block_hit_points[(i*BLOCKS_PER_ROW)+j] != 0) {
                  image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos-SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE,bullet_id);
                  bunker_hit = TRUE;
                  break;
                }
                // check the position of the point
                else if(current_pos+ONE_PIXEL == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && bunker_set[b].block_hit_points[(i*BLOCKS_PER_ROW)+j] != 0) {
                  image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos-SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE,bullet_id);
                  bunker_hit = TRUE;
                  break;
                }
                // check the position of the pixel to the left of the point
                else if(current_pos-ONE_PIXEL == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && bunker_set[b].block_hit_points[(i*BLOCKS_PER_ROW)+j] != 0) {
                  image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos-SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE,bullet_id);
                  bunker_hit = TRUE;
                  break;
                }
                // check the pixel to the right of the point
                else if(current_pos+TWO_PIXELS == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && bunker_set[b].block_hit_points[(i*BLOCKS_PER_ROW)+j] != 0) {
                  image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos-SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE,bullet_id);
                  bunker_hit = TRUE;
                  break;
                }
                // check the position of the pixel to the left of the point
                else if(current_pos-TWO_PIXELS == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && bunker_set[b].block_hit_points[(i*BLOCKS_PER_ROW)+j] != 0) {
                  image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos-SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE,bullet_id);
                  bunker_hit = TRUE;
                  break;
                }
                // check the pixel to the right of the point
                else if(current_pos+THREE_PIXELS == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && bunker_set[b].block_hit_points[(i*BLOCKS_PER_ROW)+j] != 0) {
                  image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos-SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE,bullet_id);
                  bunker_hit = TRUE;
                  break;
                }
                // check the pixel to the right of the point
                else if(current_pos+FOUR_PIXELS == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && bunker_set[b].block_hit_points[(i*BLOCKS_PER_ROW)+j] != 0) {
                  image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos-SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE,bullet_id);
                  bunker_hit = TRUE;
                  break;
                }
                // check the pixel to the right of the point
                else if(current_pos+FIVE_PIXELS == bunker_pos+(bw*IMAGE_RENDER_BYTES_PER_PIXEL)+(bh*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH) && bunker_set[b].block_hit_points[(i*BLOCKS_PER_ROW)+j] != 0) {
                  image_render_check_bunker_block(b,(i*BLOCKS_PER_ROW)+j,i,j,current_pos-SPRITES_BULLET_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*ALIEN_SIZE,bullet_id);
                  bunker_hit = TRUE;
                  break;
                }
              }
            }
          }
        }
      }
    }
  }
}

// moves the tank bullet up the screen
void image_render_move_tank_bullet() {
  uint32_t current_pos = globals_get_tank_bullet_position(); // fetches current bullet position
  globals_set_tank_bullet_position(current_pos-(BULLET_MOVEMENT_TWO_PIXELS)); // sets the new bullet position moving up the screen
  current_pos = globals_get_tank_bullet_position(); // fetch the updated bullet position
  sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
  if(current_pos < GLOBALS_SAUCER_ROW_START_LOCATION) { // if the bullet reaches the top of the screen, delete the bullet
    globals_set_tank_bullet_position(current_pos-(BULLET_MOVEMENT_TWO_PIXELS)); // sets the new bullet position moving up the screen
    current_pos = globals_get_tank_bullet_position(); // fetch the updated bullet position
    sprites_render_buffer(tankbullet_gone_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
    globals_tank_bullet_stopped();
  }
  else { // if the bullet hasn't hit anything or reached the top of the screen, move it up
    globals_set_tank_bullet_position(current_pos-(BULLET_MOVEMENT_TWO_PIXELS)); // sets the new bullet position moving up the screen
    current_pos = globals_get_tank_bullet_position(); // fetch the updated bullet position
    sprites_render_buffer(tankbullet_1x7,SPRITES_TANK_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos,ALIEN_SIZE,white);
    /* check for saucer location */
    image_render_check_for_saucer(current_pos);
    image_render_check_for_aliens(current_pos);
    image_render_check_for_bunker(current_pos, TANK_BULLET);
  }
}
// moves the alien bullet down the screen
void image_render_move_alien_bullet_0(){
  uint32_t current_pos_0 = globals_get_alien_bullet_position_0(); // fetches current bullet position
  globals_set_alien_bullet_position_0(current_pos_0+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
  current_pos_0 = globals_get_alien_bullet_position_0(); // fetch the updated bullet position
  sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_0,ALIEN_SIZE,pink);
  if(current_pos_0 > (BOTTOM_LEFT_CORNER_OF_SCREEN)) { // if the bullet reaches the top of the screen, delete the bullet
    globals_set_alien_bullet_position_0(current_pos_0+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
    current_pos_0 = globals_get_alien_bullet_position_0(); // fetch the updated bullet position
    sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_0,ALIEN_SIZE,pink);
    globals_alien_bullet_stopped_0();
  }
  else { // if the bullet hasn't hit anything or reached the top of the screen, move it up
    globals_set_alien_bullet_position_0(current_pos_0+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
    current_pos_0 = globals_get_alien_bullet_position_0(); // fetch the updated bullet position
    if(odd_0){
      sprites_render_buffer(alienbullet2_up_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_0,ALIEN_SIZE,pink);
    }
    else{
      sprites_render_buffer(alienbullet2_down_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_0,ALIEN_SIZE,pink);
    }
    /* check for saucer location */
    image_render_check_for_tank(current_pos_0, ALIEN_BULLET_0);
    image_render_check_for_bunker(current_pos_0, ALIEN_BULLET_0);
  }
  odd_0 = ~odd_0;
}

// moves the alien bullet down the screen
void image_render_move_alien_bullet_1(){
  uint32_t current_pos_1 = globals_get_alien_bullet_position_1(); // fetches current bullet position
  globals_set_alien_bullet_position_1(current_pos_1+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
  current_pos_1 = globals_get_alien_bullet_position_1(); // fetch the updated bullet position
  sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_1,ALIEN_SIZE,pink);
  if(current_pos_1 > (BOTTOM_LEFT_CORNER_OF_SCREEN)) { // if the bullet reaches the top of the screen, delete the bullet
    globals_set_alien_bullet_position_1(current_pos_1+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
    current_pos_1 = globals_get_alien_bullet_position_1(); // fetch the updated bullet position
    sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_1,ALIEN_SIZE,pink);
    globals_alien_bullet_stopped_1();
  }
  else { // if the bullet hasn't hit anything or reached the top of the screen, move it up
    globals_set_alien_bullet_position_1(current_pos_1+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
    current_pos_1 = globals_get_alien_bullet_position_1(); // fetch the updated bullet position
    if(odd_1){
      sprites_render_buffer(alienbullet1_up_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_1,ALIEN_SIZE,pink);
    }
    else{
      sprites_render_buffer(alienbullet1_down_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_1,ALIEN_SIZE,pink);
    }
    /* check for saucer location */
    image_render_check_for_tank(current_pos_1, ALIEN_BULLET_1);
    image_render_check_for_bunker(current_pos_1, ALIEN_BULLET_1);
  }
  odd_1 = ~odd_1;
}

// moves the alien bullet down the screen
void image_render_move_alien_bullet_2(){
  uint32_t current_pos_2 = globals_get_alien_bullet_position_2(); // fetches current bullet position
  globals_set_alien_bullet_position_2(current_pos_2+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
  current_pos_2 = globals_get_alien_bullet_position_2(); // fetch the updated bullet position
  sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_2,ALIEN_SIZE,pink);
  if(current_pos_2 > (BOTTOM_LEFT_CORNER_OF_SCREEN)) { // if the bullet reaches the top of the screen, delete the bullet
    globals_set_alien_bullet_position_2(current_pos_2+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
    current_pos_2 = globals_get_alien_bullet_position_2(); // fetch the updated bullet position
    sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_2,ALIEN_SIZE,pink);
    globals_alien_bullet_stopped_2();
  }
  else { // if the bullet hasn't hit anything or reached the top of the screen, move it up
    globals_set_alien_bullet_position_2(current_pos_2+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
    current_pos_2 = globals_get_alien_bullet_position_2(); // fetch the updated bullet position
    if(odd_2){
      sprites_render_buffer(alienbullet2_up_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_2,ALIEN_SIZE,pink);
    }
    else{
      sprites_render_buffer(alienbullet2_down_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_2,ALIEN_SIZE,pink);
    }
    /* check for saucer location */
    image_render_check_for_tank(current_pos_2, ALIEN_BULLET_2);
    image_render_check_for_bunker(current_pos_2, ALIEN_BULLET_2);
  }
  odd_2 = ~odd_2;
}

// moves the alien bullet down the screen
void image_render_move_alien_bullet_3(){
  uint32_t current_pos_3 = globals_get_alien_bullet_position_3(); // fetches current bullet position
  globals_set_alien_bullet_position_3(current_pos_3+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
  current_pos_3 = globals_get_alien_bullet_position_3(); // fetch the updated bullet position
  sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_3,ALIEN_SIZE,pink);
  if(current_pos_3 > (BOTTOM_LEFT_CORNER_OF_SCREEN)) { // if the bullet reaches the top of the screen, delete the bullet
    globals_set_alien_bullet_position_3(current_pos_3+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
    current_pos_3 = globals_get_alien_bullet_position_3(); // fetch the updated bullet position
    sprites_render_buffer(alienbullet2_gone_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_3,ALIEN_SIZE,pink);
    globals_alien_bullet_stopped_3();
  }
  else { // if the bullet hasn't hit anything or reached the top of the screen, move it up
    globals_set_alien_bullet_position_3(current_pos_3+(BULLET_MOVEMENT_ONE_PIXELS)); // sets the new bullet position moving up the screen
    current_pos_3 = globals_get_alien_bullet_position_3(); // fetch the updated bullet position
    if(odd_3) {
      sprites_render_buffer(alienbullet1_up_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_3,ALIEN_SIZE,pink);
    }
    else {
      sprites_render_buffer(alienbullet1_down_3x7,SPRITES_ALIEN_BULLET_WIDTH,SPRITES_BULLET_HEIGHT,current_pos_3,ALIEN_SIZE,pink);
    }
    /* check for saucer location */
    image_render_check_for_tank(current_pos_3, ALIEN_BULLET_3);
    image_render_check_for_bunker(current_pos_3, ALIEN_BULLET_3);
  }
  odd_3 = ~odd_3;
}

// moves the saucer around the screen
void image_render_saucer(){
  uint32_t saucer_pos = globals_get_saucer_pos();
  // if the saucer hasn't reached the end of the screen
  if(saucer_pos > (GLOBALS_SAUCER_ROW_START_LOCATION+SAUCER_END_POSITION*IMAGE_RENDER_BYTES_PER_PIXEL)){
    sprites_render_buffer(saucer_18x9,SPRITES_SAUCER_WIDTH,SPRITES_SAUCER_HEIGHT,saucer_pos,ALIEN_SIZE,black);
    // only for milestone 2
    saucer_pos = GLOBALS_SAUCER_ROW_START_LOCATION-IMAGE_RENDER_BYTES_PER_PIXEL;
    globals_set_saucer_pos(saucer_pos);
  }
  saucer_pos = saucer_pos+IMAGE_RENDER_BYTES_PER_PIXEL;
  globals_set_saucer_pos(saucer_pos);
  sprites_render_buffer(saucer_18x9,SPRITES_SAUCER_WIDTH,SPRITES_SAUCER_HEIGHT,saucer_pos,ALIEN_SIZE,red);
}

// checks to see if the bottom of the alien block has reached the top of the bunker line
void check_for_overrun() {
  for(int i = 0; i < ALIEN_BLOCK_SIZE; i++) { // iterates through each alien
    if(alien_block[i].alive == ALIEN_ALIVE) { // checks to see if the alien is alive
      uint32_t pos = alien_block[i].current_location; // grabs its current location
      // if the alien is alive and its current location reaches the top of the bunker line, set a flag that will indicate game over
      if(pos+IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*SPRITES_ALIEN_HEIGHT*ALIEN_SIZE > BUNKER_HEIGHTS) {
        globals_assert_alien_overrun_flag();
      }
    }
  }
}

// moves the alien block around the screen
void image_render_move_alien_block() {
  check_for_overrun(); // checks to see if the bottom row has reached the bunkers
  image_render_set_left_bound(); // sets the left boundary
  image_render_set_right_bound(); // sets the right boundary
  // if an alien has been killed, then write a blank over it
  if(alien_killed) {
    uint32_t loc = globals_get_dead_alien_loc();
    sprites_render_buffer(alien_top_in_14x10,SPRITES_ALIEN_WIDTH,SPRITES_ALIEN_HEIGHT,loc,ALIEN_SIZE,black);
    alien_killed = 0;
  }
  /* If we run out of aliens in the block, create a new alien block */
  if(globals_get_total_alien_count() == 0) {
    image_render_create_alien_block();
    globals_reset_total_alien_count();
    globals_increment_current_lives();
    /* Prints the alien block completely */
    for(int i = 0; i < ALIEN_BLOCK_SIZE; i++) {
      Alien alien_temp = alien_block[i];
      sprites_render_buffer(alien_temp.image_out,alien_temp.width,alien_temp.height,alien_temp.current_location,ALIEN_SIZE,pink);
    }
  }
  /* Moves the alien block according to bullet, direction and boundaries */
  uint16_t dir = current_alien_direction;
  if(dir == IMAGE_RENDER_RIGHT_MOVEMENT) { // if the alien block is moving right
    if(alien_block_right_bound >= maximum_bound_right_alien) { // if it has reached the right bound
      for(uint32_t i = 0; i < ALIEN_BLOCK_SIZE; i++){ // if it has reached the right bound, the block should move down & continue left
        if(current_alien_position == ALIEN_OUT) { // if the alien is in the "out" position, write it in the "in" position, move down a few pixels
          if(alien_block[i].alive == ALIEN_ALIVE) { // if the alien is alive, print it
            for(int d = 0; d < ALIEN_PIXEL_MOVEMENT; d++) { // repeat this for how many pixels you want to move down
              alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
              sprites_render_buffer(alien_block[i].image_in,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
              alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
              sprites_render_buffer(alien_block[i].image_out,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
            }
          }
          else { // this means the alien is dead and needs to move down anyway
            alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
            alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
          }
        }
        else { // if the alien is in the "in" position, write it in the "out" position, move down a few pixels
          if(alien_block[i].alive == ALIEN_ALIVE) { // if the alien is alive, print it
            for(int d = 0; d < ALIEN_PIXEL_MOVEMENT; d++) { // repeat this for how many pixels you want to move down
              alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
              sprites_render_buffer(alien_block[i].image_out,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
              alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
              sprites_render_buffer(alien_block[i].image_in,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
            }
          }
          else { // this means the alien is dead and needs to move down anyway
            alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
            alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
          }
        }
      }
      current_alien_direction = ~current_alien_direction; // change the direction of the block
    }
    else { // if it hasn't reached the right bound and is moving right
      alien_block_right_bound = alien_block_right_bound+ALIEN_PIXEL_MOVEMENT*IMAGE_RENDER_BYTES_PER_PIXEL;
      alien_block_left_bound = alien_block_left_bound+ALIEN_PIXEL_MOVEMENT*IMAGE_RENDER_BYTES_PER_PIXEL;
      for(uint16_t i = 0; i < ALIEN_BLOCK_SIZE; i++) { // we will shift every alien over 2 pixels to the right
        alien_block[i].current_location = alien_block[i].current_location+ALIEN_PIXEL_MOVEMENT*IMAGE_RENDER_BYTES_PER_PIXEL;
        if(current_alien_position == ALIEN_OUT) { // if the alien is "out", rewrite it "in"
          if(alien_block[i].alive == ALIEN_ALIVE) { // only write this alien if its alive
            sprites_render_buffer(alien_block[i].image_in,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
          }
        }
        else { // if the alien is "in", rewrite it "out"
          if(alien_block[i].alive == ALIEN_ALIVE) { // if the alien is alive, print it
              sprites_render_buffer(alien_block[i].image_out,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
          }
        }
      }
      current_alien_position = ~current_alien_position; // change the global alien position (not direction)
    }
  }
  else { // if the alien block is moving left
    if(alien_block_left_bound <= maximum_bound_left_alien) { // if it has reached the left bound
      for(uint32_t i = 0; i < ALIEN_BLOCK_SIZE; i++){ // if it has reached the left bound, the block should move down & continue right
        if(current_alien_position == ALIEN_OUT) { // if the alien is in the "out" position, write it in the "in" position, move down a few pixels
          if(alien_block[i].alive == ALIEN_ALIVE) { // if the alien is alive, print it
            for(int d = 0; d < ALIEN_PIXEL_MOVEMENT; d++) { // repeat this for how many pixels you want to move down
              alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
              sprites_render_buffer(alien_block[i].image_in,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
              alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
              sprites_render_buffer(alien_block[i].image_out,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
            }
          }
          else { // this means the alien is dead and needs to move down anyway
            alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
            alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
          }
        }
        else { // if the alien is in the "in" position, write it in the "out" position, move down a few pixels
          if(alien_block[i].alive == ALIEN_ALIVE) { // if the alien is alive, print it
            for(int d = 0; d < ALIEN_PIXEL_MOVEMENT; d++) { // repeat this for how many pixels you want to move down
              alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
              sprites_render_buffer(alien_block[i].image_out,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
              alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
              sprites_render_buffer(alien_block[i].image_in,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
            }
          }
          else { // this means the alien is dead and needs to move down anyway
            alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
            alien_block[i].current_location = alien_block[i].current_location+FULL_ALIEN_MOVEMENT;
          }
        }
      }
      current_alien_direction = ~current_alien_direction; // change the direction of the block
    }
    else { // if it hasn't reached the left bound
      alien_block_right_bound = alien_block_right_bound-ALIEN_PIXEL_MOVEMENT*IMAGE_RENDER_BYTES_PER_PIXEL;
      alien_block_left_bound = alien_block_left_bound-ALIEN_PIXEL_MOVEMENT*IMAGE_RENDER_BYTES_PER_PIXEL;
      for(uint16_t i = 0; i < ALIEN_BLOCK_SIZE; i++) { // we will shift every alien in the block over to the left 2 pixels
        alien_block[i].current_location = alien_block[i].current_location-ALIEN_PIXEL_MOVEMENT*IMAGE_RENDER_BYTES_PER_PIXEL;
        if(current_alien_position == ALIEN_OUT) { // if the alien is "out", rewrite it "in"
          if(alien_block[i].alive == ALIEN_ALIVE) { // only write this alien if its alive
            sprites_render_buffer(alien_block[i].image_in,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
          }
        }
        else { // if the alien is "in", rewrite it "out"
          if(alien_block[i].alive == ALIEN_ALIVE) { // if the alien is alive, print it
              sprites_render_buffer(alien_block[i].image_out,alien_block[i].width,alien_block[i].height,alien_block[i].current_location,ALIEN_SIZE,pink);
          }
        }
      }
      current_alien_position = ~current_alien_position; // change the global alien position (not direction)
    }
  }
}

// aliens fire bullets
void image_render_alien_fire_bullet_0(){
  total_shooters_0 = 0;
  /* Intializes random number generator */
  srand(time(0));
  for(int32_t k = 0; k < ELEVEN_COLUMNS; k++){ // iterate through each column
    for(int32_t i = 0; i < FIVE_ROWS; i++){ // iterate through each row
      if(alien_block[(k+((ROW_COUNTER-i)*ELEVEN_COLUMNS))].alive){ // if the alien is alive, set it to the shooter
        alien_block[(k+((ROW_COUNTER-i)*ELEVEN_COLUMNS))].shooter = ALIEN_SHOOTING;
        total_shooters_0++;
        i = FIVE_ROWS;
      }
    }
  }
  /* Print 1 random numbers from 0 to 10 */
  uint32_t temp_0 = rand() % total_shooters_0;
  for(uint32_t j = 0; j < ALIEN_BLOCK_SIZE; j++){ // iterate through the whole block
    if(alien_block[j].shooter){ // if the alien is a shooter
      if (temp_0 == 0){ // set it as a current shooter
        current_shooter_0 = alien_block[j].current_location+IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*SPRITES_CHARACTER_WIDTH+IMAGE_RENDER_SCREEN_WIDTH*ONE_PIXEL*SPRITES_CHARACTER_HEIGHT*ALIEN_SIZE*ALIEN_SIZE;
      }
      temp_0--;
    }
  }
}

// aliens fire bullets
void image_render_alien_fire_bullet_1(){
  total_shooters_1 = 0;
  /* Intializes random number generator */
  srand(time(0)+1);
  for(int32_t k = 0; k < ELEVEN_COLUMNS; k++){ // iterate through each column
    for(int32_t i = 0; i < FIVE_ROWS; i++){ // iterate through each row
      if(alien_block[(k+((ROW_COUNTER-i)*ELEVEN_COLUMNS))].alive){ // if the alien is alive, set it to the shooter
        alien_block[(k+((ROW_COUNTER-i)*ELEVEN_COLUMNS))].shooter = ALIEN_SHOOTING;
        total_shooters_1++;
        i = FIVE_ROWS;
      }
    }
  }
  /* Print 1 random numbers from 0 to 10 */
  uint32_t temp_1 = rand() % total_shooters_1;
  for(uint32_t j = 0; j < (ALIEN_BLOCK_SIZE); j++){ // iterate through the whole block
    if(alien_block[j].shooter){ // if the alien is a shooter
      if (temp_1 == 0){ // set it as a current shooter
        current_shooter_1 = alien_block[j].current_location + IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*SPRITES_CHARACTER_WIDTH+IMAGE_RENDER_SCREEN_WIDTH*ONE_PIXEL*SPRITES_CHARACTER_HEIGHT*ALIEN_SIZE*ALIEN_SIZE;

      }
      temp_1--;
    }
  }
}

// aliens fire bullets
void image_render_alien_fire_bullet_2(){
  total_shooters_2 = 0;
  /* Intializes random number generator */
  srand(time(0)+2);
  for(int32_t k = 0; k < ELEVEN_COLUMNS; k++){  // iterate through each column
    for(int32_t i = 0; i < FIVE_ROWS; i++){ // iterate through each row
      if(alien_block[(k+((ROW_COUNTER-i)*ELEVEN_COLUMNS))].alive){ // if the alien is alive, set it to the shooter
        alien_block[(k+((ROW_COUNTER-i)*ELEVEN_COLUMNS))].shooter = ALIEN_SHOOTING;
        total_shooters_2++;
        i = FIVE_ROWS;
      }
    }
  }
  /* Print 1 random numbers from 0 to 10 */
  uint32_t temp_2 = rand() % total_shooters_2;
  for(uint32_t j = 0; j < (ALIEN_BLOCK_SIZE); j++){ // iterate through the whole block
    if(alien_block[j].shooter){ // if the alien is a shooter
      if (temp_2 == 0){ // set it as a current shooter
        current_shooter_2 = alien_block[j].current_location + IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*SPRITES_CHARACTER_WIDTH+IMAGE_RENDER_SCREEN_WIDTH*ONE_PIXEL*SPRITES_CHARACTER_HEIGHT*ALIEN_SIZE*ALIEN_SIZE;
      }
      temp_2--;
    }
  }
}

// aliens fire bullets
void image_render_alien_fire_bullet_3() {
  total_shooters_3 = 0;
  /* Intializes random number generator */
  srand(time(0)+3);
  for(int32_t k = 0; k < ELEVEN_COLUMNS; k++){  // iterate through each column
    for(int32_t i = 0; i < FIVE_ROWS; i++){ // iterate through each row
      if(alien_block[(k+((ROW_COUNTER-i)*ELEVEN_COLUMNS))].alive){ // if the alien is alive, set it to the shooter
        alien_block[(k+((ROW_COUNTER-i)*ELEVEN_COLUMNS))].shooter = ALIEN_SHOOTING;
        total_shooters_3++;
        i = FIVE_ROWS;
      }
    }
  }
  /* Print 1 random numbers from 0 to 10 */
  uint32_t temp_3 = rand() % total_shooters_3;
  for(uint32_t j = 0; j < (ALIEN_BLOCK_SIZE); j++){ // iterate through the whole block
    if(alien_block[j].shooter){ // if the alien is a shooter
      if (temp_3 == 0){ // set it as a current shooter
        current_shooter_3 = alien_block[j].current_location + IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE*SPRITES_CHARACTER_WIDTH+IMAGE_RENDER_SCREEN_WIDTH*ONE_PIXEL*SPRITES_CHARACTER_HEIGHT*ALIEN_SIZE*ALIEN_SIZE;
      }
      temp_3--;
    }
  }
}

// closes the hdmi connection
void image_render_close() {
  hdmi_exit();
}
