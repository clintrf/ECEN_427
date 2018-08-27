#include "score_board.h"
#include "../sprites/sprites.h"
#include "../hdmi/hdmi.h"

/**************************** function prototypes ****************************/

/********************************** macros ***********************************/
#define TOP_TEN_SCORES 10
#define PIXEL_SPACING 3
#define ONE_PIXEL 3
#define DEFAULT_CHAR_SCALE 1
#define SCORE_BOARD_START_POS 333*ONE_PIXEL
#define SCALE_NEXT_LINE 640*ONE_PIXEL*10

char white_test[3] = {0xFF, 0xFF, 0xFF};

struct player_stats high_scores[TOP_TEN_SCORES];

void init_score_board(){
  for(int i = 0; i < TOP_TEN_SCORES; i++){
    high_scores[i].score = 0;
    for(int j = 0; j < 25; j++){
      high_scores[i].score_char[0][j] = number0_5x5[j];
      high_scores[i].score_char[1][j] = number0_5x5[j];
      high_scores[i].score_char[2][j] = number0_5x5[j];
      high_scores[i].name_char[0][j] = letterA_5x5[j];
      high_scores[i].name_char[1][j] = letterA_5x5[j];
      high_scores[i].name_char[2][j] = letterA_5x5[j];
    }
  }
}

// function to print the top 10 high scores
void print_high_scores(){
  printf("%s\r\n" , "IN LOOP");
  for(int i = 0; i < TOP_TEN_SCORES; i++){
    sprites_render_image(high_scores[i].name_char[0],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE*i)+(0*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[1],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE*i)+(1*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[2],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE*i)+(2*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);

    sprites_render_image(high_scores[i].score_char[0],5,5,((SCORE_BOARD_START_POS+150)+(SCALE_NEXT_LINE*i)+(0*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[1],5,5,((SCORE_BOARD_START_POS+150)+(SCALE_NEXT_LINE*i)+(1*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[2],5,5,((SCORE_BOARD_START_POS+150)+(SCALE_NEXT_LINE*i)+(2*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
  }
}

// function to update the high scores list.
void update_stats(){


}
