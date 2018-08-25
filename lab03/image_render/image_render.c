#include "hdmi.h"
#include "sprites.h"

/********************************** macros ***********************************/
#define IMAGE_RENDER_SCREEN_WIDTH 640 /* number of pixels across the screen */
#define IMAGE_RENDER_SCREEN_HEIGHT 480 /* number of pixels down the screen */
#define IMAGE_RENDER_BYTES_PER_PIXEL 3 /* number of bytes for every pixel */
/* number of bytes for the whole screen */
#define IMAGE_RENDER_WHOLE_SCREEN IMAGE_RENDER_SCREEN_WIDTH*IMAGE_RENDER_BYTES_PER_PIXEL*IMAGE_RENDER_SCREEN_HEIGHT

/********************************** globals **********************************/
char black[3] = {0x00, 0x00, 0x00};
char pink[3] = {0xFF, 0x69, 0xB4};
char white[3] = {0xFF, 0xFF, 0xFF};

/********************************* functions *********************************/
void image_render_print_black_screen() {
  int32_t test3 = hdmi_init("/dev/ecen427_hdmi"); /* opens a path to the HDMI driver (enables/read write) */
  uint32_t test = hdmi_set_offset(0); /* this will reset the fd offset back to the top left of the screen */
  uint32_t test2 = hdmi_write(pink,IMAGE_RENDER_WHOLE_SCREEN); /* this will write a black screen to the hdmi monitor */

  printf("Is this thing opening? : %d\n",test3);
  printf("Current Offset: %zu\n",test);
  printf("Number of Bytes Written: %zu\n",test2);
}

void image_render_print_game_over_screen() {}

void image_render_print_start_screen() {}

void image_render_update_screen() {}

void image_render_print_high_scores() {}

void image_render_print_gameover() {}

void image_render_close() {
  hdmi_exit();
}
