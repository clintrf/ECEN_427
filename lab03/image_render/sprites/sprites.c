#include "sprites.h"
#include "../hdmi/hdmi.h"

/**************************** function prototypes ****************************/

/********************************** macros ***********************************/
#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 7
#define ALIEN_EXPLOSION_WIDTH 12
#define ALIEN_EXPLOSION_HEIGHT 10
#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define TANK_WIDTH 15
#define TANK_HEIGHT 8
#define BUNKER_WIDTH 24
#define BUNKER_HEIGHT 18
#define BUNKER_DAMAGE_WIDTH 6
#define BUNKER_DAMAGE_HEIGHT 6
#define TANK_BULLET_WIDTH 1
#define ALIEN_BULLET_WIDTH 3
#define BULLET_HEIGHT 5
#define CHARACTER_WIDTH 5
#define CHARATCER_HEIGHT 5
#define SCALE_NEXT_LINE 640
#define ONE_PIXEL 3

/********************************** globals **********************************/

// prints an image to the screen
// image[] : the image to be printed
// current_location : top left pixel of the character
// scaling_factor : scale of the figure. must be int. size >= 1. place 1 for normal image
void sprites_render_image(const uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location, uint32_t scaling_factor, const void *color) {
  hdmi_set_offset(starting_location);
  uint32_t current_start_line = starting_location;
  uint32_t true_width = width*ONE_PIXEL;
  for(int h = 0; h < height; h++) {
    for(int w = 0; w < width; w++) {
      uint32_t offset = h*true_width+(w*ONE_PIXEL);
      if(image[h*width+w] == 1) {
        printf("I'm writing now\n");
        hdmi_write(color,ONE_PIXEL);
      }
      else {
        hdmi_set_offset(starting_location+offset+ONE_PIXEL);
      }
    }
    current_start_line = current_start_line+SCALE_NEXT_LINE;
    hdmi_set_offset(current_start_line);
  }
}
