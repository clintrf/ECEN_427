#include "score_board.h"
#include "../sprites/sprites.h"
#include "../hdmi/hdmi.h"

#ifndef GLOBALS_C
#include "../../globals/globals.c"
#endif

/********************************** macros ***********************************/
#define TEN_THOUSAND_SCALE 10000
#define THOUSAND_SCALE 1000
#define HUNDRED_SCALE 100
#define TEN_SCALE 10
#define ONE_SCALE 1
#define BYTES_PER_PIXEL 3
#define DEFAULT_CHAR_SCALE 3
#define SCALE_NEXT_LINE_ 640*SPRITES_ONE_PIXEL*10
#define CHARACTER_ONE_POSITION (SPRITES_SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(0*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10))
#define CHARACTER_TWO_POSITION (SPRITES_SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(1*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10))
#define CHARACTER_THREE_POSITION (SPRITES_SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(2*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10))
#define NUMBER_ONE_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(0*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10))
#define NUMBER_TWO_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(1*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10))
#define NUMBER_THREE_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(2*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10))
#define NUMBER_FOUR_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(3*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10))
#define NUMBER_FIVE_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(4*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10))
#define POSTION_0 0
#define POSTION_1 1
#define POSTION_2 2
#define POSTION_3 3
#define POSTION_4 4
#define POSTION_5 5
#define POSTION_6 6
#define POSTION_7 7
#define POSTION_8 8
#define POSTION_9 9
#define POSTION_10 10
#define POSTION_11 11
#define POSTION_12 12
#define POSTION_13 13
#define POSTION_14 14
#define POSTION_15 15
#define POSTION_16 16
#define POSTION_17 17
#define POSTION_18 18
#define POSTION_19 19
#define POSTION_20 20
#define POSTION_21 21
#define POSTION_22 22
#define POSTION_23 23
#define POSTION_24 24
#define POSTION_25 25
#define POSTION_26 26
#define POSTION_27 27
#define POSTION_28 28
#define POSTION_29 29
#define POSTION_30 30
#define POSTION_31 31
#define POSTION_32 32
#define POSTION_33 33
#define POSTION_34 34
#define POSTION_35 35
#define POSTION_36 36
#define TOP_TEN_SCORES 10
#define CHAR_OFFSET 48


/*********************************** globals ***********************************/
char white_test[BYTES_PER_PIXEL] = {0xFF, 0xFF, 0xFF};
struct player_stats high_scores[TOP_TEN_SCORES];

/*********************************** functions ***********************************/
// converts an integer or char passed into the function out as its array form for printing to the screen
// chrt : the char we wish to convert to an array form
// returns : the array form of the passed in character
const uint32_t *convert_to_array(int chrt){
  char temp = chrt;
  switch(temp){ // simple switch statement that maps to the correct index containing pixelized version of the letters
    case '0':
      return char_array[POSTION_0];
    case '1':
      return char_array[POSTION_1];
    case '2':
      return char_array[POSTION_2];
    case '3':
      return char_array[POSTION_3];
    case '4':
      return char_array[POSTION_4];
    case '5':
      return char_array[POSTION_5];
    case '6':
      return char_array[POSTION_6];
    case '7':
      return char_array[POSTION_7];
    case '8':
      return char_array[POSTION_8];
    case '9':
      return char_array[POSTION_9];
    case ' ':
      return char_array[POSTION_10];
    case 'A':
      return char_array[POSTION_11];
    case 'B':
      return char_array[POSTION_12];
    case 'C':
      return char_array[POSTION_13];
    case 'D':
      return char_array[POSTION_14];
    case 'E':
      return char_array[POSTION_15];
    case 'F':
      return char_array[POSTION_16];
    case 'G':
      return char_array[POSTION_17];
    case 'H':
      return char_array[POSTION_18];
    case 'I':
      return char_array[POSTION_19];
    case 'J':
      return char_array[POSTION_20];
    case 'K':
      return char_array[POSTION_21];
    case 'L':
      return char_array[POSTION_22];
    case 'M':
      return char_array[POSTION_23];
    case 'N':
      return char_array[POSTION_24];
    case 'O':
      return char_array[POSTION_25];
    case 'P':
      return char_array[POSTION_26];
    case 'Q':
      return char_array[POSTION_27];
    case 'R':
      return char_array[POSTION_28];
    case 'S':
      return char_array[POSTION_29];
    case 'T':
      return char_array[POSTION_30];
    case 'U':
      return char_array[POSTION_31];
    case 'V':
      return char_array[POSTION_32];
    case 'W':
      return char_array[POSTION_33];
    case 'X':
      return char_array[POSTION_34];
    case 'Y':
      return char_array[POSTION_35];
    case 'Z':
      return char_array[POSTION_36];
  }
};

// saves the scores to the board data structures in the program
void save_score_board(){
  uint32_t nam1, nam2, nam3, num, num1, num2, num3, num4, num5, test;
  FILE * fp;
  fp = fopen("/home/xilinx/ECEN_427/lab03/image_render/score_board/high_scores.txt", "r");
  for(int i = 0; i < TOP_TEN_SCORES; i++){ // reads the file and grabs the top scores from the file
    nam1 = getc(fp);
    nam2 = getc(fp);
    nam3 = getc(fp);
    getc(fp);
    num1 = getc(fp);
    num2 = getc(fp);
    num3 = getc(fp);
    num4 = getc(fp);
    num5 = getc(fp);
    while (test != '\n'){ // goes until the end of file
      test = getc(fp);
      // printf("%s ,%c \r\n" , "extra ", test);
    }
    test = '~';

    int temp1 = (TEN_THOUSAND_SCALE*(num1-CHAR_OFFSET));
    int temp2 = (THOUSAND_SCALE*(num2-CHAR_OFFSET));
    int temp3 = (HUNDRED_SCALE*(num3-CHAR_OFFSET));
    int temp4 = (TEN_SCALE*(num4-CHAR_OFFSET));
    int temp5 = (ONE_SCALE*(num5-CHAR_OFFSET));

    num = temp1+temp2+temp3+temp4+temp5; // establihes the high score
    high_scores[i].score = num; // transfer all the data into the high scores data structure
    high_scores[i].letter1 = nam1;
    high_scores[i].letter2 = nam2;
    high_scores[i].letter3 = nam3;

    for(int j = 0; j < SCORE_BOARD_CHAR_ARRAY_SIZE;j++){ // converts those numbers to the array version
      high_scores[i].name_char[POSTION_0][j] = convert_to_array(nam1)[j];
      high_scores[i].name_char[POSTION_1][j] = convert_to_array(nam2)[j];
      high_scores[i].name_char[POSTION_2][j] = convert_to_array(nam3)[j];
      high_scores[i].score_char[POSTION_0][j] = convert_to_array(num1)[j];
      high_scores[i].score_char[POSTION_1][j] = convert_to_array(num2)[j];
      high_scores[i].score_char[POSTION_2][j] = convert_to_array(num3)[j];
      high_scores[i].score_char[POSTION_3][j] = convert_to_array(num4)[j];
      high_scores[i].score_char[POSTION_4][j] = convert_to_array(num5)[j];
    }
  }
  fclose(fp); // closes the file
}

// function to print the top 10 high scores
void print_high_scores(){
  for(int i = 0; i < TOP_TEN_SCORES; i++){ // prints the high scores onto the screen via for loop
    sprites_render_image(high_scores[i].name_char[POSTION_0],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,CHARACTER_ONE_POSITION,DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[POSTION_1],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,CHARACTER_TWO_POSITION,DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[POSTION_2],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,CHARACTER_THREE_POSITION,DEFAULT_CHAR_SCALE,white_test);

    sprites_render_image(high_scores[i].score_char[POSTION_0],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_ONE_POSITION,DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[POSTION_1],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_TWO_POSITION,DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[POSTION_2],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_THREE_POSITION,DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[POSTION_3],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_FOUR_POSITION,DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].score_char[POSTION_4],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_FIVE_POSITION,DEFAULT_CHAR_SCALE,white_test);
  }
}

// updates the scores on the board
// name1 : first character of the players name
// name2 : second character of the players name
// name3 : third character of the players name
// final_score : thescore the player received in the game
void update_stats(uint32_t name1,uint32_t name2 ,uint32_t name3, uint32_t final_score){
  uint16_t insert_flag = 1;
  uint32_t numb1,numb2,numb3,numb4,numb5;
  printf("%s %d\r\n" , "Final Score:", final_score);
  save_score_board();

  FILE * fpp;
  int i;
  /* open the file for writing*/
  fpp = fopen ("/home/xilinx/ECEN_427/lab03/image_render/score_board/high_scores.txt","r+");
  uint32_t whole_numb[SCORE_BOARD_NUMBER_OF_NUMBERS_IN_SCORE];
  for(int i = 0; i < TOP_TEN_SCORES; i++){

    //whole_numb = convert_num(high_scores[i].score);
    numb1 = ((high_scores[i].score)/TEN_THOUSAND_SCALE);
    numb2 = ((high_scores[i].score-numb1*TEN_THOUSAND_SCALE)/THOUSAND_SCALE);
    numb3 = ((high_scores[i].score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE)/HUNDRED_SCALE);
    numb4 = ((high_scores[i].score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE-numb3*HUNDRED_SCALE)/TEN_SCALE);
    numb5 = ((high_scores[i].score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE-numb3*HUNDRED_SCALE-numb4*TEN_SCALE)/ONE_SCALE);

    if(high_scores[i].score > final_score) { // if the player score is lower than a high score, write the high score to the file
      fprintf (fpp, "%c%c%c %d%d%d%d%d\n" , high_scores[i].letter1,high_scores[i].letter2,high_scores[i].letter3,numb1,numb2,numb3,numb4,numb5);
    }
    else { // if the player's score is higher than the current high score position, then write the player score to the file
      if (insert_flag){ // only do this if the insert flg is up
        numb1 = ((final_score)/TEN_THOUSAND_SCALE);
        numb2 = ((final_score-numb1*TEN_THOUSAND_SCALE)/THOUSAND_SCALE);
        numb3 = ((final_score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE)/HUNDRED_SCALE);
        numb4 = ((final_score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE-numb3*HUNDRED_SCALE)/TEN_SCALE);
        numb5 = ((final_score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE-numb3*HUNDRED_SCALE-numb4*TEN_SCALE)/ONE_SCALE);
        fprintf (fpp, "%c%c%c %d%d%d%d%d\n" , name1,name2,name3,numb1,numb2,numb3,numb4,numb5);
        insert_flag = 0;
        i--;
      }
      else{ // if the insert flag is down, then the high score has already been written
        //numb5,numb4,numb3,numb2,numb1 = convert_num(high_scores[i].score);
        fprintf (fpp, "%c%c%c %d%d%d%d%d\n" , high_scores[i].letter1,high_scores[i].letter2,high_scores[i].letter3,numb1,numb2,numb3,numb4,numb5);
      }
    }
  }
  fprintf (fpp, "\n" );
  fprintf (fpp, "\n" );
  fprintf (fpp, "\n" );
  save_score_board(); // save this score board
  fclose(fpp); // close the file
}
