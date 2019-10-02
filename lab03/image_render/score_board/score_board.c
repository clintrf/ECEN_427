#include "score_board.h
#include "../sprites/sprites.h"
#include "../hdmi/hdmi.h"

/**************************** function prototypes ****************************/

/********************************** macros ***********************************/
#define TOP_TEN_SCORES 10

player_stats high_scores[TOP_TEN_SCORES];

// function to print the top 10 high scores
void print_high_scores(){
  for(int i = 0; i < TOP_TEN_SCORES; i++){
    sprites_render_image(saucer_16x7,16,7,0,1,white);
  }
}

// function to update the high scores list.
void update_stats(){

}
