#include "score_board.h"
#include "../sprites/sprites.h"
#include "../hdmi/hdmi.h"

/**************************** function prototypes ****************************/

/********************************** macros ***********************************/
#define TOP_TEN_SCORES 10
#define PIXEL_SPACING 3
#define ONE_PIXEL 3
#define DEFAULT_CHAR_SCALE 2
#define SCORE_BOARD_START_POS 640*100*3+(220*3)
#define SCALE_NEXT_LINE_ 640*ONE_PIXEL*10
#define CHAR_OFFSET 48

char white_test[3] = {0xFF, 0xFF, 0xFF};

struct player_stats high_scores[TOP_TEN_SCORES];

const uint32_t *convert_to_array(int chrt){
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
    case 'Z':
      return char_array[36];

  }
};

void init_score_board(){
  uint32_t nam1, nam2, nam3, num, num1, num2, num3, num4, num5;
  FILE * fp;
  fp = fopen("/home/xilinx/ECEN_427/lab03/image_render/score_board/high_scores.txt", "r");

  for(int i = 0; i < TOP_TEN_SCORES; i++){
    nam1 = getc(fp);
    nam2 = getc(fp);
    nam3 = getc(fp);
    getc(fp);
    num1 = getc(fp);
    num2 = getc(fp);
    num3 = getc(fp);
    num4 = getc(fp);
    num5 = getc(fp);
    getc(fp);

    num = 10000*(num1-CHAR_OFFSET)+1000*(num2-CHAR_OFFSET)+100*(num3-CHAR_OFFSET)+10*(num4-CHAR_OFFSET)+(num5-CHAR_OFFSET);
    high_scores[i].score = num;

    for(int j = 0; j<25;j++){
      high_scores[i].name_char[0][j] = convert_to_array(nam1)[j];
      high_scores[i].name_char[1][j] = convert_to_array(nam2)[j];
      high_scores[i].name_char[2][j] = convert_to_array(nam3)[j];
      high_scores[i].score_char[0][j] = convert_to_array(num1)[j];
      high_scores[i].score_char[1][j] = convert_to_array(num2)[j];
      high_scores[i].score_char[2][j] = convert_to_array(num3)[j];
      high_scores[i].score_char[3][j] = convert_to_array(num4)[j];
      high_scores[i].score_char[4][j] = convert_to_array(num5)[j];
    }
  }
}

// function to print the top 10 high scores
void print_high_scores(){
  for(int i = 0; i < TOP_TEN_SCORES; i++){
    sprites_render_image(high_scores[i].name_char[0],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(0*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[1],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(1*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[2],5,5,(SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(2*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);

    sprites_render_image(high_scores[i].score_char[0],5,5,((SCORE_BOARD_START_POS+50*ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(0*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[1],5,5,((SCORE_BOARD_START_POS+50*ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(1*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[2],5,5,((SCORE_BOARD_START_POS+50*ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(2*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[3],5,5,((SCORE_BOARD_START_POS+50*ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(3*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[4],5,5,((SCORE_BOARD_START_POS+50*ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(4*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_test);
  }
}

// function to update the high scores list.
void update_stats(){

}
