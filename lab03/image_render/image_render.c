#include "hdmi.h"
#include "sprites.h"
#include "score_board.h"

/********************************** macros ***********************************/
#define IMAGE_RENDER_SCREEN_WIDTH 640 /* number of pixels across the screen */
#define IMAGE_RENDER_SCREEN_HEIGHT 480 /* number of pixels down the screen */
#define IMAGE_RENDER_BYTES_PER_PIXEL 3 /* number of bytes for every pixel */
/* number of bytes for the whole screen */
#define IMAGE_RENDER_WHOLE_SCREEN IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_SCREEN_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL
#define NUMBER_OF_CHARS_FOR_NAME 3
#define NUMBER_OF_BITS_PER_CHAR 25

/********************************** globals **********************************/
char full_screen_black[IMAGE_RENDER_WHOLE_SCREEN];
char black[3] = {0x00, 0x00, 0x00};
char pink[3] = {0xFF, 0x69, 0xB4};
char white[3] = {0xFF, 0xFF, 0xFF};

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

// prints the first part of the game over screen
// this will display "GAME OVER" and "Enter your name" and a place to enter the player name
void image_render_print_game_over_screen_enter_name() {
  image_render_print_black_screen();
  sprites_render_image(letterG_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,G_START_LOCATION_GO,GAME_OVER_SCALING,white);
  sprites_render_image(letterA_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,A_START_LOCATION_GO,GAME_OVER_SCALING,white);
  sprites_render_image(letterM_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,M_START_LOCATION_GO,GAME_OVER_SCALING,white);
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E_START_LOCATION_GO,GAME_OVER_SCALING,white);
  sprites_render_image(letterO_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,O_START_LOCATION_GO,GAME_OVER_SCALING,white);
  sprites_render_image(letterV_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,V_START_LOCATION_GO,GAME_OVER_SCALING,white);
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E2_START_LOCATION_GO,GAME_OVER_SCALING,white);
  sprites_render_image(letterR_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,R_START_LOCATION_GO,GAME_OVER_SCALING,white);
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterN_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,N_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterT_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,T_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E2_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterR_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,R_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterY_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,Y_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterO_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,O_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterU_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,U_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterR_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,R2_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterN_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,N2_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterA_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,A_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterM_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,M_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E3_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,white);
}

// prints the second part of the game over screen
// this will display "GAME OVER" and "High Scores" and the list of high scores
void image_render_print_game_over_screen_high_scores() {
  /* clear the bottom portion of the screen */
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterN_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,N_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterT_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,T_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E2_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterR_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,R_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterY_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,Y_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterO_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,O_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterU_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,U_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterR_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,R2_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterN_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,N2_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterA_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,A_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterM_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,M_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E3_START_LOCATION_YN,NORMAL_CHARACTER_SCALING,black);
  /* print high scores image */
  sprites_render_image(letterH_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,H_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterI_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,I_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterG_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,G_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterH_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,H2_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterS_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,S_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterC_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,C_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterO_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,O_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterR_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,R_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterE_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,E_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  sprites_render_image(letterS_5x5,CHARACTER_WIDTH,CHARACTER_HEIGHT,S2_START_LOCATION_HS,NORMAL_CHARACTER_SCALING,white);
  save_score_board();
  update_stats('Z','Z','Z',120);
  update_stats('A','B','C',121);
  update_stats('A','B','A',1210);
  update_stats('A','A','C',111);
  update_stats('C','L','T',10);
  update_stats('D','A','X',99999);
  //init_score_board();
  print_high_scores();
}

void image_render_print_start_screen() {}

void image_render_update_screen() {}

void image_render_print_high_scores() {}

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

  save_score_board();
  update_stats('Z','Z','Z',120);
  update_stats('A','B','C',121);
  update_stats('A','B','A',1210);
  update_stats('A','A','C',111);
  update_stats('C','L','T',10);
  update_stats('D','A','X',999);
  //init_score_board();
  print_high_scores();
}

// closes the hdmi connection
void image_render_close() {
  hdmi_exit();
}
