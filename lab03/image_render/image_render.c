#include "hdmi.h"
#include "sprites.h"

/********************************** macros ***********************************/
#define IMAGE_RENDER_SCREEN_WIDTH 640 /* number of pixels across the screen */
#define IMAGE_RENDER_SCREEN_HEIGHT 480 /* number of pixels down the screen */
#define IMAGE_RENDER_BYTES_PER_PIXEL 3 /* number of bytes for every pixel */
/* number of bytes for the whole screen */
#define IMAGE_RENDER_WHOLE_SCREEN IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_HEIGHT

/********************************** globals **********************************/
char black[3] = {0x00,0x00,0x00};

/********************************* functions *********************************/
void image_render_print_black_screen() {
  hdmi_init(HDMI_FILE_PATH);
  hdmi_write(black,IMAGE_RENDER_WHOLE_SCREEN);
}

void image_render_print_game_over_screen() {}

void image_render_print_start_screen() {}

void image_render_update_screen() {}

void image_render_print_high_scores() {}

void image_render_print_gameover() {}
