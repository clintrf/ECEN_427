#include "sprites.h"
#include "../hdmi/hdmi.h"

#ifndef GLOBALS_C
#include "../../globals/globals.c"
#endif

char black_image[3] = {0x00, 0x00, 0x00};

// prints an image to the screen
// image[] : the image to be printed
// width : the pixel width of the image
// height : the pixel height of the image
// starting_location : top left pixel of the character
// scaling_factor : scale of the figure. must be int. size >= 1. place 1 for normal image
// color : desired color of the image


void sprites_render_image(const uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location, uint32_t scaling_factor, const void *color) {
  hdmi_set_offset(starting_location); // bring the screen cursor to the top left corner of where we will write
  uint32_t current_start_line = starting_location; // set the current line to the same location
  for(int h=0; h<height; h++) { // this for loop will iterate through the height (rows of the image)
    for(int j=0; j<scaling_factor; j++) { // this for loop tells us how much we should repeat each line (for scaling the image)
      for(int w=0; w<width; w++) { // this for loop iterates across the image (columns of the image)
        if(image[h*width+w]==1) { // if the image pixel is a 1, then draw that pixel with given color
          for(int i=0; i<scaling_factor; i++) { // this will iterate the amount of times we draw a pixl for scaling
            hdmi_write(color,SPRITES_ONE_PIXEL); // draw the pixel
          }
        }
        else { // if the pixel is a 0, then draw black in that space
          for(int i=0; i<scaling_factor; i++) { // this will iterate the amount of times we draw a pixl for scaling
            hdmi_write(black_image,SPRITES_ONE_PIXEL); // draw black
          }
        }
      }
      current_start_line = current_start_line+SPRITES_SCALE_NEXT_LINE; // sets the next line
      hdmi_set_offset(current_start_line); // this will go down to the next line of the character and restart at the beginning
    }
  }
}
