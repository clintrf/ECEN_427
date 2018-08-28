#include "sprites.h"
#include "../hdmi/hdmi.h"

char black_image[3] = {0x00, 0x00, 0x00};

// prints an image to the screen
// image[] : the image to be printed
// current_location : top left pixel of the character
// scaling_factor : scale of the figure. must be int. size >= 1. place 1 for normal image
void sprites_render_image(const uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location, uint32_t scaling_factor, const void *color) {
  hdmi_set_offset(starting_location);
  uint32_t current_start_line = starting_location;
  for(int h=0; h<height; h++) {
    for(int j=0; j<scaling_factor; j++) {
      for(int w=0; w<width; w++) {
        if(image[h*width+w]==1) {
          for(int i=0; i<scaling_factor; i++) {
            hdmi_write(color,ONE_PIXEL);
          }
        }
        else {
          for(int i=0; i<scaling_factor; i++) {
            // uint32_t scaled_h = h*scaling_factor*SCALE_NEXT_LINE+(SCALE_NEXT_LINE*j);
            // uint32_t scaled_w = w*scaling_factor*ONE_PIXEL+(ONE_PIXEL*i);
            // hdmi_set_offset(starting_location+scaled_h+scaled_w+ONE_PIXEL);
            hdmi_write(black_image,ONE_PIXEL);
          }
        }
      }
      current_start_line = current_start_line+SCALE_NEXT_LINE;
      hdmi_set_offset(current_start_line);
    }
  }
}
