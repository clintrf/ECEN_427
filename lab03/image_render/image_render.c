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
  sprites_render_image(letterR_5x5_test,5,5,0,2,white_buffer);
  sprites_render_image(letterR_5x5_test,5,5,60,1,white_buffer);
  sprites_render_image(alien_top_in_12x8_test,12,8,108,1,pink);
  sprites_render_image(alien_top_in_12x8_test,12,8,999,5,pink);
}

// closes the hdmi connection
void image_render_close() {
  hdmi_exit();
}
