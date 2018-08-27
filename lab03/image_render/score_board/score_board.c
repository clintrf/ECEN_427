#include "score_board.h"
#include "../sprites/sprites.h"
#include "../hdmi/hdmi.h"

/**************************** function prototypes ****************************/

/********************************** macros ***********************************/
#define TOP_TEN_SCORES 10
#define PIXEL_SPACING 3
#define ONE_PIXEL 3
#define DEFAULT_CHAR_SCALE 1
#define SCORE_BOARD_START_POS 640*300+201
#define SCALE_NEXT_LINE_ 640*ONE_PIXEL*10

char white_test[3] = {0xFF, 0xFF, 0xFF};

struct player_stats high_scores[TOP_TEN_SCORES];

uint32_t convert_to_array(int chrt){
  char temp = chrt;
  switch(temp){
    case '0':
      return char_array[0];
    case '1':
      return char_array[1];
    case '2':
      return char_array[2];
    case '3':
      return char_array[3];
    case '4':
      return char_array[4];
    case '5':
      return char_array[5];
    case '6':
      return char_array[6];
    case '7':
      return char_array[7];
    case '8':
      return char_array[8];
    case '9':
      return char_array[9];
    case ' ':
      return char_array[10];
    case 'A':
      return char_array[11];
    case 'B':
      return char_array[12];
    case 'C':
      return char_array[13];
    case 'D':
      return char_array[14];
    case 'E':
      return char_array[15];
    case 'F':
      return char_array[16];
    case 'G':
      return char_array[17];
    case 'H':
      return char_array[18];
    case 'I':
      return char_array[19];
    case 'J':
      return char_array[20];
    case 'K':
      return char_array[21];
    case 'L':
      return char_array[22];
    case 'M':
      return char_array[23];
    case 'N':
      return char_array[24];
    case 'O':
      return char_array[25];
    case 'P':
      return char_array[26];
    case 'Q':
      return char_array[27];
    case 'R':
      return char_array[28];
    case 'S':
      return char_array[29];
    case 'T':
      return char_array[30];
    case 'U':
      return char_array[31];
    case 'V':
      return char_array[32];
    case 'W':
      return char_array[33];
    case 'X':
      return char_array[34];
    case 'Y':
      return char_array[35];
    case "Z":
      return char_array[36];
    default:
  }
};

void init_score_board(){




  int one;
  int two;
  int three;

  FILE * fp;
  fp = fopen("/home/xilinx/ECEN_427/lab03/image_render/score_board/high_scores.txt", "r");

  for(int i = 0; i < TOP_TEN_SCORES; i++){
    for(int j = 0; j < 25; j++){
      high_scores[i].name_char[0][j] = convert_to_array(getc(fp))[j];
      high_scores[i].name_char[1][j] = convert_to_array(getc(fp))[j];
      high_scores[i].name_char[2][j] = convert_to_array(getc(fp))[j];
      getc(fp);
      high_scores[i].score_char[0][j] = convert_to_array(getc(fp))[j];
      high_scores[i].score_char[1][j] = convert_to_array(getc(fp))[j];
      high_scores[i].score_char[2][j] = convert_to_array(getc(fp))[j];
      high_scores[i].score_char[3][j] = convert_to_array(getc(fp))[j];
      high_scores[i].score_char[4][j] = convert_to_array(getc(fp))[j];
      getc(fp);
    }
  }
}

// function to print the top 10 high scores
void print_high_scores(){
  for(int i = 0; i < TOP_TEN_SCORES; i++){
    sprites_render_image(high_scores[i].name_char[0],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i)+(0*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[1],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i)+(1*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[2],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i)+(2*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);

    sprites_render_image(high_scores[i].score_char[0],5,5,((SCORE_BOARD_START_POS+150)+(SCALE_NEXT_LINE_*i)+(0*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[1],5,5,((SCORE_BOARD_START_POS+150)+(SCALE_NEXT_LINE_*i)+(1*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[2],5,5,((SCORE_BOARD_START_POS+150)+(SCALE_NEXT_LINE_*i)+(2*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
  }
}

// function to update the high scores list.
void update_stats(){

}
