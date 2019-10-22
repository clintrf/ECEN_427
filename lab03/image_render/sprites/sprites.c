#include "sprites.h"
#include "../hdmi/hdmi.h"

#define BYTES_PER_PIXEL 3                                                       // defines number of bytes in each pixel
#define MAX_BUFFER 24*18*3*8                                                    // defines the max number of pixels that need to be buffered when printing a sprite
#define MAX_SCALING_FACTOR 8                                                    // the max scaling that can be preformed on a sprite
#define ON_PIXEL 1                                                              // defines one pixel
#define POSITION_ZERO 0                                                         // defines the start pixel
#define POSITION_ONE 1                                                          // defines the position 1
#define POSITION_TWO 2                                                          // defines the position 2

char black_image[BYTES_PER_PIXEL] = {0x00, 0x00, 0x00};                         // defines the color black

// prints an image to the screen
// image[] : the image to be printed
// width : the pixel width of the image
// height : the pixel height of the image
// starting_location : top left pixel of the character
// scaling_factor : scale of the figure. must be int. size >= 1. place 1 for normal image
// color : desired color of the image
void sprites_render_image(const uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location, uint32_t scaling_factor, const void *color) {
  hdmi_set_offset(starting_location);                                           // bring the screen cursor to the top left corner of where we will write
  uint32_t current_start_line = starting_location;                              // set the current line to the same location
  for(int h=0; h<height; h++) {                                                 // this for loop will iterate through the height (rows of the image)
    for(int j=0; j<scaling_factor; j++) {                                       // this for loop tells us how much we should repeat each line (for scaling the image)
      for(int w=0; w<width; w++) {                                              // this for loop iterates across the image (columns of the image)
        if(image[h*width+w] == ON_PIXEL) {                                      // if the image pixel is a 1, then draw that pixel with given color
          for(int i=0; i<scaling_factor; i++) {                                 // this will iterate the amount of times we draw a pixl for scaling
            hdmi_write(color,SPRITES_ONE_PIXEL);                                // draw the pixel
          }
        }
        else {                                                                  // if the pixel is a 0, then draw black in that space
          for(int i=0; i<scaling_factor; i++) {                                 // this will iterate the amount of times we draw a pixl for scaling
            hdmi_write(black_image,SPRITES_ONE_PIXEL);                          // draw black
          }
        }
      }
      current_start_line = current_start_line+SPRITES_SCALE_NEXT_LINE;          // sets the next line
      hdmi_set_offset(current_start_line);                                      // this will go down to the next line of the character and restart at the beginning
    }
  }
}

// adds the image into a buffer for printing to the screen (performance increase)
// image[] : the image to be printed
// width : the pixel width of the image
// height : the pixel height of the image
// starting_location : top left pixel of the character
// scaling_factor : scale of the figure. must be int. size >= 1. place 1 for normal image
// color : desired color of the image
void sprites_render_buffer(const uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location, uint32_t scaling_factor, uint32_t color[BYTES_PER_PIXEL]) {
  char color_image[MAX_BUFFER];                                                 // 450*8 base sprite times a max scaler of 8
  if (scaling_factor > MAX_SCALING_FACTOR) {                                    // if the scaling factor becomes too large
    scaling_factor = MAX_SCALING_FACTOR;
  }
  uint32_t j = 0;
  for(uint32_t h = 0; h < height; h++){                                         // will loop through each pixel along the height
    for(uint32_t hs = 0; hs < scaling_factor; hs++) {                           // this for loop tells us how much we should repeat each line (for scaling the image)
      for(uint32_t w = 0; w < width; w++){                                      // will loop through each pixel along the width
        for(uint32_t ws = 0; ws < scaling_factor; ws++) {                       // this for loop tells us how much we should repeat each line (for scaling the image)
          uint32_t temp = (w+(h*width));
          if(image[temp] == ON_PIXEL){                                          // if the array position reads in a 1 (on), draw the color
            color_image[j++] = color[POSITION_ZERO];                            // save byte 1 of pixel
            color_image[j++] = color[POSITION_ONE];                             // save byte 2 of pixel
            color_image[j++] = color[POSITION_TWO];                             // save byte 3 of pixel
          }
          else{                                                                 // if the array reads in a 0 (off), draw black
            color_image[j++] = black_image[POSITION_ZERO];                      // save black byte 1 of pixel
            color_image[j++] = black_image[POSITION_ONE];                       // save black byte 2 of pixel
            color_image[j++] = black_image[POSITION_TWO];                       // save black byte 3 of pixel
          }
        }
      }
    }
  }
  for(int h = 0; h < (height*scaling_factor); h++) {                            // this for loop will iterate through the height (rows of the image)
    hdmi_set_offset(starting_location+h*(SPRITES_SCALE_NEXT_LINE));             // sets the offset for drawing pixel 
    hdmi_write(color_image+(h*width*scaling_factor*SPRITES_ONE_PIXEL),width*SPRITES_ONE_PIXEL*scaling_factor); // draw the pixel
  }
}
