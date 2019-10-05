#include "score_board.h"
#include "hdmi.h"
#include "image_render.h"
#include "sprites.h"

/********************************** macros ***********************************/
#define IMAGE_RENDER_SCREEN_WIDTH 640 /* number of pixels across the screen */
#define IMAGE_RENDER_SCREEN_HEIGHT 480 /* number of pixels down the screen */
#define IMAGE_RENDER_BYTES_PER_PIXEL 3 /* number of bytes for every pixel */
/* number of bytes for the whole screen */
#define IMAGE_RENDER_WHOLE_SCREEN IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_SCREEN_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL
#define NUMBER_OF_CHARS_FOR_NAME 3
#define NUMBER_OF_BITS_PER_CHAR 25
#define LETTER_A 11
#define ALIEN_DEPTH ALIEN_SIZE*SPRITES_ALIEN_HEIGHT
#define ALIEN_BLOCK_ROW_0 IMAGE_RENDER_SCREEN_WIDTH*45
#define ALIEN_BLOCK_ROW_1 ALIEN_BLOCK_ROW_0+IMAGE_RENDER_SCREEN_WIDTH*(ALIEN_DEPTH+15)
#define ALIEN_BLOCK_ROW_2 ALIEN_BLOCK_ROW_1+IMAGE_RENDER_SCREEN_WIDTH*(ALIEN_DEPTH+15)
#define ALIEN_BLOCK_ROW_3 ALIEN_BLOCK_ROW_2+IMAGE_RENDER_SCREEN_WIDTH*(ALIEN_DEPTH+15)
#define ALIEN_BLOCK_ROW_4 ALIEN_BLOCK_ROW_3+IMAGE_RENDER_SCREEN_WIDTH*(ALIEN_DEPTH+15)
#define ALIEN_TOP_POINTS 40
#define ALIEN_MIDDLE_POINTS 20
#define ALIEN_BOTTOM_POINTS 10
#define ROW_0_TOP 0
#define ROW_1 11
#define ROW_2 22
#define ROW_3 33
#define ROW_4_BOTTOM 44
#define COLUMN_O_LEFT 0
#define COLUMN_1 1
#define COLUMN_2 2
#define COLUMN_3 3
#define COLUMN_4 4
#define COLUMN_5 5
#define COLUMN_6 6
#define COLUMN_7 7
#define COLUMN_8 8
#define COLUMN_9 9
#define COLUMN_10_RIGHT 10
#define ALIEN_SIZE 3
#define ALIEN_OFFSET SPRITES_ALIEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE
#define ALIEN_BLOCK_SIZE 55

/********************************** globals **********************************/
char full_screen_black[IMAGE_RENDER_WHOLE_SCREEN];
char black[3] = {0x00, 0x00, 0x00};
char pink[3] = {0xFF, 0x69, 0xB4};
char white[3] = {0xFF, 0xFF, 0xFF};
// Alien alien_0_0;
// Alien alien_0_1;
// Alien alien_0_2;
// Alien alien_0_3;
// Alien alien_0_4;
// Alien alien_0_5;
// Alien alien_0_6;
// Alien alien_0_7;
// Alien alien_0_8;
// Alien alien_0_9;
// Alien alien_0_10;
// Alien alien_1_0;
// Alien alien_1_1;
// Alien alien_1_2;
// Alien alien_1_3;
// Alien alien_1_4;
// Alien alien_1_5;
// Alien alien_1_6;
// Alien alien_1_7;
// Alien alien_1_8;
// Alien alien_1_9;
// Alien alien_1_10;
// Alien alien_2_0;
// Alien alien_2_1;
// Alien alien_2_2;
// Alien alien_2_3;
// Alien alien_2_4;
// Alien alien_2_5;
// Alien alien_2_6;
// Alien alien_2_7;
// Alien alien_2_8;
// Alien alien_2_9;
// Alien alien_2_10;
// Alien alien_3_0;
// Alien alien_3_1;
// Alien alien_3_2;
// Alien alien_3_3;
// Alien alien_3_4;
// Alien alien_3_5;
// Alien alien_3_6;
// Alien alien_3_7;
// Alien alien_3_8;
// Alien alien_3_9;
// Alien alien_3_10;
// Alien alien_4_0;
// Alien alien_4_1;
// Alien alien_4_2;
// Alien alien_4_3;
// Alien alien_4_4;
// Alien alien_4_5;
// Alien alien_4_6;
// Alien alien_4_7;
// Alien alien_4_8;
// Alien alien_4_9;
// Alien alien_4_10;
Alien alien_block[ALIEN_BLOCK_SIZE];

/**************************** function prototypes ****************************/
void image_render_print_black_screen();

/********************************* functions *********************************/
// initializes a few of our necessary variables and starts the screen black
void image_render_init() {
  hdmi_init(HDMI_FILE_PATH); /* opens a path to the HDMI driver (enables/read write) */
  // initializes a buffer that contains all zeroes to print black to the entire screen
  for(uint32_t i = 0; i < IMAGE_RENDER_WHOLE_SCREEN; i++) {
    full_screen_black[i++] = 0x00; // if all bits are set to 0x00, the pixel will be black
    full_screen_black[i++] = 0x00;
    full_screen_black[i] = 0x00;
  }
  image_render_print_black_screen(); // call the print screen function
}

// prints the screen black and resets the cursor to the top left corner of the screen
void image_render_print_black_screen() {
  hdmi_set_offset(0); /* this will set the fd offset back to the top left of the screen */
  hdmi_write(full_screen_black,IMAGE_RENDER_WHOLE_SCREEN); /* this will write a black screen to the hdmi monitor */
  hdmi_set_offset(0); /* this will reset the fd offset back to the top left of the screen */
}

// creates a single alien object
// image : the image of the alien to be created
// starting_location : starting location of the alien
// points : how many points the alien is worth
// placement : where the alien is inside the block
Alien image_render_create_alien(const uint32_t image[], uint32_t starting_location, uint32_t points, uint32_t placement) {
  Alien alien;
  for (int i = 0; i < IMAGE_RENDER_ALIEN_PIXEL_TOTAL; i++) { // sets the alien image
    alien.image[i] = image[i];
  }
  alien.current_location = starting_location;
  alien.points = points;
  alien.block_placement = placement;
  alien.position = IMAGE_RENDER_ALIEN_OUT;
  alien.width = SPRITES_ALIEN_WIDTH;
  alien.height = SPRITES_ALIEN_HEIGHT;
  alien.alive = IMAGE_RENDER_ALIEN_ALIVE;
  return alien;
}

// modifies a single alien
// alien : passed in the alien to be modified
// modifer : how the alien is to be modified (moving or destroyed)
void image_render_modify_alien(Alien alien, uint16_t modifier) {

}

// creates the entire alien block
void image_render_create_alien_block() {
  alien_block[ROW_0_TOP+COLUMN_0_LEFT)] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_O_LEFT);
  alien_block[ROW_0_TOP+COLUMN_1] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_1,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_1);
  alien_block[ROW_0_TOP+COLUMN_2] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_2,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_2);
  alien_block[ROW_0_TOP+COLUMN_3] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_3,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_3);
  alien_block[ROW_0_TOP+COLUMN_4] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_4,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_4);
  alien_block[ROW_0_TOP+COLUMN_5] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_5,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_5);
  alien_block[ROW_0_TOP+COLUMN_6] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_6,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_6);
  alien_block[ROW_0_TOP+COLUMN_7] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_7,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_7);
  alien_block[ROW_0_TOP+COLUMN_8] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_8,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_8);
  alien_block[ROW_0_TOP+COLUMN_9] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_9,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_9);
  alien_block[ROW_0_TOP+COLUMN_10_RIGHT] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0+ALIEN_OFFSET*COLUMN_10_RIGHT,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_10_RIGHT);
  alien_block[ROW_1+COLUMN_0_LEFT)] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_0_LEFT);
  alien_block[ROW_1+COLUMN_1] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_1,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_1);
  alien_block[ROW_1+COLUMN_2] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_2,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_2);
  alien_block[ROW_1+COLUMN_3] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_3,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_3);
  alien_block[ROW_1+COLUMN_4] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_4,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_4);
  alien_block[ROW_1+COLUMN_5] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_5,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_5);
  alien_block[ROW_1+COLUMN_6] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_6,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_6);
  alien_block[ROW_1+COLUMN_7] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_7,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_7);
  alien_block[ROW_1+COLUMN_8] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_8,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_8);
  alien_block[ROW_1+COLUMN_9] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_9,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_9);
  alien_block[ROW_1+COLUMN_10_RIGHT] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1+ALIEN_OFFSET*COLUMN_10_RIGHT,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_10_RIGHT);
  alien_block[ROW_2+COLUMN_0_LEFT)] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_0_LEFT);
  alien_block[ROW_2+COLUMN_1] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_1,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_1);
  alien_block[ROW_2+COLUMN_2] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_2,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_2);
  alien_block[ROW_2+COLUMN_3] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_3,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_3);
  alien_block[ROW_2+COLUMN_4] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_4,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_4);
  alien_block[ROW_2+COLUMN_5] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_5,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_5);
  alien_block[ROW_2+COLUMN_6] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_6,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_6);
  alien_block[ROW_2+COLUMN_7] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_7,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_7);
  alien_block[ROW_2+COLUMN_8] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_8,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_8);
  alien_block[ROW_2+COLUMN_9] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_9,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_9);
  alien_block[ROW_2+COLUMN_10_RIGHT] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2+ALIEN_OFFSET*COLUMN_10_RIGHT,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_10_RIGHT);
  alien_block[ROW_3+COLUMN_0_LEFT)] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_0_LEFT);
  alien_block[ROW_3+COLUMN_1] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_1,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_1);
  alien_block[ROW_3+COLUMN_2] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_2,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_2);
  alien_block[ROW_3+COLUMN_3] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_3,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_3);
  alien_block[ROW_3+COLUMN_4] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_4,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_4);
  alien_block[ROW_3+COLUMN_5] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_5,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_5);
  alien_block[ROW_3+COLUMN_6] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_6,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_6);
  alien_block[ROW_3+COLUMN_7] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_7,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_7);
  alien_block[ROW_3+COLUMN_8] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_8,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_8);
  alien_block[ROW_3+COLUMN_9] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_9,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_9);
  alien_block[ROW_3+COLUMN_10_RIGHT] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_10_RIGHT,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_10_RIGHT);
  alien_block[ROW_4_BOTTOM+COLUMN_0_LEFT)] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_0_LEFT);
  alien_block[ROW_4_BOTTOM+COLUMN_1] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_1,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_1);
  alien_block[ROW_4_BOTTOM+COLUMN_2] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_2,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_2);
  alien_block[ROW_4_BOTTOM+COLUMN_3] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_3,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_3);
  alien_block[ROW_4_BOTTOM+COLUMN_4] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_4,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_4);
  alien_block[ROW_4_BOTTOM+COLUMN_5] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_5,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_5);
  alien_block[ROW_4_BOTTOM+COLUMN_6] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_6,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_6);
  alien_block[ROW_4_BOTTOM+COLUMN_7] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_7,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_7);
  alien_block[ROW_4_BOTTOM+COLUMN_8] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_8,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_8);
  alien_block[ROW_4_BOTTOM+COLUMN_9] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_9,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_9);
  alien_block[ROW_4_BOTTOM+COLUMN_10_RIGHT] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_10_RIGHT,ALIEN_BOTTOM_POINTS,ROW_4+COLUMN_10_RIGHT);
}

// prints the first part of the game over screen
// this will display "GAME OVER" and "Enter your name" and a place to enter the player name
void image_render_print_game_over_screen_enter_name() {
  image_render_print_black_screen(); // print the whole screen black
  /* print out the words GAME OVER */
  sprites_render_image(letterG_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_G_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_image(letterA_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_A_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_image(letterM_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_M_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_image(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_O_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_image(letterV_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_V_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E2_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  sprites_render_image(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R_START_LOCATION_GO,SPRITES_GAME_OVER_SCALING,white);
  /* print out the words ENTER YOUR NAME */
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterN_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_N_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterT_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_T_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterY_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_Y_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_O_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterU_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_U_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterN_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_N2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterA_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_A_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterM_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_M_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E3_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,white);
  /* print out three original characters for player name */
  sprites_render_image(char_array[LETTER_A],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_CHARACTER_ONE_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(char_array[LETTER_A],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_CHARACTER_TWO_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(char_array[LETTER_A],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_CHARACTER_THREE_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,white);
}

// prints the second part of the game over screen
// this will display "GAME OVER" and "High Scores" and the list of high scores
void image_render_print_game_over_screen_high_scores() {
  /* clear the bottom portion of the screen */
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterN_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_N_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterT_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_T_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterY_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_Y_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_O_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterU_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_U_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterN_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_N2_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterA_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_A_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterM_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_M_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E3_START_LOCATION_YN,SPRITES_NORMAL_CHARACTER_SCALING,black);
  /* print high scores image */
  sprites_render_image(letterH_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_H_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterI_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_I_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterG_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_G_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterH_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_H2_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterS_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_S_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterC_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_C_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_O_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_R_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_E_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterS_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SPRITES_S2_START_LOCATION_HS,SPRITES_NORMAL_CHARACTER_SCALING,white);
}

// prints the start screen and everything on it
void image_render_print_start_screen() {
  for(int i = 0; i < ALIEN_BLOCK_SIZE; i++) { // prints the alien block completely
    Alien alien_temp = alien_block[i];
    sprites_render_image(alien_temp.image,alien_temp.height,alien_temp.width,alien_temp.current_location,ALIEN_SIZE,white);
  }
}

void image_render_update_screen() {}

// ONLY FOR TESTING, DELETE LATER
void image_render_test_image() {
  // uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location,
  // uint32_t scaling_factor, const void *color
  sprites_render_image(saucer_16x7,16,7,0,1,white);
  sprites_render_image(alien_explosion_12x10,12,10,60,1,white);
  sprites_render_image(alien_top_in_12x8,12,8,108,1,pink);
  sprites_render_image(alien_top_out_12x8,12,8,156,1,pink);
  sprites_render_image(alien_middle_in_12x8,12,8,204,1,pink);
  sprites_render_image(alien_middle_out_12x8,12,8,252,1,pink);
  sprites_render_image(alien_bottom_in_12x8,12,8,300,1,pink);
  sprites_render_image(alien_bottom_out_12x8,12,8,348,1,pink);
  sprites_render_image(tank_15x8,15,8,405,1,white);
  sprites_render_image(bunker_24x18,24,18,462,1,white);
  sprites_render_image(bunkerDamage0_6x6,6,6,546,1,white);
  sprites_render_image(tankbullet_1x5,1,5,576,1,pink);
  sprites_render_image(alienbullet1_down_3x5,3,5,591,1,pink);
  sprites_render_image(alienbullet2_up_3x5,3,5,612,1,pink);
  sprites_render_image(letterD_5x5,5,5,633,1,white);
  sprites_render_image(letterA_5x5,5,5,660,1,white);
  sprites_render_image(letterX_5x5,5,5,687,1,white);
  sprites_render_image(number2_5x5,5,5,714,1,white);
  sprites_render_image(number1_5x5,5,5,741,1,white);
}

// closes the hdmi connection
void image_render_close() {
  hdmi_exit();
}
