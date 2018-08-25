#include "hdmi.h"
#include "sprites.h"

/********************************** macros ***********************************/
#define IMAGE_RENDER_SCREEN_WIDTH 640 /* number of pixels across the screen */
#define IMAGE_RENDER_SCREEN_HEIGHT 480 /* number of pixels down the screen */
#define IMAGE_RENDER_BYTES_PER_PIXEL 3 /* number of bytes for every pixel */
/* number of bytes for the whole screen */
#define IMAGE_RENDER_WHOLE_SCREEN IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_SCREEN_HEIGHT*IMAGE_RENDER_BYTES_PER_PIXEL

/********************************** globals **********************************/
char full_screen_black[IMAGE_RENDER_WHOLE_SCREEN];
char white_buffer[IMAGE_RENDER_WHOLE_SCREEN];
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
  for(uint32_t i = 0; i < IMAGE_RENDER_WHOLE_SCREEN; i++) {
    white_buffer[i++] = 0xff; // if all bits are set to 0x00, the pixel will be black
    white_buffer[i++] = 0xff;
    white_buffer[i] = 0xff;
  }
  image_render_print_black_screen(); // call the print screen function
}

// prints the screen black and resets the cursor to the top left corner of the screen
void image_render_print_black_screen() {
  hdmi_write(full_screen_black,IMAGE_RENDER_WHOLE_SCREEN); /* this will write a black screen to the hdmi monitor */
  hdmi_set_offset(0); /* this will reset the fd offset back to the top left of the screen */
}

void image_render_print_game_over_screen() {}

void image_render_print_start_screen() {}

void image_render_update_screen() {}

void image_render_print_high_scores() {}

void image_render_print_gameover() {}

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
