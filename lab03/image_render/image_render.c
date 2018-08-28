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
#define ROW_ONE 0
#define ROW_TWO 1
#define ROW_THREE 2
#define ROW_FOUR 3
#define ROW_FIVE 4
#define FIVE_ROWS 5
#define ELEVEN_COLUMNS 11
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
#define ALIEN_OFFSET ((SPRITES_ALIEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*ALIEN_SIZE)+30)
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
#define BOTTOM_LEFT_CORNER_OF_SCREEN 640*3*450
#define CHARACTER_ZERO 0
#define LEFT_BOUND 2
#define RIGHT_BOUND_TANK 604
#define COLUMN_ZERO_LEFT_BOUND ALIEN_BLOCK_ROW_0
#define COLUMN_ZERO_RIGHT_BOUND ALIEN_BLOCK_ROW_0+ALIEN_SIZE*IMAGE_RENDER_BYTES_PER_PIXEL*SPRITES_ALIEN_WIDTH
#define COLUMN_ONE_LEFT_BOUND COLUMN_ZERO_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_ONE_RIGHT_BOUND COLUMN_ZERO_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_TWO_LEFT_BOUND COLUMN_ONE_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_TWO_RIGHT_BOUND COLUMN_ONE_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_THREE_LEFT_BOUND COLUMN_TWO_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_THREE_RIGHT_BOUND COLUMN_TWO_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_FOUR_LEFT_BOUND COLUMN_THREE_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_FOUR_RIGHT_BOUND COLUMN_THREE_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_FIVE_LEFT_BOUND COLUMN_FOUR_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_FIVE_RIGHT_BOUND COLUMN_FOUR_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_SIX_LEFT_BOUND COLUMN_FIVE_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_SIX_RIGHT_BOUND COLUMN_FIVE_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_SEVEN_LEFT_BOUND COLUMN_SIX_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_SEVEN_RIGHT_BOUND COLUMN_SIX_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_EIGHT_LEFT_BOUND COLUMN_SEVEN_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_EIGHT_RIGHT_BOUND COLUMN_SEVEN_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_NINE_LEFT_BOUND COLUMN_EIGHT_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_NINE_RIGHT_BOUND COLUMN_EIGHT_RIGHT_BOUND+ALIEN_OFFSET
#define COLUMN_TEN_LEFT_BOUND COLUMN_NINE_LEFT_BOUND+ALIEN_OFFSET
#define COLUMN_TEN_RIGHT_BOUND COLUMN_NINE_RIGHT_BOUND+ALIEN_OFFSET
#define TANK_START_POSITION 200
#define ALIEN_OUT 0

/********************************** globals **********************************/
char full_screen_black[IMAGE_RENDER_WHOLE_SCREEN];
uint32_t black[3] = {0x00, 0x00, 0x00};
uint32_t pink[3] = {0xFF, 0x69, 0xB4};
uint32_t white[3] = {0xFF, 0xFF, 0xFF};
uint32_t green[3] = {0x00,0x80,0x00};
uint32_t red[3] = {0xFF,0x00,0x00};
Alien alien_block[ALIEN_BLOCK_SIZE];

char green_char[3] = {0x00,0x80,0x00};

uint32_t tank_start_pos;
uint32_t saucer_start_pos;
uint32_t alien_block_right_bound;
uint32_t alien_block_left_bound;
uint16_t current_alien_direction;
uint32_t maximum_bound_right_alien;
uint32_t maximum_bound_left_alien;
uint16_t current_alien_position;

/**************************** function prototypes ****************************/
void image_render_print_black_screen();
void image_render_create_alien_block(uint16_t in_out);

/********************************* functions *********************************/
// initializes a few of our necessary variables and starts the screen black
void image_render_init() {
  hdmi_init(HDMI_FILE_PATH); /* opens a path to the HDMI driver (enables/read write) */
  tank_start_pos = TANK_START_POSITION;
  saucer_start_pos = SAUCER_ROW_START_LOCATION;
  alien_block_right_bound = COLUMN_TEN_RIGHT_BOUND;
  alien_block_left_bound = COLUMN_ZERO_LEFT_BOUND;
  current_alien_direction = IMAGE_RENDER_RIGHT_MOVEMENT;
  maximum_bound_right_alien = ALIEN_BLOCK_ROW_0+IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL-30;
  maximum_bound_left_alien = ALIEN_BLOCK_ROW_0;
  current_alien_position = ALIEN_OUT;
  // initializes a buffer that contains all zeroes to print black to the entire screen
  for(uint32_t i = 0; i < IMAGE_RENDER_WHOLE_SCREEN; i++) {
    full_screen_black[i++] = 0x00; // if all bits are set to 0x00, the pixel will be black
    full_screen_black[i++] = 0x00;
    full_screen_black[i] = 0x00;
  }
  image_render_print_black_screen(); // call the print screen function
  image_render_create_alien_block(0); // creates the original alien block
}

// prints the screen black and resets the cursor to the top left corner of the screen
void image_render_print_black_screen() {
  hdmi_set_offset(0); /* this will set the fd offset back to the top left of the screen */
  hdmi_write(full_screen_black,IMAGE_RENDER_WHOLE_SCREEN); /* this will write a black screen to the hdmi monitor */
  hdmi_set_offset(0); /* this will reset the fd offset back to the top left of the screen */
}

// creates a single alien object
// image_in : the image of the alien to be created (in)
// image_out : the image of the alien to be created (out)
// starting_location : starting location of the alien
// points : how many points the alien is worth
// placement : where the alien is inside the block
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
  return alien;
}

// modifies a single alien
// alien : passed in the alien to be modified
// modifer : how the alien is to be modified (moving or destroyed)
void image_render_modify_alien(Alien alien, uint16_t modifier) {

}

// creates the entire alien block
void image_render_create_alien_block(uint16_t in_out) {
  int r = 0;
  for(int i=0; i<FIVE_ROWS; i++){
    for(int c=0; c<ELEVEN_COLUMNS; c++){
      if(i==ROW_ONE){
        alien_block[r+c] = image_render_create_alien(alien_top_in_14x10,alien_top_out_14x10,ALIEN_BLOCK_ROW_0+i*((IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15))+ALIEN_OFFSET*c,ALIEN_TOP_POINTS,r+c);
      }
      if(i==ROW_TWO || i==ROW_THREE){
        alien_block[r+c] = image_render_create_alien(alien_middle_in_14x10,alien_middle_out_14x10,ALIEN_BLOCK_ROW_0+i*((IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15))+ALIEN_OFFSET*c,ALIEN_MIDDLE_POINTS,r+c);
      }
      if(i==ROW_FOUR || i==ROW_FIVE){
        alien_block[r+c] = image_render_create_alien(alien_bottom_in_14x10,alien_bottom_out_14x10,ALIEN_BLOCK_ROW_0+i*((IMAGE_RENDER_SCREEN_WIDTH*3)*(ALIEN_DEPTH+15))+ALIEN_OFFSET*c,ALIEN_BOTTOM_POINTS,r+c);
      }
    }
    r = r+ROW_1;
  }
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
  sprites_render_buffer(tank_15x8,15,8,FIRST_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_buffer(tank_15x8,15,8,SECOND_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_buffer(tank_15x8,15,8,THIRD_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_buffer(tank_15x8,15,8,FOURTH_LIFE_START_LOCATION,ALIEN_SIZE,green);
  sprites_render_buffer(tank_15x8,15,8,FIFTH_LIFE_START_LOCATION,ALIEN_SIZE,green);

  /* Prints bunkers */
  sprites_render_buffer(bunker_24x18,SPRITES_BUNKER_WIDTH,SPRITES_BUNKER_HEIGHT,(400*(IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)+(72*3)),ALIEN_SIZE,green);
  sprites_render_buffer(bunker_24x18,SPRITES_BUNKER_WIDTH,SPRITES_BUNKER_HEIGHT,(400*(IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)+(3*72*3)),ALIEN_SIZE,green);
    sprites_render_buffer(bunker_24x18,SPRITES_BUNKER_WIDTH,SPRITES_BUNKER_HEIGHT,(400*(IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)+(5*72*3)),ALIEN_SIZE,green);
      sprites_render_buffer(bunker_24x18,SPRITES_BUNKER_WIDTH,SPRITES_BUNKER_HEIGHT,(400*(IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL)+(7*72*3)),ALIEN_SIZE,green);
  /* Prints the saucer as a starter */
  sprites_render_buffer(saucer_18x9,SPRITES_SAUCER_WIDTH,SPRITES_SAUCER_HEIGHT,SAUCER_ROW_START_LOCATION,ALIEN_SIZE,red);

  /* Prints starting tank*/
  sprites_render_buffer(tank_17x10,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,BOTTOM_LEFT_CORNER_OF_SCREEN+(tank_start_pos*IMAGE_RENDER_BYTES_PER_PIXEL),ALIEN_SIZE,green);

  /* Prints the alien block completely */
  for(int i = 0; i < ALIEN_BLOCK_SIZE; i++) {
    Alien alien_temp = alien_block[i];
    sprites_render_buffer(alien_temp.image_out,alien_temp.width,alien_temp.height,alien_temp.current_location,ALIEN_SIZE,white);
  }
}

void image_render_tank(uint32_t move, uint16_t dir){
  if(dir == IMAGE_RENDER_RIGHT_MOVEMENT && tank_start_pos <= RIGHT_BOUND_TANK) {
    tank_start_pos = tank_start_pos+move;
    sprites_render_buffer(tank_17x10,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,BOTTOM_LEFT_CORNER_OF_SCREEN+(tank_start_pos*IMAGE_RENDER_BYTES_PER_PIXEL),ALIEN_SIZE,green);
  }
  else if(dir == IMAGE_RENDER_LEFT_MOVEMENT && tank_start_pos >= LEFT_BOUND) {
    tank_start_pos = tank_start_pos-move;
    sprites_render_buffer(tank_17x10,SPRITES_TANK_WIDTH,SPRITES_TANK_HEIGHT,BOTTOM_LEFT_CORNER_OF_SCREEN+(tank_start_pos*IMAGE_RENDER_BYTES_PER_PIXEL),ALIEN_SIZE,green);
  }
}

void image_render_saucer(){
  if(saucer_start_pos > (SAUCER_ROW_START_LOCATION + 580*IMAGE_RENDER_BYTES_PER_PIXEL)){
    sprites_render_buffer(saucer_18x9,SPRITES_SAUCER_WIDTH,SPRITES_SAUCER_HEIGHT,(saucer_start_pos),ALIEN_SIZE,black);
    // only for milestone 2
    saucer_start_pos = SAUCER_ROW_START_LOCATION - IMAGE_RENDER_BYTES_PER_PIXEL;
  }
  saucer_start_pos= saucer_start_pos + IMAGE_RENDER_BYTES_PER_PIXEL;
  sprites_render_buffer(saucer_18x9,SPRITES_SAUCER_WIDTH,SPRITES_SAUCER_HEIGHT,(saucer_start_pos),ALIEN_SIZE,red);

}

// moves the alien block around the screen
void image_render_move_alien_block() {
  uint16_t dir = current_alien_direction;
  if(dir == IMAGE_RENDER_RIGHT_MOVEMENT) { // if the alien block is moving right
    if(alien_block_right_bound >= maximum_bound_right_alien) { // if it has reached the right bound
      for(uint32_t d = 0; d<55; d++){
        Alien alien_temp = alien_block[d];
        alien_block[d].current_location = alien_temp.current_location+IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*2;
      }
      current_alien_direction = ~current_alien_direction;
    }
    else { // if it hasn't reached the right bound and is moving right
      alien_block_right_bound = alien_block_right_bound+2*IMAGE_RENDER_BYTES_PER_PIXEL;
      alien_block_left_bound = alien_block_left_bound+2*IMAGE_RENDER_BYTES_PER_PIXEL;
      uint16_t j = 0;
      for(uint16_t i = 0; i < 5; i++) {
        Alien alien_temp0 = alien_block[j];
        alien_block[j++].current_location = alien_temp0.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp1 = alien_block[j];
        alien_block[j++].current_location = alien_temp1.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp2 = alien_block[j];
        alien_block[j++].current_location = alien_temp2.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp3 = alien_block[j];
        alien_block[j++].current_location = alien_temp3.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp4 = alien_block[j];
        alien_block[j++].current_location = alien_temp4.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp5 = alien_block[j];
        alien_block[j++].current_location = alien_temp5.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp6 = alien_block[j];
        alien_block[j++].current_location = alien_temp6.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp7 = alien_block[j];
        alien_block[j++].current_location = alien_temp7.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp8 = alien_block[j];
        alien_block[j++].current_location = alien_temp8.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp9 = alien_block[j];
        alien_block[j++].current_location = alien_temp9.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp10 = alien_block[j];
        alien_block[j++].current_location = alien_temp10.current_location+2*IMAGE_RENDER_BYTES_PER_PIXEL;

        if(current_alien_position == ALIEN_OUT) {
          sprites_render_buffer(alien_temp0.image_in,alien_temp0.width,alien_temp0.height,alien_temp0.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp1.image_in,alien_temp1.width,alien_temp1.height,alien_temp1.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp2.image_in,alien_temp2.width,alien_temp2.height,alien_temp2.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp3.image_in,alien_temp3.width,alien_temp3.height,alien_temp3.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp4.image_in,alien_temp4.width,alien_temp4.height,alien_temp4.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp5.image_in,alien_temp5.width,alien_temp5.height,alien_temp5.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp6.image_in,alien_temp6.width,alien_temp6.height,alien_temp6.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp7.image_in,alien_temp7.width,alien_temp7.height,alien_temp7.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp8.image_in,alien_temp8.width,alien_temp8.height,alien_temp8.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp9.image_in,alien_temp9.width,alien_temp9.height,alien_temp9.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp10.image_in,alien_temp10.width,alien_temp10.height,alien_temp10.current_location,ALIEN_SIZE,white);
        }
        else {
          sprites_render_buffer(alien_temp0.image_out,alien_temp0.width,alien_temp0.height,alien_temp0.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp1.image_out,alien_temp1.width,alien_temp1.height,alien_temp1.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp2.image_out,alien_temp2.width,alien_temp2.height,alien_temp2.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp3.image_out,alien_temp3.width,alien_temp3.height,alien_temp3.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp4.image_out,alien_temp4.width,alien_temp4.height,alien_temp4.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp5.image_out,alien_temp5.width,alien_temp5.height,alien_temp5.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp6.image_out,alien_temp6.width,alien_temp6.height,alien_temp6.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp7.image_out,alien_temp7.width,alien_temp7.height,alien_temp7.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp8.image_out,alien_temp8.width,alien_temp8.height,alien_temp8.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp9.image_out,alien_temp9.width,alien_temp9.height,alien_temp9.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp10.image_out,alien_temp10.width,alien_temp10.height,alien_temp10.current_location,ALIEN_SIZE,white);
        }
      }
      current_alien_position = ~current_alien_position;
    }
  }
  else { // if the alien block is moving left
    if(alien_block_left_bound <= maximum_bound_left_alien) { // if it has reached the left bound
      for(uint32_t d = 0; d<55; d++){
        Alien alien_temp = alien_block[d];
        alien_block[d].current_location = alien_temp.current_location+IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_WIDTH*2;
      }
      current_alien_direction = ~current_alien_direction;
    }
    else { // if it hasn't reached the left bound
      alien_block_right_bound = alien_block_right_bound-2*IMAGE_RENDER_BYTES_PER_PIXEL;
      alien_block_left_bound = alien_block_left_bound-2*IMAGE_RENDER_BYTES_PER_PIXEL;
      /* rendering all alien images */
      uint16_t j = 0;
      for(uint16_t i = 0; i < 5; i++) {
        Alien alien_temp0 = alien_block[j];
        alien_block[j++].current_location = alien_temp0.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp1 = alien_block[j];
        alien_block[j++].current_location = alien_temp1.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp2 = alien_block[j];
        alien_block[j++].current_location = alien_temp2.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp3 = alien_block[j];
        alien_block[j++].current_location = alien_temp3.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp4 = alien_block[j];
        alien_block[j++].current_location = alien_temp4.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp5 = alien_block[j];
        alien_block[j++].current_location = alien_temp5.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp6 = alien_block[j];
        alien_block[j++].current_location = alien_temp6.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp7 = alien_block[j];
        alien_block[j++].current_location = alien_temp7.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp8 = alien_block[j];
        alien_block[j++].current_location = alien_temp8.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp9 = alien_block[j];
        alien_block[j++].current_location = alien_temp9.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;
        Alien alien_temp10 = alien_block[j];
        alien_block[j++].current_location = alien_temp10.current_location-2*IMAGE_RENDER_BYTES_PER_PIXEL;

        if(current_alien_position == ALIEN_OUT) {
          sprites_render_buffer(alien_temp0.image_in,alien_temp0.width,alien_temp0.height,alien_temp0.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp1.image_in,alien_temp1.width,alien_temp1.height,alien_temp1.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp2.image_in,alien_temp2.width,alien_temp2.height,alien_temp2.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp3.image_in,alien_temp3.width,alien_temp3.height,alien_temp3.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp4.image_in,alien_temp4.width,alien_temp4.height,alien_temp4.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp5.image_in,alien_temp5.width,alien_temp5.height,alien_temp5.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp6.image_in,alien_temp6.width,alien_temp6.height,alien_temp6.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp7.image_in,alien_temp7.width,alien_temp7.height,alien_temp7.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp8.image_in,alien_temp8.width,alien_temp8.height,alien_temp8.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp9.image_in,alien_temp9.width,alien_temp9.height,alien_temp9.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp10.image_in,alien_temp10.width,alien_temp10.height,alien_temp10.current_location,ALIEN_SIZE,white);
        }
        else {
          sprites_render_buffer(alien_temp0.image_out,alien_temp0.width,alien_temp0.height,alien_temp0.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp1.image_out,alien_temp1.width,alien_temp1.height,alien_temp1.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp2.image_out,alien_temp2.width,alien_temp2.height,alien_temp2.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp3.image_out,alien_temp3.width,alien_temp3.height,alien_temp3.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp4.image_out,alien_temp4.width,alien_temp4.height,alien_temp4.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp5.image_out,alien_temp5.width,alien_temp5.height,alien_temp5.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp6.image_out,alien_temp6.width,alien_temp6.height,alien_temp6.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp7.image_out,alien_temp7.width,alien_temp7.height,alien_temp7.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp8.image_out,alien_temp8.width,alien_temp8.height,alien_temp8.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp9.image_out,alien_temp9.width,alien_temp9.height,alien_temp9.current_location,ALIEN_SIZE,white);
          sprites_render_buffer(alien_temp10.image_out,alien_temp10.width,alien_temp10.height,alien_temp10.current_location,ALIEN_SIZE,white);
        }
      }
      current_alien_position = ~current_alien_position;
    }
  }
}

// closes the hdmi connection
void image_render_close() {
  hdmi_exit();
}
