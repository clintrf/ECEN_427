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
#define ALIEN_DEPTH ALIEN_SIZE*SPRITES_ALIEN_HEIGHT // 24
#define SAUCER_ROW_START_LOCATION (15+(IMAGE_RENDER_SCREEN_WIDTH*3)*20)
#define ALIEN_BLOCK_ROW_0 (15+(IMAGE_RENDER_SCREEN_WIDTH*3)*39)
#define ALIEN_BLOCK_ROW_1 ALIEN_BLOCK_ROW_0+(IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15)
#define ALIEN_BLOCK_ROW_2 ALIEN_BLOCK_ROW_1+(IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15)
#define ALIEN_BLOCK_ROW_3 ALIEN_BLOCK_ROW_2+(IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15)
#define ALIEN_BLOCK_ROW_4 ALIEN_BLOCK_ROW_3+(IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15)
#define FIFTH_LIFE_START_LOCATION ((IMAGE_RENDER_SCREEN_WIDTH*3)-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9)
#define FOURTH_LIFE_START_LOCATION (FIFTH_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9)
#define THIRD_LIFE_START_LOCATION (FOURTH_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9)
#define SECOND_LIFE_START_LOCATION (THIRD_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9)
#define FIRST_LIFE_START_LOCATION (SECOND_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9)
#define S_START_LOCATION_LVS (FIRST_LIFE_START_LOCATION-(ALIEN_SIZE*SPRITES_TANK_WIDTH*3)-9)
#define E_START_LOCATION_LVS (S_START_LOCATION_LVS-(SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_WIDTH*3)-9)
#define V_START_LOCATION_LVS (E_START_LOCATION_LVS-(SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_WIDTH*3)-9)
#define I_START_LOCATION_LVS (V_START_LOCATION_LVS-(SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_WIDTH*3)-9)
#define L_START_LOCATION_LVS (I_START_LOCATION_LVS-(SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_WIDTH*3)-9)
#define ALIEN_TOP_POINTS 40
#define ALIEN_MIDDLE_POINTS 20
#define ALIEN_BOTTOM_POINTS 10
#define ROW_0_TOP 0
#define ROW_1 11
#define ROW_2 22
#define ROW_3 33
#define ROW_4_BOTTOM 44
#define COLUMN_0_LEFT 0
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
#define ALIEN_BUFFER_AREA 15
#define ALIEN_SIZE 2
#define ALIEN_OFFSET ((SPRITES_ALIEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE)+15)
#define ALIEN_BLOCK_SIZE 55
#define S_START_LOCATION_SC 60
#define C_START_LOCATION_SC S_START_LOCATION_SC+54
#define O_START_LOCATION_SC C_START_LOCATION_SC+54
#define R_START_LOCATION_SC O_START_LOCATION_SC+54
#define E_START_LOCATION_SC R_START_LOCATION_SC+54
#define SCORE_0_START_LOCATION E_START_LOCATION_SC+108
#define SCORE_1_START_LOCATION SCORE_0_START_LOCATION+54
#define SCORE_2_START_LOCATION SCORE_1_START_LOCATION+54
#define SCORE_3_START_LOCATION SCORE_2_START_LOCATION+54
#define SCORE_4_START_LOCATION SCORE_3_START_LOCATION+54
#define CHARACTER_ZERO 0

/********************************** globals **********************************/
char full_screen_black[IMAGE_RENDER_WHOLE_SCREEN];
char black[3] = {0x00, 0x00, 0x00};
char pink[3] = {0xFF, 0x69, 0xB4};
char white[3] = {0xFF, 0xFF, 0xFF};
char green[3] = {0x00,0x80,0x00};
char red[3] = {0xFF,0x00,0x00};
Alien alien_block[ALIEN_BLOCK_SIZE];

uint32_t tank_start_pos = 200;

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
  image_render_create_alien_block(); // creates the original alien block
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
    printf("%zu\n",image[i]);
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
  alien_block[ROW_0_TOP+COLUMN_0_LEFT] = image_render_create_alien(alien_top_out_12x8,ALIEN_BLOCK_ROW_0,ALIEN_TOP_POINTS,ROW_0_TOP+COLUMN_0_LEFT);
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
  alien_block[ROW_1+COLUMN_0_LEFT] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_1,ALIEN_MIDDLE_POINTS,ROW_1+COLUMN_0_LEFT);
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
  alien_block[ROW_2+COLUMN_0_LEFT] = image_render_create_alien(alien_middle_out_12x8,ALIEN_BLOCK_ROW_2,ALIEN_MIDDLE_POINTS,ROW_2+COLUMN_0_LEFT);
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
  alien_block[ROW_3+COLUMN_0_LEFT] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_0_LEFT);
  alien_block[ROW_3+COLUMN_1] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_1,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_1);
  alien_block[ROW_3+COLUMN_2] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_2,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_2);
  alien_block[ROW_3+COLUMN_3] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_3,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_3);
  alien_block[ROW_3+COLUMN_4] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_4,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_4);
  alien_block[ROW_3+COLUMN_5] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_5,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_5);
  alien_block[ROW_3+COLUMN_6] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_6,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_6);
  alien_block[ROW_3+COLUMN_7] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_7,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_7);
  alien_block[ROW_3+COLUMN_8] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_8,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_8);
  alien_block[ROW_3+COLUMN_9] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_9,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_9);
  alien_block[ROW_3+COLUMN_10_RIGHT] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_3+ALIEN_OFFSET*COLUMN_10_RIGHT,ALIEN_BOTTOM_POINTS,ROW_3+COLUMN_10_RIGHT);
  alien_block[ROW_4_BOTTOM+COLUMN_0_LEFT] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_0_LEFT);
  alien_block[ROW_4_BOTTOM+COLUMN_1] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_1,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_1);
  alien_block[ROW_4_BOTTOM+COLUMN_2] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_2,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_2);
  alien_block[ROW_4_BOTTOM+COLUMN_3] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_3,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_3);
  alien_block[ROW_4_BOTTOM+COLUMN_4] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_4,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_4);
  alien_block[ROW_4_BOTTOM+COLUMN_5] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_5,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_5);
  alien_block[ROW_4_BOTTOM+COLUMN_6] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_6,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_6);
  alien_block[ROW_4_BOTTOM+COLUMN_7] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_7,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_7);
  alien_block[ROW_4_BOTTOM+COLUMN_8] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_8,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_8);
  alien_block[ROW_4_BOTTOM+COLUMN_9] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_9,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_9);
  alien_block[ROW_4_BOTTOM+COLUMN_10_RIGHT] = image_render_create_alien(alien_bottom_out_12x8,ALIEN_BLOCK_ROW_4+ALIEN_OFFSET*COLUMN_10_RIGHT,ALIEN_BOTTOM_POINTS,ROW_4_BOTTOM+COLUMN_10_RIGHT);
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
  printf("Checkpoint2\n");
  /* Print the word score in the top left corner and then print the score */
  sprites_render_image(letterS_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,S_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterC_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,C_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterO_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,O_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterR_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,R_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,E_START_LOCATION_SC,SPRITES_NORMAL_CHARACTER_SCALING,white);
  /* Print the current score on the screen (starts at 00000) */
  sprites_render_image(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_0_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  sprites_render_image(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_1_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  sprites_render_image(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_2_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  sprites_render_image(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_3_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  sprites_render_image(char_array[CHARACTER_ZERO],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_4_START_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,green);
  /* Print the lives section of the start screen */
  sprites_render_image(letterL_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,L_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterI_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,I_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterV_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,V_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterE_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,E_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterS_5x5,SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,S_START_LOCATION_LVS,SPRITES_NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(tank_15x8,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,FIRST_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_image(tank_15x8,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,SECOND_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_image(tank_15x8,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,THIRD_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_image(tank_15x8,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,FOURTH_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_image(tank_15x8,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,FIFTH_LIFE_START_LOCATION,ALIEN_SIZE,green);
  /* Prints the saucer as a starter */
  sprites_render_image(saucer_16x7,SPRITES_SAUCER_WIDTH,SPRITES_SAUCER_HEIGHT,SAUCER_ROW_START_LOCATION,ALIEN_SIZE,red);
  /* Prints the alien block completely */
  for(int i = 0; i < ALIEN_BLOCK_SIZE; i++) {
    Alien alien_temp = alien_block[i];
    sprites_render_image(alien_temp.image,alien_temp.width,alien_temp.height,alien_temp.current_location,ALIEN_SIZE,white);
  }
}

void image_render_update_screen() {}

void image_render_tank(uint32_t move, char dir){
  if(dir == 1){
    tank_pos = tank_pos+move;
    sprites_render_image(tank_17x10,17,10,640*3*450+(tank_start_pos*3),2,white);

  }
  if(dir == 0){
    tank_pos = tank_pos-move;
    sprites_render_image(tank_17x10,17,10,640*3*450+(tank_start_pos*3),2,white);
  }
  else{
    printf("not move");
  }
};

// closes the hdmi connection
void image_render_close() {
  hdmi_exit();
}
